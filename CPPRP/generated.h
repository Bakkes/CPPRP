#pragma once
#include "CPPBitReader.h"
#include "NetworkData.h"
#include <sstream>
namespace CPPRP {
template<>
inline const std::string ToString(const ReplicatedRBState& item) 
{
	std::stringstream ss;
	ss << "rotation = " << ToString(item.rotation);
	ss << ",\n " << "position = " << ToString(item.position);
	ss << ",\n " << "linear_velocity = " << ToString(item.linear_velocity);
	ss << ",\n " << "angular_velocity = " << ToString(item.angular_velocity);
	ss << ",\n " << "sleeping = " << item.sleeping;
	return ss.str();
}

template<>
inline const LogoData Consume(CPPBitReader<BitReaderType>& reader) 
{
	LogoData item;
	item.unknown = reader.read<bool>();
	item.logo_id = reader.read<uint32_t>();
	return item;
}

template<>
inline const std::string ToString(const LogoData& item) 
{
	std::stringstream ss;
	ss << "unknown = " << item.unknown;
	ss << ",\n " << "logo_id = " << item.logo_id;
	return ss.str();
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
inline const std::string ToString(const ActiveActor& item) 
{
	std::stringstream ss;
	ss << "active = " << item.active;
	ss << ",\n " << "actor_id = " << item.actor_id;
	return ss.str();
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
inline const std::string ToString(const ObjectTarget& item) 
{
	std::stringstream ss;
	ss << "unknown = " << item.unknown;
	ss << ",\n " << "object_index = " << item.object_index;
	return ss.str();
}

template<>
inline const std::string ToString(const Reservation& item) 
{
	std::stringstream ss;
	ss << "unknown = " << ToString(item.unknown);
	ss << ",\n " << "player_id = " << ToString(item.player_id);
	ss << ",\n " << "player_name = " << item.player_name;
	ss << ",\n " << "unknown2 = " << ToString(item.unknown2);
	return ss.str();
}

template<>
inline const std::string ToString(const ClientLoadout& item) 
{
	std::stringstream ss;
	ss << "version = " << ToString(item.version);
	ss << ",\n " << "body = " << item.body;
	ss << ",\n " << "skin = " << item.skin;
	ss << ",\n " << "wheels = " << item.wheels;
	ss << ",\n " << "boost = " << item.boost;
	ss << ",\n " << "antenna = " << item.antenna;
	ss << ",\n " << "hat = " << item.hat;
	ss << ",\n " << "unknown2 = " << item.unknown2;
	ss << ",\n " << "unknown3 = " << item.unknown3;
	ss << ",\n " << "engine_audio = " << item.engine_audio;
	ss << ",\n " << "trail = " << item.trail;
	ss << ",\n " << "goal_explosion = " << item.goal_explosion;
	ss << ",\n " << "banner = " << item.banner;
	ss << ",\n " << "unknown4 = " << item.unknown4;
	ss << ",\n " << "unknown5 = " << item.unknown5;
	ss << ",\n " << "unknown6 = " << item.unknown6;
	ss << ",\n " << "unknown7 = " << item.unknown7;
	return ss.str();
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
	if(reader.owner->header.engineVersion >= 868 
		&& reader.owner->header.licenseeVersion >= 20) { 
		item.transitionspeed = reader.read<float>();
	} else {
		item.transitionspeed = 0;
	}
	return item;
}

template<>
inline const std::string ToString(const CameraSettings& item) 
{
	std::stringstream ss;
	ss << "FOV = " << item.FOV;
	ss << ",\n " << "height = " << item.height;
	ss << ",\n " << "pitch = " << item.pitch;
	ss << ",\n " << "distance = " << item.distance;
	ss << ",\n " << "stiffness = " << item.stiffness;
	ss << ",\n " << "swivelspeed = " << item.swivelspeed;
	ss << ",\n " << "transitionspeed = " << item.transitionspeed;
	return ss.str();
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
inline const std::string ToString(const ReplicatedPickupData& item) 
{
	std::stringstream ss;
	ss << "unknown1 = " << item.unknown1;
	ss << ",\n " << "actor_id = " << item.actor_id;
	ss << ",\n " << "picked_up = " << item.picked_up;
	return ss.str();
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
inline const std::string ToString(const TeamPaint& item) 
{
	std::stringstream ss;
	ss << "team_number = " << ToString(item.team_number);
	ss << ",\n " << "team_color_id = " << ToString(item.team_color_id);
	ss << ",\n " << "custom_color_id = " << ToString(item.custom_color_id);
	ss << ",\n " << "team_finish_id = " << item.team_finish_id;
	ss << ",\n " << "custom_finish_id = " << item.custom_finish_id;
	return ss.str();
}

template<>
inline const ReplicatedDemolish Consume(CPPBitReader<BitReaderType>& reader) 
{
	ReplicatedDemolish item;
	item.unknown1 = reader.read<bool>();
	item.attacker_actor_id = reader.read<int32_t>();
	item.unknown2 = reader.read<bool>();
	item.victim_actor_id = reader.read<int32_t>();
	item.attacker_velocity = reader.read<Vector3>();
	item.victim_velocity = reader.read<Vector3>();
	return item;
}

template<>
inline const std::string ToString(const ReplicatedDemolish& item) 
{
	std::stringstream ss;
	ss << "unknown1 = " << item.unknown1;
	ss << ",\n " << "attacker_actor_id = " << item.attacker_actor_id;
	ss << ",\n " << "unknown2 = " << item.unknown2;
	ss << ",\n " << "victim_actor_id = " << item.victim_actor_id;
	ss << ",\n " << "attacker_velocity = " << ToString(item.attacker_velocity);
	ss << ",\n " << "victim_velocity = " << ToString(item.victim_velocity);
	return ss.str();
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
inline const std::string ToString(const ReplicatedMusicStringer& item) 
{
	std::stringstream ss;
	ss << "unknown1 = " << item.unknown1;
	ss << ",\n " << "object_index = " << item.object_index;
	ss << ",\n " << "trigger = " << item.trigger;
	return ss.str();
}

template<>
inline const std::string ToString(const GameMode& item) 
{
	std::stringstream ss;
	ss << "gamemode = " << item.gamemode;
	return ss.str();
}

template<>
inline const ReplicatedStateIndex Consume(CPPBitReader<BitReaderType>& reader) 
{
	ReplicatedStateIndex item;
	item.value = reader.readBitsMax<uint32_t>(140);
	return item;
}

template<>
inline const std::string ToString(const ReplicatedStateIndex& item) 
{
	std::stringstream ss;
	ss << "value = " << item.value;
	return ss.str();
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
inline const std::string ToString(const PrivateMatchSettings& item) 
{
	std::stringstream ss;
	ss << "mutators = " << item.mutators;
	ss << ",\n " << "map_name = " << item.map_name;
	ss << ",\n " << "max_player_count = " << item.max_player_count;
	ss << ",\n " << "game_name = " << item.game_name;
	ss << ",\n " << "password = " << item.password;
	ss << ",\n " << "is_public = " << item.is_public;
	return ss.str();
}

template<>
inline const std::string ToString(const ProductAttribute& item) 
{
	std::stringstream ss;
	ss << "class_index = " << item.class_index;
	ss << ",\n " << "class_name = " << item.class_name;
	ss << ",\n " << "value = " << item.value;
	ss << ",\n " << "has_value = " << item.has_value;
	ss << ",\n " << "unknown1 = " << item.unknown1;
	return ss.str();
}

template<>
inline const Attributes Consume(CPPBitReader<BitReaderType>& reader) 
{
	Attributes item;
	item.product_attributes = ConsumeVector<ProductAttribute>(reader);
	return item;
}

template<>
inline const std::string ToString(const Attributes& item) 
{
	std::stringstream ss;
	ss << "product_attributes = " << ToString(item.product_attributes);
	return ss.str();
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
inline const std::string ToString(const ActorBase& item) 
{
	std::stringstream ss;
	ss << "value = " << item.value;
	ss << ",\n " << "unknown1 = " << item.unknown1;
	ss << ",\n " << "unknown2 = " << item.unknown2;
	return ss.str();
}

template<>
inline const std::string ToString(const PartyLeader& item) 
{
	std::stringstream ss;
	ss << "id = " << ToString(item.id);
	return ss.str();
}

template<>
inline const OnlineLoadout Consume(CPPBitReader<BitReaderType>& reader) 
{
	OnlineLoadout item;
	item.attributes_list = ConsumeVector<Attributes>(reader);
	return item;
}

template<>
inline const std::string ToString(const OnlineLoadout& item) 
{
	std::stringstream ss;
	ss << "attributes_list = " << ToString(item.attributes_list);
	return ss.str();
}

template<>
inline const UserColorAttribute Consume(CPPBitReader<BitReaderType>& reader) 
{
	UserColorAttribute item;
	item.r = reader.read<uint8_t>();
	item.g = reader.read<uint8_t>();
	item.b = reader.read<uint8_t>();
	item.a = reader.read<uint8_t>();
	return item;
}

template<>
inline const std::string ToString(const UserColorAttribute& item) 
{
	std::stringstream ss;
	ss << "r = " << item.r;
	ss << ",\n " << "g = " << item.g;
	ss << ",\n " << "b = " << item.b;
	ss << ",\n " << "a = " << item.a;
	return ss.str();
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
inline const std::string ToString(const ClientLoadoutsOnline& item) 
{
	std::stringstream ss;
	ss << "online_one = " << ToString(item.online_one);
	ss << ",\n " << "online_two = " << ToString(item.online_two);
	ss << ",\n " << "loadout_set = " << item.loadout_set;
	ss << ",\n " << "is_deprecated = " << item.is_deprecated;
	return ss.str();
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
inline const std::string ToString(const ClientLoadouts& item) 
{
	std::stringstream ss;
	ss << "loadout_one = " << ToString(item.loadout_one);
	ss << ",\n " << "loadout_two = " << ToString(item.loadout_two);
	return ss.str();
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
inline const std::string ToString(const ClubColors& item) 
{
	std::stringstream ss;
	ss << "team_color_set = " << item.team_color_set;
	ss << ",\n " << "team_color_id = " << item.team_color_id;
	ss << ",\n " << "custom_color_set = " << item.custom_color_set;
	ss << ",\n " << "custom_color_id = " << item.custom_color_id;
	return ss.str();
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
inline const std::string ToString(const WeldedInfo& item) 
{
	std::stringstream ss;
	ss << "active = " << item.active;
	ss << ",\n " << "actor_id = " << item.actor_id;
	ss << ",\n " << "offset = " << ToString(item.offset);
	ss << ",\n " << "mass = " << item.mass;
	ss << ",\n " << "rotation = " << ToString(item.rotation);
	return ss.str();
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
inline const std::string ToString(const DamageState& item) 
{
	std::stringstream ss;
	ss << "damage_state = " << item.damage_state;
	ss << ",\n " << "unknown2 = " << item.unknown2;
	ss << ",\n " << "causer_actor_id = " << item.causer_actor_id;
	ss << ",\n " << "damage_location = " << ToString(item.damage_location);
	ss << ",\n " << "direct_damage = " << item.direct_damage;
	ss << ",\n " << "immediate = " << item.immediate;
	return ss.str();
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
inline const std::string ToString(const AppliedDamage& item) 
{
	std::stringstream ss;
	ss << "id = " << item.id;
	ss << ",\n " << "position = " << ToString(item.position);
	ss << ",\n " << "damage_index = " << item.damage_index;
	ss << ",\n " << "total_damage = " << item.total_damage;
	return ss.str();
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
inline const std::string ToString(const ReplicatedExplosionData& item) 
{
	std::stringstream ss;
	ss << "unknown1 = " << item.unknown1;
	ss << ",\n " << "actor_id = " << item.actor_id;
	ss << ",\n " << "position = " << ToString(item.position);
	return ss.str();
}

template<>
inline const ReplicatedExplosionDataExtended Consume(CPPBitReader<BitReaderType>& reader) 
{
	ReplicatedExplosionDataExtended item;
	item.unknown1 = reader.read<bool>();
	item.actor_id = reader.read<uint32_t>();
	item.position = reader.read<Vector3>();
	item.unknown3 = reader.read<bool>();
	item.unknown4 = reader.read<uint32_t>();
	return item;
}

template<>
inline const std::string ToString(const ReplicatedExplosionDataExtended& item) 
{
	std::stringstream ss;
	ss << "unknown1 = " << item.unknown1;
	ss << ",\n " << "actor_id = " << item.actor_id;
	ss << ",\n " << "position = " << ToString(item.position);
	ss << ",\n " << "unknown3 = " << item.unknown3;
	ss << ",\n " << "unknown4 = " << item.unknown4;
	return ss.str();
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
inline const std::string ToString(const ReplicatedTitle& item) 
{
	std::stringstream ss;
	ss << "unknown1 = " << item.unknown1;
	ss << ",\n " << "unknown2 = " << item.unknown2;
	ss << ",\n " << "unknown3 = " << item.unknown3;
	ss << ",\n " << "unknown4 = " << item.unknown4;
	ss << ",\n " << "unknown5 = " << item.unknown5;
	ss << ",\n " << "unknown6 = " << item.unknown6;
	ss << ",\n " << "unknown7 = " << item.unknown7;
	ss << ",\n " << "unknown8 = " << item.unknown8;
	return ss.str();
}

template<>
inline const HistoryKey Consume(CPPBitReader<BitReaderType>& reader) 
{
	HistoryKey item;
	item.data = reader.read<uint16_t>(14);
	return item;
}

template<>
inline const std::string ToString(const HistoryKey& item) 
{
	std::stringstream ss;
	ss << "data = " << item.data;
	return ss.str();
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
inline const std::string ToString(const ReplicatedStatEvent& item) 
{
	std::stringstream ss;
	ss << "unknown1 = " << item.unknown1;
	ss << ",\n " << "object_id = " << item.object_id;
	return ss.str();
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
inline const std::string ToString(const RepStatTitle& item) 
{
	std::stringstream ss;
	ss << "unknown1 = " << item.unknown1;
	ss << ",\n " << "name = " << item.name;
	ss << ",\n " << "object_target = " << ToString(item.object_target);
	ss << ",\n " << "value = " << item.value;
	return ss.str();
}

template<>
inline const SkillTier Consume(CPPBitReader<BitReaderType>& reader) 
{
	SkillTier item;
	item.tier = reader.readBitsMax<uint32_t>(500);
	return item;
}

template<>
inline const std::string ToString(const SkillTier& item) 
{
	std::stringstream ss;
	ss << "tier = " << item.tier;
	return ss.str();
}


};