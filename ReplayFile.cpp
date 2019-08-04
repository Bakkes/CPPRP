#include "ReplayFile.h"
#include <fstream>
#include "networkdata.h"
#include "rapidjson/filewritestream.h"
#include <rapidjson/writer.h>

ReplayFile::ReplayFile(std::filesystem::path path_) : path(path_)
{
	
}


ReplayFile::~ReplayFile()
{
}

bool ReplayFile::Load()
{
	if (!std::filesystem::exists(path))
		return false;
	std::ifstream file(path, std::ios::binary | std::ios::ate);
	/*file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		file.open(path, std::ios::binary);
	}
	catch (std::ifstream::failure e) {
		std::cerr << "Exception opening file: " << std::strerror(errno) << "\n";
	}*/
	
	std::streamsize size = file.tellg();
	data.resize((size_t)size);
	file.seekg(0, std::ios::beg);
	
	if (file.bad())
		return false;
	return (bool)file.read(data.data(), size);
}

void ReplayFile::DeserializeHeader()
{
	const uint32_t dataSize = data.size() * 8;
	replayFile = std::make_shared<ReplayFileData>();
	fullReplayBitReader = CPPBitReader<uint32_t>((const uint32_t*)data.data(), dataSize, replayFile); //They're read as bytes, since we're retrieving them in memory as uint32_t, divide size by 4 (bytes)
	//replayFile->header.crc = fullReplayBitReader.read<uint32_t>();
	replayFile->header = {
		fullReplayBitReader.read<uint32_t>(),	//Size
		fullReplayBitReader.read<uint32_t>(),	//CRC
		fullReplayBitReader.read<uint32_t>(),	//engineVersion
		fullReplayBitReader.read<uint32_t>()	//licenseeVersion
	};

	if (replayFile->header.engineVersion >= 868 && replayFile->header.licenseeVersion >= 18)
	{
		replayFile->header.netVersion = fullReplayBitReader.read<uint32_t>();
	}
	replayFile->replayType = fullReplayBitReader.read<std::string>(); //Not sure what this is


	while (true) {
		auto baseProperty = std::make_shared<Property>();
		const bool moreToParse = ParseProperty(baseProperty);
		if (!moreToParse)
		{
			break;
		}
		replayFile->properties[baseProperty->property_name] = baseProperty;
	}
	//replayFile->properties = baseProperty;
	replayFile->body_size = fullReplayBitReader.read<uint32_t>();
	replayFile->crc2 = fullReplayBitReader.read<uint32_t>();

	const uint32_t levelCount = fullReplayBitReader.read<uint32_t>();
	for (uint32_t i = 0; i < levelCount; ++i)
	{
		replayFile->levels.push_back(fullReplayBitReader.read<std::string>());
	}

	const uint32_t keyframeCount = fullReplayBitReader.read<uint32_t>();
	for (uint32_t i = 0; i < keyframeCount; ++i)
	{
		replayFile->keyframes.push_back(
			{
				fullReplayBitReader.read<float>(),	//Time
				fullReplayBitReader.read<uint32_t>(),	//Frame
				fullReplayBitReader.read<uint32_t>()	//File position
			});
	}

	const uint32_t netstreamCount = static_cast<uint32_t>(fullReplayBitReader.read<int32_t>());
	replayFile->netstream_data = data.data() + fullReplayBitReader.GetBytePosition(); //We know this is always aligned, so valid
	uint32_t test = netstreamCount * 8;
	fullReplayBitReader.skip(test);
	replayFile->netstream_size = netstreamCount;

	if (!fullReplayBitReader.canRead())
	{
		//Replay is corrupt
		throw GeneralParseException("ReplayFile corrupt. header + netstream_size > filesize", fullReplayBitReader);
	}

	const int32_t debugStringSize = fullReplayBitReader.read<int32_t>();
	for (int32_t i = 0; i < debugStringSize; ++i)
	{
		uint32_t frame = fullReplayBitReader.read<uint32_t>();
		std::string key = fullReplayBitReader.read<std::string>();
		std::string value = fullReplayBitReader.read<std::string>();
		///printf("%s = %s", key.c_str(), value.c_str());
	}
	//fullReplayBitReader.skip(4*8); //debug_log apparently

	const uint32_t replayTickCount = fullReplayBitReader.read<uint32_t>();
	for (uint32_t i = 0; i < replayTickCount; ++i)
	{
		replayFile->replayticks.push_back(
			{
				fullReplayBitReader.read<std::string>(),	//Type
				fullReplayBitReader.read<uint32_t>()		//Frame
			});
	}


	const uint32_t replicatedPackagesCount = fullReplayBitReader.read<uint32_t>();
	for (uint32_t i = 0; i < replicatedPackagesCount; ++i)
	{
		replayFile->replicated_packages.push_back(
			{
				fullReplayBitReader.read<std::string>()
			});
	}

	const uint32_t objectsCount = fullReplayBitReader.read<uint32_t>();
	for (uint32_t i = 0; i < objectsCount; ++i)
	{
		replayFile->objects.push_back(
			{
				fullReplayBitReader.read<std::string>()
			});
	}

	const uint32_t namesCount = fullReplayBitReader.read<uint32_t>();
	for (uint32_t i = 0; i < namesCount; ++i)
	{
		replayFile->names.push_back(
			{
				fullReplayBitReader.read<std::string>()
			});
	}

	const uint32_t classIndexCount = fullReplayBitReader.read<uint32_t>();
	for (uint32_t i = 0; i < classIndexCount; ++i)
	{
		replayFile->class_indices.push_back(
			{
				fullReplayBitReader.read<std::string>(),	//Class_name
				fullReplayBitReader.read<uint32_t>()		//Index
			});
	}

	const uint32_t classNetsCount = fullReplayBitReader.read<uint32_t>();
	replayFile->classnets.resize(classNetsCount);
	for (uint32_t i = 0; i < classNetsCount; ++i)
	{
		ClassNet cn = {
			fullReplayBitReader.read<int32_t>(),		//Index
			fullReplayBitReader.read<int32_t>(),		//Parent
			NULL,							//Parent class, not known yet
			fullReplayBitReader.read<int32_t>(),		//Id
			fullReplayBitReader.read<int32_t>(),		//Prop_indexes_size
			std::vector<PropIndexId>(),		//Empty propindexid array
			0,								//Max_prop_id
			std::vector<uint16_t>()			//Property_id_cache
		};

		const uint32_t newSize = cn.prop_indexes_size;
		cn.prop_indexes.resize(newSize);
		for (uint32_t j = 0; j < newSize; ++j)
		{
			cn.prop_indexes[j] = (
				PropIndexId	{
					fullReplayBitReader.read<int32_t>(),	//Prop_index
					fullReplayBitReader.read<int32_t>()	//Prop_id
				});
		}
		std::shared_ptr<ClassNet> classNet = std::make_shared<ClassNet>(cn);
		replayFile->classnets[i] = (classNet);

		//Set parent class if exists
		int ow = 0;
		for (int32_t k = (int32_t)i - 1; k >= 0; --k)
		{
			if (replayFile->classnets[i]->parent == replayFile->classnets[k]->id)
			{
				replayFile->classnets[i]->parent_class = replayFile->classnets[k];
				break;
			}
		}
	}
	if (replayFile->header.netVersion >= 10)
	{
		fullReplayBitReader.read<int32_t>();
	}
	if (replayFile->classnets.size() > 0) 
	{
		networkParser.RegisterParsers(replayFile);
	}
}

