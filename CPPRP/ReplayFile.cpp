#include "ReplayFile.h"
#include <fstream>
#include "./data/NetworkData.h"
#include "CRC.h"
#include <functional>
#include "./data/GameClasses.h"
namespace CPPRP
{
	constexpr bool IncludeParseLog = false;
	ReplayFile::ReplayFile(std::filesystem::path path_) : path(path_)
	{

	}


	ReplayFile::~ReplayFile()
	{
	}

	const bool ReplayFile::Load()
	{
		if (!std::filesystem::exists(path))
			return false;
		std::ifstream file(path, std::ios::binary | std::ios::ate);

		const std::streamsize size = file.tellg();
		data.resize((size_t)size);
		file.seekg(0, std::ios::beg);

		if (file.bad())
			return false;
		return (bool)file.read(data.data(), size);
	}

	void ReplayFile::DeserializeHeader()
	{
		const size_t dataSizeBits = data.size() * 8;
		replayFile = std::make_shared<ReplayFileData>();
		fullReplayBitReader = std::make_shared<CPPBitReader<BitReaderType>>((const BitReaderType*)data.data(), dataSizeBits, replayFile);

		replayFile->header = {
			fullReplayBitReader->read<uint32_t>(),	//Size
			fullReplayBitReader->read<uint32_t>(),	//CRC
			fullReplayBitReader->read<uint32_t>(),	//engineVersion
			fullReplayBitReader->read<uint32_t>()	//licenseeVersion
		};

		if (replayFile->header.engineVersion >= 868 && replayFile->header.licenseeVersion >= 18)
		{
			replayFile->header.netVersion = fullReplayBitReader->read<uint32_t>();
		}

		//Reconstruct cause we got version info now,  find something better for this
		size_t bitPos = fullReplayBitReader->GetAbsoluteBitPosition();
		fullReplayBitReader = std::make_shared<CPPBitReader<BitReaderType>>((const BitReaderType*)data.data(), dataSizeBits, replayFile);
		fullReplayBitReader->skip(bitPos);

		replayFile->replayType = fullReplayBitReader->read<std::string>(); //Not sure what this is


		while (true) 
		{
			auto baseProperty = std::make_shared<Property>();
			const bool moreToParse = ParseProperty(baseProperty);
			if (!moreToParse)
			{
				break;
			}
			replayFile->properties[baseProperty->property_name] = baseProperty;
		}

		replayFile->body_size = fullReplayBitReader->read<uint32_t>();
		replayFile->crc2 = fullReplayBitReader->read<uint32_t>();

		const uint32_t levelCount = fullReplayBitReader->read<uint32_t>();
		for (uint32_t i = 0; i < levelCount; ++i)
		{
			replayFile->levels.push_back(fullReplayBitReader->read<std::string>());
		}

		const uint32_t keyframeCount = fullReplayBitReader->read<uint32_t>();
		for (uint32_t i = 0; i < keyframeCount; ++i)
		{
			replayFile->keyframes.push_back(
				{
					fullReplayBitReader->read<float>(),		//Time
					fullReplayBitReader->read<uint32_t>(),	//Frame
					fullReplayBitReader->read<uint32_t>()	//File position
				});
		}

		const uint32_t netstreamCount = static_cast<uint32_t>(fullReplayBitReader->read<int32_t>());
		replayFile->netstream_data = data.data() + fullReplayBitReader->GetAbsoluteBytePosition(); //We know this is always aligned, so valid
		uint32_t test = netstreamCount * 8;
		fullReplayBitReader->skip(test);
		replayFile->netstream_size = netstreamCount;

		if (!fullReplayBitReader->canRead())
		{
			//Replay is corrupt
			throw GeneralParseException("ReplayFile corrupt. header + netstream_size > filesize", *fullReplayBitReader);
		}

		const int32_t debugStringSize = fullReplayBitReader->read<int32_t>();
		for (int32_t i = 0; i < debugStringSize; ++i)
		{
			uint32_t frame = fullReplayBitReader->read<uint32_t>();
			std::string key = fullReplayBitReader->read<std::string>();
			std::string value = fullReplayBitReader->read<std::string>();
			///printf("%s = %s", key.c_str(), value.c_str());
		}

		const uint32_t replayTickCount = fullReplayBitReader->read<uint32_t>();
		for (uint32_t i = 0; i < replayTickCount; ++i)
		{
			replayFile->replayticks.push_back(
				{
					fullReplayBitReader->read<std::string>(),	//Type
					fullReplayBitReader->read<uint32_t>()		//Frame
				});
		}


		const uint32_t replicatedPackagesCount = fullReplayBitReader->read<uint32_t>();
		for (uint32_t i = 0; i < replicatedPackagesCount; ++i)
		{
			replayFile->replicated_packages.push_back(
				{
					fullReplayBitReader->read<std::string>()
				});
		}

		const uint32_t objectsCount = fullReplayBitReader->read<uint32_t>();
		for (uint32_t i = 0; i < objectsCount; ++i)
		{
			replayFile->objects.push_back(
				{
					fullReplayBitReader->read<std::string>()
				});
		}

		const uint32_t namesCount = fullReplayBitReader->read<uint32_t>();
		for (uint32_t i = 0; i < namesCount; ++i)
		{
			replayFile->names.push_back(
				{
					fullReplayBitReader->read<std::string>()
				});
		}

		const uint32_t classIndexCount = fullReplayBitReader->read<uint32_t>();
		for (uint32_t i = 0; i < classIndexCount; ++i)
		{
			replayFile->class_indices.push_back(
				{
					fullReplayBitReader->read<std::string>(),	//Class_name
					fullReplayBitReader->read<uint32_t>()		//Index
				});
		}

		const uint32_t classNetsCount = fullReplayBitReader->read<uint32_t>();
		replayFile->classnets.resize(classNetsCount);
		for (uint32_t i = 0; i < classNetsCount; ++i)
		{
			ClassNet cn = {
				fullReplayBitReader->read<int32_t>(),		//Index
				fullReplayBitReader->read<int32_t>(),		//Parent
				NULL,							//Parent class, not known yet
				fullReplayBitReader->read<int32_t>(),		//Id
				fullReplayBitReader->read<int32_t>(),		//Prop_indexes_size
				std::vector<PropIndexId>(),		//Empty propindexid array
				0,								//Max_prop_id
				std::vector<uint16_t>()			//Property_id_cache
			};

			const uint32_t newSize = cn.prop_indexes_size;
			cn.prop_indexes.resize(newSize);
			for (uint32_t j = 0; j < newSize; ++j)
			{
				cn.prop_indexes[j] = (
					PropIndexId{
						fullReplayBitReader->read<int32_t>(),	//Prop_index
						fullReplayBitReader->read<int32_t>()	//Prop_id
					});
			}
			std::shared_ptr<ClassNet> classNet = std::make_shared<ClassNet>(cn);
			replayFile->classnets[i] = (classNet);

			//Set parent class if exists
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
			fullReplayBitReader->read<int32_t>();
		}
		this->FixParents();

	}

