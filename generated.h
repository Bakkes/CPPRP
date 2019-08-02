#pragma once
#include "CPPBitReader.h"
#include "NetworkData.h"
#include <sstream>
template<>
inline const std::string ToString(const ReplicatedRBState& item) 
{
	std::stringstream ss;
	ss << "rotation = " << ToString<Quat>(item.rotation);
	ss << ",\n " << "position = " << ToString<Vector3>(item.position);
	ss << ",\n " << "linear_velocity = " << ToString<Vector3>(item.linear_velocity);
	ss << ",\n " << "angular_velocity = " << ToString<Vector3>(item.angular_velocity);
	ss << ",\n " << "sleeping = " << item.sleeping;
	return ss.str();
}

template<typename Writer>
inline const void Serialize(Writer& writer, const ReplicatedRBState& item)
{
	writer.StartObject();
	writer.String("rotation");
	Serialize(writer, item.rotation);
	writer.String("position");
	Serialize(writer, item.position);
	writer.String("linear_velocity");
	Serialize(writer, item.linear_velocity);
	writer.String("angular_velocity");
	Serialize(writer, item.angular_velocity);
	writer.String("sleeping");
	Serialize(writer, item.sleeping);
	writer.EndObject();
}

template<>
inline const LogoData Consume(CPPBitReader<uint32_t>& reader) 
{
	LogoData item;
	item.unknown = reader.read<uint8_t>();
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

template<typename Writer>
inline const void Serialize(Writer& writer, const LogoData& item)
{
	writer.StartObject();
	writer.String("unknown");
	Serialize(writer, item.unknown);
	writer.String("logo_id");
	Serialize(writer, item.logo_id);
	writer.EndObject();
}

template<>
inline const ActiveActor Consume(CPPBitReader<uint32_t>& reader) 
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

template<typename Writer>
inline const void Serialize(Writer& writer, const ActiveActor& item)
{
	writer.StartObject();
	writer.String("active");
	Serialize(writer, item.active);
	writer.String("actor_id");
	Serialize(writer, item.actor_id);
	writer.EndObject();
}

template<>
inline const ObjectTarget Consume(CPPBitReader<uint32_t>& reader) 
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

template<typename Writer>
inline const void Serialize(Writer& writer, const ObjectTarget& item)
{
	writer.StartObject();
	writer.String("unknown");
	Serialize(writer, item.unknown);
	writer.String("object_index");
	Serialize(writer, item.object_index);
	writer.EndObject();
}

template<>
inline const std::string ToString(const Reservation& item) 
{
	std::stringstream ss;
	ss << "unknown = " << item.unknown;
	ss << ",\n " << "player_id = " << ToString<UniqueId>(item.player_id);
	ss << ",\n " << "player_name = " << item.player_name;
	ss << ",\n " << "unknown2 = " << item.unknown2;
	return ss.str();
}

template<typename Writer>
inline const void Serialize(Writer& writer, const Reservation& item)
{
	writer.StartObject();
	writer.String("unknown");
	Serialize(writer, item.unknown);
	writer.String("player_id");
	Serialize(writer, item.player_id);
	writer.String("player_name");
	Serialize(writer, item.player_name);
	writer.String("unknown2");
	Serialize(writer, item.unknown2);
	writer.EndObject();
}

template<>
inline const std::string ToString(const ClientLoadout& item) 
{
	std::stringstream ss;
	ss << "version = " << item.version;
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

template<typename Writer>
inline const void Serialize(Writer& writer, const ClientLoadout& item)
{
	writer.StartObject();
	writer.String("version");
	Serialize(writer, item.version);
	writer.String("body");
	Serialize(writer, item.body);
	writer.String("skin");
	Serialize(writer, item.skin);
	writer.String("wheels");
	Serialize(writer, item.wheels);
	writer.String("boost");
	Serialize(writer, item.boost);
	writer.String("antenna");
	Serialize(writer, item.antenna);
	writer.String("hat");
	Serialize(writer, item.hat);
	writer.String("unknown2");
	Serialize(writer, item.unknown2);
	writer.String("unknown3");
	Serialize(writer, item.unknown3);
	writer.String("engine_audio");
	Serialize(writer, item.engine_audio);
	writer.String("trail");
	Serialize(writer, item.trail);
	writer.String("goal_explosion");
	Serialize(writer, item.goal_explosion);
	writer.String("banner");
	Serialize(writer, item.banner);
	writer.String("unknown4");
	Serialize(writer, item.unknown4);
	writer.String("unknown5");
	Serialize(writer, item.unknown5);
	writer.String("unknown6");
	Serialize(writer, item.unknown6);
	writer.String("unknown7");
	Serialize(writer, item.unknown7);
	writer.EndObject();
}

template<>
inline const CameraSettings Consume(CPPBitReader<uint32_t>& reader) 
{
	CameraSettings item;
	item.FOV = reader.read<float>();
	item.height = reader.read<float>();
	item.pitch = reader.read<float>();
	item.distance = reader.read<float>();
	item.stiffness = reader.read<float>();
	item.swivelspeed = reader.read<float>();
	item.transitionspeed = reader.read<float>();
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

template<typename Writer>
inline const void Serialize(Writer& writer, const CameraSettings& item)
{
	writer.StartObject();
	writer.String("FOV");
	Serialize(writer, item.FOV);
	writer.String("height");
	Serialize(writer, item.height);
	writer.String("pitch");
	Serialize(writer, item.pitch);
	writer.String("distance");
	Serialize(writer, item.distance);
	writer.String("stiffness");
	Serialize(writer, item.stiffness);
	writer.String("swivelspeed");
	Serialize(writer, item.swivelspeed);
	writer.String("transitionspeed");
	Serialize(writer, item.transitionspeed);
	writer.EndObject();
}

template<>
inline const ReplicatedPickupData Consume(CPPBitReader<uint32_t>& reader) 
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

template<typename Writer>
inline const void Serialize(Writer& writer, const ReplicatedPickupData& item)
{
	writer.StartObject();
	writer.String("unknown1");
	Serialize(writer, item.unknown1);
	writer.String("actor_id");
	Serialize(writer, item.actor_id);
	writer.String("picked_up");
	Serialize(writer, item.picked_up);
	writer.EndObject();
}

template<>
inline const TeamPaint Consume(CPPBitReader<uint32_t>& reader) 
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
	ss << "team_number = " << item.team_number;
	ss << ",\n " << "team_color_id = " << item.team_color_id;
	ss << ",\n " << "custom_color_id = " << item.custom_color_id;
	ss << ",\n " << "team_finish_id = " << item.team_finish_id;
	ss << ",\n " << "custom_finish_id = " << item.custom_finish_id;
	return ss.str();
}

template<typename Writer>
inline const void Serialize(Writer& writer, const TeamPaint& item)
{
	writer.StartObject();
	writer.String("team_number");
	Serialize(writer, item.team_number);
	writer.String("team_color_id");
	Serialize(writer, item.team_color_id);
	writer.String("custom_color_id");
	Serialize(writer, item.custom_color_id);
	writer.String("team_finish_id");
	Serialize(writer, item.team_finish_id);
	writer.String("custom_finish_id");
	Serialize(writer, item.custom_finish_id);
	writer.EndObject();
}

template<>
inline const ReplicatedDemolish Consume(CPPBitReader<uint32_t>& reader) 
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
	ss << ",\n " << "attacker_velocity = " << ToString<Vector3>(item.attacker_velocity);
	ss << ",\n " << "victim_velocity = " << ToString<Vector3>(item.victim_velocity);
	return ss.str();
}

template<typename Writer>
inline const void Serialize(Writer& writer, const ReplicatedDemolish& item)
{
	writer.StartObject();
	writer.String("unknown1");
	Serialize(writer, item.unknown1);
	writer.String("attacker_actor_id");
	Serialize(writer, item.attacker_actor_id);
	writer.String("unknown2");
	Serialize(writer, item.unknown2);
	writer.String("victim_actor_id");
	Serialize(writer, item.victim_actor_id);
	writer.String("attacker_velocity");
	Serialize(writer, item.attacker_velocity);
	writer.String("victim_velocity");
	Serialize(writer, item.victim_velocity);
	writer.EndObject();
}

template<>
inline const ReplicatedMusicStringer Consume(CPPBitReader<uint32_t>& reader) 
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

template<typename Writer>
inline const void Serialize(Writer& writer, const ReplicatedMusicStringer& item)
{
	writer.StartObject();
	writer.String("unknown1");
	Serialize(writer, item.unknown1);
	writer.String("object_index");
	Serialize(writer, item.object_index);
	writer.String("trigger");
	Serialize(writer, item.trigger);
	writer.EndObject();
}

template<>
inline const PrivateMatchSettings Consume(CPPBitReader<uint32_t>& reader) 
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

template<typename Writer>
inline const void Serialize(Writer& writer, const PrivateMatchSettings& item)
{
	writer.StartObject();
	writer.String("mutators");
	Serialize(writer, item.mutators);
	writer.String("map_name");
	Serialize(writer, item.map_name);
	writer.String("max_player_count");
	Serialize(writer, item.max_player_count);
	writer.String("game_name");
	Serialize(writer, item.game_name);
	writer.String("password");
	Serialize(writer, item.password);
	writer.String("is_public");
	Serialize(writer, item.is_public);
	writer.EndObject();
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

template<typename Writer>
inline const void Serialize(Writer& writer, const ProductAttribute& item)
{
	writer.StartObject();
	writer.String("class_index");
	Serialize(writer, item.class_index);
	writer.String("class_name");
	Serialize(writer, item.class_name);
	writer.String("value");
	Serialize(writer, item.value);
	writer.String("has_value");
	Serialize(writer, item.has_value);
	writer.String("unknown1");
	Serialize(writer, item.unknown1);
	writer.EndObject();
}

template<>
inline const Attributes Consume(CPPBitReader<uint32_t>& reader) 
{
	Attributes item;
	item.product_attributes = ConsumeVector<ProductAttribute>(reader);
	return item;
}

template<>
inline const std::string ToString(const Attributes& item) 
{
	std::stringstream ss;
	ss << "product_attributes = " << ToString<std::vector<ProductAttribute>>(item.product_attributes);
	return ss.str();
}

template<typename Writer>
inline const void Serialize(Writer& writer, const Attributes& item)
{
	writer.StartObject();
	writer.String("product_attributes");
	Serialize(writer, item.product_attributes);
	writer.EndObject();
}

template<>
inline const OnlineLoadout Consume(CPPBitReader<uint32_t>& reader) 
{
	OnlineLoadout item;
	item.attributes_list = ConsumeVector<Attributes>(reader);
	return item;
}

template<>
inline const std::string ToString(const OnlineLoadout& item) 
{
	std::stringstream ss;
	ss << "attributes_list = " << ToString<std::vector<Attributes>>(item.attributes_list);
	return ss.str();
}

template<typename Writer>
inline const void Serialize(Writer& writer, const OnlineLoadout& item)
{
	writer.StartObject();
	writer.String("attributes_list");
	Serialize(writer, item.attributes_list);
	writer.EndObject();
}

template<>
inline const UserColorAttribute Consume(CPPBitReader<uint32_t>& reader) 
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

template<typename Writer>
inline const void Serialize(Writer& writer, const UserColorAttribute& item)
{
	writer.StartObject();
	writer.String("r");
	Serialize(writer, item.r);
	writer.String("g");
	Serialize(writer, item.g);
	writer.String("b");
	Serialize(writer, item.b);
	writer.String("a");
	Serialize(writer, item.a);
	writer.EndObject();
}

template<>
inline const ClientLoadoutsOnline Consume(CPPBitReader<uint32_t>& reader) 
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
	ss << "online_one = " << ToString<OnlineLoadout>(item.online_one);
	ss << ",\n " << "online_two = " << ToString<OnlineLoadout>(item.online_two);
	ss << ",\n " << "loadout_set = " << item.loadout_set;
	ss << ",\n " << "is_deprecated = " << item.is_deprecated;
	return ss.str();
}

template<typename Writer>
inline const void Serialize(Writer& writer, const ClientLoadoutsOnline& item)
{
	writer.StartObject();
	writer.String("online_one");
	Serialize(writer, item.online_one);
	writer.String("online_two");
	Serialize(writer, item.online_two);
	writer.String("loadout_set");
	Serialize(writer, item.loadout_set);
	writer.String("is_deprecated");
	Serialize(writer, item.is_deprecated);
	writer.EndObject();
}

template<>
inline const ClientLoadouts Consume(CPPBitReader<uint32_t>& reader) 
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
	ss << "loadout_one = " << ToString<ClientLoadout>(item.loadout_one);
	ss << ",\n " << "loadout_two = " << ToString<ClientLoadout>(item.loadout_two);
	return ss.str();
}