void ReplayFile::MergeDuplicates()
{
	std::unordered_map<std::string, int> counts;
	for (auto it : classnetMap)
	{
		if (it.second)
		{
			counts[replayFile->objects[it.second->index]]++;
		}
		
	}
	assert(counts.size() == classnetMap.size());
}

const std::unordered_map<std::string, std::string> class_extensions = 
{
	{"Engine.Actor", "Core.Object"}
  , {"TAGame.CarComponent_Boost_TA", "TAGame.CarComponent_TA"}
  , {"TAGame.CarComponent_Dodge_TA", "TAGame.CarComponent_TA"}
  , {"TAGame.CarComponent_DoubleJump_TA", "TAGame.CarComponent_TA"}
  , {"TAGame.CarComponent_FlipCar_TA", "TAGame.CarComponent_TA"}
  , {"TAGame.CarComponent_Jump_TA", "TAGame.CarComponent_TA"}
  , {"Engine.GameReplicationInfo", "Engine.ReplicationInfo"}
  , {"Engine.Info", "Engine.Actor"}
  , {"Engine.Pawn", "Engine.Actor"}
  , {"Engine.PlayerReplicationInfo", "Engine.ReplicationInfo"}
  , {"Engine.ReplicationInfo", "Engine.Info"}
  , {"Engine.TeamInfo", "Engine.ReplicationInfo"}
  , {"ProjectX.GRI_X", "Engine.GameReplicationInfo"}
  , {"ProjectX.Pawn_X", "Engine.Pawn"}
  , {"ProjectX.PRI_X", "Engine.PlayerReplicationInfo"}
  , {"TAGame.Ball_TA", "TAGame.RBActor_TA"}
  , {"TAGame.CameraSettingsActor_TA", "Engine.ReplicationInfo"}
  , {"TAGame.Car_Season_TA", "TAGame.PRI_TA"}
  , {"TAGame.Car_TA", "TAGame.Vehicle_TA"}
 
  , {"TAGame.CarComponent_TA", "Engine.ReplicationInfo"}
  , {"TAGame.CrowdActor_TA", "Engine.ReplicationInfo"}
  , {"TAGame.CrowdManager_TA", "Engine.ReplicationInfo"}
  , {"TAGame.GameEvent_Season_TA", "TAGame.GameEvent_Soccar_TA"}
  , {"TAGame.GameEvent_Soccar_TA", "TAGame.GameEvent_Team_TA"}
  , {"TAGame.GameEvent_SoccarPrivate_TA", "TAGame.GameEvent_Soccar_TA"}
  , {"TAGame.GameEvent_SoccarSplitscreen_TA", "TAGame.GameEvent_SoccarPrivate_TA"}
  , {"TAGame.GameEvent_TA", "Engine.ReplicationInfo"}
  , {"TAGame.GameEvent_Team_TA", "TAGame.GameEvent_TA"}
  , {"TAGame.GRI_TA", "ProjectX.GRI_X"}
  , {"TAGame.InMapScoreboard_TA", "Engine.Actor"}
  , {"TAGame.PRI_TA", "ProjectX.PRI_X"}
  , {"TAGame.RBActor_TA", "ProjectX.Pawn_X"}
  , {"TAGame.SpecialPickup_BallCarSpring_TA", "TAGame.SpecialPickup_Spring_TA"}
  , {"TAGame.SpecialPickup_BallFreeze_TA", "TAGame.SpecialPickup_Targeted_TA"}
  , {"TAGame.SpecialPickup_BallGravity_TA", "TAGame.SpecialPickup_TA"}
  , {"TAGame.SpecialPickup_BallLasso_TA", "TAGame.SpecialPickup_GrapplingHook_TA"}
  , {"TAGame.SpecialPickup_BallVelcro_TA", "TAGame.SpecialPickup_TA"}
  , {"TAGame.SpecialPickup_Batarang_TA", "TAGame.SpecialPickup_BallLasso_TA"}
  , {"TAGame.SpecialPickup_BoostOverride_TA", "TAGame.SpecialPickup_Targeted_TA"}
  , {"TAGame.SpecialPickup_GrapplingHook_TA", "TAGame.SpecialPickup_Targeted_TA"}
  , {"TAGame.SpecialPickup_HitForce_TA", "TAGame.SpecialPickup_TA"}
  , {"TAGame.SpecialPickup_Spring_TA", "TAGame.SpecialPickup_Targeted_TA"}
  , {"TAGame.SpecialPickup_Swapper_TA", "TAGame.SpecialPickup_Targeted_TA"}
  , {"TAGame.SpecialPickup_TA", "TAGame.CarComponent_TA"}
  , {"TAGame.SpecialPickup_Targeted_TA", "TAGame.SpecialPickup_TA"}
  , {"TAGame.SpecialPickup_Tornado_TA", "TAGame.SpecialPickup_TA"}
  , {"TAGame.Team_Soccar_TA", "TAGame.Team_TA"}
  , {"TAGame.Team_TA", "Engine.TeamInfo"}
  , {"TAGame.Vehicle_TA", "TAGame.RBActor_TA"}
  , {"TAGame.VehiclePickup_Boost_TA", "TAGame.VehiclePickup_TA"}
  , {"TAGame.VehiclePickup_TA", "Engine.ReplicationInfo"}
  , {"TAGame.SpecialPickup_HauntedBallBeam_TA", "TAGame.SpecialPickup_TA"}
  , {"TAGame.CarComponent_TA", "Engine.Actor"}
  , {"Engine.Info", "Engine.Actor"}
  , {"Engine.Pawn", "Engine.Actor"}

};

