#include "ReplayFile.h"
#include <fstream>
#include "networkdata.h"

ReplayFile::ReplayFile(std::filesystem::path path_) : path(path_)
{
	networkParser.RegisterParsers();
}


ReplayFile::~ReplayFile()
{
}

bool ReplayFile::Load()
{
	if (!std::filesystem::exists(path))
		return false;
	std::ifstream file(path, std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	data.reserve((size_t)size);
	file.seekg(0, std::ios::beg);
	if (file.bad())
		return false;
	 return (bool)file.read(data.data(), size);
}

void ReplayFile::DeserializeHeader()
{
	const uint32_t dataSize = data.size() / 4;
	replayFile = std::make_shared<ReplayFileData>();
	fullReplayBitReader = CPPBitReader<uint32_t>((const uint32_t*)data.data(), dataSize, replayFile); //They're read as bytes, since we're retrieving them in memory as uint32_t, divide size by 4 (bytes)
	//replayFile->header.crc = fullReplayBitReader.read<uint32_t>();
	replayFile->header = {
		fullReplayBitReader.read<uint32_t>(),	//Size
		fullReplayBitReader.read<uint32_t>(),	//CRC
		fullReplayBitReader.read<uint32_t>(),	//version1
		fullReplayBitReader.read<uint32_t>()	//Version2
	};
	if (replayFile->header.version1 >= 868 && replayFile->header.version2 >= 18)
	{
		fullReplayBitReader.skip(4*8);
	}
	std::string replayType = fullReplayBitReader.read<std::string>(); //Not sure what this is


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

	const uint32_t netstreamCount = fullReplayBitReader.read<uint32_t>();
	replayFile->netstream_data = data.data() + fullReplayBitReader.GetBytePosition(); //We know this is always aligned, so valid
	fullReplayBitReader.skip(netstreamCount*8); //TODO: implement proper skip
	replayFile->netstream_size = netstreamCount;

	fullReplayBitReader.skip(4*8); //debug_log apparently

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
		for (int32_t k = (int32_t)i - 1; k >= 0; --k)
		{
			if (replayFile->classnets[i]->parent == replayFile->classnets[k]->id)
			{
				replayFile->classnets[i]->parent_class = replayFile->classnets[k];
				break;
			}
		}
	}
}

const std::unordered_map<std::string, std::string> class_extensions = 
{
	{"ProjectX.PRI_X", "Engine.PlayerReplicationInfo"},
	{"TAGame.PRI_TA", "ProjectX.PRI_X"},
	{"TAGame.CarComponent_Boost_TA", "TAGame.CarComponent_TA"},
	{"TAGame.CarComponent_FlipCar_TA", "TAGame.CarComponent_TA"},
	{"TAGame.CarComponent_Jump_TA", "TAGame.CarComponent_TA"},
	{"TAGame.CarComponent_Dodge_TA", "TAGame.CarComponent_TA"},
	{"TAGame.CarComponent_DoubleJump_TA", "TAGame.CarComponent_TA"},
	{"TAGame.GameEvent_TA", "Engine.Actor"},
	{"TAGame.SpecialPickup_TA", "TAGame.CarComponent_TA"},
	{"TAGame.SpecialPickup_BallVelcro_TA", "TAGame.SpecialPickup_TA"},
	{"TAGame.SpecialPickup_Targeted_TA", "TAGame.SpecialPickup_TA"},
	{"TAGame.SpecialPickup_Spring_TA", "TAGame.SpecialPickup_Targeted_TA"},
	{"TAGame.SpecialPickup_BallLasso_TA", "TAGame.SpecialPickup_Spring_TA"},
	{"TAGame.SpecialPickup_BoostOverride_TA", "TAGame.SpecialPickup_Targeted_TA"},
	{"TAGame.SpecialPickup_BallCarSpring_TA", "TAGame.SpecialPickup_Spring_TA"},
	{"TAGame.SpecialPickup_BallFreeze_TA", "TAGame.SpecialPickup_Targeted_TA"},
	{"TAGame.SpecialPickup_Swapper_TA", "TAGame.SpecialPickup_Targeted_TA"},
	{"TAGame.SpecialPickup_GrapplingHook_TA", "TAGame.SpecialPickup_Targeted_TA"},
	{"TAGame.SpecialPickup_BallGravity_TA", "TAGame.SpecialPickup_TA"},
	{"TAGame.SpecialPickup_HitForce_TA", "TAGame.SpecialPickup_TA"},
	{"TAGame.SpecialPickup_Tornado_TA", "TAGame.SpecialPickup_TA"},
	{"Engine.Pawn", "Engine.Actor"},
	{"Engine.TeamInfo", "Engine.ReplicationInfo"},
	{"TAGame.Team_TA", "Engine.TeamInfo"}
};