template<typename Writer>
inline const void Serialize(Writer& writer, const ClientLoadouts& item)
{
	writer.StartObject();
	writer.String("loadout_one");
	Serialize(writer, item.loadout_one);
	writer.String("loadout_two");
	Serialize(writer, item.loadout_two);
	writer.EndObject();
}

template<>
inline const ClubColors Consume(CPPBitReader<uint32_t>& reader) 
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

template<typename Writer>
inline const void Serialize(Writer& writer, const ClubColors& item)
{
	writer.StartObject();
	writer.String("team_color_set");
	Serialize(writer, item.team_color_set);
	writer.String("team_color_id");
	Serialize(writer, item.team_color_id);
	writer.String("custom_color_set");
	Serialize(writer, item.custom_color_set);
	writer.String("custom_color_id");
	Serialize(writer, item.custom_color_id);
	writer.EndObject();
}

template<>
inline const WeldedInfo Consume(CPPBitReader<uint32_t>& reader) 
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
	ss << ",\n " << "offset = " << ToString<Vector3>(item.offset);
	ss << ",\n " << "mass = " << item.mass;
	ss << ",\n " << "rotation = " << ToString<Rotator>(item.rotation);
	return ss.str();
}

template<typename Writer>
inline const void Serialize(Writer& writer, const WeldedInfo& item)
{
	writer.StartObject();
	writer.String("active");
	Serialize(writer, item.active);
	writer.String("actor_id");
	Serialize(writer, item.actor_id);
	writer.String("offset");
	Serialize(writer, item.offset);
	writer.String("mass");
	Serialize(writer, item.mass);
	writer.String("rotation");
	Serialize(writer, item.rotation);
	writer.EndObject();
}

