template<>
LogoData Consume<LogoData>(CPPBitReader<uint32_t& reader>) { 
	LogoData item;
	item.logo_id = reader.read<uint32_t>();
	item.unknown = reader.read<uint8_t>();
	return item;
}}

template<>
std::string ToString(LogoData& item) { 
	std::string ss;
	ss << "logo_id = " item.logo_id;
	ss << ",\n " << "unknown = " item.unknown;
	return ss.str();
}}

template<>
ActiveActor Consume<ActiveActor>(CPPBitReader<uint32_t& reader>) { 
	ActiveActor item;
	item.actor_id = reader.read<int32_t>();
	item.active = reader.read<uint8_t>();
	return item;
}}

template<>
std::string ToString(ActiveActor& item) { 
	std::string ss;
	ss << "actor_id = " item.actor_id;
	ss << ",\n " << "active = " item.active;
	return ss.str();
}}

template<>
ObjectTarget Consume<ObjectTarget>(CPPBitReader<uint32_t& reader>) { 
	ObjectTarget item;
	item.object_index = reader.read<int32_t>();
	item.unknown = reader.read<bool>();
	return item;
}}

template<>
std::string ToString(ObjectTarget& item) { 
	std::string ss;
	ss << "object_index = " item.object_index;
	ss << ",\n " << "unknown = " item.unknown;
	return ss.str();
}}

template<>
UniqueId Consume<UniqueId>(CPPBitReader<uint32_t& reader>) { 
	UniqueId item;
	item.platform = reader.read<uint8_t>();
	item.player_number = reader.read<uint8_t>();
	item.id = reader.read<uint8_t*>();
	return item;
}}

template<>
std::string ToString(UniqueId& item) { 
	std::string ss;
	ss << "platform = " item.platform;
	ss << ",\n " << "player_number = " item.player_number;
	ss << ",\n " << "id = " item.id;
	return ss.str();
}}

template<>
Reservation Consume<Reservation>(CPPBitReader<uint32_t& reader>) { 
	Reservation item;
	item.unknown = reader.read<uint32_t>();
	item.player_id = reader.read<UniqueId>();
	item.player_name = reader.read<std::string>();
	item.unknown2 = reader.read<uint8_t>();
	return item;
}}

template<>
std::string ToString(Reservation& item) { 
	std::string ss;
	ss << "unknown = " item.unknown;
	ss << ",\n " << "player_id = " item.player_id;
	ss << ",\n " << "player_name = " item.player_name;
	ss << ",\n " << "unknown2 = " item.unknown2;
	return ss.str();
}}

template<>
ClientLoadout Consume<ClientLoadout>(CPPBitReader<uint32_t& reader>) { 
	ClientLoadout item;
	item.version = reader.read<uint8_t>();
	item.body = reader.read<uint32_t>();
	item.skin = reader.read<uint32_t>();
	item.wheels = reader.read<uint32_t>();
	item.boost = reader.read<uint32_t>();
	item.antenna = reader.read<uint32_t>();
	item.hat = reader.read<uint32_t>();
	item.unknown2 = reader.read<uint32_t>();
	item.unknown3 = reader.read<uint32_t>();
	item.engine_audio = reader.read<uint32_t>();
	item.trail = reader.read<uint32_t>();
	item.goal_explosion = reader.read<uint32_t>();
	item.banner = reader.read<uint32_t>();
	item.unknown4 = reader.read<uint32_t>();
	item.unknown5 = reader.read<uint32_t>();
	item.unknown6 = reader.read<uint32_t>();
	item.unknown7 = reader.read<uint32_t>();
	return item;
}}

