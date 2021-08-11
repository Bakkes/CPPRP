template<>
inline const LogoData Consume(CPPBitReader<BitReaderType>& reader) 
{
	LogoData item;
	item.swap_colors = reader.read<bool>();
	item.logo_id = reader.read<uint32_t>();
	return item;
}


template<>
inline const ActiveActor Consume(CPPBitReader<BitReaderType>& reader) 
{
	ActiveActor item;
	item.active = reader.read<bool>();
	item.actor_id = reader.read<int32_t>();
	return item;
}


template<>
inline const ObjectTarget Consume(CPPBitReader<BitReaderType>& reader) 
{
	ObjectTarget item;
	item.unknown = reader.read<bool>();
	item.object_index = reader.read<int32_t>();
	return item;
}


template<>
inline const CameraSettings Consume(CPPBitReader<BitReaderType>& reader) 
{
	CameraSettings item;
	item.FOV = reader.read<float>();
	item.height = reader.read<float>();
	item.pitch = reader.read<float>();
	item.distance = reader.read<float>();
	item.stiffness = reader.read<float>();
	item.swivelspeed = reader.read<float>();
	if(reader.engineVersion >= 868 
		&& reader.licenseeVersion >= 20) { 
		item.transitionspeed = reader.read<float>();
	} else {
		item.transitionspeed = 0;
	}
	return item;
}


template<>
inline const ReplicatedPickupData Consume(CPPBitReader<BitReaderType>& reader) 
{
	ReplicatedPickupData item;
	item.unknown1 = reader.read<bool>();
	item.actor_id = reader.read<int32_t>();
	item.picked_up = reader.read<bool>();
	return item;
}


template<>
inline const ReplicatedPickupData2 Consume(CPPBitReader<BitReaderType>& reader) 
{
	ReplicatedPickupData2 item;
	item.unknown1 = reader.read<bool>();
	item.actor_id = reader.read<int32_t>();
	item.picked_up = reader.read<uint8_t>();
	return item;
}


template<>
inline const TeamPaint Consume(CPPBitReader<BitReaderType>& reader) 
{
	TeamPaint item;
	item.team_number = reader.read<uint8_t>();
	item.team_color_id = reader.read<uint8_t>();
	item.custom_color_id = reader.read<uint8_t>();
	item.team_finish_id = reader.read<uint32_t>();
	item.custom_finish_id = reader.read<uint32_t>();
	return item;
}


template<>
inline const ReplicatedDemolish Consume(CPPBitReader<BitReaderType>& reader) 
{
	ReplicatedDemolish item;
	item.attacker_flag = reader.read<bool>();
	item.attacker_actor_id = reader.read<int32_t>();
	item.victim_flag = reader.read<bool>();
	item.victim_actor_id = reader.read<int32_t>();
	item.attacker_velocity = reader.read<Vector3>();
	item.victim_velocity = reader.read<Vector3>();
	return item;
}


template<>
inline const ReplicatedDemolish2 Consume(CPPBitReader<BitReaderType>& reader) 
{
	ReplicatedDemolish2 item;
	item.custom_demo_flag = reader.read<bool>();
	item.custom_demo_id = reader.read<int32_t>();
	item.attacker_flag = reader.read<bool>();
	item.attacker_actor_id = reader.read<int32_t>();
	item.victim_flag = reader.read<bool>();
	item.victim_actor_id = reader.read<int32_t>();
	item.attacker_velocity = reader.read<Vector3>();
	item.victim_velocity = reader.read<Vector3>();
	return item;
}


template<>
inline const DemolishDataGoalExplosion Consume(CPPBitReader<BitReaderType>& reader) 
{
	DemolishDataGoalExplosion item;
	item.goal_explosion_owner_flag = reader.read<bool>();
	item.goal_explosion_owner = reader.read<int32_t>();
	item.attacker_flag = reader.read<bool>();
	item.attacker_actor_id = reader.read<int32_t>();
	item.victim_flag = reader.read<bool>();
	item.victim_actor_id = reader.read<int32_t>();
	item.attacker_velocity = reader.read<Vector3>();
	item.victim_velocity = reader.read<Vector3>();
	return item;
}


template<>
inline const ReplicatedMusicStringer Consume(CPPBitReader<BitReaderType>& reader) 
{
	ReplicatedMusicStringer item;
	item.unknown1 = reader.read<bool>();
	item.object_index = reader.read<uint32_t>();
	item.trigger = reader.read<uint8_t>();
	return item;
}


template<>
inline const ReplicatedStateIndex Consume(CPPBitReader<BitReaderType>& reader) 
{
	ReplicatedStateIndex item;
	item.value = reader.readBitsMax<uint32_t>(140);
	return item;
}


template<>
inline const PrivateMatchSettings Consume(CPPBitReader<BitReaderType>& reader) 
{
	PrivateMatchSettings item;
	item.mutators = reader.read<std::string>();
	item.map_name = reader.read<uint32_t>();
	item.max_player_count = reader.read<uint32_t>();
	item.game_name = reader.read<std::string>();
	item.password = reader.read<std::string>();
	item.is_public = reader.read<bool>();
	return item;
}


template<>
inline const ActorBase Consume(CPPBitReader<BitReaderType>& reader) 
{
	ActorBase item;
	item.value = reader.read<uint32_t>();
	item.unknown1 = reader.read<bool>();
	item.unknown2 = reader.read<bool>();
	return item;
}


