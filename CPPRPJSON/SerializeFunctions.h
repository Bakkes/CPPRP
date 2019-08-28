#pragma once
#include "../CPPRP/data/ReplayFileData.h"
#include "../CPPRP/data/NetworkData.h"
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
namespace CPPRP
{
	namespace JSON
	{
		typedef rapidjson::Writer<rapidjson::StringBuffer> Writer;

		template<typename T>
		inline const void Serialize(Writer& writer, const T& item)
		{
			std::stringstream ss;
			ss << "unsupported " << typeid(T).name();
			std::string o = ss.str();
			printf("%s\n", o.c_str());
			//assert(1 == 2);
			writer.String(o.c_str(), o.size());
		}

		//template<typename Writer>
		//inline const void Serialize(Writer& writer, const UniqueId& item)
		//{
		//	writer.StartObject();
		//	writer.String("platform");
		//	writer.Uint(item.platform);
		//	writer.String("playernumber");
		//	writer.Uint(item.playerNumber);
		//	writer.String("uniqueid");
		//	//writer.Uint64(item.uniqueID);
		//	writer.EndObject();
		//}

		template<>
		inline const void Serialize(Writer& writer, const uint64_t& item)
		{
			writer.Uint64(item);
		}

		template<>
		inline const void Serialize(Writer& writer, const int64_t& item)
		{
			writer.Int64(item);
		}

		template<>
		inline const void Serialize(Writer& writer, const uint8_t& item)
		{
			writer.Uint(item);
		}

		template<>
		inline const void Serialize(Writer& writer, const char& item)
		{
			writer.Int((int)item);
		}

		template<>
		inline const void Serialize(Writer& writer, const float& item)
		{
			writer.Double(item);
		}

		template<>
		inline const void Serialize(Writer& writer, const std::string& item)
		{
			writer.String(item.c_str(), item.size());
		}

		template<>
		inline const void Serialize(Writer& writer, const Vector3& item)
		{
			writer.StartObject();
			writer.String("X");
			writer.Double(item.x);
			writer.String("Y");
			writer.Double(item.y);
			writer.String("Z");
			writer.Double(item.z);
			writer.EndObject();
		}

		template<>
		inline const void Serialize(Writer& writer, const Vector3I& item)
		{
			writer.StartObject();
			writer.String("X");
			writer.Int(item.x);
			writer.String("Y");
			writer.Int(item.y);
			writer.String("Z");
			writer.Int(item.z);
			writer.EndObject();
		}

		template<>
		inline const void Serialize(Writer& writer, const struct Quat& item)
		{
			writer.StartObject();
			writer.String("X");
			writer.Double(item.x);
			writer.String("Y");
			writer.Double(item.y);
			writer.String("Z");
			writer.Double(item.z);
			writer.String("W");
			writer.Double(item.w);
			writer.EndObject();
		}

		template<>
		inline const void Serialize(Writer& writer, const Rotator& item)
		{
			writer.StartObject();
			writer.String("Pitch");
			writer.Int(item.pitch);
			writer.String("Yaw");
			writer.Int(item.yaw);
			writer.String("Roll");
			writer.Int(item.roll);
			writer.EndObject();
		}

		template<typename T>
		inline const void Serialize(Writer& writer, const std::vector<T>& item) {
			writer.StartArray();
			const size_t size = item.size();
			for (size_t i = 0; i < size; ++i)
			{
				Serialize(writer, item.at(i));
			}
			writer.EndArray();
		}

		template<>
		inline const void Serialize(Writer& writer, const bool& item)
		{
			writer.Bool(item);
		}

		template<>
		inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::UniqueId>& item)
		{
			writer.StartObject();
			writer.String("Platform");
			writer.Uint(item->platform);
			writer.String("PlayerNumber");
			writer.Uint(item->playerNumber);
			writer.String("UniqueID");
			if (std::shared_ptr<CPPRP::SteamID> steamId = std::dynamic_pointer_cast<CPPRP::SteamID>(item); steamId)
			{
				writer.Uint64(steamId->steamID);
			}
			else if (std::shared_ptr<CPPRP::XBoxID> xboxId = std::dynamic_pointer_cast<CPPRP::XBoxID>(item); xboxId)
			{
				writer.Uint64(xboxId->xboxID);
			}
			else if (std::shared_ptr<CPPRP::SwitchID> switchId = std::dynamic_pointer_cast<CPPRP::SwitchID>(item); switchId)
			{
				writer.StartArray();
				writer.Uint64(switchId->a);
				writer.Uint64(switchId->b);
				writer.Uint64(switchId->c);
				writer.Uint64(switchId->d);
				writer.EndArray();
			}
			else if (std::shared_ptr<CPPRP::PS4ID> ps4Id = std::dynamic_pointer_cast<CPPRP::PS4ID>(item); ps4Id)
			{
				writer.Uint64(ps4Id->psId);
			}
			else if (std::shared_ptr<CPPRP::PsyNetID> psynetId = std::dynamic_pointer_cast<CPPRP::PsyNetID>(item); psynetId)
			{
				writer.StartArray();
				writer.Uint64(psynetId->a);
				writer.Uint64(psynetId->b);
				writer.Uint64(psynetId->c);
				writer.Uint64(psynetId->d);
				writer.EndArray();
			}
			else
			{
				writer.Uint64(1337);
			}
			writer.EndObject();
		}

		//template<>
		//inline const void Serialize(Writer& writer, const uint8_t& item)
		//{
		//	writer.Uint(item);
		//}

		template<>
		inline const void Serialize(Writer& writer, const uint16_t& item)
		{
			writer.Uint(item);
		}

		template<>
		inline const void Serialize(Writer& writer, const uint32_t& item)
		{
			writer.Uint(item);
		}

		template<>
		inline const void Serialize(Writer& writer, const int8_t& item)
		{
			writer.Int(item);
		}

		template<>
		inline const void Serialize(Writer& writer, const int16_t& item)
		{
			writer.Int(item);
		}

		template<>
		inline const void Serialize(Writer& writer, const int32_t& item)
		{
			writer.Int(item);
		}


		template<>
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
		inline const void Serialize(Writer& writer, const GameMode& item)
		{
			writer.StartObject();
			writer.String("gamemode");
			Serialize(writer, item.gamemode);
			writer.EndObject();
		}