const std::vector<std::pair<std::string, std::vector<std::string>>> archetypeMap =
{
	{{"TAGame.Car_TA"}, {"Archetypes.Car.Car_Default"}},
	{{"TAGame.Car_TA"}, {"Mutators.Mutators.Mutators.FreePlay:CarArchetype"}},
	{{"TAGame.Ball_TA"},  {"Archetypes.Ball.Ball_GameEditor", "Archetypes.Ball.Ball_Training", "Archetypes.Ball.Ball_Default", "Archetypes.Ball.Ball_Basketball", "Archetypes.Ball.Ball_BasketBall", "Archetypes.Ball.Ball_BasketBall_Mutator", "Archetypes.Ball.Ball_Puck", "Archetypes.Ball.CubeBall", "Archetypes.Ball.Ball_Beachball"}},
	{{"TAGame.Ball_Breakout_TA"}, {"Archetypes.Ball.Ball_Breakout"}},
	{{"TAGame.Ball_Trajectory_TA"},{"Archetypes.Ball.Ball_Trajectory"}},
	{{"TAGame.CarComponent_Boost_TA"}, {"Archetypes.CarComponents.CarComponent_Boost"}},
	{{"TAGame.CarComponent_Dodge_TA"}, {"Archetypes.CarComponents.CarComponent_Dodge"}},
	{{"TAGame.CarComponent_DoubleJump_TA"}, {"Archetypes.CarComponents.CarComponent_DoubleJump"}},
	{{"TAGame.CarComponent_FlipCar_TA"}, {"Archetypes.CarComponents.CarComponent_FlipCar"}},
	{{"TAGame.CarComponent_Jump_TA"}, {"Archetypes.CarComponents.CarComponent_Jump"}},
	{{"TAGame.Team_Soccar_TA"}, {"Archetypes.Teams.Team0", "Archetypes.Teams.Team1"}},
	{{"TAGame.PRI_TA"}, {"TAGame.Default__PRI_TA"}},
	{{"TAGame.GameEvent_TrainingEditor_TA"}, {"Archetypes.GameEvent.GameEvent_TrainingEditor"}},
	{{"TAGame.GameEvent_Tutorial_TA"}, {"GameInfo_Tutorial.GameEvent.GameEvent_Tutorial_Aerial"}},
	{{"TAGame.GameEvent_Soccar_TA"}, {/*"GameInfo_Tutorial.GameEvent.GameEvent_Tutorial_Aerial",*/ "Archetypes.GameEvent.GameEvent_Basketball", "Archetypes.GameEvent.GameEvent_Hockey", "Archetypes.GameEvent.GameEvent_Soccar", "Archetypes.GameEvent.GameEvent_Items", "Archetypes.GameEvent.GameEvent_SoccarLan"}},
	{{"TAGame.GameEvent_SoccarPrivate_TA"}, {"Archetypes.GameEvent.GameEvent_SoccarPrivate", "Archetypes.GameEvent.GameEvent_BasketballPrivate", "Archetypes.GameEvent.GameEvent_HockeyPrivate"}},
	{{"TAGame.GameEvent_SoccarSplitscreen_TA"}, {"Archetypes.GameEvent.GameEvent_SoccarSplitscreen", "Archetypes.GameEvent.GameEvent_BasketballSplitscreen", "Archetypes.GameEvent.GameEvent_HockeySplitscreen"}},
	{{"TAGame.GameEvent_Season_TA"}, {"Archetypes.GameEvent.GameEvent_Season"}},
	{{"TAGame.Car_TA"}, {"Archetypes.GameEvent.GameEvent_Season:CarArchetype"}},
	{{"TAGame.GameEvent_Breakout_TA"}, {"Archetypes.GameEvent.GameEvent_Breakout"}},
	{{"TAGame.GRI_TA"}, {"GameInfo_TrainingEditor.GameInfo.GameInfo_TrainingEditor:GameReplicationInfoArchetype", "GameInfo_Tutorial.GameInfo.GameInfo_Tutorial:GameReplicationInfoArchetype", "GameInfo_Basketball.GameInfo.GameInfo_Basketball:GameReplicationInfoArchetype", "Gameinfo_Hockey.GameInfo.Gameinfo_Hockey:GameReplicationInfoArchetype", "GameInfo_Season.GameInfo.GameInfo_Season:GameReplicationInfoArchetype", "GameInfo_Soccar.GameInfo.GameInfo_Soccar:GameReplicationInfoArchetype", "GameInfo_Items.GameInfo.GameInfo_Items:GameReplicationInfoArchetype", "GameInfo_Breakout.GameInfo.GameInfo_Breakout:GameReplicationInfoArchetype"}},
	{{"TAGame.CameraSettingsActor_TA"}, {"TAGame.Default__CameraSettingsActor_TA"}},
	{{"TAGame.InMapScoreboard_TA"}, {"Neotokyo_p.TheWorld:PersistentLevel.InMapScoreboard_TA_0", "NeoTokyo_P.TheWorld:PersistentLevel.InMapScoreboard_TA_0", "NeoTokyo_P.TheWorld:PersistentLevel.InMapScoreboard_TA_1", "NeoTokyo_Standard_P.TheWorld:PersistentLevel.InMapScoreboard_TA_1", "NeoTokyo_Standard_P.TheWorld:PersistentLevel.InMapScoreboard_TA_0", "Neotokyo_p.TheWorld:PersistentLevel.InMapScoreboard_TA_1"}},
	{{"TAGame.SpecialPickup_BallGravity_TA"}, {"Archetypes.SpecialPickups.SpecialPickup_GravityWell"}},
	{{"TAGame.SpecialPickup_BallVelcro_TA"}, {"Archetypes.SpecialPickups.SpecialPickup_BallVelcro"}},
	{{"TAGame.SpecialPickup_BallLasso_TA"}, {"Archetypes.SpecialPickups.SpecialPickup_BallLasso"}},
	{{"TAGame.SpecialPickup_GrapplingHook_TA"}, {"Archetypes.SpecialPickups.SpecialPickup_BallGrapplingHook"}},
	{{"TAGame.SpecialPickup_Swapper_TA"}, {"Archetypes.SpecialPickups.SpecialPickup_Swapper"}},
	{{"TAGame.SpecialPickup_BallFreeze_TA"}, {"Archetypes.SpecialPickups.SpecialPickup_BallFreeze"}},
	{{"TAGame.SpecialPickup_BoostOverride_TA"}, {"Archetypes.SpecialPickups.SpecialPickup_BoostOverride"}},
	{{"TAGame.SpecialPickup_Tornado_TA"}, {"Archetypes.SpecialPickups.SpecialPickup_Tornado"}},
	{{"TAGame.SpecialPickup_BallCarSpring_TA"}, {"Archetypes.SpecialPickups.SpecialPickup_CarSpring", "Archetypes.SpecialPickups.SpecialPickup_BallSpring"}},
	{{"TAGame.SpecialPickup_HitForce_TA"}, {"Archetypes.SpecialPickups.SpecialPickup_StrongHit"}},
	{{"TAGame.SpecialPickup_Batarang_TA"}, {"Archetypes.SpecialPickups.SpecialPickup_Batarang"}},
	//{{"TAGame.SpecialPickup_TA"}, {"Archetypes.SpecialPickups.SpecialPickup_Swapper"}},
	//{{"TAGame.SpecialPickup_TA"}, {"Archetypes.SpecialPickups.SpecialPickup_GravityWell"}},
	//{{"TAGame.InMapScoreboard_TA"}, {"Neotokyo_p.TheWorld:PersistentLevel.InMapScoreboard_TA_1"}},
	{{"TAGame.Ball_Haunted_TA"}, {"Archetypes.Ball.Ball_Haunted"}},
	{{"TAGame.HauntedBallTrapTrigger_TA"}, {"Haunted_TrainStation_P.TheWorld:PersistentLevel.HauntedBallTrapTrigger_TA_1", "Haunted_TrainStation_P.TheWorld:PersistentLevel.HauntedBallTrapTrigger_TA_0"}},
	{{"TAGame.SpecialPickup_HauntedBallBeam_TA"}, {"Archetypes.SpecialPickups.SpecialPickup_HauntedBallBeam"}},
	{{"TAGame.SpecialPickup_Rugby_TA"}, {"Archetypes.SpecialPickups.SpecialPickup_Rugby"}},
	{{"TAGame.Cannon_TA"},  {"Archetypes.Tutorial.Cannon"}}
};