template<>
std::string ToString(ClientLoadout& item) { 
	std::string ss;
	ss << "version = " item.version;
	ss << ",\n " << "body = " item.body;
	ss << ",\n " << "skin = " item.skin;
	ss << ",\n " << "wheels = " item.wheels;
	ss << ",\n " << "boost = " item.boost;
	ss << ",\n " << "antenna = " item.antenna;
	ss << ",\n " << "hat = " item.hat;
	ss << ",\n " << "unknown2 = " item.unknown2;
	ss << ",\n " << "unknown3 = " item.unknown3;
	ss << ",\n " << "engine_audio = " item.engine_audio;
	ss << ",\n " << "trail = " item.trail;
	ss << ",\n " << "goal_explosion = " item.goal_explosion;
	ss << ",\n " << "banner = " item.banner;
	ss << ",\n " << "unknown4 = " item.unknown4;
	ss << ",\n " << "unknown5 = " item.unknown5;
	ss << ",\n " << "unknown6 = " item.unknown6;
	ss << ",\n " << "unknown7 = " item.unknown7;
	return ss.str();
}}

template<>
CameraSettings Consume<CameraSettings>(CPPBitReader<uint32_t& reader>) { 
	CameraSettings item;
	item.FOV = reader.read<float>();
	item.height = reader.read<float>();
	item.pitch = reader.read<float>();
	item.distance = reader.read<float>();
	item.stiffness = reader.read<float>();
	item.swivelspeed = reader.read<float>();
	item.transitionspeed = reader.read<float>();
	return item;
}}

template<>
std::string ToString(CameraSettings& item) { 
	std::string ss;
	ss << "FOV = " item.FOV;
	ss << ",\n " << "height = " item.height;
	ss << ",\n " << "pitch = " item.pitch;
	ss << ",\n " << "distance = " item.distance;
	ss << ",\n " << "stiffness = " item.stiffness;
	ss << ",\n " << "swivelspeed = " item.swivelspeed;
	ss << ",\n " << "transitionspeed = " item.transitionspeed;
	return ss.str();
}}

template<>
ReplicatedPickupData Consume<ReplicatedPickupData>(CPPBitReader<uint32_t& reader>) { 
	ReplicatedPickupData item;
	item.actor_id = reader.read<int32_t>();
	item.unknown1 = reader.read<bool>();
	item.picked_up = reader.read<bool>();
	return item;
}}

template<>
std::string ToString(ReplicatedPickupData& item) { 
	std::string ss;
	ss << "actor_id = " item.actor_id;
	ss << ",\n " << "unknown1 = " item.unknown1;
	ss << ",\n " << "picked_up = " item.picked_up;
	return ss.str();
}}

template<>
TeamPaint Consume<TeamPaint>(CPPBitReader<uint32_t& reader>) { 
	TeamPaint item;
	item.team_number = reader.read<uint8_t>();
	item.team_color_id = reader.read<uint8_t>();
	item.custom_color_id = reader.read<uint8_t>();
	item.team_finish_id = reader.read<uint32_t>();
	item.custom_finish_id = reader.read<uint32_t>();
	return item;
}}

template<>
std::string ToString(TeamPaint& item) { 
	std::string ss;
	ss << "team_number = " item.team_number;
	ss << ",\n " << "team_color_id = " item.team_color_id;
	ss << ",\n " << "custom_color_id = " item.custom_color_id;
	ss << ",\n " << "team_finish_id = " item.team_finish_id;
	ss << ",\n " << "custom_finish_id = " item.custom_finish_id;
	return ss.str();
}}

template<>
ReplicatedDemolish Consume<ReplicatedDemolish>(CPPBitReader<uint32_t& reader>) { 
	ReplicatedDemolish item;
	item.attacker_actor_id = reader.read<int32_t>();
	item.victim_actor_id = reader.read<int32_t>();
	item.attacker_velocity = reader.read<Vector3>();
	item.victim_velocity = reader.read<Vector3>();
	item.unknown1 = reader.read<bool>();
	item.unknown2 = reader.read<bool>();
	return item;
}}

template<>
std::string ToString(ReplicatedDemolish& item) { 
	std::string ss;
	ss << "attacker_actor_id = " item.attacker_actor_id;
	ss << ",\n " << "victim_actor_id = " item.victim_actor_id;
	ss << ",\n " << "attacker_velocity = " item.attacker_velocity;
	ss << ",\n " << "victim_velocity = " item.victim_velocity;
	ss << ",\n " << "unknown1 = " item.unknown1;
	ss << ",\n " << "unknown2 = " item.unknown2;
	return ss.str();
}}