	const bool ReplayFile::VerifyCRC(CrcCheck verifyWhat)
	{
		if ((verifyWhat & CRC_Both) == 0) return false; //User supplied invalid value, < 0 or >= 4

		const size_t dataSizeBits = data.size() * 8;
		//Replay not loaded, less than 8 bytes
		if (dataSizeBits < sizeof(uint32_t) * 2 * 8)
		{
			return false;
		}
		CPPBitReader<BitReaderType> bitReader((const BitReaderType*)data.data(), 
			dataSizeBits, replayFile, 0, 0, 0);
		const uint32_t headerSize = bitReader.read<uint32_t>();
		const uint32_t headerReadCrc = bitReader.read<uint32_t>();

		//File is lying about its size
		if (bitReader.GetAbsoluteBytePosition() + headerSize > data.size())
		{
			return false;
		}

		constexpr uint32_t CRC_SEED = 0xEFCBF201;
		if (verifyWhat & CRC_Header)
		{
			const uint32_t headerCalculatedCRC = CalculateCRC(data,
				static_cast<size_t>(bitReader.GetAbsoluteBytePosition()), 
				static_cast<size_t>(headerSize), CRC_SEED);
			const bool result = headerCalculatedCRC == headerReadCrc;
			//If only verify header, or if already failed here
			if (!(verifyWhat & CRC_Body) || !result)
			{
				return result;
			}
		}
		bitReader.skip(headerSize * 8);

		if (bitReader.GetAbsoluteBytePosition() + 2 > data.size())
		{
			//Won't be able to read body size and crc, so false
			return false;
		}


		const uint32_t bodySize = bitReader.read<uint32_t>();
		const uint32_t bodyReadCrc = bitReader.read<uint32_t>();
		if (bitReader.GetAbsoluteBytePosition() + bodySize > data.size())
		{
			return false;
		}

		const uint32_t bodyCalculatedCRC = CalculateCRC(data, 
			static_cast<size_t>(bitReader.GetAbsoluteBytePosition()), 
			static_cast<size_t>(bodySize), CRC_SEED);
		return bodyReadCrc == bodyCalculatedCRC;
	}