template<>
inline const DamageState Consume(CPPBitReader<uint32_t>& reader) 
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
	ss << ",\n " << "damage_location = " << ToString<Vector3>(item.damage_location);
	ss << ",\n " << "direct_damage = " << item.direct_damage;
	ss << ",\n " << "immediate = " << item.immediate;
	return ss.str();
}

template<typename Writer>
inline const void Serialize(Writer& writer, const DamageState& item)
{
	writer.StartObject();
	writer.String("damage_state");
	Serialize(writer, item.damage_state);
	writer.String("unknown2");
	Serialize(writer, item.unknown2);
	writer.String("causer_actor_id");
	Serialize(writer, item.causer_actor_id);
	writer.String("damage_location");
	Serialize(writer, item.damage_location);
	writer.String("direct_damage");
	Serialize(writer, item.direct_damage);
	writer.String("immediate");
	Serialize(writer, item.immediate);
	writer.EndObject();
}

template<>
inline const AppliedDamage Consume(CPPBitReader<uint32_t>& reader) 
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
	ss << ",\n " << "position = " << ToString<Vector3>(item.position);
	ss << ",\n " << "damage_index = " << item.damage_index;
	ss << ",\n " << "total_damage = " << item.total_damage;
	return ss.str();
}

template<typename Writer>
inline const void Serialize(Writer& writer, const AppliedDamage& item)
{
	writer.StartObject();
	writer.String("id");
	Serialize(writer, item.id);
	writer.String("position");
	Serialize(writer, item.position);
	writer.String("damage_index");
	Serialize(writer, item.damage_index);
	writer.String("total_damage");
	Serialize(writer, item.total_damage);
	writer.EndObject();
}