void ReplayFile::FixParents()
{

	for (uint32_t i = 0; i < replayFile->classnets.size(); ++i)
	{
		const uint32_t index = replayFile->classnets.at(i)->index;
		const std::string objectName = replayFile->objects.at(index);
		classnetMap[objectName] = replayFile->classnets.at(i);
	}
	this->MergeDuplicates();


	for (auto& archetypeMapping : archetypeMap)
	{
		const auto found = classnetMap.find(archetypeMapping.first);
		if (found == classnetMap.end())
		{
			//printf("%s not found\n", archetypeMapping.first.c_str());
			continue;
		}
		std::shared_ptr<ClassNet>& headClassnet = found->second;
		for (auto& archetype : archetypeMapping.second)
		{
			classnetMap[archetype] = headClassnet;
		}
	}

	
	for (auto kv : class_extensions)
	{
		std::shared_ptr<ClassNet> childClass = GetClassnetByNameWithLookup(kv.first);
		std::shared_ptr<ClassNet> parentClass = GetClassnetByNameWithLookup(kv.second);
		if (parentClass != nullptr && childClass != nullptr && (childClass->parent_class == nullptr || (childClass->parent_class->index != parentClass->index)))
		{
			childClass->parent_class = parentClass;
		}
	}
	for (auto cn : replayFile->classnets)
	{
		uint32_t i = 0;
		uint32_t result = GetPropertyIndexById(cn, i);
		while (result != 0)
		{
			cn->property_id_cache.push_back(result);
			result = GetPropertyIndexById(cn, ++i);
		}
	}

}