	void ReplayFile::PreprocessTables()
	{
		const size_t size = replayFile->objects.size();
		for(size_t i = 0; i < size; ++i)
		{
			objectToId[replayFile->objects.at(i)] = (uint32_t)i;
			//printf("[%i] %s", i, replayFile->objects.at(i).c_str());
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
		if (counts.size() != classnetMap.size())
		{
			throw GeneralParseException("Found duplicate class!", *fullReplayBitReader);
		}
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
	  , {"ProjectX.NetModeReplicator_X", "Engine.ReplicationInfo"}
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
		{{"TAGame.Ball_TA"}, { "Archetypes.Ball.Ball_Training"}},
		{{"TAGame.Ball_TA"},  {"Archetypes.Ball.Ball_GameEditor", "Archetypes.Ball.Ball_Default", "Archetypes.Ball.Ball_Basketball", "Archetypes.Ball.Ball_BasketBall", "Archetypes.Ball.Ball_BasketBall_Mutator", "Archetypes.Ball.Ball_Puck", "Archetypes.Ball.CubeBall", "Archetypes.Ball.Ball_Beachball"}},
		{{"TAGame.Ball_Breakout_TA"}, {"Archetypes.Ball.Ball_Breakout"}},
		{{"TAGame.Ball_TA"},{"Archetypes.Ball.Ball_Trajectory"}},
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
		{{"TAGame.Cannon_TA"},  {"Archetypes.Tutorial.Cannon"}},
		{{"ProjectX.NetModeReplicator_X"}, {"ProjectX.Default__NetModeReplicator_X"}}
		
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
				continue;
			}
			std::shared_ptr<ClassNet>& headClassnet = found->second;
			for (auto& archetype : archetypeMapping.second)
			{
				classnetMap[archetype] = headClassnet;
			}
		}

		//TODO: derive this from gameclasses
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

		