		template<>
		inline const void Serialize(Writer& writer, const ReplicatedStateIndex& item)
		{
			writer.StartObject();
			writer.String("value");
			Serialize(writer, item.value);
			writer.EndObject();
		}


		template<>
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
		inline const void Serialize(Writer& writer, const Attributes& item)
		{
			writer.StartObject();
			writer.String("product_attributes");
			Serialize(writer, item.product_attributes);
			writer.EndObject();
		}


		template<>
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


		template<>
		inline const void Serialize(Writer& writer, const PartyLeader& item)
		{
			writer.StartObject();
			writer.String("id");
			Serialize(writer, item.id);
			writer.EndObject();
		}


		template<>
		inline const void Serialize(Writer& writer, const OnlineLoadout& item)
		{
			writer.StartObject();
			writer.String("attributes_list");
			Serialize(writer, item.attributes_list);
			writer.EndObject();
		}


		template<>
		inline const void Serialize(Writer& writer, const ProductAttributeUserColorSingle& item)
		{
			writer.StartObject();
			writer.String("hasValue");
			Serialize(writer, item.hasValue);
			writer.String("value");
			Serialize(writer, item.value);
			writer.EndObject();
		}


		template<>
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


		template<>
		inline const void Serialize(Writer& writer, const ProductAttributeTitle& item)
		{
			writer.StartObject();
			writer.String("title");
			Serialize(writer, item.title);
			writer.EndObject();
		}


		template<>
		inline const void Serialize(Writer& writer, const ProductAttributeSingleValue& item)
		{
			writer.StartObject();
			writer.String("value");
			Serialize(writer, item.value);
			writer.EndObject();
		}


		template<>
		inline const void Serialize(Writer& writer, const ProductAttributePainted& item)
		{
			writer.StartObject();
			writer.EndObject();
		}


		template<>
		inline const void Serialize(Writer& writer, const ProductAttributeTeamEdition& item)
		{
			writer.StartObject();
			writer.EndObject();
		}


		template<>
		inline const void Serialize(Writer& writer, const ProductAttributeSpecialEdition& item)
		{
			writer.StartObject();
			writer.EndObject();
		}


		template<>
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
		inline const void Serialize(Writer& writer, const HistoryKey& item)
		{
			writer.StartObject();
			writer.String("data");
			Serialize(writer, item.data);
			writer.EndObject();
		}


		template<>
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
		inline const void Serialize(Writer& writer, const SkillTier& item)
		{
			writer.StartObject();
			writer.String("tier");
			Serialize(writer, item.tier);
			writer.EndObject();
		}

		


			
			typedef std::function<void(Writer&, std::shared_ptr<Engine::Actor>&)> writerFunc;
			static std::unordered_map<std::string, writerFunc> writerFuncs;

			template<typename T>
			inline static void RegisterSerializer(const std::string& str, T callback)
			{
				writerFuncs[str] = callback;
			}