uint32_t val = 0;
static uint32_t i = 0;
void ReplayFile::Parse(const std::string& fileName, const uint32_t startPos, int32_t endPos, const uint32_t frameCount)
{
	if (endPos < 0)
	{
		endPos = replayFile->netstream_size  * 8;// / 4 * 8;
	}

	const uint32_t numFrames = frameCount > 0 ? frameCount : static_cast<uint32_t>(GetProperty<int32_t>("NumFrames"));
	
	//Divide by 4 since netstream_data is bytes, but we read uint32_ts
	CPPBitReader<uint32_t> networkReader((uint32_t*)(replayFile->netstream_data), ((uint32_t)endPos), replayFile);

	++val;
	//FILE* fp = fopen(("./json/" + fileName + ".json").c_str(), "wb");

	try 
	{
		if (fileName.compare("000900040016000B001C02990682C5D1.replay") == 0)
		{
			int fdsdf = 5;
		}
		//char writeBuffer[65536 * 5];
		//rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

		rapidjson::Writer<rapidjson::FileWriteStream> writer;
		int t = 0;
		std::unordered_map<uint32_t, std::string> test;
		
		networkReader.skip(startPos);

	 	//writer.StartObject();
	 	//writer.String("frames");
		const int32_t maxChannels = GetProperty<int32_t>("MaxChannels");
		const bool isLan = GetProperty<std::string>("MatchType").compare("Lan") == 0;
		
	 	//writer.StartArray();
		uint32_t currentFrame = 0;
		while (networkReader.canRead() && currentFrame < numFrames)
		{
		 	//writer.StartObject();
			//printf("Parsing frame no %i\n", i);
			currentFrame++;
			Frame f;
			f.time = networkReader.read<float>();
			f.delta = networkReader.read<float>();
			if (f.time < 0 || f.delta < 0
				|| (f.time > 0 && f.time < 1E-10)
				|| (f.delta > 0 && f.delta < 1E-10))
			{
				//printf("Out of range, calling again\n");
				throw GeneralParseException("Frame time incorrect (parser at wrong position)", networkReader);
				
			}
		 	//writer.String("time");
		 	//writer.Double(f.time);
		 	//writer.String("delta");
		 	//writer.Double(f.delta);
			int k = 5;

		 	//writer.String("actors");
		 	//writer.StartArray();
			//While there are actors in buffer (this frame)
			while (networkReader.read<bool>())
			{
			 	//writer.StartObject();
				const uint32_t actorId = networkReader.readBitsMax<uint32_t>(maxChannels);
				//parseMutex.lock();
				ActorState& actorState = actorStates[actorId];
				//parseMutex.unlock();
			 	//writer.String("actorid");
			 	//writer.Uint(actorId);

			 	//writer.String("status");
				if (networkReader.read<bool>())
				{

					//Is new state
					if (networkReader.read<bool>())
					{
					 	//writer.String("created");
						if (replayFile->header.engineVersion > 868 || (replayFile->header.engineVersion == 868 && replayFile->header.licenseeVersion >= 20) || (replayFile->header.engineVersion == 868 && replayFile->header.licenseeVersion >= 14 && !isLan))
						{
							const uint32_t nameId = networkReader.read<uint32_t>();
							actorState.name_id = nameId;

							if (nameId > replayFile->names.size())
							{
								throw GeneralParseException("nameId not in replayFile->objects " + std::to_string(nameId) + " > " + std::to_string(replayFile->names.size()), networkReader);
							}

						 	//writer.String("nameid");
						 	//writer.Uint(actorState.name_id);
						}
						else
						{ 
							actorState.name_id = 0;
						}
						const bool unknownBool = networkReader.read<bool>();
						const uint32_t typeId = networkReader.read<uint32_t>();
					 	//writer.String("typeid");
					 	//writer.Uint(typeId);
						//const uint32_t bit_pos = networkReader.GetAbsoluteBitPosition();

						if (typeId > replayFile->objects.size())
						{
							throw GeneralParseException("Typeid not in replayFile->objects " + std::to_string(typeId) + " > " + std::to_string(replayFile->objects.size()), networkReader);
						}

						const std::string typeName = replayFile->objects.at(typeId);
						//printf("New object of type %s\n", typeName.c_str());
					 	//writer.String("typename");
					 	//writer.String(typeName.c_str(), typeName.size());
						auto classNet = GetClassnetByNameWithLookup(typeName);

						if (classNet == nullptr)
						{
							throw GeneralParseException("Classnet for " + typeName + " not found", networkReader);
						}
						parseLog.push_back("New actor for " + typeName);

						actorState.classNet = classNet;

						const uint32_t classId = classNet->index;
						const std::string className = replayFile->objects.at(classId);

					 	//writer.String("classname");
					 	//writer.String(className.c_str(), className.size());

						if (HasInitialPosition(className))
						{
							actorState.position = static_cast<Vector3>(networkReader.read<Vector3I>());
						 	//writer.String("initialposition");
							//Serialize(writer, actorState.position);
						}
						if (HasRotation(className))
						{
							actorState.rotation = networkReader.read<Rotator>();
						 	//writer.String("initialrotation");
							//Serialize(writer, actorState.rotation);
						}
					}
					else //Is existing state
					{
					 	//writer.String("updated");
					 	//writer.String("updates");
					 	//writer.StartArray();
						//While there's data for this state to be updated
						while (networkReader.read<bool>())
						{
						 	//writer.StartObject();
							//if (std::string("CarComponent_Boost_TA_0").compare(replayFile->names[actorState.name_id]) == 0)
							//{
							//	int k = 5;
							//}
							const uint16_t maxPropId = GetMaxPropertyId(actorState.classNet);
							const uint32_t propertyId = networkReader.readBitsMax<uint32_t>(maxPropId + 1);
							const uint32_t propertyIndex = actorState.classNet->property_id_cache[propertyId];
							
							
							//if (replayFile->objects[propertyIndex].find("Replicated") == std::string::npos)
							//{
							//	//printf("Calling parser for %s (%i, %i, %s, %i)\n", replayFile->objects[propertyIndex].c_str(), propertyIndex, actorId, replayFile->names[actorState.name_id].c_str(), i);
							//}
							//if (i > 2092749)
							//{
							//	//printf("Calling parser for %s (%i, %i, %s, %i)\n", replayFile->objects[propertyIndex].c_str(), propertyIndex, actorId, replayFile->names[actorState.name_id].c_str(), i);
							//	int owow = 5;
							//}
						 	//writer.String("class");
						 	//writer.String(replayFile->objects[propertyIndex].c_str(), replayFile->objects[propertyIndex].size());

						 	//writer.String("data");
							//printf("Calling parse for %s", )
							if constexpr (IncludeParseLog) 
							{
								char buff[1024];
								snprintf(buff, sizeof(buff), "Calling parser for %s (%i, %i, %s, %i)\n", replayFile->objects[propertyIndex].c_str(), propertyIndex, actorId, actorState.name_id >= replayFile->names.size() ? "unknown" : replayFile->names[actorState.name_id].c_str(), i);
								parseLog.push_back(std::string(buff));
							}
							networkParser.Parse(propertyIndex, networkReader, writer);
							i++;
						 	//writer.EndObject();
						}
					 	//writer.EndArray();
					}
				}
				else
				{
				 	//writer.String("deleted");
				}
			 	//writer.EndObject();
			}
		 	//writer.EndArray();
		 	//writer.EndObject();
		}
		if (numFrames != currentFrame)
		{
			throw new GeneralParseException("Number of expected frames does not match number of parsed frames. Expected: " + std::to_string(numFrames) + ", parsed: " + std::to_string(currentFrame), networkReader);
		}
	 	//writer.EndArray();
	 	//writer.EndObject();
	}
	catch (...)
	{
		//printf("Caught ex\n");
		//Parse(fileName, startPos, endPos);
		//fclose(fp);
		throw;
	}
	
	//int ow = 5;
	//printf("%i", ow);
	//fclose(fp);
	//printf("Parsed\n");
}