template<>
ReplicatedMusicStringer Consume<ReplicatedMusicStringer>(CPPBitReader<uint32_t& reader>) { 
	ReplicatedMusicStringer item;
	item.object_index = reader.read<uint32_t>();
	item.trigger = reader.read<uint8_t>();
	item.unknown1 = reader.read<bool>();
	return item;
}}

template<>
std::string ToString(ReplicatedMusicStringer& item) { 
	std::string ss;
	ss << "object_index = " item.object_index;
	ss << ",\n " << "trigger = " item.trigger;
	ss << ",\n " << "unknown1 = " item.unknown1;
	return ss.str();
}}

template<>
PrivateMatchSettings Consume<PrivateMatchSettings>(CPPBitReader<uint32_t& reader>) { 
	PrivateMatchSettings item;
	item.map_name = reader.read<uint32_t>();
	item.max_player_count = reader.read<uint32_t>();
	item.mutators = reader.read<std::string>();
	item.game_name = reader.read<std::string>();
	item.password = reader.read<std::string>();
	item.is_public = reader.read<bool>();
	return item;
}}

template<>
std::string ToString(PrivateMatchSettings& item) { 
	std::string ss;
	ss << "map_name = " item.map_name;
	ss << ",\n " << "max_player_count = " item.max_player_count;
	ss << ",\n " << "mutators = " item.mutators;
	ss << ",\n " << "game_name = " item.game_name;
	ss << ",\n " << "password = " item.password;
	ss << ",\n " << "is_public = " item.is_public;
	return ss.str();
}}

template<>
ProductAttribute Consume<ProductAttribute>(CPPBitReader<uint32_t& reader>) { 
	ProductAttribute item;
	item.class_index = reader.read<uint32_t>();
	item.class_name = reader.read<std::string>();
	item.value = reader.read<void*>();
	item.has_value = reader.read<bool>();
	item.unknown1 = reader.read<bool>();
	return item;
}}

template<>
std::string ToString(ProductAttribute& item) { 
	std::string ss;
	ss << "class_index = " item.class_index;
	ss << ",\n " << "class_name = " item.class_name;
	ss << ",\n " << "value = " item.value;
	ss << ",\n " << "has_value = " item.has_value;
	ss << ",\n " << "unknown1 = " item.unknown1;
	return ss.str();
}}

template<>
Attributes Consume<Attributes>(CPPBitReader<uint32_t& reader>) { 
	Attributes item;
	item.attributes_count = reader.read<uint8_t>();
	item.product_attributes = reader.read<ProductAttribute*>();
	return item;
}}

template<>
std::string ToString(Attributes& item) { 
	std::string ss;
	ss << "attributes_count = " item.attributes_count;
	ss << ",\n " << "product_attributes = " item.product_attributes;
	return ss.str();
}}

template<>
OnlineLoadout Consume<OnlineLoadout>(CPPBitReader<uint32_t& reader>) { 
	OnlineLoadout item;
	item.attributes_list_count = reader.read<uint8_t>();
	item.attributes_list = reader.read<std::vector<Attributes>>();
	return item;
}}

template<>
std::string ToString(OnlineLoadout& item) { 
	std::string ss;
	ss << "attributes_list_count = " item.attributes_list_count;
	ss << ",\n " << "attributes_list = " item.attributes_list;
	return ss.str();
}}

template<>
UserColorAttribute Consume<UserColorAttribute>(CPPBitReader<uint32_t& reader>) { 
	UserColorAttribute item;
	item.r, = reader.read<uint8_t>();
	return item;
}}

template<>
std::string ToString(UserColorAttribute& item) { 
	std::string ss;
	ss << "r, = " item.r,;
	return ss.str();
}}