		const size_t objectsSize = replayFile->objects.size();
		parseFunctions.resize(objectsSize);
		createFunctions.resize(objectsSize);
		for(size_t i = 0; i < objectsSize; i++)
		{
			const std::string& name = replayFile->objects.at(i);
			auto found = parsePropertyFuncs.find(name);
			if(found != parsePropertyFuncs.end())
			{
				parseFunctions[i] = found->second;
			}

			auto found2 = createObjectFuncs.find(name);
			if(found2 != createObjectFuncs.end())
			{
				createFunctions[i] = found2->second;
			}
		}


	}

	void ReplayFile::Parse(const uint32_t startPos, int32_t endPos, const uint32_t frameCount)
	{
		/*
		Replay is corrupt, no way we'll parse this correctly
		Parsing header is fine though, so only throw this in parse
		*/
		if (replayFile->header.engineVersion == 0 &&
			replayFile->header.licenseeVersion == 0 &&
			replayFile->header.netVersion == 0)
		{
			throw InvalidVersionException(0, 0, 0);
		}
		if (endPos < 0)
		{
			endPos = replayFile->netstream_size * 8;
		}

		
		CPPBitReader<BitReaderType> networkReader((BitReaderType*)(replayFile->netstream_data), static_cast<size_t>(endPos), replayFile);

		//FILE* fp = fopen(("./json/" + fileName + ".json").c_str(), "wb");

		//try
		{
			//char writeBuffer[65536 * 5];
			//rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

			//rapidjson::Writer<rapidjson::FileWriteStream> writer;

			networkReader.skip(startPos);

			//Get some const data we're gonna need repeatedly during parsing and store for performance reasons
			const uint32_t numFrames = frameCount > 0 ? frameCount : static_cast<uint32_t>(GetProperty<int32_t>("NumFrames"));

			const int32_t maxChannels = GetProperty<int32_t>("MaxChannels");
			const bool isLan = GetProperty<std::string>("MatchType").compare("Lan") == 0;

			const size_t namesSize = replayFile->names.size();
			const size_t objectsSize = replayFile->objects.size();

			const uint32_t engineVersion = replayFile->header.engineVersion;
			const uint32_t licenseeVersion = replayFile->header.licenseeVersion;

			frames.resize(numFrames);
			uint32_t currentFrame = 0;
			while (
				#ifndef PARSE_UNSAFE
				networkReader.canRead() && 
				#endif
				currentFrame < numFrames)
			{

				Frame& f = frames[currentFrame];
				f.position = networkReader.GetAbsoluteBitPosition();
				f.time = networkReader.read<float>();
				f.delta = networkReader.read<float>();
				if constexpr (IncludeParseLog)
				{
					parseLog.push_back("New frame " + std::to_string(currentFrame) + " at " + std::to_string(f.time) + ", pos " + std::to_string(f.position));
				}
				#ifndef PARSE_UNSAFE
				if (f.time < 0 || f.delta < 0
					|| (f.time > 0 && f.time < 1E-10)
					|| (f.delta > 0 && f.delta < 1E-10))
				{
					throw GeneralParseException("Frame time incorrect (parser at wrong position)", networkReader);
				}
				#endif

				//While there are actors in buffer (this frame)
				while (networkReader.read<bool>())
				{
					const uint32_t actorId = networkReader.readBitsMax<uint32_t>(maxChannels);
					if (networkReader.read<bool>())
					{
						//Is new state
						if (networkReader.read<bool>())
						{
							uint32_t name_id;
							if (engineVersion > 868 || (engineVersion == 868 && licenseeVersion >= 20) || (engineVersion == 868 && licenseeVersion >= 14 && !isLan))
							{
								const uint32_t nameId = networkReader.read<uint32_t>();
								name_id = nameId;
								#ifndef PARSE_UNSAFE
								if (nameId > namesSize)
								{
									throw GeneralParseException("nameId not in replayFile->objects " + std::to_string(nameId) + " > " + std::to_string(namesSize), networkReader);
								}
								#endif
							}
							else
							{
								name_id = 0;
							}
							const bool unknownBool = networkReader.read<bool>();
							const uint32_t typeId = networkReader.read<uint32_t>();

							#ifndef PARSE_UNSAFE
							if (typeId > objectsSize)
							{
								throw GeneralParseException("Typeid not in replayFile->objects " + std::to_string(typeId) + " > " + std::to_string(objectsSize), networkReader);
							}
							#endif

							const std::string typeName = replayFile->objects.at(typeId);
							auto classNet = GetClassnetByNameWithLookup(typeName);

							#ifndef PARSE_UNSAFE
							if (classNet == nullptr)
							{
								throw GeneralParseException("Classnet for " + typeName + " not found", networkReader);
							}
							#endif



							const uint32_t classId = classNet->index;
							const std::string className = replayFile->objects.at(classId);
							//auto found = createObjectFuncs.find(className);

							const auto& funcPtr = createFunctions[classId];
							#ifndef PARSE_UNSAFE
							if (funcPtr == nullptr)
							{
								std::cout << "Could not find class " << className << "\n";
								throw GeneralParseException("Could not find class " + className , networkReader);
								return;
							}
							#endif
							std::shared_ptr<Engine::Actor> actorObject = funcPtr();
							ActorStateData asd = { actorObject, classNet, actorId, name_id };
							if constexpr (IncludeParseLog)
							{
								parseLog.push_back("New actor for " + typeName + ", classname " + className);
							}

							if (HasInitialPosition(className))
							{
								actorObject->Location = networkReader.read<Vector3I>();
							}
							if (HasRotation(className))
							{
								actorObject->Rotation = networkReader.read<Rotator>();
							}
							actorStates[actorId] = asd;
							for(const auto& createdFunc : createdCallbacks)
							{
								createdFunc(asd);
							}
						}
						else //Is existing state
						{
							ActorStateData& actorState = actorStates[actorId];
							std::vector<uint32_t> updatedProperties;
							//While there's data for this state to be updated
							while (networkReader.read<bool>())
							{
								const uint16_t maxPropId = GetMaxPropertyId(actorState.classNet);
								const uint32_t propertyId = networkReader.readBitsMax<uint32_t>(maxPropId + 1);
								const uint32_t propertyIndex = actorState.classNet->property_id_cache[propertyId];

								if constexpr (IncludeParseLog)
								{
									char buff[1024];
									snprintf(buff, sizeof(buff), "Calling parser for %s (%i, %i, %s)", replayFile->objects[propertyIndex].c_str(), propertyIndex, actorId, actorState.nameId >= namesSize ? "unknown" : replayFile->names[actorState.nameId].c_str());
									parseLog.push_back(std::string(buff));
								}
								updatedProperties.push_back(propertyIndex);
								const auto& funcPtr = parseFunctions[propertyIndex];
								
								#ifndef PARSE_UNSAFE
								if(funcPtr == nullptr)
								{
									const std::string& objName = replayFile->objects[propertyIndex];
									std::cout << "Property " << objName << " is undefined\n";

									throw GeneralParseException("Property " + objName + " is undefined", networkReader);

								}
								#endif
								funcPtr(actorState.actorObject, networkReader);
							}
							for(const auto& updateFunc : updatedCallbacks)
							{
								updateFunc(actorState, updatedProperties);
							}
						}
					}
					else
					{
						ActorStateData& actorState = actorStates[actorId];
						for(const auto& deleteFunc : actorDeleteCallbacks)
						{
							deleteFunc(actorState);
						}
						actorStates.erase(actorId);
					}
				}
				for (const auto& tick : tickables)
				{
					tick(currentFrame, actorStates);
				}
				currentFrame++;
			}
			if (numFrames != currentFrame)
			{
				throw GeneralParseException("Number of expected frames does not match number of parsed frames. Expected: " + std::to_string(numFrames) + ", parsed: " + std::to_string(currentFrame), networkReader);
			}
			if (networkReader.size - networkReader.GetAbsoluteBitPosition() > 8192)
			{
				//Unsure how big RL buffer sizes are, 8192 seems fair
				throw GeneralParseException("Not enough bytes parsed! Expected ~" + std::to_string(networkReader.size) + ", parsed: " + std::to_string(networkReader.GetAbsoluteBitPosition()) + ". Diff(" + std::to_string(networkReader.size - networkReader.GetAbsoluteBitPosition()) + ")", networkReader);
			}
		}
		//catch (...)
		{
			//printf("Caught ex\n");
			//Parse(fileName, startPos, endPos);
			//fclose(fp);
			//throw;
		}

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
		currentProperty->property_name = fullReplayBitReader->read<std::string>();
		if (currentProperty->property_name.compare("None") == 0) //We're done parsing this prop
		{
			return false;
		}
		currentProperty->property_type = fullReplayBitReader->read<std::string>();
		const uint32_t propertySize = fullReplayBitReader->read<uint32_t>();
		const uint32_t idk = fullReplayBitReader->read<uint32_t>();

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
				currentProperty->value = fullReplayBitReader->read<std::string>();
			}
		}
		break;
		case 'I': //IntProperty
		{
			currentProperty->value = fullReplayBitReader->read<int32_t>();
		}
		break;
		case 'S': //StringProperty
		{
			currentProperty->value = fullReplayBitReader->read<std::string>();
		}
		break;
		case 'B':
		{
			if (currentProperty->property_type[1] == 'y') //Type is "ByteProperty"
			{
				EnumProperty ep;
				ep.type = fullReplayBitReader->read<std::string>();
				if (ep.type.compare("OnlinePlatform_Steam") == 0 || ep.type.compare("OnlinePlatform_PS4") == 0) //for some reason if string is this, there's no value.
				{
					ep.value = "";
					//fullReplayBitReader->read<uint32_t>();
				}
				else
				{
					ep.value = fullReplayBitReader->read<std::string>(); //Value
				}
				currentProperty->value = ep;
			}
			else //Type is "BoolProperty", but unlike network data, is stored as entire byte
			{
				if (replayFile->header.engineVersion == 0 &&
					replayFile->header.licenseeVersion == 0 &&
					replayFile->header.netVersion == 0)
				{
					currentProperty->value = fullReplayBitReader->read<uint32_t>();
				}
				else
				{
					currentProperty->value = fullReplayBitReader->read<uint8_t>();
				}

			}
		}
		break;
		case 'Q': //QWordProperty
		{
			currentProperty->value = fullReplayBitReader->read<uint64_t>();
		}
		break;
		case 'F': //FloatProperty
		{
			currentProperty->value = fullReplayBitReader->read<float>();
		}
		break;
		case 'A': //ArrayProperty
		{
			const int32_t count = fullReplayBitReader->read<int32_t>();
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
					props[baseProperty->property_name] = baseProperty;
				}
				properties[i] = props;
			}
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
if (found == classnetMap.end()) \
{\
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
			throw std::runtime_error("ClassNet is nullptr");
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

	const bool ReplayFile::HasProperty(const std::string & key) const
	{
		return replayFile->properties.find(key) != replayFile->properties.end();
	}
}