template<>
inline const ReplicatedExplosionData Consume(CPPBitReader<uint32_t>& reader) 
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
	ss << ",\n " << "position = " << ToString<Vector3>(item.position);
	return ss.str();
}

template<typename Writer>
inline const void Serialize(Writer& writer, const ReplicatedExplosionData& item)
{
	writer.StartObject();
	writer.String("unknown1");
	Serialize(writer, item.unknown1);
	writer.String("actor_id");
	Serialize(writer, item.actor_id);
	writer.String("position");
	Serialize(writer, item.position);
	writer.EndObject();
}

template<>
inline const ReplicatedExplosionDataExtended Consume(CPPBitReader<uint32_t>& reader) 
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
	ss << ",\n " << "position = " << ToString<Vector3>(item.position);
	ss << ",\n " << "unknown3 = " << item.unknown3;
	ss << ",\n " << "unknown4 = " << item.unknown4;
	return ss.str();
}

template<typename Writer>
inline const void Serialize(Writer& writer, const ReplicatedExplosionDataExtended& item)
{
	writer.StartObject();
	writer.String("unknown1");
	Serialize(writer, item.unknown1);
	writer.String("actor_id");
	Serialize(writer, item.actor_id);
	writer.String("position");
	Serialize(writer, item.position);
	writer.String("unknown3");
	Serialize(writer, item.unknown3);
	writer.String("unknown4");
	Serialize(writer, item.unknown4);
	writer.EndObject();
}