const bool ReplayFile::HasInitialPosition(const std::string & name) const
{
	return !(name.compare("TAGame.CrowdActor_TA") == 0
		|| name.compare("TAGame.CrowdManager_TA") == 0
		|| name.compare("TAGame.VehiclePickup_Boost_TA") == 0
		|| name.compare("TAGame.InMapScoreboard_TA") == 0
		|| name.compare("TAGame.BreakOutActor_Platform_TA") == 0
		|| name.compare("Engine.WorldInfo") == 0
		|| name.compare("TAGame.HauntedBallTrapTrigger_TA") == 0);
}

const bool ReplayFile::HasRotation(const std::string & name) const
{
	return name.compare("TAGame.Ball_TA") == 0
		|| name.compare("TAGame.Car_TA") == 0
		|| name.compare("TAGame.Car_Season_TA") == 0
		|| name.compare("TAGame.Ball_Breakout_TA") == 0
		|| name.compare("TAGame.Ball_Haunted_TA") == 0;
}

const std::pair<const uint32_t, const KeyFrame> ReplayFile::GetNearestKeyframe(uint32_t frame) const
{
	if (replayFile->keyframes.size() == 0)
	{
		return std::make_pair<const uint32_t, KeyFrame>(0, { 0.f, 0,0 });
	}
	
	const size_t size = replayFile->keyframes.size();
	size_t currentKeyframeIndex = 0;
	for (currentKeyframeIndex; currentKeyframeIndex < size; ++currentKeyframeIndex)
	{
		if (replayFile->keyframes.at(currentKeyframeIndex).frame > frame)
		{
			break;
		}
	}
	
	const KeyFrame nearestKeyFrame = replayFile->keyframes.at(currentKeyframeIndex);
	const uint32_t frameNumber = nearestKeyFrame.frame;
	return std::make_pair(frameNumber, nearestKeyFrame);
}

