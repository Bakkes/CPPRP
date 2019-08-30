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


		template<typename Writer>
		inline const void Serialize(Writer& writer, const Reservation& item)
		{
			writer.StartObject();
			writer.String("unknown");
			Serialize(writer, item.number);
			writer.String("player_id");
			Serialize(writer, item.player_id);
			writer.String("player_name");
			Serialize(writer, item.player_name);
			writer.String("unknown2");
			Serialize(writer, item.unknown2);
			writer.EndObject();
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


		template<typename Writer>
		inline const void Serialize(Writer& writer, const ReplicatedDemolish& item)
		{
			writer.StartObject();
			writer.String("attacker_flag");
			Serialize(writer, item.attacker_flag);
			writer.String("attacker_actor_id");
			Serialize(writer, item.attacker_actor_id);
			writer.String("victim_flag");
			Serialize(writer, item.victim_flag);
			writer.String("victim_actor_id");
			Serialize(writer, item.victim_actor_id);
			writer.String("attacker_velocity");
			Serialize(writer, item.attacker_velocity);
			writer.String("victim_velocity");
			Serialize(writer, item.victim_velocity);
			writer.EndObject();
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


		template<typename Writer>
		inline const void Serialize(Writer& writer, const GameMode& item)
		{
			writer.StartObject();
			writer.String("gamemode");
			Serialize(writer, item.gamemode);
			writer.EndObject();
		}


		template<typename Writer>
		inline const void Serialize(Writer& writer, const ReplicatedStateIndex& item)
		{
			writer.StartObject();
			writer.String("value");
			Serialize(writer, item.value);
			writer.EndObject();
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


		template<typename Writer>
		inline const void Serialize(Writer& writer, const Attributes& item)
		{
			writer.StartObject();
			writer.String("product_attributes");
			Serialize(writer, item.product_attributes);
			writer.EndObject();
		}


		template<typename Writer>
		inline const void Serialize(Writer& writer, const ActorBase& item)
		{
			writer.StartObject();
			writer.String("value");
			Serialize(writer, item.value);
			writer.String("unknown1");
			Serialize(writer, item.unknown1);
			writer.String("unknown2");
			Serialize(writer, item.unknown2);
			writer.EndObject();
		}


		template<typename Writer>
		inline const void Serialize(Writer& writer, const PartyLeader& item)
		{
			writer.StartObject();
			writer.String("id");
			Serialize(writer, item.id);
			writer.EndObject();
		}


		template<typename Writer>
		inline const void Serialize(Writer& writer, const OnlineLoadout& item)
		{
			writer.StartObject();
			writer.String("attributes_list");
			Serialize(writer, item.attributes_list);
			writer.EndObject();
		}


		template<typename Writer>
		inline const void Serialize(Writer& writer, const ProductAttributeUserColorSingle& item)
		{
			writer.StartObject();
			writer.String("hasValue");
			Serialize(writer, item.hasValue);
			writer.String("value");
			Serialize(writer, item.value);
			writer.EndObject();
		}


		template<typename Writer>
		inline const void Serialize(Writer& writer, const ProductAttributeUserColorRGB& item)
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


		template<typename Writer>
		inline const void Serialize(Writer& writer, const ProductAttributeTitle& item)
		{
			writer.StartObject();
			writer.String("title");
			Serialize(writer, item.title);
			writer.EndObject();
		}


		template<typename Writer>
		inline const void Serialize(Writer& writer, const ProductAttributeSingleValue& item)
		{
			writer.StartObject();
			writer.String("value");
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


		template<typename Writer>
		inline const void Serialize(Writer& writer, const HistoryKey& item)
		{
			writer.StartObject();
			writer.String("data");
			Serialize(writer, item.data);
			writer.EndObject();
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


		template<typename Writer>
		inline const void Serialize(Writer& writer, const SkillTier& item)
		{
			writer.StartObject();
			writer.String("tier");
			Serialize(writer, item.tier);
			writer.EndObject();
		}