template<>
inline const Attributes Consume(CPPBitReader<BitReaderType>& reader) 
{
	Attributes item;
	item.product_attributes = ConsumeVector<AttributeType>(reader);
	return item;
}


template<>
inline const OnlineLoadout Consume(CPPBitReader<BitReaderType>& reader) 
{
	OnlineLoadout item;
	item.attributes_list = ConsumeVector<Attributes>(reader);
	return item;
}


template<>
inline const ClientLoadoutsOnline Consume(CPPBitReader<BitReaderType>& reader) 
{
	ClientLoadoutsOnline item;
	item.online_one = Consume<OnlineLoadout>(reader);
	item.online_two = Consume<OnlineLoadout>(reader);
	item.loadout_set = reader.read<bool>();
	item.is_deprecated = reader.read<bool>();
	return item;
}


template<>
inline const ClientLoadouts Consume(CPPBitReader<BitReaderType>& reader) 
{
	ClientLoadouts item;
	item.loadout_one = Consume<ClientLoadout>(reader);
	item.loadout_two = Consume<ClientLoadout>(reader);
	return item;
}


template<>
inline const ClubColors Consume(CPPBitReader<BitReaderType>& reader) 
{
	ClubColors item;
	item.team_color_set = reader.read<bool>();
	item.team_color_id = reader.read<uint8_t>();
	item.custom_color_set = reader.read<bool>();
	item.custom_color_id = reader.read<uint8_t>();
	return item;
}


template<>
inline const WeldedInfo Consume(CPPBitReader<BitReaderType>& reader) 
{
	WeldedInfo item;
	item.active = reader.read<bool>();
	item.actor_id = reader.read<int32_t>();
	item.offset = reader.read<Vector3>();
	item.mass = reader.read<float>();
	item.rotation = reader.read<Rotator>();
	return item;
}


template<>
inline const DamageState Consume(CPPBitReader<BitReaderType>& reader) 
{
	DamageState item;
	item.damage_state = reader.read<uint8_t>();
	item.unknown2 = reader.read<bool>();
	item.causer_actor_id = reader.read<int32_t>();
	item.damage_location = reader.read<Vector3>();
	item.direct_damage = reader.read<bool>();
	item.immediate = reader.read<bool>();
	return item;
}


template<>
inline const AppliedDamage Consume(CPPBitReader<BitReaderType>& reader) 
{
	AppliedDamage item;
	item.id = reader.read<uint8_t>();
	item.position = reader.read<Vector3>();
	item.damage_index = reader.read<int32_t>();
	item.total_damage = reader.read<int32_t>();
	return item;
}


template<>
inline const ReplicatedExplosionData Consume(CPPBitReader<BitReaderType>& reader) 
{
	ReplicatedExplosionData item;
	item.unknown1 = reader.read<bool>();
	item.actor_id = reader.read<uint32_t>();
	item.position = reader.read<Vector3>();
	return item;
}


template<>
inline const ReplicatedExplosionDataExtended Consume(CPPBitReader<BitReaderType>& reader) 
{
	ReplicatedExplosionDataExtended item;
	item.unknown1 = reader.read<bool>();
	item.actor_id = reader.read<int32_t>();
	item.position = reader.read<Vector3>();
	item.unknown3 = reader.read<bool>();
	item.secondary_actor_id = reader.read<int32_t>();
	return item;
}


template<>
inline const ReplicatedTitle Consume(CPPBitReader<BitReaderType>& reader) 
{
	ReplicatedTitle item;
	item.unknown1 = reader.read<bool>();
	item.unknown2 = reader.read<bool>();
	item.unknown3 = reader.read<uint32_t>();
	item.unknown4 = reader.read<uint32_t>();
	item.unknown5 = reader.read<uint32_t>();
	item.unknown6 = reader.read<uint32_t>();
	item.unknown7 = reader.read<uint32_t>();
	item.unknown8 = reader.read<bool>();
	return item;
}


template<>
inline const HistoryKey Consume(CPPBitReader<BitReaderType>& reader) 
{
	HistoryKey item;
	item.data = reader.read<uint16_t>(14);
	return item;
}


template<>
inline const ReplicatedStatEvent Consume(CPPBitReader<BitReaderType>& reader) 
{
	ReplicatedStatEvent item;
	item.unknown1 = reader.read<bool>();
	item.object_id = reader.read<int32_t>();
	return item;
}


template<>
inline const RepStatTitle Consume(CPPBitReader<BitReaderType>& reader) 
{
	RepStatTitle item;
	item.unknown1 = reader.read<bool>();
	item.name = reader.read<std::string>();
	item.object_target = Consume<ObjectTarget>(reader);
	item.value = reader.read<uint32_t>();
	return item;
}


template<>
inline const SkillTier Consume(CPPBitReader<BitReaderType>& reader) 
{
	SkillTier item;
	item.tier = reader.readBitsMax<uint32_t>(500);
	return item;
}


template<>
inline const RigidBodyState Consume(CPPBitReader<BitReaderType>& reader) 
{
	RigidBodyState item;
	item.position = reader.read<Vector3>();
	item.lin_vel = reader.read<Vector3>();
	item.quaternion = reader.read<Quat>();
	item.ang_vel = reader.read<Vector3>();
	item.flags = reader.read<uint32_t>();
	return item;
}