			static inline int Test()
			{
				
				RegisterSerializer("Engine.Actor:DrawScale", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.Actor:DrawScale"); Serialize<float>(writer, std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->DrawScale); });
				RegisterSerializer("Engine.Actor:bCollideActors", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.Actor:bCollideActors"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->bCollideActors); });
				RegisterSerializer("Engine.Actor:bCollideWorld", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.Actor:bCollideWorld"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->bCollideWorld); });
				RegisterSerializer("Engine.Actor:bNetOwner", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.Actor:bNetOwner"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->bNetOwner); });
				RegisterSerializer("Engine.Actor:Base", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.Actor:Base"); Serialize<struct ActorBase>(writer, std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->Base); });
				RegisterSerializer("Engine.Actor:bBlockActors", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.Actor:bBlockActors"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->bBlockActors); });
				RegisterSerializer("Engine.Actor:bHidden", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.Actor:bHidden"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->bHidden); });
				RegisterSerializer("Engine.Actor:bTearOff", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.Actor:bTearOff"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->bTearOff); });
				RegisterSerializer("Engine.Actor:Location", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.Actor:Location"); Serialize<struct Vector3I>(writer, std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->Location); });
				RegisterSerializer("Engine.Actor:Rotation", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.Actor:Rotation"); Serialize<struct Rotator>(writer, std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->Rotation); });
				RegisterSerializer("Engine.GameReplicationInfo:ServerName", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.GameReplicationInfo:ServerName"); Serialize<std::string>(writer, std::static_pointer_cast<CPPRP::Engine::GameReplicationInfo>(struc)->ServerName); });
				RegisterSerializer("Engine.GameReplicationInfo:GameClass", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.GameReplicationInfo:GameClass"); Serialize<struct ObjectTarget>(writer, std::static_pointer_cast<CPPRP::Engine::GameReplicationInfo>(struc)->GameClass); });
				RegisterSerializer("Engine.GameReplicationInfo:bStopCountDown", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.GameReplicationInfo:bStopCountDown"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::Engine::GameReplicationInfo>(struc)->bStopCountDown); });
				RegisterSerializer("Engine.GameReplicationInfo:bMatchIsOver", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.GameReplicationInfo:bMatchIsOver"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::Engine::GameReplicationInfo>(struc)->bMatchIsOver); });
				RegisterSerializer("Engine.GameReplicationInfo:bMatchHasBegun", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.GameReplicationInfo:bMatchHasBegun"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::Engine::GameReplicationInfo>(struc)->bMatchHasBegun); });
				RegisterSerializer("Engine.Pawn:PlayerReplicationInfo", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.Pawn:PlayerReplicationInfo"); Serialize<struct ActiveActor>(writer, std::static_pointer_cast<CPPRP::Engine::Pawn>(struc)->PlayerReplicationInfo); });
				RegisterSerializer("Engine.Pawn:HealthMax", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.Pawn:HealthMax"); Serialize<uint32_t>(writer, std::static_pointer_cast<CPPRP::Engine::Pawn>(struc)->HealthMax); });
				RegisterSerializer("Engine.Pawn:bIsCrouched", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.Pawn:bIsCrouched"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::Engine::Pawn>(struc)->bIsCrouched); });
				RegisterSerializer("Engine.PlayerReplicationInfo:Team", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.PlayerReplicationInfo:Team"); Serialize<struct ActiveActor>(writer, std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->Team); });
				RegisterSerializer("Engine.PlayerReplicationInfo:bReadyToPlay", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.PlayerReplicationInfo:bReadyToPlay"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->bReadyToPlay); });
				RegisterSerializer("Engine.PlayerReplicationInfo:PlayerName", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.PlayerReplicationInfo:PlayerName"); Serialize<std::string>(writer, std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->PlayerName); });
				RegisterSerializer("Engine.PlayerReplicationInfo:RemoteUserData", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.PlayerReplicationInfo:RemoteUserData"); Serialize<std::string>(writer, std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->RemoteUserData); });
				RegisterSerializer("Engine.PlayerReplicationInfo:bWaitingPlayer", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.PlayerReplicationInfo:bWaitingPlayer"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->bWaitingPlayer); });
				RegisterSerializer("Engine.PlayerReplicationInfo:Score", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.PlayerReplicationInfo:Score"); Serialize<uint32_t>(writer, std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->Score); });
				RegisterSerializer("Engine.PlayerReplicationInfo:PlayerID", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.PlayerReplicationInfo:PlayerID"); Serialize<uint32_t>(writer, std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->PlayerID); });
				RegisterSerializer("Engine.PlayerReplicationInfo:bBot", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.PlayerReplicationInfo:bBot"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->bBot); });
				RegisterSerializer("Engine.PlayerReplicationInfo:bIsSpectator", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.PlayerReplicationInfo:bIsSpectator"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->bIsSpectator); });
				RegisterSerializer("Engine.PlayerReplicationInfo:bTimedOut", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.PlayerReplicationInfo:bTimedOut"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->bTimedOut); });
				RegisterSerializer("Engine.PlayerReplicationInfo:bAdmin", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.PlayerReplicationInfo:bAdmin"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->bAdmin); });
				RegisterSerializer("Engine.PlayerReplicationInfo:bIsInactive", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.PlayerReplicationInfo:bIsInactive"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->bIsInactive); });
				RegisterSerializer("Engine.PlayerReplicationInfo:bOnlySpectator", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.PlayerReplicationInfo:bOnlySpectator"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->bOnlySpectator); });
				RegisterSerializer("Engine.PlayerReplicationInfo:Ping", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.PlayerReplicationInfo:Ping"); Serialize<unsigned char>(writer, std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->Ping); });
				RegisterSerializer("Engine.PlayerReplicationInfo:UniqueId", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.PlayerReplicationInfo:UniqueId"); Serialize<class std::shared_ptr<struct UniqueId>>(writer, std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->UniqueId); });
				RegisterSerializer("Engine.TeamInfo:Score", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.TeamInfo:Score"); Serialize<uint32_t>(writer, std::static_pointer_cast<CPPRP::Engine::TeamInfo>(struc)->Score); });
				RegisterSerializer("Engine.WorldInfo:WorldGravityZ", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.WorldInfo:WorldGravityZ"); Serialize<float>(writer, std::static_pointer_cast<CPPRP::Engine::WorldInfo>(struc)->WorldGravityZ); });
				RegisterSerializer("Engine.WorldInfo:TimeDilation", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.WorldInfo:TimeDilation"); Serialize<float>(writer, std::static_pointer_cast<CPPRP::Engine::WorldInfo>(struc)->TimeDilation); });
				RegisterSerializer("ProjectX.GRI_X:MatchGUID", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("ProjectX.GRI_X:MatchGUID"); Serialize<std::string>(writer, std::static_pointer_cast<CPPRP::ProjectX::GRI_X>(struc)->MatchGUID); });
				RegisterSerializer("ProjectX.GRI_X:ReplicatedGameMutatorIndex", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("ProjectX.GRI_X:ReplicatedGameMutatorIndex"); Serialize<int>(writer, std::static_pointer_cast<CPPRP::ProjectX::GRI_X>(struc)->ReplicatedGameMutatorIndex); });
				RegisterSerializer("ProjectX.GRI_X:bGameStarted", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("ProjectX.GRI_X:bGameStarted"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::ProjectX::GRI_X>(struc)->bGameStarted); });
				RegisterSerializer("ProjectX.GRI_X:ReplicatedGamePlaylist", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("ProjectX.GRI_X:ReplicatedGamePlaylist"); Serialize<uint32_t>(writer, std::static_pointer_cast<CPPRP::ProjectX::GRI_X>(struc)->ReplicatedGamePlaylist); });
				RegisterSerializer("ProjectX.GRI_X:GameServerID", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("ProjectX.GRI_X:GameServerID"); Serialize<uint64_t>(writer, std::static_pointer_cast<CPPRP::ProjectX::GRI_X>(struc)->GameServerID); });
				RegisterSerializer("ProjectX.GRI_X:Reservations", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("ProjectX.GRI_X:Reservations"); Serialize<struct Reservation>(writer, std::static_pointer_cast<CPPRP::ProjectX::GRI_X>(struc)->Reservations); });
				RegisterSerializer("TAGame.PRI_TA:MatchShots", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:MatchShots"); Serialize<uint32_t>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->MatchShots); });
				RegisterSerializer("TAGame.PRI_TA:PersistentCamera", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:PersistentCamera"); Serialize<struct ActiveActor>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->PersistentCamera); });
				RegisterSerializer("TAGame.PRI_TA:SkillTier", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:SkillTier"); Serialize<struct SkillTier>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->SkillTier); });
				RegisterSerializer("TAGame.PRI_TA:bUsingBehindView", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:bUsingBehindView"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bUsingBehindView); });
				RegisterSerializer("TAGame.PRI_TA:MatchAssists", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:MatchAssists"); Serialize<uint32_t>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->MatchAssists); });
				RegisterSerializer("TAGame.PRI_TA:RespawnTimeRemaining", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:RespawnTimeRemaining"); Serialize<int>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->RespawnTimeRemaining); });
				RegisterSerializer("TAGame.PRI_TA:bOnlineLoadoutSet", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:bOnlineLoadoutSet"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bOnlineLoadoutSet); });
				RegisterSerializer("TAGame.PRI_TA:MatchGoals", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:MatchGoals"); Serialize<uint32_t>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->MatchGoals); });
				RegisterSerializer("TAGame.PRI_TA:ReplicatedGameEvent", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:ReplicatedGameEvent"); Serialize<struct ActiveActor>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->ReplicatedGameEvent); });
				RegisterSerializer("TAGame.PRI_TA:TotalXP", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:TotalXP"); Serialize<uint32_t>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->TotalXP); });
				RegisterSerializer("TAGame.PRI_TA:MatchScore", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:MatchScore"); Serialize<uint32_t>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->MatchScore); });
				RegisterSerializer("TAGame.PRI_TA:MatchSaves", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:MatchSaves"); Serialize<uint32_t>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->MatchSaves); });
				RegisterSerializer("TAGame.PRI_TA:Title", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:Title"); Serialize<uint32_t>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->Title); });
				RegisterSerializer("TAGame.PRI_TA:ClubID", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:ClubID"); Serialize<uint64_t>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->ClubID); });
				RegisterSerializer("TAGame.PRI_TA:MaxTimeTillItem", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:MaxTimeTillItem"); Serialize<int>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->MaxTimeTillItem); });
				RegisterSerializer("TAGame.PRI_TA:MatchBreakoutDamage", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:MatchBreakoutDamage"); Serialize<uint32_t>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->MatchBreakoutDamage); });
				RegisterSerializer("TAGame.PRI_TA:BotProductName", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:BotProductName"); Serialize<uint32_t>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->BotProductName); });
				RegisterSerializer("TAGame.PRI_TA:bReady", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:bReady"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bReady); });
				RegisterSerializer("TAGame.PRI_TA:SpectatorShortcut", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:SpectatorShortcut"); Serialize<uint32_t>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->SpectatorShortcut); });
				RegisterSerializer("TAGame.PRI_TA:bUsingSecondaryCamera", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:bUsingSecondaryCamera"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bUsingSecondaryCamera); });
				RegisterSerializer("TAGame.PRI_TA:PlayerHistoryValid", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:PlayerHistoryValid"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->PlayerHistoryValid); });
				RegisterSerializer("TAGame.PRI_TA:bIsInSplitScreen", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:bIsInSplitScreen"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bIsInSplitScreen); });
				RegisterSerializer("TAGame.PRI_TA:bMatchMVP", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:bMatchMVP"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bMatchMVP); });
				RegisterSerializer("TAGame.PRI_TA:RepStatTitles", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:RepStatTitles"); Serialize<struct RepStatTitle>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->RepStatTitles); });
				RegisterSerializer("TAGame.PRI_TA:bOnlineLoadoutsSet", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:bOnlineLoadoutsSet"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bOnlineLoadoutsSet); });
				RegisterSerializer("TAGame.PRI_TA:bUsingItems", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:bUsingItems"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bUsingItems); });
				RegisterSerializer("TAGame.PRI_TA:PrimaryTitle", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:PrimaryTitle"); Serialize<struct ReplicatedTitle>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->PrimaryTitle); });
				RegisterSerializer("TAGame.PRI_TA:bMatchAdmin", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:bMatchAdmin"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bMatchAdmin); });
				RegisterSerializer("TAGame.PRI_TA:bBusy", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:bBusy"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bBusy); });
				RegisterSerializer("TAGame.PRI_TA:bVoteToForfeitDisabled", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:bVoteToForfeitDisabled"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bVoteToForfeitDisabled); });
				RegisterSerializer("TAGame.PRI_TA:bUsingFreecam", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:bUsingFreecam"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bUsingFreecam); });
				RegisterSerializer("TAGame.PRI_TA:ClientLoadoutOnline", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:ClientLoadoutOnline"); Serialize<struct OnlineLoadout>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->ClientLoadoutOnline); });
				RegisterSerializer("TAGame.PRI_TA:CameraYaw", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:CameraYaw"); Serialize<unsigned char>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->CameraYaw); });
				RegisterSerializer("TAGame.PRI_TA:CameraPitch", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:CameraPitch"); Serialize<unsigned char>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->CameraPitch); });
				RegisterSerializer("TAGame.PRI_TA:PawnType", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:PawnType"); Serialize<unsigned char>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->PawnType); });
				RegisterSerializer("TAGame.PRI_TA:ReplicatedWorstNetQualityBeyondLatency", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:ReplicatedWorstNetQualityBeyondLatency"); Serialize<unsigned char>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->ReplicatedWorstNetQualityBeyondLatency); });
				RegisterSerializer("TAGame.PRI_TA:SteeringSensitivity", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:SteeringSensitivity"); Serialize<float>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->SteeringSensitivity); });
				RegisterSerializer("TAGame.PRI_TA:PartyLeader", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:PartyLeader"); Serialize<struct PartyLeader>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->PartyLeader); });
				RegisterSerializer("TAGame.PRI_TA:TimeTillItem", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:TimeTillItem"); Serialize<int>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->TimeTillItem); });
				RegisterSerializer("TAGame.PRI_TA:ClientLoadoutsOnline", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:ClientLoadoutsOnline"); Serialize<struct ClientLoadoutsOnline>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->ClientLoadoutsOnline); });
				RegisterSerializer("TAGame.PRI_TA:ClientLoadouts", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:ClientLoadouts"); Serialize<struct ClientLoadouts>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->ClientLoadouts); });
				RegisterSerializer("TAGame.PRI_TA:ClientLoadout", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:ClientLoadout"); Serialize<struct ClientLoadout>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->ClientLoadout); });
				RegisterSerializer("TAGame.PRI_TA:CameraSettings", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:CameraSettings"); Serialize<struct CameraSettings>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->CameraSettings); });
				RegisterSerializer("TAGame.PRI_TA:SecondaryTitle", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:SecondaryTitle"); Serialize<struct ReplicatedTitle>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->SecondaryTitle); });
				RegisterSerializer("TAGame.PRI_TA:PlayerHistoryKey", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:PlayerHistoryKey"); Serialize<struct HistoryKey>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->PlayerHistoryKey); });
				RegisterSerializer("TAGame.RBActor_TA:ReplicatedRBState", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.RBActor_TA:ReplicatedRBState"); Serialize<struct ReplicatedRBState>(writer, std::static_pointer_cast<CPPRP::TAGame::RBActor_TA>(struc)->ReplicatedRBState); });
				RegisterSerializer("TAGame.RBActor_TA:bReplayActor", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.RBActor_TA:bReplayActor"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::RBActor_TA>(struc)->bReplayActor); });
				RegisterSerializer("TAGame.RBActor_TA:bFrozen", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.RBActor_TA:bFrozen"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::RBActor_TA>(struc)->bFrozen); });
				RegisterSerializer("TAGame.RBActor_TA:WeldedInfo", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.RBActor_TA:WeldedInfo"); Serialize<struct WeldedInfo>(writer, std::static_pointer_cast<CPPRP::TAGame::RBActor_TA>(struc)->WeldedInfo); });
				RegisterSerializer("TAGame.RBActor_TA:bIgnoreSyncing", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.RBActor_TA:bIgnoreSyncing"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::RBActor_TA>(struc)->bIgnoreSyncing); });
				RegisterSerializer("TAGame.CarComponent_TA:Vehicle", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CarComponent_TA:Vehicle"); Serialize<struct ActiveActor>(writer, std::static_pointer_cast<CPPRP::TAGame::CarComponent_TA>(struc)->Vehicle); });
				RegisterSerializer("TAGame.CarComponent_TA:ReplicatedActive", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CarComponent_TA:ReplicatedActive"); Serialize<unsigned char>(writer, std::static_pointer_cast<CPPRP::TAGame::CarComponent_TA>(struc)->ReplicatedActive); });
				RegisterSerializer("TAGame.CarComponent_TA:ReplicatedActivityTime", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CarComponent_TA:ReplicatedActivityTime"); Serialize<float>(writer, std::static_pointer_cast<CPPRP::TAGame::CarComponent_TA>(struc)->ReplicatedActivityTime); });
				RegisterSerializer("TAGame.CarComponent_Boost_TA:RechargeDelay", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CarComponent_Boost_TA:RechargeDelay"); Serialize<float>(writer, std::static_pointer_cast<CPPRP::TAGame::CarComponent_Boost_TA>(struc)->RechargeDelay); });
				RegisterSerializer("TAGame.CarComponent_Boost_TA:bUnlimitedBoost", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CarComponent_Boost_TA:bUnlimitedBoost"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::CarComponent_Boost_TA>(struc)->bUnlimitedBoost); });
				RegisterSerializer("TAGame.CarComponent_Boost_TA:UnlimitedBoostRefCount", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CarComponent_Boost_TA:UnlimitedBoostRefCount"); Serialize<uint32_t>(writer, std::static_pointer_cast<CPPRP::TAGame::CarComponent_Boost_TA>(struc)->UnlimitedBoostRefCount); });
				RegisterSerializer("TAGame.CarComponent_Boost_TA:bNoBoost", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CarComponent_Boost_TA:bNoBoost"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::CarComponent_Boost_TA>(struc)->bNoBoost); });
				RegisterSerializer("TAGame.CarComponent_Boost_TA:ReplicatedBoostAmount", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CarComponent_Boost_TA:ReplicatedBoostAmount"); Serialize<unsigned char>(writer, std::static_pointer_cast<CPPRP::TAGame::CarComponent_Boost_TA>(struc)->ReplicatedBoostAmount); });
				RegisterSerializer("TAGame.CarComponent_Boost_TA:RechargeRate", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CarComponent_Boost_TA:RechargeRate"); Serialize<float>(writer, std::static_pointer_cast<CPPRP::TAGame::CarComponent_Boost_TA>(struc)->RechargeRate); });
				RegisterSerializer("TAGame.CarComponent_Boost_TA:BoostModifier", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CarComponent_Boost_TA:BoostModifier"); Serialize<float>(writer, std::static_pointer_cast<CPPRP::TAGame::CarComponent_Boost_TA>(struc)->BoostModifier); });
				RegisterSerializer("TAGame.CarComponent_Dodge_TA:DodgeTorque", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CarComponent_Dodge_TA:DodgeTorque"); Serialize<struct Vector3I>(writer, std::static_pointer_cast<CPPRP::TAGame::CarComponent_Dodge_TA>(struc)->DodgeTorque); });
				RegisterSerializer("TAGame.CarComponent_FlipCar_TA:bFlipRight", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CarComponent_FlipCar_TA:bFlipRight"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::CarComponent_FlipCar_TA>(struc)->bFlipRight); });
				RegisterSerializer("TAGame.CarComponent_FlipCar_TA:FlipCarTime", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CarComponent_FlipCar_TA:FlipCarTime"); Serialize<float>(writer, std::static_pointer_cast<CPPRP::TAGame::CarComponent_FlipCar_TA>(struc)->FlipCarTime); });
				RegisterSerializer("TAGame.Ball_TA:GameEvent", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_TA:GameEvent"); Serialize<struct ActiveActor>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->GameEvent); });
				RegisterSerializer("TAGame.Ball_TA:ReplicatedPhysMatOverride", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_TA:ReplicatedPhysMatOverride"); Serialize<struct ObjectTarget>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->ReplicatedPhysMatOverride); });
				RegisterSerializer("TAGame.Ball_TA:ReplicatedBallGravityScale", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_TA:ReplicatedBallGravityScale"); Serialize<float>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->ReplicatedBallGravityScale); });
				RegisterSerializer("TAGame.Ball_TA:ReplicatedBallScale", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_TA:ReplicatedBallScale"); Serialize<float>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->ReplicatedBallScale); });
				RegisterSerializer("TAGame.Ball_TA:HitTeamNum", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_TA:HitTeamNum"); Serialize<unsigned char>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->HitTeamNum); });
				RegisterSerializer("TAGame.Ball_TA:ReplicatedWorldBounceScale", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_TA:ReplicatedWorldBounceScale"); Serialize<float>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->ReplicatedWorldBounceScale); });
				RegisterSerializer("TAGame.Ball_TA:ReplicatedAddedCarBounceScale", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_TA:ReplicatedAddedCarBounceScale"); Serialize<float>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->ReplicatedAddedCarBounceScale); });
				RegisterSerializer("TAGame.Ball_TA:ReplicatedExplosionData", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_TA:ReplicatedExplosionData"); Serialize<struct ReplicatedExplosionData>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->ReplicatedExplosionData); });
				RegisterSerializer("TAGame.Ball_TA:ReplicatedBallMaxLinearSpeedScale", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_TA:ReplicatedBallMaxLinearSpeedScale"); Serialize<float>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->ReplicatedBallMaxLinearSpeedScale); });
				RegisterSerializer("TAGame.Ball_TA:ReplicatedExplosionDataExtended", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_TA:ReplicatedExplosionDataExtended"); Serialize<struct ReplicatedExplosionDataExtended>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->ReplicatedExplosionDataExtended); });
				RegisterSerializer("TAGame.Team_TA:LogoData", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Team_TA:LogoData"); Serialize<struct LogoData>(writer, std::static_pointer_cast<CPPRP::TAGame::Team_TA>(struc)->LogoData); });
				RegisterSerializer("TAGame.Team_TA:GameEvent", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Team_TA:GameEvent"); Serialize<struct ActiveActor>(writer, std::static_pointer_cast<CPPRP::TAGame::Team_TA>(struc)->GameEvent); });
				RegisterSerializer("TAGame.Team_TA:CustomTeamName", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Team_TA:CustomTeamName"); Serialize<std::string>(writer, std::static_pointer_cast<CPPRP::TAGame::Team_TA>(struc)->CustomTeamName); });
				RegisterSerializer("TAGame.Team_TA:ClubID", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Team_TA:ClubID"); Serialize<uint64_t>(writer, std::static_pointer_cast<CPPRP::TAGame::Team_TA>(struc)->ClubID); });
				RegisterSerializer("TAGame.Team_TA:ClubColors", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Team_TA:ClubColors"); Serialize<struct ClubColors>(writer, std::static_pointer_cast<CPPRP::TAGame::Team_TA>(struc)->ClubColors); });
				RegisterSerializer("TAGame.Team_Soccar_TA:GameScore", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Team_Soccar_TA:GameScore"); Serialize<uint32_t>(writer, std::static_pointer_cast<CPPRP::TAGame::Team_Soccar_TA>(struc)->GameScore); });
				RegisterSerializer("TAGame.BreakOutActor_Platform_TA:DamageState", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.BreakOutActor_Platform_TA:DamageState"); Serialize<struct DamageState>(writer, std::static_pointer_cast<CPPRP::TAGame::BreakOutActor_Platform_TA>(struc)->DamageState); });
				RegisterSerializer("TAGame.SpecialPickup_Targeted_TA:Targeted", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.SpecialPickup_Targeted_TA:Targeted"); Serialize<struct ActiveActor>(writer, std::static_pointer_cast<CPPRP::TAGame::SpecialPickup_Targeted_TA>(struc)->Targeted); });
				RegisterSerializer("TAGame.SpecialPickup_BallVelcro_TA:bHit", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.SpecialPickup_BallVelcro_TA:bHit"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::SpecialPickup_BallVelcro_TA>(struc)->bHit); });
				RegisterSerializer("TAGame.SpecialPickup_BallVelcro_TA:bBroken", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.SpecialPickup_BallVelcro_TA:bBroken"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::SpecialPickup_BallVelcro_TA>(struc)->bBroken); });
				RegisterSerializer("TAGame.SpecialPickup_BallVelcro_TA:AttachTime", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.SpecialPickup_BallVelcro_TA:AttachTime"); Serialize<float>(writer, std::static_pointer_cast<CPPRP::TAGame::SpecialPickup_BallVelcro_TA>(struc)->AttachTime); });
				RegisterSerializer("TAGame.SpecialPickup_BallVelcro_TA:BreakTime", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.SpecialPickup_BallVelcro_TA:BreakTime"); Serialize<float>(writer, std::static_pointer_cast<CPPRP::TAGame::SpecialPickup_BallVelcro_TA>(struc)->BreakTime); });
				RegisterSerializer("TAGame.SpecialPickup_Rugby_TA:bBallWelded", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.SpecialPickup_Rugby_TA:bBallWelded"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::SpecialPickup_Rugby_TA>(struc)->bBallWelded); });
				RegisterSerializer("TAGame.SpecialPickup_BallFreeze_TA:RepOrigSpeed", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.SpecialPickup_BallFreeze_TA:RepOrigSpeed"); Serialize<float>(writer, std::static_pointer_cast<CPPRP::TAGame::SpecialPickup_BallFreeze_TA>(struc)->RepOrigSpeed); });
				RegisterSerializer("TAGame.CrowdManager_TA:GameEvent", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CrowdManager_TA:GameEvent"); Serialize<struct ActiveActor>(writer, std::static_pointer_cast<CPPRP::TAGame::CrowdManager_TA>(struc)->GameEvent); });
				RegisterSerializer("TAGame.CrowdManager_TA:ReplicatedGlobalOneShotSound", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CrowdManager_TA:ReplicatedGlobalOneShotSound"); Serialize<struct ObjectTarget>(writer, std::static_pointer_cast<CPPRP::TAGame::CrowdManager_TA>(struc)->ReplicatedGlobalOneShotSound); });
				RegisterSerializer("TAGame.CrowdActor_TA:GameEvent", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CrowdActor_TA:GameEvent"); Serialize<struct ActiveActor>(writer, std::static_pointer_cast<CPPRP::TAGame::CrowdActor_TA>(struc)->GameEvent); });
				RegisterSerializer("TAGame.CrowdActor_TA:ReplicatedOneShotSound", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CrowdActor_TA:ReplicatedOneShotSound"); Serialize<struct ObjectTarget>(writer, std::static_pointer_cast<CPPRP::TAGame::CrowdActor_TA>(struc)->ReplicatedOneShotSound); });
				RegisterSerializer("TAGame.CrowdActor_TA:ReplicatedRoundCountDownNumber", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CrowdActor_TA:ReplicatedRoundCountDownNumber"); Serialize<uint32_t>(writer, std::static_pointer_cast<CPPRP::TAGame::CrowdActor_TA>(struc)->ReplicatedRoundCountDownNumber); });
				RegisterSerializer("TAGame.CrowdActor_TA:ReplicatedCountDownNumber", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CrowdActor_TA:ReplicatedCountDownNumber"); Serialize<uint32_t>(writer, std::static_pointer_cast<CPPRP::TAGame::CrowdActor_TA>(struc)->ReplicatedCountDownNumber); });
				RegisterSerializer("TAGame.CrowdActor_TA:ModifiedNoise", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CrowdActor_TA:ModifiedNoise"); Serialize<float>(writer, std::static_pointer_cast<CPPRP::TAGame::CrowdActor_TA>(struc)->ModifiedNoise); });
				RegisterSerializer("TAGame.Vehicle_TA:ReplicatedThrottle", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Vehicle_TA:ReplicatedThrottle"); Serialize<unsigned char>(writer, std::static_pointer_cast<CPPRP::TAGame::Vehicle_TA>(struc)->ReplicatedThrottle); });
				RegisterSerializer("TAGame.Vehicle_TA:bReplicatedHandbrake", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Vehicle_TA:bReplicatedHandbrake"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::Vehicle_TA>(struc)->bReplicatedHandbrake); });
				RegisterSerializer("TAGame.Vehicle_TA:bDriving", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Vehicle_TA:bDriving"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::Vehicle_TA>(struc)->bDriving); });
				RegisterSerializer("TAGame.Vehicle_TA:ReplicatedSteer", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Vehicle_TA:ReplicatedSteer"); Serialize<unsigned char>(writer, std::static_pointer_cast<CPPRP::TAGame::Vehicle_TA>(struc)->ReplicatedSteer); });
				RegisterSerializer("TAGame.Car_TA:AttachedPickup", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Car_TA:AttachedPickup"); Serialize<struct ActiveActor>(writer, std::static_pointer_cast<CPPRP::TAGame::Car_TA>(struc)->AttachedPickup); });
				RegisterSerializer("TAGame.Car_TA:AddedCarForceMultiplier", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Car_TA:AddedCarForceMultiplier"); Serialize<float>(writer, std::static_pointer_cast<CPPRP::TAGame::Car_TA>(struc)->AddedCarForceMultiplier); });
				RegisterSerializer("TAGame.Car_TA:ReplicatedCarScale", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Car_TA:ReplicatedCarScale"); Serialize<float>(writer, std::static_pointer_cast<CPPRP::TAGame::Car_TA>(struc)->ReplicatedCarScale); });
				RegisterSerializer("TAGame.Car_TA:AddedBallForceMultiplier", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Car_TA:AddedBallForceMultiplier"); Serialize<float>(writer, std::static_pointer_cast<CPPRP::TAGame::Car_TA>(struc)->AddedBallForceMultiplier); });
				RegisterSerializer("TAGame.Car_TA:TeamPaint", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Car_TA:TeamPaint"); Serialize<struct TeamPaint>(writer, std::static_pointer_cast<CPPRP::TAGame::Car_TA>(struc)->TeamPaint); });
				RegisterSerializer("TAGame.Car_TA:ReplicatedDemolish", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Car_TA:ReplicatedDemolish"); Serialize<struct ReplicatedDemolish>(writer, std::static_pointer_cast<CPPRP::TAGame::Car_TA>(struc)->ReplicatedDemolish); });
				RegisterSerializer("TAGame.Car_TA:ClubColors", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Car_TA:ClubColors"); Serialize<struct ClubColors>(writer, std::static_pointer_cast<CPPRP::TAGame::Car_TA>(struc)->ClubColors); });
				RegisterSerializer("TAGame.CameraSettingsActor_TA:PRI", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CameraSettingsActor_TA:PRI"); Serialize<struct ActiveActor>(writer, std::static_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(struc)->PRI); });
				RegisterSerializer("TAGame.CameraSettingsActor_TA:CameraPitch", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CameraSettingsActor_TA:CameraPitch"); Serialize<unsigned char>(writer, std::static_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(struc)->CameraPitch); });
				RegisterSerializer("TAGame.CameraSettingsActor_TA:bMouseCameraToggleEnabled", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CameraSettingsActor_TA:bMouseCameraToggleEnabled"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(struc)->bMouseCameraToggleEnabled); });
				RegisterSerializer("TAGame.CameraSettingsActor_TA:bUsingSecondaryCamera", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CameraSettingsActor_TA:bUsingSecondaryCamera"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(struc)->bUsingSecondaryCamera); });
				RegisterSerializer("TAGame.CameraSettingsActor_TA:bUsingBehindView", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CameraSettingsActor_TA:bUsingBehindView"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(struc)->bUsingBehindView); });
				RegisterSerializer("TAGame.CameraSettingsActor_TA:ProfileSettings", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CameraSettingsActor_TA:ProfileSettings"); Serialize<struct CameraSettings>(writer, std::static_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(struc)->ProfileSettings); });
				RegisterSerializer("TAGame.CameraSettingsActor_TA:bUsingSwivel", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CameraSettingsActor_TA:bUsingSwivel"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(struc)->bUsingSwivel); });
				RegisterSerializer("TAGame.CameraSettingsActor_TA:CameraYaw", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CameraSettingsActor_TA:CameraYaw"); Serialize<unsigned char>(writer, std::static_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(struc)->CameraYaw); });
				RegisterSerializer("TAGame.GRI_TA:NewDedicatedServerIP", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GRI_TA:NewDedicatedServerIP"); Serialize<std::string>(writer, std::static_pointer_cast<CPPRP::TAGame::GRI_TA>(struc)->NewDedicatedServerIP); });
				RegisterSerializer("TAGame.Ball_Breakout_TA:DamageIndex", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_Breakout_TA:DamageIndex"); Serialize<uint32_t>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_Breakout_TA>(struc)->DamageIndex); });
				RegisterSerializer("TAGame.Ball_Breakout_TA:AppliedDamage", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_Breakout_TA:AppliedDamage"); Serialize<struct AppliedDamage>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_Breakout_TA>(struc)->AppliedDamage); });
				RegisterSerializer("TAGame.Ball_Breakout_TA:LastTeamTouch", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_Breakout_TA:LastTeamTouch"); Serialize<unsigned char>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_Breakout_TA>(struc)->LastTeamTouch); });
				RegisterSerializer("TAGame.VehiclePickup_TA:bNoPickup", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.VehiclePickup_TA:bNoPickup"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::VehiclePickup_TA>(struc)->bNoPickup); });
				RegisterSerializer("TAGame.VehiclePickup_TA:ReplicatedPickupData", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.VehiclePickup_TA:ReplicatedPickupData"); Serialize<struct ReplicatedPickupData>(writer, std::static_pointer_cast<CPPRP::TAGame::VehiclePickup_TA>(struc)->ReplicatedPickupData); });
				RegisterSerializer("TAGame.Ball_Haunted_TA:DeactivatedGoalIndex", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_Haunted_TA:DeactivatedGoalIndex"); Serialize<unsigned char>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_Haunted_TA>(struc)->DeactivatedGoalIndex); });
				RegisterSerializer("TAGame.Ball_Haunted_TA:bIsBallBeamed", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_Haunted_TA:bIsBallBeamed"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_Haunted_TA>(struc)->bIsBallBeamed); });
				RegisterSerializer("TAGame.Ball_Haunted_TA:ReplicatedBeamBrokenValue", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_Haunted_TA:ReplicatedBeamBrokenValue"); Serialize<unsigned char>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_Haunted_TA>(struc)->ReplicatedBeamBrokenValue); });
				RegisterSerializer("TAGame.Ball_Haunted_TA:LastTeamTouch", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_Haunted_TA:LastTeamTouch"); Serialize<unsigned char>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_Haunted_TA>(struc)->LastTeamTouch); });
				RegisterSerializer("TAGame.Ball_Haunted_TA:TotalActiveBeams", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_Haunted_TA:TotalActiveBeams"); Serialize<unsigned char>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_Haunted_TA>(struc)->TotalActiveBeams); });
				RegisterSerializer("TAGame.GameEvent_TA:ReplicatedRoundCountDownNumber", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_TA:ReplicatedRoundCountDownNumber"); Serialize<uint32_t>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->ReplicatedRoundCountDownNumber); });
				RegisterSerializer("TAGame.GameEvent_TA:ActivatorCar", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_TA:ActivatorCar"); Serialize<struct ActiveActor>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->ActivatorCar); });
				RegisterSerializer("TAGame.GameEvent_TA:ReplicatedGameStateTimeRemaining", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_TA:ReplicatedGameStateTimeRemaining"); Serialize<uint32_t>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->ReplicatedGameStateTimeRemaining); });
				RegisterSerializer("TAGame.GameEvent_TA:ReplicatedStateName", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_TA:ReplicatedStateName"); Serialize<uint32_t>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->ReplicatedStateName); });
				RegisterSerializer("TAGame.GameEvent_TA:MatchTypeClass", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_TA:MatchTypeClass"); Serialize<struct ObjectTarget>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->MatchTypeClass); });
				RegisterSerializer("TAGame.GameEvent_TA:BotSkill", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_TA:BotSkill"); Serialize<uint32_t>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->BotSkill); });
				RegisterSerializer("TAGame.GameEvent_TA:bHasLeaveMatchPenalty", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_TA:bHasLeaveMatchPenalty"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->bHasLeaveMatchPenalty); });
				RegisterSerializer("TAGame.GameEvent_TA:bCanVoteToForfeit", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_TA:bCanVoteToForfeit"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->bCanVoteToForfeit); });
				RegisterSerializer("TAGame.GameEvent_TA:bAllowReadyUp", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_TA:bAllowReadyUp"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->bAllowReadyUp); });
				RegisterSerializer("TAGame.GameEvent_TA:GameMode", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_TA:GameMode"); Serialize<struct GameMode>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->GameMode); });
				RegisterSerializer("TAGame.GameEvent_TA:ReplicatedStateIndex", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_TA:ReplicatedStateIndex"); Serialize<struct ReplicatedStateIndex>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->ReplicatedStateIndex); });
				RegisterSerializer("TAGame.GameEvent_Team_TA:MaxTeamSize", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Team_TA:MaxTeamSize"); Serialize<uint32_t>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Team_TA>(struc)->MaxTeamSize); });
				RegisterSerializer("TAGame.GameEvent_Team_TA:bForfeit", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Team_TA:bForfeit"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Team_TA>(struc)->bForfeit); });
				RegisterSerializer("TAGame.GameEvent_Team_TA:bDisableMutingOtherTeam", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Team_TA:bDisableMutingOtherTeam"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Team_TA>(struc)->bDisableMutingOtherTeam); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:bOverTime", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:bOverTime"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->bOverTime); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:GameTime", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:GameTime"); Serialize<uint32_t>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->GameTime); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:MVP", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:MVP"); Serialize<struct ActiveActor>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->MVP); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:MatchWinner", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:MatchWinner"); Serialize<struct ActiveActor>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->MatchWinner); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:GameWinner", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:GameWinner"); Serialize<struct ActiveActor>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->GameWinner); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:SubRulesArchetype", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:SubRulesArchetype"); Serialize<struct ObjectTarget>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->SubRulesArchetype); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:SecondsRemaining", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:SecondsRemaining"); Serialize<uint32_t>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->SecondsRemaining); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:RoundNum", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:RoundNum"); Serialize<uint32_t>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->RoundNum); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:SeriesLength", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:SeriesLength"); Serialize<uint32_t>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->SeriesLength); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:ReplicatedMusicStinger", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:ReplicatedMusicStinger"); Serialize<struct ReplicatedMusicStringer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->ReplicatedMusicStinger); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:bBallHasBeenHit", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:bBallHasBeenHit"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->bBallHasBeenHit); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:bUnlimitedTime", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:bUnlimitedTime"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->bUnlimitedTime); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:ReplicatedStatEvent", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:ReplicatedStatEvent"); Serialize<struct ReplicatedStatEvent>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->ReplicatedStatEvent); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:bShowIntroScene", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:bShowIntroScene"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->bShowIntroScene); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:bClubMatch", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:bClubMatch"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->bClubMatch); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:ReplicatedScoredOnTeam", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:ReplicatedScoredOnTeam"); Serialize<unsigned char>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->ReplicatedScoredOnTeam); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:bMatchEnded", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:bMatchEnded"); Serialize<bool>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->bMatchEnded); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:ReplicatedServerPerformanceState", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:ReplicatedServerPerformanceState"); Serialize<unsigned char>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->ReplicatedServerPerformanceState); });
				RegisterSerializer("TAGame.GameEvent_SoccarPrivate_TA:MatchSettings", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_SoccarPrivate_TA:MatchSettings"); Serialize<struct PrivateMatchSettings>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_SoccarPrivate_TA>(struc)->MatchSettings); });

				return 0;
			}
	
		
		
		
	};
};