template<>
ClientLoadoutsOnline Consume<ClientLoadoutsOnline>(CPPBitReader<uint32_t& reader>) { 
	ClientLoadoutsOnline item;
	item.online_one = reader.read<OnlineLoadout>();
	item.online_two = reader.read<OnlineLoadout>();
	item.loadout_set = reader.read<bool>();
	item.is_deprecated = reader.read<bool>();
	return item;
}}

template<>
std::string ToString(ClientLoadoutsOnline& item) { 
	std::string ss;
	ss << "online_one = " item.online_one;
	ss << ",\n " << "online_two = " item.online_two;
	ss << ",\n " << "loadout_set = " item.loadout_set;
	ss << ",\n " << "is_deprecated = " item.is_deprecated;
	return ss.str();
}}

template<>
ClientLoadouts Consume<ClientLoadouts>(CPPBitReader<uint32_t& reader>) { 
	ClientLoadouts item;
	item.loadout_one = reader.read<ClientLoadout>();
	item.loadout_two = reader.read<ClientLoadout>();
	return item;
}}

template<>
std::string ToString(ClientLoadouts& item) { 
	std::string ss;
	ss << "loadout_one = " item.loadout_one;
	ss << ",\n " << "loadout_two = " item.loadout_two;
	return ss.str();
}}

template<>
ClubColors Consume<ClubColors>(CPPBitReader<uint32_t& reader>) { 
	ClubColors item;
	item.team_color_id = reader.read<uint8_t>();
	item.custom_color_id = reader.read<uint8_t>();
	item.team_color_set = reader.read<uint8_t>();
	item.custom_color_set = reader.read<uint8_t>();
	return item;
}}

template<>
std::string ToString(ClubColors& item) { 
	std::string ss;
	ss << "team_color_id = " item.team_color_id;
	ss << ",\n " << "custom_color_id = " item.custom_color_id;
	ss << ",\n " << "team_color_set = " item.team_color_set;
	ss << ",\n " << "custom_color_set = " item.custom_color_set;
	return ss.str();
}}

template<>
WeldedInfo Consume<WeldedInfo>(CPPBitReader<uint32_t& reader>) { 
	WeldedInfo item;
	item.actor_id = reader.read<int32_t>();
	item.offset = reader.read<Vector3>();
	item.mass = reader.read<float>();
	item.rotation = reader.read<Rotator>();
	item.active = reader.read<uint8_t>();
	return item;
}}

template<>
std::string ToString(WeldedInfo& item) { 
	std::string ss;
	ss << "actor_id = " item.actor_id;
	ss << ",\n " << "offset = " item.offset;
	ss << ",\n " << "mass = " item.mass;
	ss << ",\n " << "rotation = " item.rotation;
	ss << ",\n " << "active = " item.active;
	return ss.str();
}}

template<>
DamageState Consume<DamageState>(CPPBitReader<uint32_t& reader>) { 
	DamageState item;
	item.damage_state = reader.read<uint8_t>();
	item.causer_actor_id = reader.read<int32_t>();
	item.damage_location = reader.read<Vector3>();
	item.unknown2 = reader.read<bool>();
	item.direct_damage = reader.read<bool>();
	item.immediate = reader.read<bool>();
	return item;
}}

template<>
std::string ToString(DamageState& item) { 
	std::string ss;
	ss << "damage_state = " item.damage_state;
	ss << ",\n " << "causer_actor_id = " item.causer_actor_id;
	ss << ",\n " << "damage_location = " item.damage_location;
	ss << ",\n " << "unknown2 = " item.unknown2;
	ss << ",\n " << "direct_damage = " item.direct_damage;
	ss << ",\n " << "immediate = " item.immediate;
	return ss.str();
}}

template<>
AppliedDamage Consume<AppliedDamage>(CPPBitReader<uint32_t& reader>) { 
	AppliedDamage item;
	item.id = reader.read<uint8_t>();
	item.position = reader.read<Vector3>();
	item.damage_index = reader.read<int32_t>();
	item.total_damage = reader.read<int32_t>();
	return item;
}}