void ReplayFile::FixParents()
{
	for (auto kv : class_extensions)
	{
		std::shared_ptr<ClassNet> clas = GetClassnetByName(kv.first);
		std::shared_ptr<ClassNet> classBase = GetClassnetByName(kv.second);
		if (clas != nullptr && classBase != nullptr && (clas->parent_class == nullptr || (clas->parent_class->id != classBase->id)))
		{
			clas->parent_class = classBase;
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

void ReplayFile::Parse()
{
	//Divide by 4 since netstream_data is bytes, but we read uint32_ts
	CPPBitReader<uint32_t> networkReader((uint32_t*)replayFile->netstream_data, replayFile->netstream_size / 4, replayFile);

	const int32_t maxChannels = GetProperty<int32_t>("MaxChannels");
	
	while (networkReader.canRead())
	{
		Frame f;
		f.time = networkReader.read<float>();
		f.delta = networkReader.read<float>();
		int k = 5;

		//While there are actors in buffer (this frame)
		while (networkReader.read<bool>())
		{
			const uint32_t actorId = networkReader.readBitsMax<uint32_t>(maxChannels);
			ActorState& actorState = actorStates[actorId];

			//Not sure, but needs to be true to proceed, probably for aligning things?
			if (networkReader.read<bool>())
			{
				//Is new state
				if (networkReader.read<bool>())
				{
					if (replayFile->header.version1 > 868 || (replayFile->header.version1 == 868 && replayFile->header.version2 >= 14))
					{
						actorState.name_id = networkReader.read<uint32_t>();
					}
					const bool unknownBool = networkReader.read<bool>();
					const uint32_t typeId = networkReader.read<uint32_t>();

					//const uint32_t bit_pos = networkReader.GetAbsoluteBitPosition();

					const std::string typeName = replayFile->objects.at(typeId);

					actorState.classNet = GetClassnetByNameWithLookup(typeName);
					
					const uint32_t classId = actorState.classNet->index;
					const std::string className = replayFile->objects.at(classId);

					if (HasInitialPosition(className))
					{
						actorState.position = static_cast<Vector3>(networkReader.read<Vector3I>());
					}
					if (HasRotation(className))
					{
						actorState.rotation = networkReader.read<Rotator>();
					}
				}
				else //Is existing state
				{
					//While there's data for this state to be updated
					while (networkReader.read<bool>())
					{
						const uint16_t maxPropId = GetMaxPropertyId(actorState.classNet);
						const uint32_t propertyId = networkReader.readBitsMax<uint32_t>(maxPropId + 1);
						const uint32_t propertyIndex = actorState.classNet->property_id_cache[propertyId];
						//printf("Calling parser for %s\n", replayFile->objects[propertyIndex].c_str());
						networkParser.Parse(replayFile->objects[propertyIndex], networkReader);
					}
				}
			}
		}
	}

}

const bool ReplayFile::HasInitialPosition(const std::string & name) const
{
	return !(name.compare("TAGame.CrowdActor_TA") == 0
		|| name.compare("TAGame.CrowdManager_TA") == 0
		|| name.compare("TAGame.VehiclePickup_Boost_TA") == 0
		|| name.compare("TAGame.InMapScoreboard_TA") == 0
		|| name.compare("TAGame.BreakOutActor_Platform_TA") == 0
		|| name.compare("Engine.WorldInfo") == 0);
}

const bool ReplayFile::HasRotation(const std::string & name) const
{
	return name.compare("TAGame.Ball_TA") == 0
		|| name.compare("TAGame.Car_TA") == 0
		|| name.compare("TAGame.Car_Season_TA") == 0
		|| name.compare("TAGame.Ball_Breakout_TA") == 0;
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
			currentProperty->value = EnumProperty
			{ 
				fullReplayBitReader.read<std::string>(),	//Type
				fullReplayBitReader.read<std::string>()	//Value
			};
		}
		else //Type is "BoolProperty", but unlike network data, is stored as entire byte
		{
			currentProperty->value = fullReplayBitReader.read<uint8_t>();
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
				props[currentProperty->property_name] = baseProperty;
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
		assert(1 == 2);
		break;
	}
	
	return true;
}

const std::shared_ptr<ClassNet>& ReplayFile::GetClassnetByName(const std::string& name)
{
	if (!classNetMapCached)
	{
		for (uint32_t i = 0; i < replayFile->classnets.size(); ++i)
		{
			const uint32_t index = replayFile->classnets.at(i)->index;
			const std::string objectName = replayFile->objects.at(index);
			if (objectName.compare(objectName) == 0)
			{
				classnetMap[objectName] = replayFile->classnets.at(i);
			}
		}

		classNetMapCached = true;
	}
	return classnetMap[name];
}

const std::shared_ptr<ClassNet>& ReplayFile::GetClassnetByNameWithLookup(const std::string & name)
{
	if (name.compare("Archetypes.Car.Car_Default") == 0) {
		return GetClassnetByName("TAGame.Car_TA");
	}
	if (name.compare("Archetypes.Ball.Ball_Default") == 0 || name.compare("Archetypes.Ball.Ball_Basketball") == 0 ||
		name.compare("Archetypes.Ball.Ball_BasketBall") == 0 || name.compare("Archetypes.Ball.Ball_BasketBall_Mutator") == 0 ||
		name.compare("Archetypes.Ball.Ball_Puck") == 0 || name.compare("Archetypes.Ball.CubeBall") == 0) {
		return GetClassnetByName("TAGame.Ball_TA");
	}
	if (name.compare("Archetypes.Ball.Ball_Breakout") == 0) {
		return GetClassnetByName("TAGame.Ball_Breakout_TA");
	}
	if (name.compare("Archetypes.CarComponents.CarComponent_Boost") == 0) {
		return GetClassnetByName("TAGame.CarComponent_Boost_TA");
	}
	if (name.compare("Archetypes.CarComponents.CarComponent_Dodge") == 0) {
		return GetClassnetByName("TAGame.CarComponent_Dodge_TA");
	}
	if (name.compare("Archetypes.CarComponents.CarComponent_DoubleJump") == 0) {
		return GetClassnetByName("TAGame.CarComponent_DoubleJump_TA");
	}
	if (name.compare("Archetypes.CarComponents.CarComponent_FlipCar") == 0) {
		return GetClassnetByName("TAGame.CarComponent_FlipCar_TA");
	}
	if (name.compare("Archetypes.CarComponents.CarComponent_Jump") == 0) {
		return GetClassnetByName("TAGame.CarComponent_Jump_TA");
	}
	if (name.compare("Archetypes.Teams.Team0") == 0 || name.compare("Archetypes.Teams.Team1") == 0) {
		return GetClassnetByName("TAGame.Team_Soccar_TA");
	}
	if (name.compare("TAGame.Default__PRI_TA") == 0) {
		return GetClassnetByName("TAGame.PRI_TA");
	}
	if (name.compare("Archetypes.GameEvent.GameEvent_Basketball") == 0 || name.compare("Archetypes.GameEvent.GameEvent_Hockey") == 0 ||
		name.compare("Archetypes.GameEvent.GameEvent_Soccar") == 0 || name.compare("Archetypes.GameEvent.GameEvent_Items") == 0 || name.compare("Archetypes.GameEvent.GameEvent_SoccarLan") == 0) {
		return GetClassnetByName("TAGame.GameEvent_Soccar_TA");
	}
	if (name.compare("Archetypes.GameEvent.GameEvent_SoccarPrivate") == 0 || name.compare("Archetypes.GameEvent.GameEvent_BasketballPrivate") == 0 || name.compare("Archetypes.GameEvent.GameEvent_HockeyPrivate") == 0) {
		return GetClassnetByName("TAGame.GameEvent_SoccarPrivate_TA");
	}
	if (name.compare("Archetypes.GameEvent.GameEvent_SoccarSplitscreen") == 0 || name.compare("Archetypes.GameEvent.GameEvent_BasketballSplitscreen") == 0 || name.compare("Archetypes.GameEvent.GameEvent_HockeySplitscreen") == 0) {
		return GetClassnetByName("TAGame.GameEvent_SoccarSplitscreen_TA");
	}
	if (name.compare("Archetypes.GameEvent.GameEvent_Season") == 0) {
		return GetClassnetByName("TAGame.GameEvent_Season_TA");
	}
	if (name.compare("Archetypes.GameEvent.GameEvent_Season:CarArchetype") == 0) {
		return GetClassnetByName("TAGame.Car_TA");
	}
	if (name.compare("Archetypes.GameEvent.GameEvent_Breakout") == 0) {
		return GetClassnetByName("TAGame.GameEvent_Breakout_TA");
	}
	if (name.compare("GameInfo_Basketball.GameInfo.GameInfo_Basketball:GameReplicationInfoArchetype") == 0 || name.compare("Gameinfo_Hockey.GameInfo.Gameinfo_Hockey:GameReplicationInfoArchetype") == 0
		|| name.compare("GameInfo_Season.GameInfo.GameInfo_Season:GameReplicationInfoArchetype") == 0 || name.compare("GameInfo_Soccar.GameInfo.GameInfo_Soccar:GameReplicationInfoArchetype") == 0
		|| name.compare("GameInfo_Items.GameInfo.GameInfo_Items:GameReplicationInfoArchetype") == 0 || name.compare("GameInfo_Breakout.GameInfo.GameInfo_Breakout:GameReplicationInfoArchetype") == 0) {
		return GetClassnetByName("TAGame.GRI_TA");
	}
	if (name.compare("TAGame.Default__CameraSettingsActor_TA") == 0) {
		return GetClassnetByName("TAGame.CameraSettingsActor_TA");
	}
	if (name.compare("Neotokyo_p.TheWorld:PersistentLevel.InMapScoreboard_TA_0") == 0 || name.compare("NeoTokyo_P.TheWorld:PersistentLevel.InMapScoreboard_TA_0") == 0 ||
		name.compare("NeoTokyo_P.TheWorld:PersistentLevel.InMapScoreboard_TA_1") == 0 || name.compare("NeoTokyo_Standard_P.TheWorld:PersistentLevel.InMapScoreboard_TA_1") == 0 || name.compare("NeoTokyo_Standard_P.TheWorld:PersistentLevel.InMapScoreboard_TA_0") == 0) {
		return GetClassnetByName("TAGame.InMapScoreboard_TA");
	}
	if (name.compare("Archetypes.SpecialPickups.SpecialPickup_GravityWell") == 0) {
		return GetClassnetByName("TAGame.SpecialPickup_BallGravity_TA");
	}
	if (name.compare("Archetypes.SpecialPickups.SpecialPickup_BallVelcro") == 0) {
		return GetClassnetByName("TAGame.SpecialPickup_BallVelcro_TA");
	}
	if (name.compare("Archetypes.SpecialPickups.SpecialPickup_BallLasso") == 0) {
		return GetClassnetByName("TAGame.SpecialPickup_BallLasso_TA");
	}
	if (name.compare("Archetypes.SpecialPickups.SpecialPickup_BallGrapplingHook") == 0) {
		return GetClassnetByName("TAGame.SpecialPickup_GrapplingHook_TA");
	}
	if (name.compare("Archetypes.SpecialPickups.SpecialPickup_Swapper") == 0) {
		return GetClassnetByName("TAGame.SpecialPickup_Swapper_TA");
	}
	if (name.compare("Archetypes.SpecialPickups.SpecialPickup_BallFreeze") == 0) {
		return GetClassnetByName("TAGame.SpecialPickup_BallFreeze_TA");
	}
	if (name.compare("Archetypes.SpecialPickups.SpecialPickup_BoostOverride") == 0) {
		return GetClassnetByName("TAGame.SpecialPickup_BoostOverride_TA");
	}
	if (name.compare("Archetypes.SpecialPickups.SpecialPickup_Tornado") == 0) {
		return GetClassnetByName("TAGame.SpecialPickup_Tornado_TA");
	}
	if (name.compare("Archetypes.SpecialPickups.SpecialPickup_CarSpring") == 0 || name.compare("Archetypes.SpecialPickups.SpecialPickup_BallSpring") == 0) {
		return GetClassnetByName("TAGame.SpecialPickup_BallCarSpring_TA");
	}
	if (name.compare("Archetypes.SpecialPickups.SpecialPickup_StrongHit") == 0) {
		return GetClassnetByName("TAGame.SpecialPickup_HitForce_TA");
	}
	if (name.compare("Archetypes.SpecialPickups.SpecialPickup_Batarang") == 0) {
		return GetClassnetByName("TAGame.SpecialPickup_Batarang_TA");
	}
	if (name.compare("Neotokyo_p.TheWorld:PersistentLevel.InMapScoreboard_TA_1") == 0) {
		return GetClassnetByName("TAGame.InMapScoreboard_TA");
	}

	if (name.find("CrowdActor_TA") != std::string::npos)
	{
		return GetClassnetByName("TAGame.CrowdActor_TA");
	}
	else if (name.find("VehiclePickup_Boost_TA") != std::string::npos)
	{
		return GetClassnetByName("TAGame.VehiclePickup_Boost_TA");
	}
	else if (name.find("CrowdManager_TA") != std::string::npos)
	{
		return GetClassnetByName("TAGame.CrowdManager_TA");
	}
	else if (name.find("BreakOutActor_Platform_TA") != std::string::npos)
	{
		return GetClassnetByName("TAGame.BreakOutActor_Platform_TA");
	}
	else if (name.find("WorldInfo") != std::string::npos)
	{
		return GetClassnetByName("Engine.WorldInfo");
	}
	else if (name.find("Archetypes.Teams.TeamWhite") != std::string::npos)
	{
		return GetClassnetByName("Engine.TeamInfo");
	}
	return GetClassnetByName(name);
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
