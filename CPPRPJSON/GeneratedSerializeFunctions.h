template<typename Writer>
inline const void Serialize(Writer& writer, const ReplicatedRBState& item)
{
	writer.StartObject();
	writer.Key("rotation");
	Serialize(writer, item.rotation);
	writer.Key("position");
	Serialize(writer, item.position);
	writer.Key("linear_velocity");
	Serialize(writer, item.linear_velocity);
	writer.Key("angular_velocity");
	Serialize(writer, item.angular_velocity);
	writer.Key("sleeping");
	Serialize(writer, item.sleeping);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const GameServer& item)
{
	writer.StartObject();
	writer.Key("GameServerID");
	Serialize(writer, item.GameServerID);
	writer.Key("GameServerIDString");
	Serialize(writer, item.GameServerIDString);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const LogoData& item)
{
	writer.StartObject();
	writer.Key("swap_colors");
	Serialize(writer, item.swap_colors);
	writer.Key("logo_id");
	Serialize(writer, item.logo_id);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const ActiveActor& item)
{
	writer.StartObject();
	writer.Key("active");
	Serialize(writer, item.active);
	writer.Key("actor_id");
	Serialize(writer, item.actor_id);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const ObjectTarget& item)
{
	writer.StartObject();
	writer.Key("unknown");
	Serialize(writer, item.unknown);
	writer.Key("object_index");
	Serialize(writer, item.object_index);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const Reservation& item)
{
	writer.StartObject();
	writer.Key("number");
	Serialize(writer, item.number);
	writer.Key("player_id");
	Serialize(writer, item.player_id);
	writer.Key("player_name");
	Serialize(writer, item.player_name);
	writer.Key("status");
	Serialize(writer, item.status);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const ClientLoadout& item)
{
	writer.StartObject();
	writer.Key("version");
	Serialize(writer, item.version);
	writer.Key("body");
	Serialize(writer, item.body);
	writer.Key("skin");
	Serialize(writer, item.skin);
	writer.Key("wheels");
	Serialize(writer, item.wheels);
	writer.Key("boost");
	Serialize(writer, item.boost);
	writer.Key("antenna");
	Serialize(writer, item.antenna);
	writer.Key("hat");
	Serialize(writer, item.hat);
	writer.Key("unknown2");
	Serialize(writer, item.unknown2);
	writer.Key("unknown3");
	Serialize(writer, item.unknown3);
	writer.Key("engine_audio");
	Serialize(writer, item.engine_audio);
	writer.Key("trail");
	Serialize(writer, item.trail);
	writer.Key("goal_explosion");
	Serialize(writer, item.goal_explosion);
	writer.Key("banner");
	Serialize(writer, item.banner);
	writer.Key("unknown4");
	Serialize(writer, item.unknown4);
	writer.Key("unknown5");
	Serialize(writer, item.unknown5);
	writer.Key("unknown6");
	Serialize(writer, item.unknown6);
	writer.Key("unknown7");
	Serialize(writer, item.unknown7);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const CameraSettings& item)
{
	writer.StartObject();
	writer.Key("FOV");
	Serialize(writer, item.FOV);
	writer.Key("height");
	Serialize(writer, item.height);
	writer.Key("pitch");
	Serialize(writer, item.pitch);
	writer.Key("distance");
	Serialize(writer, item.distance);
	writer.Key("stiffness");
	Serialize(writer, item.stiffness);
	writer.Key("swivelspeed");
	Serialize(writer, item.swivelspeed);
	writer.Key("transitionspeed");
	Serialize(writer, item.transitionspeed);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const ReplicatedPickupData& item)
{
	writer.StartObject();
	writer.Key("unknown1");
	Serialize(writer, item.unknown1);
	writer.Key("actor_id");
	Serialize(writer, item.actor_id);
	writer.Key("picked_up");
	Serialize(writer, item.picked_up);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const ReplicatedPickupData2& item)
{
	writer.StartObject();
	writer.Key("unknown1");
	Serialize(writer, item.unknown1);
	writer.Key("actor_id");
	Serialize(writer, item.actor_id);
	writer.Key("picked_up");
	Serialize(writer, item.picked_up);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const TeamPaint& item)
{
	writer.StartObject();
	writer.Key("team_number");
	Serialize(writer, item.team_number);
	writer.Key("team_color_id");
	Serialize(writer, item.team_color_id);
	writer.Key("custom_color_id");
	Serialize(writer, item.custom_color_id);
	writer.Key("team_finish_id");
	Serialize(writer, item.team_finish_id);
	writer.Key("custom_finish_id");
	Serialize(writer, item.custom_finish_id);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const ReplicatedDemolish& item)
{
	writer.StartObject();
	writer.Key("attacker_flag");
	Serialize(writer, item.attacker_flag);
	writer.Key("attacker_actor_id");
	Serialize(writer, item.attacker_actor_id);
	writer.Key("victim_flag");
	Serialize(writer, item.victim_flag);
	writer.Key("victim_actor_id");
	Serialize(writer, item.victim_actor_id);
	writer.Key("attacker_velocity");
	Serialize(writer, item.attacker_velocity);
	writer.Key("victim_velocity");
	Serialize(writer, item.victim_velocity);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const ReplicatedDemolish2& item)
{
	writer.StartObject();
	writer.Key("custom_demo_flag");
	Serialize(writer, item.custom_demo_flag);
	writer.Key("custom_demo_id");
	Serialize(writer, item.custom_demo_id);
	writer.Key("attacker_flag");
	Serialize(writer, item.attacker_flag);
	writer.Key("attacker_actor_id");
	Serialize(writer, item.attacker_actor_id);
	writer.Key("victim_flag");
	Serialize(writer, item.victim_flag);
	writer.Key("victim_actor_id");
	Serialize(writer, item.victim_actor_id);
	writer.Key("attacker_velocity");
	Serialize(writer, item.attacker_velocity);
	writer.Key("victim_velocity");
	Serialize(writer, item.victim_velocity);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const DemolishDataGoalExplosion& item)
{
	writer.StartObject();
	writer.Key("goal_explosion_owner_flag");
	Serialize(writer, item.goal_explosion_owner_flag);
	writer.Key("goal_explosion_owner");
	Serialize(writer, item.goal_explosion_owner);
	writer.Key("attacker_flag");
	Serialize(writer, item.attacker_flag);
	writer.Key("attacker_actor_id");
	Serialize(writer, item.attacker_actor_id);
	writer.Key("victim_flag");
	Serialize(writer, item.victim_flag);
	writer.Key("victim_actor_id");
	Serialize(writer, item.victim_actor_id);
	writer.Key("attacker_velocity");
	Serialize(writer, item.attacker_velocity);
	writer.Key("victim_velocity");
	Serialize(writer, item.victim_velocity);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const ReplicatedMusicStringer& item)
{
	writer.StartObject();
	writer.Key("unknown1");
	Serialize(writer, item.unknown1);
	writer.Key("object_index");
	Serialize(writer, item.object_index);
	writer.Key("trigger");
	Serialize(writer, item.trigger);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const GameMode& item)
{
	writer.StartObject();
	writer.Key("gamemode");
	Serialize(writer, item.gamemode);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const ReplicatedStateIndex& item)
{
	writer.StartObject();
	writer.Key("value");
	Serialize(writer, item.value);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const PrivateMatchSettings& item)
{
	writer.StartObject();
	writer.Key("mutators");
	Serialize(writer, item.mutators);
	writer.Key("map_name");
	Serialize(writer, item.map_name);
	writer.Key("max_player_count");
	Serialize(writer, item.max_player_count);
	writer.Key("game_name");
	Serialize(writer, item.game_name);
	writer.Key("password");
	Serialize(writer, item.password);
	writer.Key("is_public");
	Serialize(writer, item.is_public);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const ProductAttribute& item)
{
	writer.StartObject();
	writer.Key("class_index");
	Serialize(writer, item.class_index);
	writer.Key("class_name");
	Serialize(writer, item.class_name);
	writer.Key("value");
	Serialize(writer, item.value);
	writer.Key("has_value");
	Serialize(writer, item.has_value);
	writer.Key("unknown1");
	Serialize(writer, item.unknown1);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const ActorBase& item)
{
	writer.StartObject();
	writer.Key("value");
	Serialize(writer, item.value);
	writer.Key("unknown1");
	Serialize(writer, item.unknown1);
	writer.Key("unknown2");
	Serialize(writer, item.unknown2);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const PartyLeader& item)
{
	writer.StartObject();
	writer.Key("id");
	Serialize(writer, item.id);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const ProductAttributeUserColorSingle& item)
{
	writer.StartObject();
	writer.Key("hasValue");
	Serialize(writer, item.hasValue);
	writer.Key("value");
	Serialize(writer, item.value);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const ProductAttributeUserColorRGB& item)
{
	writer.StartObject();
	writer.Key("r");
	Serialize(writer, item.r);
	writer.Key("g");
	Serialize(writer, item.g);
	writer.Key("b");
	Serialize(writer, item.b);
	writer.Key("a");
	Serialize(writer, item.a);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const ProductAttributeTitle& item)
{
	writer.StartObject();
	writer.Key("title");
	Serialize(writer, item.title);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const ProductAttributeSingleValue& item)
{
	writer.StartObject();
	writer.Key("value");
	Serialize(writer, item.value);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const ProductAttributePainted& item)
{
	writer.StartObject();
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const ProductAttributeTeamEdition& item)
{
	writer.StartObject();
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const ProductAttributeSpecialEdition& item)
{
	writer.StartObject();
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const Attributes& item)
{
	writer.StartObject();
	writer.Key("product_attributes");
	Serialize(writer, item.product_attributes);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const OnlineLoadout& item)
{
	writer.StartObject();
	writer.Key("attributes_list");
	Serialize(writer, item.attributes_list);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const ClientLoadoutsOnline& item)
{
	writer.StartObject();
	writer.Key("online_one");
	Serialize(writer, item.online_one);
	writer.Key("online_two");
	Serialize(writer, item.online_two);
	writer.Key("loadout_set");
	Serialize(writer, item.loadout_set);
	writer.Key("is_deprecated");
	Serialize(writer, item.is_deprecated);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const ClientLoadouts& item)
{
	writer.StartObject();
	writer.Key("loadout_one");
	Serialize(writer, item.loadout_one);
	writer.Key("loadout_two");
	Serialize(writer, item.loadout_two);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const ClubColors& item)
{
	writer.StartObject();
	writer.Key("team_color_set");
	Serialize(writer, item.team_color_set);
	writer.Key("team_color_id");
	Serialize(writer, item.team_color_id);
	writer.Key("custom_color_set");
	Serialize(writer, item.custom_color_set);
	writer.Key("custom_color_id");
	Serialize(writer, item.custom_color_id);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const WeldedInfo& item)
{
	writer.StartObject();
	writer.Key("active");
	Serialize(writer, item.active);
	writer.Key("actor_id");
	Serialize(writer, item.actor_id);
	writer.Key("offset");
	Serialize(writer, item.offset);
	writer.Key("mass");
	Serialize(writer, item.mass);
	writer.Key("rotation");
	Serialize(writer, item.rotation);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const DamageState& item)
{
	writer.StartObject();
	writer.Key("damage_state");
	Serialize(writer, item.damage_state);
	writer.Key("unknown2");
	Serialize(writer, item.unknown2);
	writer.Key("causer_actor_id");
	Serialize(writer, item.causer_actor_id);
	writer.Key("damage_location");
	Serialize(writer, item.damage_location);
	writer.Key("direct_damage");
	Serialize(writer, item.direct_damage);
	writer.Key("immediate");
	Serialize(writer, item.immediate);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const AppliedDamage& item)
{
	writer.StartObject();
	writer.Key("id");
	Serialize(writer, item.id);
	writer.Key("position");
	Serialize(writer, item.position);
	writer.Key("damage_index");
	Serialize(writer, item.damage_index);
	writer.Key("total_damage");
	Serialize(writer, item.total_damage);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const ReplicatedExplosionData& item)
{
	writer.StartObject();
	writer.Key("unknown1");
	Serialize(writer, item.unknown1);
	writer.Key("actor_id");
	Serialize(writer, item.actor_id);
	writer.Key("position");
	Serialize(writer, item.position);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const ReplicatedExplosionDataExtended& item)
{
	writer.StartObject();
	writer.Key("unknown1");
	Serialize(writer, item.unknown1);
	writer.Key("actor_id");
	Serialize(writer, item.actor_id);
	writer.Key("position");
	Serialize(writer, item.position);
	writer.Key("unknown3");
	Serialize(writer, item.unknown3);
	writer.Key("secondary_actor_id");
	Serialize(writer, item.secondary_actor_id);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const ReplicatedTitle& item)
{
	writer.StartObject();
	writer.Key("unknown1");
	Serialize(writer, item.unknown1);
	writer.Key("unknown2");
	Serialize(writer, item.unknown2);
	writer.Key("unknown3");
	Serialize(writer, item.unknown3);
	writer.Key("unknown4");
	Serialize(writer, item.unknown4);
	writer.Key("unknown5");
	Serialize(writer, item.unknown5);
	writer.Key("unknown6");
	Serialize(writer, item.unknown6);
	writer.Key("unknown7");
	Serialize(writer, item.unknown7);
	writer.Key("unknown8");
	Serialize(writer, item.unknown8);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const ImpulseData& item)
{
	writer.StartObject();
	writer.Key("CompressedRotation");
	Serialize(writer, item.CompressedRotation);
	writer.Key("ImpulseSpeed");
	Serialize(writer, item.ImpulseSpeed);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const HistoryKey& item)
{
	writer.StartObject();
	writer.Key("data");
	Serialize(writer, item.data);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const ReplicatedStatEvent& item)
{
	writer.StartObject();
	writer.Key("unknown1");
	Serialize(writer, item.unknown1);
	writer.Key("object_id");
	Serialize(writer, item.object_id);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const RepStatTitle& item)
{
	writer.StartObject();
	writer.Key("unknown1");
	Serialize(writer, item.unknown1);
	writer.Key("name");
	Serialize(writer, item.name);
	writer.Key("object_target");
	Serialize(writer, item.object_target);
	writer.Key("value");
	Serialize(writer, item.value);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const SkillTier& item)
{
	writer.StartObject();
	writer.Key("tier");
	Serialize(writer, item.tier);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const RigidBodyState& item)
{
	writer.StartObject();
	writer.Key("position");
	Serialize(writer, item.position);
	writer.Key("lin_vel");
	Serialize(writer, item.lin_vel);
	writer.Key("quaternion");
	Serialize(writer, item.quaternion);
	writer.Key("ang_vel");
	Serialize(writer, item.ang_vel);
	writer.Key("flags");
	Serialize(writer, item.flags);
	writer.EndObject();
}


template<typename Writer>
inline const void Serialize(Writer& writer, const PickupInfo_TA& item)
{
	writer.StartObject();
	writer.Key("AvailablePickups");
	Serialize(writer, item.AvailablePickups);
	writer.Key("unknown");
	Serialize(writer, item.unknown);
	writer.Key("bItemsArePreview");
	Serialize(writer, item.bItemsArePreview);
	writer.EndObject();
}