const bool ReplayFile::ParseProperty(const std::shared_ptr<Property>& currentProperty)
{
	currentProperty->property_name = fullReplayBitReader.read<std::string>();
	if (currentProperty->property_name.compare("None") == 0) //We're done parsing this prop
	{
		return false;
	}
	currentProperty->property_type = fullReplayBitReader.read<std::string>();
	const uint32_t propertySize = fullReplayBitReader.read<uint32_t>();
	const uint32_t idk = fullReplayBitReader.read<uint32_t>();

	//Not sure why I'm doing these micro optimizations here, kinda hurts readability and its only like a nanosecond
	switch (currentProperty->property_type[0])
	{
	case 'N':
	{
		if (currentProperty->property_type[1] == 'o') //Type is "None"
		{
			return false;
		}
		else //Type is "Name"
		{
			currentProperty->value = fullReplayBitReader.read<std::string>();
		}
	}
		break;
	case 'I': //IntProperty
	{
		currentProperty->value = fullReplayBitReader.read<int32_t>();
	}
	break;
	case 'S': //StringProperty
	{
		currentProperty->value = fullReplayBitReader.read<std::string>();
	}
	break;
	case 'B':
	{
		if (currentProperty->property_type[1] == 'y') //Type is "ByteProperty"
		{
			EnumProperty ep;
			ep.type = fullReplayBitReader.read<std::string>();
			if(ep.type.compare("OnlinePlatform_Steam") == 0 || ep.type.compare("OnlinePlatform_PS4") == 0) //for some reason if string is this, there's no value.
			{ 
				ep.value = "";	
				//fullReplayBitReader.read<uint32_t>();
			}
			else
			{
				ep.value = fullReplayBitReader.read<std::string>(); //Value
			}
			currentProperty->value = ep;
		}
		else //Type is "BoolProperty", but unlike network data, is stored as entire byte
		{
			if (replayFile->header.engineVersion == 0 &&
				replayFile->header.licenseeVersion == 0 &&
				replayFile->header.netVersion == 0) 
			{
				currentProperty->value = fullReplayBitReader.read<uint32_t>();
			}
			else
			{
				currentProperty->value = fullReplayBitReader.read<uint8_t>();
			}
			
		}
	}
	break;
	case 'Q': //QWordProperty
	{
		currentProperty->value = fullReplayBitReader.read<uint64_t>();
	}
	break;
	case 'F': //FloatProperty
	{
		currentProperty->value = fullReplayBitReader.read<float>();
	}
	break;
	case 'A': //ArrayProperty
	{
		const int32_t count = fullReplayBitReader.read<int32_t>();
		std::vector<std::unordered_map<std::string, std::shared_ptr<Property>>> properties;
		properties.resize(count);
		
		for (int32_t i = 0; i < count; ++i) 
		{
			std::unordered_map<std::string, std::shared_ptr<Property>> props;
			while (true) 
			{
				auto baseProperty = std::make_shared<Property>();
				const bool moreToParse = ParseProperty(baseProperty);
				if (!moreToParse)
				{
					break;
				}
				//printf("%s", baseProperty->property_name.c_str());
				props[baseProperty->property_name] = baseProperty;
			}
			properties[i] = props;
		}

		/*for (int32_t i = 0; i < count; ++i)
		{
			std::shared_ptr<Property> prop = std::make_shared<Property>();
			ParseProperty(prop);
			properties[i] = prop;
		}*/
		currentProperty->value = properties;
	}
	break;
	default: //Die
		//assert(1 == 2);
		break;
	}
	
	return true;
}