template<>
inline const ReplicatedTitle Consume(CPPBitReader<uint32_t>& reader) 
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

template<typename Writer>
inline const void Serialize(Writer& writer, const ReplicatedTitle& item)
{
	writer.StartObject();
	writer.String("unknown1");
	Serialize(writer, item.unknown1);
	writer.String("unknown2");
	Serialize(writer, item.unknown2);
	writer.String("unknown3");
	Serialize(writer, item.unknown3);
	writer.String("unknown4");
	Serialize(writer, item.unknown4);
	writer.String("unknown5");
	Serialize(writer, item.unknown5);
	writer.String("unknown6");
	Serialize(writer, item.unknown6);
	writer.String("unknown7");
	Serialize(writer, item.unknown7);
	writer.String("unknown8");
	Serialize(writer, item.unknown8);
	writer.EndObject();
}

template<>
inline const HistoryKey Consume(CPPBitReader<uint32_t>& reader) 
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

template<typename Writer>
inline const void Serialize(Writer& writer, const HistoryKey& item)
{
	writer.StartObject();
	writer.String("data");
	Serialize(writer, item.data);
	writer.EndObject();
}

template<>
inline const ReplicatedStatEvent Consume(CPPBitReader<uint32_t>& reader) 
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

template<typename Writer>
inline const void Serialize(Writer& writer, const ReplicatedStatEvent& item)
{
	writer.StartObject();
	writer.String("unknown1");
	Serialize(writer, item.unknown1);
	writer.String("object_id");
	Serialize(writer, item.object_id);
	writer.EndObject();
}

template<>
inline const RepStatTitle Consume(CPPBitReader<uint32_t>& reader) 
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
	ss << ",\n " << "object_target = " << ToString<ObjectTarget>(item.object_target);
	ss << ",\n " << "value = " << item.value;
	return ss.str();
}

template<typename Writer>
inline const void Serialize(Writer& writer, const RepStatTitle& item)
{
	writer.StartObject();
	writer.String("unknown1");
	Serialize(writer, item.unknown1);
	writer.String("name");
	Serialize(writer, item.name);
	writer.String("object_target");
	Serialize(writer, item.object_target);
	writer.String("value");
	Serialize(writer, item.value);
	writer.EndObject();
}

template<>
inline const SkillTier Consume(CPPBitReader<uint32_t>& reader) 
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

template<typename Writer>
inline const void Serialize(Writer& writer, const SkillTier& item)
{
	writer.StartObject();
	writer.String("tier");
	Serialize(writer, item.tier);
	writer.EndObject();
}