template<>
std::string ToString(AppliedDamage& item) { 
	std::string ss;
	ss << "id = " item.id;
	ss << ",\n " << "position = " item.position;
	ss << ",\n " << "damage_index = " item.damage_index;
	ss << ",\n " << "total_damage = " item.total_damage;
	return ss.str();
}}

template<>
ReplicatedExplosionData Consume<ReplicatedExplosionData>(CPPBitReader<uint32_t& reader>) { 
	ReplicatedExplosionData item;
	item.unknown1 = reader.read<uint8_t>();
	item.actor_id = reader.read<uint32_t>();
	item.position = reader.read<Vector3>();
	return item;
}}

template<>
std::string ToString(ReplicatedExplosionData& item) { 
	std::string ss;
	ss << "unknown1 = " item.unknown1;
	ss << ",\n " << "actor_id = " item.actor_id;
	ss << ",\n " << "position = " item.position;
	return ss.str();
}}

template<>
ReplicatedExplosionDataExtended Consume<ReplicatedExplosionDataExtended>(CPPBitReader<uint32_t& reader>) { 
	ReplicatedExplosionDataExtended item;
	item.red = reader.read<ReplicatedExplosionData>();
	item.unknown3 = reader.read<uint8_t>();
	item.unknown4 = reader.read<uint32_t>();
	return item;
}}

template<>
std::string ToString(ReplicatedExplosionDataExtended& item) { 
	std::string ss;
	ss << "red = " item.red;
	ss << ",\n " << "unknown3 = " item.unknown3;
	ss << ",\n " << "unknown4 = " item.unknown4;
	return ss.str();
}}

template<>
ReplicatedTitle Consume<ReplicatedTitle>(CPPBitReader<uint32_t& reader>) { 
	ReplicatedTitle item;
	item.unknown3 = reader.read<uint32_t>();
	item.unknown4 = reader.read<uint32_t>();
	item.unknown5 = reader.read<uint32_t>();
	item.unknown6 = reader.read<uint32_t>();
	item.unknown7 = reader.read<uint32_t>();
	item.unknown1 = reader.read<bool>();
	item.unknown2 = reader.read<bool>();
	item.unknown8 = reader.read<bool>();
	return item;
}}

template<>
std::string ToString(ReplicatedTitle& item) { 
	std::string ss;
	ss << "unknown3 = " item.unknown3;
	ss << ",\n " << "unknown4 = " item.unknown4;
	ss << ",\n " << "unknown5 = " item.unknown5;
	ss << ",\n " << "unknown6 = " item.unknown6;
	ss << ",\n " << "unknown7 = " item.unknown7;
	ss << ",\n " << "unknown1 = " item.unknown1;
	ss << ",\n " << "unknown2 = " item.unknown2;
	ss << ",\n " << "unknown8 = " item.unknown8;
	return ss.str();
}}

template<>
ReplicatedStatEvent Consume<ReplicatedStatEvent>(CPPBitReader<uint32_t& reader>) { 
	ReplicatedStatEvent item;
	item.object_id = reader.read<int32_t>();
	item.unknown1 = reader.read<bool>();
	return item;
}}

template<>
std::string ToString(ReplicatedStatEvent& item) { 
	std::string ss;
	ss << "object_id = " item.object_id;
	ss << ",\n " << "unknown1 = " item.unknown1;
	return ss.str();
}}

template<>
RepStatTitle Consume<RepStatTitle>(CPPBitReader<uint32_t& reader>) { 
	RepStatTitle item;
	item.object_target = reader.read<ObjectTarget>();
	item.value = reader.read<uint32_t>();
	item.char* = reader.read<unsigned>();
	item.unknown1 = reader.read<bool>();
	return item;
}}

template<>
std::string ToString(RepStatTitle& item) { 
	std::string ss;
	ss << "object_target = " item.object_target;
	ss << ",\n " << "value = " item.value;
	ss << ",\n " << "char* = " item.char*;
	ss << ",\n " << "unknown1 = " item.unknown1;
	return ss.str();
}}