#define fffffind(a)\
auto found = classnetMap.find(a);\
if (found == classnetMap.end()) {\
return notfound;\
}\
return (*found).second;

const std::shared_ptr<ClassNet>& ReplayFile::GetClassnetByNameWithLookup(const std::string & name) const
{
	static std::shared_ptr<ClassNet> notfound = std::shared_ptr<ClassNet>(nullptr);
	if (name.find("CrowdActor_TA") != std::string::npos)
	{
		fffffind("TAGame.CrowdActor_TA");
	}
	else if (name.find("VehiclePickup_Boost_TA") != std::string::npos)
	{
		fffffind("TAGame.VehiclePickup_Boost_TA");
	}
	else if (name.find("CrowdManager_TA") != std::string::npos)
	{
		fffffind("TAGame.CrowdManager_TA");
	}
	else if (name.find("BreakOutActor_Platform_TA") != std::string::npos)
	{
		fffffind("TAGame.BreakOutActor_Platform_TA");
	}
	else if (name.find("WorldInfo") != std::string::npos)
	{
		fffffind("Engine.WorldInfo");
	}
	else if (name.find("Archetypes.Teams.TeamWhite") != std::string::npos)
	{
		fffffind("TAGame.Team_Soccar_TA");
	}
	auto found = classnetMap.find(name);
	if (found == classnetMap.end())
		return notfound;
	return found->second;
}

const uint16_t ReplayFile::GetPropertyIndexById(const std::shared_ptr<ClassNet>& cn, const int id) const
{
	for (int i = 0; i < cn->prop_indexes_size; i++)
	{
		if (cn->prop_indexes[i].prop_id == id)
		{
			return cn->prop_indexes[i].prop_index;
		}
	}
	if (cn->parent_class)
	{
		const std::shared_ptr<ClassNet>& parentNet = cn->parent_class;
		if (parentNet == NULL) //Is root?
		{
			return cn->index;
		}
		return this->GetPropertyIndexById(parentNet, id);
	}
	return 0;
}

const uint16_t ReplayFile::GetMaxPropertyId(const std::shared_ptr<ClassNet>& cn)
{
	if (cn == nullptr)
	{
		throw std::exception("ClassNet is nullptr");
	}
		
	if (cn->max_prop_id == 0)
	{
		cn->max_prop_id = FindMaxPropertyId(cn, 0);
	}
	return cn->max_prop_id;
}

const uint16_t ReplayFile::FindMaxPropertyId(const std::shared_ptr<ClassNet>& cn, uint16_t maxProp) const
{
	if (cn == nullptr)
	{
		return maxProp;
	}

	for (int32_t i = 0; i < cn->prop_indexes_size; ++i)
	{
		if (cn->prop_indexes[i].prop_id > maxProp)
		{
			maxProp = cn->prop_indexes[i].prop_id;
		}
	}
	if (cn->parent_class)
	{
		return FindMaxPropertyId(cn->parent_class, maxProp);
	}
	return maxProp;
}
