#pragma once
#include "../CPPRP/data/ReplayFileData.h"
#include "../CPPRP/data/NetworkData.h"
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
namespace CPPRP
{
	namespace JSON
	{

		template<typename Writer, typename T>
		inline const void Serialize(Writer& writer, const T& item)
		{
			std::stringstream ss;
			ss << "unsupported " << typeid(T).name();
			std::string o = ss.str();
			//printf("%s\n", o.c_str());
			//assert(1 == 2);
			writer.String(o.c_str(), o.size());
		}

		template<typename Writer>
		inline const void Serialize(Writer& writer, const uint64_t& item)
		{
			writer.Uint64(item);
		}

		template<typename Writer>
		inline const void Serialize(Writer& writer, const int64_t& item)
		{
			writer.Int64(item);
		}

		template<typename Writer>
		inline const void Serialize(Writer& writer, const uint8_t& item)
		{
			writer.Uint(item);
		}

		template<typename Writer>
		inline const void Serialize(Writer& writer, const char& item)
		{
			writer.Int((int)item);
		}

		template<typename Writer>
		inline const void Serialize(Writer& writer, const float& item)
		{
			writer.Double(item);
		}

		template<typename Writer>
		inline const void Serialize(Writer& writer, const std::string& item)
		{
			writer.String(item.c_str(), item.size());
		}

		template<typename Writer>
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

		template<typename Writer>
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

		template<typename Writer>
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

		template<typename Writer>
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

		template<typename Writer, typename T>
		inline const void Serialize(Writer& writer, const std::vector<T>& item) {
			writer.StartArray();
			const size_t size = item.size();
			for (size_t i = 0; i < size; ++i)
			{
				Serialize(writer, item.at(i));
			}
			writer.EndArray();
		}

		template<typename Writer>
		inline const void Serialize(Writer& writer, const bool& item)
		{
			writer.Bool(item);
		}

		template<typename Writer>
		inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::UniqueId>& item)
		{
			writer.StartObject();
			writer.String("Platform");
			writer.Uint(item->platform);
			writer.String("SplitscreenID");
			writer.Uint(item->splitscreenID);
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

		

		template<typename Writer>
		inline const void Serialize(Writer& writer, const uint16_t& item)
		{
			writer.Uint(item);
		}

		template<typename Writer>
		inline const void Serialize(Writer& writer, const uint32_t& item)
		{
			writer.Uint(item);
		}

		template<typename Writer>
		inline const void Serialize(Writer& writer, const int8_t& item)
		{
			writer.Int(item);
		}

		template<typename Writer>
		inline const void Serialize(Writer& writer, const int16_t& item)
		{
			writer.Int(item);
		}

		template<typename Writer>
		inline const void Serialize(Writer& writer, const int32_t& item)
		{
			writer.Int(item);
		}

		#include "GeneratedSerializeFunctions.h"
		
		template<typename Writer>
		class Serializer
		{
			public:
			
			typedef std::function<void(Writer&, std::shared_ptr<Engine::Actor>&)> writerFunc;
			std::unordered_map<std::string, writerFunc> writerFuncs;

			template<typename T>
			inline void RegisterSerializer(const std::string& str, T callback)
			{
				writerFuncs[str] = callback;
			}

			
			Serializer()
			{
				
				RegisterSerializer("Engine.Actor:DrawScale", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.Actor:DrawScale"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->DrawScale); });
				RegisterSerializer("Engine.Actor:bCollideActors", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.Actor:bCollideActors"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->bCollideActors); });
				RegisterSerializer("Engine.Actor:bCollideWorld", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.Actor:bCollideWorld"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->bCollideWorld); });
				RegisterSerializer("Engine.Actor:bNetOwner", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.Actor:bNetOwner"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->bNetOwner); });
				RegisterSerializer("Engine.Actor:Base", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.Actor:Base"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->Base); });
				RegisterSerializer("Engine.Actor:bBlockActors", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.Actor:bBlockActors"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->bBlockActors); });
				RegisterSerializer("Engine.Actor:bHidden", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.Actor:bHidden"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->bHidden); });
				RegisterSerializer("Engine.Actor:bTearOff", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.Actor:bTearOff"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->bTearOff); });
				RegisterSerializer("Engine.Actor:Location", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.Actor:Location"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->Location); });
				RegisterSerializer("Engine.Actor:Rotation", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.Actor:Rotation"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->Rotation); });
				RegisterSerializer("Engine.GameReplicationInfo:ServerName", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.GameReplicationInfo:ServerName"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::GameReplicationInfo>(struc)->ServerName); });
				RegisterSerializer("Engine.GameReplicationInfo:GameClass", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.GameReplicationInfo:GameClass"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::GameReplicationInfo>(struc)->GameClass); });
				RegisterSerializer("Engine.GameReplicationInfo:bStopCountDown", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.GameReplicationInfo:bStopCountDown"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::GameReplicationInfo>(struc)->bStopCountDown); });
				RegisterSerializer("Engine.GameReplicationInfo:bMatchIsOver", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.GameReplicationInfo:bMatchIsOver"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::GameReplicationInfo>(struc)->bMatchIsOver); });
				RegisterSerializer("Engine.GameReplicationInfo:bMatchHasBegun", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.GameReplicationInfo:bMatchHasBegun"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::GameReplicationInfo>(struc)->bMatchHasBegun); });
				RegisterSerializer("Engine.Pawn:PlayerReplicationInfo", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.Pawn:PlayerReplicationInfo"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::Pawn>(struc)->PlayerReplicationInfo); });
				RegisterSerializer("Engine.Pawn:HealthMax", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.Pawn:HealthMax"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::Pawn>(struc)->HealthMax); });
				RegisterSerializer("Engine.Pawn:bIsCrouched", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.Pawn:bIsCrouched"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::Pawn>(struc)->bIsCrouched); });
				RegisterSerializer("Engine.PlayerReplicationInfo:Team", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.PlayerReplicationInfo:Team"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->Team); });
				RegisterSerializer("Engine.PlayerReplicationInfo:bReadyToPlay", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.PlayerReplicationInfo:bReadyToPlay"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->bReadyToPlay); });
				RegisterSerializer("Engine.PlayerReplicationInfo:PlayerName", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.PlayerReplicationInfo:PlayerName"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->PlayerName); });
				RegisterSerializer("Engine.PlayerReplicationInfo:RemoteUserData", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.PlayerReplicationInfo:RemoteUserData"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->RemoteUserData); });
				RegisterSerializer("Engine.PlayerReplicationInfo:bWaitingPlayer", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.PlayerReplicationInfo:bWaitingPlayer"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->bWaitingPlayer); });
				RegisterSerializer("Engine.PlayerReplicationInfo:Score", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.PlayerReplicationInfo:Score"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->Score); });
				RegisterSerializer("Engine.PlayerReplicationInfo:PlayerID", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.PlayerReplicationInfo:PlayerID"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->PlayerID); });
				RegisterSerializer("Engine.PlayerReplicationInfo:bBot", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.PlayerReplicationInfo:bBot"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->bBot); });
				RegisterSerializer("Engine.PlayerReplicationInfo:bIsSpectator", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.PlayerReplicationInfo:bIsSpectator"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->bIsSpectator); });
				RegisterSerializer("Engine.PlayerReplicationInfo:bTimedOut", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.PlayerReplicationInfo:bTimedOut"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->bTimedOut); });
				RegisterSerializer("Engine.PlayerReplicationInfo:bAdmin", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.PlayerReplicationInfo:bAdmin"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->bAdmin); });
				RegisterSerializer("Engine.PlayerReplicationInfo:bIsInactive", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.PlayerReplicationInfo:bIsInactive"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->bIsInactive); });
				RegisterSerializer("Engine.PlayerReplicationInfo:bOnlySpectator", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.PlayerReplicationInfo:bOnlySpectator"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->bOnlySpectator); });
				RegisterSerializer("Engine.PlayerReplicationInfo:Ping", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.PlayerReplicationInfo:Ping"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->Ping); });
				RegisterSerializer("Engine.PlayerReplicationInfo:UniqueId", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.PlayerReplicationInfo:UniqueId"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->UniqueId); });
				RegisterSerializer("Engine.TeamInfo:Score", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.TeamInfo:Score"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::TeamInfo>(struc)->Score); });
				RegisterSerializer("Engine.WorldInfo:WorldGravityZ", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.WorldInfo:WorldGravityZ"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::WorldInfo>(struc)->WorldGravityZ); });
				RegisterSerializer("Engine.WorldInfo:TimeDilation", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("Engine.WorldInfo:TimeDilation"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::Engine::WorldInfo>(struc)->TimeDilation); });
				RegisterSerializer("ProjectX.GRI_X:MatchGUID", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("ProjectX.GRI_X:MatchGUID"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::ProjectX::GRI_X>(struc)->MatchGUID); });
				RegisterSerializer("ProjectX.GRI_X:ReplicatedGameMutatorIndex", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("ProjectX.GRI_X:ReplicatedGameMutatorIndex"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::ProjectX::GRI_X>(struc)->ReplicatedGameMutatorIndex); });
				RegisterSerializer("ProjectX.GRI_X:bGameStarted", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("ProjectX.GRI_X:bGameStarted"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::ProjectX::GRI_X>(struc)->bGameStarted); });
				RegisterSerializer("ProjectX.GRI_X:ReplicatedGamePlaylist", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("ProjectX.GRI_X:ReplicatedGamePlaylist"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::ProjectX::GRI_X>(struc)->ReplicatedGamePlaylist); });
				RegisterSerializer("ProjectX.GRI_X:GameServerID", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("ProjectX.GRI_X:GameServerID"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::ProjectX::GRI_X>(struc)->GameServerID); });
				RegisterSerializer("ProjectX.GRI_X:Reservations", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("ProjectX.GRI_X:Reservations"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::ProjectX::GRI_X>(struc)->Reservations); });
				RegisterSerializer("TAGame.PRI_TA:MatchShots", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:MatchShots"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->MatchShots); });
				RegisterSerializer("TAGame.PRI_TA:PersistentCamera", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:PersistentCamera"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->PersistentCamera); });
				RegisterSerializer("TAGame.PRI_TA:SkillTier", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:SkillTier"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->SkillTier); });
				RegisterSerializer("TAGame.PRI_TA:bUsingBehindView", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:bUsingBehindView"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bUsingBehindView); });
				RegisterSerializer("TAGame.PRI_TA:MatchAssists", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:MatchAssists"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->MatchAssists); });
				RegisterSerializer("TAGame.PRI_TA:RespawnTimeRemaining", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:RespawnTimeRemaining"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->RespawnTimeRemaining); });
				RegisterSerializer("TAGame.PRI_TA:bOnlineLoadoutSet", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:bOnlineLoadoutSet"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bOnlineLoadoutSet); });
				RegisterSerializer("TAGame.PRI_TA:MatchGoals", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:MatchGoals"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->MatchGoals); });
				RegisterSerializer("TAGame.PRI_TA:ReplicatedGameEvent", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:ReplicatedGameEvent"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->ReplicatedGameEvent); });
				RegisterSerializer("TAGame.PRI_TA:TotalXP", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:TotalXP"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->TotalXP); });
				RegisterSerializer("TAGame.PRI_TA:MatchScore", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:MatchScore"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->MatchScore); });
				RegisterSerializer("TAGame.PRI_TA:MatchSaves", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:MatchSaves"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->MatchSaves); });
				RegisterSerializer("TAGame.PRI_TA:Title", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:Title"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->Title); });
				RegisterSerializer("TAGame.PRI_TA:ClubID", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:ClubID"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->ClubID); });
				RegisterSerializer("TAGame.PRI_TA:MaxTimeTillItem", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:MaxTimeTillItem"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->MaxTimeTillItem); });
				RegisterSerializer("TAGame.PRI_TA:MatchBreakoutDamage", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:MatchBreakoutDamage"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->MatchBreakoutDamage); });
				RegisterSerializer("TAGame.PRI_TA:BotProductName", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:BotProductName"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->BotProductName); });
				RegisterSerializer("TAGame.PRI_TA:bReady", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:bReady"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bReady); });
				RegisterSerializer("TAGame.PRI_TA:SpectatorShortcut", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:SpectatorShortcut"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->SpectatorShortcut); });
				RegisterSerializer("TAGame.PRI_TA:bUsingSecondaryCamera", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:bUsingSecondaryCamera"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bUsingSecondaryCamera); });
				RegisterSerializer("TAGame.PRI_TA:PlayerHistoryValid", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:PlayerHistoryValid"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->PlayerHistoryValid); });
				RegisterSerializer("TAGame.PRI_TA:bIsInSplitScreen", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:bIsInSplitScreen"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bIsInSplitScreen); });
				RegisterSerializer("TAGame.PRI_TA:bMatchMVP", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:bMatchMVP"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bMatchMVP); });
				RegisterSerializer("TAGame.PRI_TA:RepStatTitles", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:RepStatTitles"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->RepStatTitles); });
				RegisterSerializer("TAGame.PRI_TA:bOnlineLoadoutsSet", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:bOnlineLoadoutsSet"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bOnlineLoadoutsSet); });
				RegisterSerializer("TAGame.PRI_TA:bUsingItems", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:bUsingItems"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bUsingItems); });
				RegisterSerializer("TAGame.PRI_TA:PrimaryTitle", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:PrimaryTitle"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->PrimaryTitle); });
				RegisterSerializer("TAGame.PRI_TA:bMatchAdmin", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:bMatchAdmin"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bMatchAdmin); });
				RegisterSerializer("TAGame.PRI_TA:bBusy", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:bBusy"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bBusy); });
				RegisterSerializer("TAGame.PRI_TA:bVoteToForfeitDisabled", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:bVoteToForfeitDisabled"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bVoteToForfeitDisabled); });
				RegisterSerializer("TAGame.PRI_TA:bUsingFreecam", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:bUsingFreecam"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bUsingFreecam); });
				RegisterSerializer("TAGame.PRI_TA:ClientLoadoutOnline", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:ClientLoadoutOnline"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->ClientLoadoutOnline); });
				RegisterSerializer("TAGame.PRI_TA:CameraYaw", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:CameraYaw"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->CameraYaw); });
				RegisterSerializer("TAGame.PRI_TA:CameraPitch", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:CameraPitch"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->CameraPitch); });
				RegisterSerializer("TAGame.PRI_TA:PawnType", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:PawnType"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->PawnType); });
				RegisterSerializer("TAGame.PRI_TA:ReplicatedWorstNetQualityBeyondLatency", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:ReplicatedWorstNetQualityBeyondLatency"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->ReplicatedWorstNetQualityBeyondLatency); });
				RegisterSerializer("TAGame.PRI_TA:SteeringSensitivity", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:SteeringSensitivity"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->SteeringSensitivity); });
				RegisterSerializer("TAGame.PRI_TA:PartyLeader", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:PartyLeader"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->PartyLeader); });
				RegisterSerializer("TAGame.PRI_TA:TimeTillItem", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:TimeTillItem"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->TimeTillItem); });
				RegisterSerializer("TAGame.PRI_TA:ClientLoadoutsOnline", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:ClientLoadoutsOnline"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->ClientLoadoutsOnline); });
				RegisterSerializer("TAGame.PRI_TA:ClientLoadouts", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:ClientLoadouts"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->ClientLoadouts); });
				RegisterSerializer("TAGame.PRI_TA:ClientLoadout", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:ClientLoadout"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->ClientLoadout); });
				RegisterSerializer("TAGame.PRI_TA:CameraSettings", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:CameraSettings"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->CameraSettings); });
				RegisterSerializer("TAGame.PRI_TA:SecondaryTitle", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:SecondaryTitle"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->SecondaryTitle); });
				RegisterSerializer("TAGame.PRI_TA:PlayerHistoryKey", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.PRI_TA:PlayerHistoryKey"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->PlayerHistoryKey); });
				RegisterSerializer("TAGame.RBActor_TA:ReplicatedRBState", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.RBActor_TA:ReplicatedRBState"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::RBActor_TA>(struc)->ReplicatedRBState); });
				RegisterSerializer("TAGame.RBActor_TA:bReplayActor", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.RBActor_TA:bReplayActor"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::RBActor_TA>(struc)->bReplayActor); });
				RegisterSerializer("TAGame.RBActor_TA:bFrozen", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.RBActor_TA:bFrozen"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::RBActor_TA>(struc)->bFrozen); });
				RegisterSerializer("TAGame.RBActor_TA:WeldedInfo", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.RBActor_TA:WeldedInfo"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::RBActor_TA>(struc)->WeldedInfo); });
				RegisterSerializer("TAGame.RBActor_TA:bIgnoreSyncing", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.RBActor_TA:bIgnoreSyncing"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::RBActor_TA>(struc)->bIgnoreSyncing); });
				RegisterSerializer("TAGame.CarComponent_TA:Vehicle", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CarComponent_TA:Vehicle"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::CarComponent_TA>(struc)->Vehicle); });
				RegisterSerializer("TAGame.CarComponent_TA:ReplicatedActive", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CarComponent_TA:ReplicatedActive"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::CarComponent_TA>(struc)->ReplicatedActive); });
				RegisterSerializer("TAGame.CarComponent_TA:ReplicatedActivityTime", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CarComponent_TA:ReplicatedActivityTime"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::CarComponent_TA>(struc)->ReplicatedActivityTime); });
				RegisterSerializer("TAGame.CarComponent_Boost_TA:RechargeDelay", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CarComponent_Boost_TA:RechargeDelay"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::CarComponent_Boost_TA>(struc)->RechargeDelay); });
				RegisterSerializer("TAGame.CarComponent_Boost_TA:bUnlimitedBoost", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CarComponent_Boost_TA:bUnlimitedBoost"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::CarComponent_Boost_TA>(struc)->bUnlimitedBoost); });
				RegisterSerializer("TAGame.CarComponent_Boost_TA:UnlimitedBoostRefCount", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CarComponent_Boost_TA:UnlimitedBoostRefCount"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::CarComponent_Boost_TA>(struc)->UnlimitedBoostRefCount); });
				RegisterSerializer("TAGame.CarComponent_Boost_TA:bNoBoost", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CarComponent_Boost_TA:bNoBoost"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::CarComponent_Boost_TA>(struc)->bNoBoost); });
				RegisterSerializer("TAGame.CarComponent_Boost_TA:ReplicatedBoostAmount", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CarComponent_Boost_TA:ReplicatedBoostAmount"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::CarComponent_Boost_TA>(struc)->ReplicatedBoostAmount); });
				RegisterSerializer("TAGame.CarComponent_Boost_TA:RechargeRate", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CarComponent_Boost_TA:RechargeRate"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::CarComponent_Boost_TA>(struc)->RechargeRate); });
				RegisterSerializer("TAGame.CarComponent_Boost_TA:BoostModifier", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CarComponent_Boost_TA:BoostModifier"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::CarComponent_Boost_TA>(struc)->BoostModifier); });
				RegisterSerializer("TAGame.CarComponent_Dodge_TA:DodgeTorque", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CarComponent_Dodge_TA:DodgeTorque"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::CarComponent_Dodge_TA>(struc)->DodgeTorque); });
				RegisterSerializer("TAGame.CarComponent_FlipCar_TA:bFlipRight", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CarComponent_FlipCar_TA:bFlipRight"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::CarComponent_FlipCar_TA>(struc)->bFlipRight); });
				RegisterSerializer("TAGame.CarComponent_FlipCar_TA:FlipCarTime", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CarComponent_FlipCar_TA:FlipCarTime"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::CarComponent_FlipCar_TA>(struc)->FlipCarTime); });
				RegisterSerializer("TAGame.Ball_TA:GameEvent", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_TA:GameEvent"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->GameEvent); });
				RegisterSerializer("TAGame.Ball_TA:ReplicatedPhysMatOverride", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_TA:ReplicatedPhysMatOverride"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->ReplicatedPhysMatOverride); });
				RegisterSerializer("TAGame.Ball_TA:ReplicatedBallGravityScale", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_TA:ReplicatedBallGravityScale"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->ReplicatedBallGravityScale); });
				RegisterSerializer("TAGame.Ball_TA:ReplicatedBallScale", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_TA:ReplicatedBallScale"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->ReplicatedBallScale); });
				RegisterSerializer("TAGame.Ball_TA:HitTeamNum", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_TA:HitTeamNum"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->HitTeamNum); });
				RegisterSerializer("TAGame.Ball_TA:ReplicatedWorldBounceScale", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_TA:ReplicatedWorldBounceScale"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->ReplicatedWorldBounceScale); });
				RegisterSerializer("TAGame.Ball_TA:ReplicatedAddedCarBounceScale", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_TA:ReplicatedAddedCarBounceScale"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->ReplicatedAddedCarBounceScale); });
				RegisterSerializer("TAGame.Ball_TA:ReplicatedExplosionData", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_TA:ReplicatedExplosionData"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->ReplicatedExplosionData); });
				RegisterSerializer("TAGame.Ball_TA:ReplicatedBallMaxLinearSpeedScale", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_TA:ReplicatedBallMaxLinearSpeedScale"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->ReplicatedBallMaxLinearSpeedScale); });
				RegisterSerializer("TAGame.Ball_TA:ReplicatedExplosionDataExtended", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_TA:ReplicatedExplosionDataExtended"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->ReplicatedExplosionDataExtended); });
				RegisterSerializer("TAGame.Team_TA:LogoData", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Team_TA:LogoData"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Team_TA>(struc)->LogoData); });
				RegisterSerializer("TAGame.Team_TA:GameEvent", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Team_TA:GameEvent"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Team_TA>(struc)->GameEvent); });
				RegisterSerializer("TAGame.Team_TA:CustomTeamName", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Team_TA:CustomTeamName"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Team_TA>(struc)->CustomTeamName); });
				RegisterSerializer("TAGame.Team_TA:ClubID", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Team_TA:ClubID"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Team_TA>(struc)->ClubID); });
				RegisterSerializer("TAGame.Team_TA:ClubColors", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Team_TA:ClubColors"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Team_TA>(struc)->ClubColors); });
				RegisterSerializer("TAGame.Team_Soccar_TA:GameScore", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Team_Soccar_TA:GameScore"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Team_Soccar_TA>(struc)->GameScore); });
				RegisterSerializer("TAGame.BreakOutActor_Platform_TA:DamageState", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.BreakOutActor_Platform_TA:DamageState"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::BreakOutActor_Platform_TA>(struc)->DamageState); });
				RegisterSerializer("TAGame.SpecialPickup_Targeted_TA:Targeted", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.SpecialPickup_Targeted_TA:Targeted"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::SpecialPickup_Targeted_TA>(struc)->Targeted); });
				RegisterSerializer("TAGame.SpecialPickup_BallVelcro_TA:bHit", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.SpecialPickup_BallVelcro_TA:bHit"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::SpecialPickup_BallVelcro_TA>(struc)->bHit); });
				RegisterSerializer("TAGame.SpecialPickup_BallVelcro_TA:bBroken", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.SpecialPickup_BallVelcro_TA:bBroken"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::SpecialPickup_BallVelcro_TA>(struc)->bBroken); });
				RegisterSerializer("TAGame.SpecialPickup_BallVelcro_TA:AttachTime", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.SpecialPickup_BallVelcro_TA:AttachTime"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::SpecialPickup_BallVelcro_TA>(struc)->AttachTime); });
				RegisterSerializer("TAGame.SpecialPickup_BallVelcro_TA:BreakTime", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.SpecialPickup_BallVelcro_TA:BreakTime"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::SpecialPickup_BallVelcro_TA>(struc)->BreakTime); });
				RegisterSerializer("TAGame.SpecialPickup_Rugby_TA:bBallWelded", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.SpecialPickup_Rugby_TA:bBallWelded"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::SpecialPickup_Rugby_TA>(struc)->bBallWelded); });
				RegisterSerializer("TAGame.SpecialPickup_BallFreeze_TA:RepOrigSpeed", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.SpecialPickup_BallFreeze_TA:RepOrigSpeed"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::SpecialPickup_BallFreeze_TA>(struc)->RepOrigSpeed); });
				RegisterSerializer("TAGame.CrowdManager_TA:GameEvent", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CrowdManager_TA:GameEvent"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::CrowdManager_TA>(struc)->GameEvent); });
				RegisterSerializer("TAGame.CrowdManager_TA:ReplicatedGlobalOneShotSound", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CrowdManager_TA:ReplicatedGlobalOneShotSound"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::CrowdManager_TA>(struc)->ReplicatedGlobalOneShotSound); });
				RegisterSerializer("TAGame.CrowdActor_TA:GameEvent", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CrowdActor_TA:GameEvent"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::CrowdActor_TA>(struc)->GameEvent); });
				RegisterSerializer("TAGame.CrowdActor_TA:ReplicatedOneShotSound", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CrowdActor_TA:ReplicatedOneShotSound"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::CrowdActor_TA>(struc)->ReplicatedOneShotSound); });
				RegisterSerializer("TAGame.CrowdActor_TA:ReplicatedRoundCountDownNumber", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CrowdActor_TA:ReplicatedRoundCountDownNumber"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::CrowdActor_TA>(struc)->ReplicatedRoundCountDownNumber); });
				RegisterSerializer("TAGame.CrowdActor_TA:ReplicatedCountDownNumber", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CrowdActor_TA:ReplicatedCountDownNumber"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::CrowdActor_TA>(struc)->ReplicatedCountDownNumber); });
				RegisterSerializer("TAGame.CrowdActor_TA:ModifiedNoise", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CrowdActor_TA:ModifiedNoise"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::CrowdActor_TA>(struc)->ModifiedNoise); });
				RegisterSerializer("TAGame.Vehicle_TA:ReplicatedThrottle", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Vehicle_TA:ReplicatedThrottle"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Vehicle_TA>(struc)->ReplicatedThrottle); });
				RegisterSerializer("TAGame.Vehicle_TA:bReplicatedHandbrake", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Vehicle_TA:bReplicatedHandbrake"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Vehicle_TA>(struc)->bReplicatedHandbrake); });
				RegisterSerializer("TAGame.Vehicle_TA:bDriving", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Vehicle_TA:bDriving"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Vehicle_TA>(struc)->bDriving); });
				RegisterSerializer("TAGame.Vehicle_TA:ReplicatedSteer", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Vehicle_TA:ReplicatedSteer"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Vehicle_TA>(struc)->ReplicatedSteer); });
				RegisterSerializer("TAGame.Car_TA:AttachedPickup", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Car_TA:AttachedPickup"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Car_TA>(struc)->AttachedPickup); });
				RegisterSerializer("TAGame.Car_TA:AddedCarForceMultiplier", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Car_TA:AddedCarForceMultiplier"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Car_TA>(struc)->AddedCarForceMultiplier); });
				RegisterSerializer("TAGame.Car_TA:ReplicatedCarScale", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Car_TA:ReplicatedCarScale"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Car_TA>(struc)->ReplicatedCarScale); });
				RegisterSerializer("TAGame.Car_TA:AddedBallForceMultiplier", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Car_TA:AddedBallForceMultiplier"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Car_TA>(struc)->AddedBallForceMultiplier); });
				RegisterSerializer("TAGame.Car_TA:TeamPaint", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Car_TA:TeamPaint"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Car_TA>(struc)->TeamPaint); });
				RegisterSerializer("TAGame.Car_TA:ReplicatedDemolish", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Car_TA:ReplicatedDemolish"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Car_TA>(struc)->ReplicatedDemolish); });
				RegisterSerializer("TAGame.Car_TA:ClubColors", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Car_TA:ClubColors"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Car_TA>(struc)->ClubColors); });
				RegisterSerializer("TAGame.CameraSettingsActor_TA:PRI", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CameraSettingsActor_TA:PRI"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(struc)->PRI); });
				RegisterSerializer("TAGame.CameraSettingsActor_TA:CameraPitch", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CameraSettingsActor_TA:CameraPitch"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(struc)->CameraPitch); });
				RegisterSerializer("TAGame.CameraSettingsActor_TA:bMouseCameraToggleEnabled", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CameraSettingsActor_TA:bMouseCameraToggleEnabled"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(struc)->bMouseCameraToggleEnabled); });
				RegisterSerializer("TAGame.CameraSettingsActor_TA:bUsingSecondaryCamera", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CameraSettingsActor_TA:bUsingSecondaryCamera"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(struc)->bUsingSecondaryCamera); });
				RegisterSerializer("TAGame.CameraSettingsActor_TA:bUsingBehindView", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CameraSettingsActor_TA:bUsingBehindView"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(struc)->bUsingBehindView); });
				RegisterSerializer("TAGame.CameraSettingsActor_TA:ProfileSettings", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CameraSettingsActor_TA:ProfileSettings"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(struc)->ProfileSettings); });
				RegisterSerializer("TAGame.CameraSettingsActor_TA:bUsingSwivel", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CameraSettingsActor_TA:bUsingSwivel"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(struc)->bUsingSwivel); });
				RegisterSerializer("TAGame.CameraSettingsActor_TA:CameraYaw", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.CameraSettingsActor_TA:CameraYaw"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(struc)->CameraYaw); });
				RegisterSerializer("TAGame.GRI_TA:NewDedicatedServerIP", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GRI_TA:NewDedicatedServerIP"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GRI_TA>(struc)->NewDedicatedServerIP); });
				RegisterSerializer("TAGame.Ball_Breakout_TA:DamageIndex", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_Breakout_TA:DamageIndex"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_Breakout_TA>(struc)->DamageIndex); });
				RegisterSerializer("TAGame.Ball_Breakout_TA:AppliedDamage", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_Breakout_TA:AppliedDamage"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_Breakout_TA>(struc)->AppliedDamage); });
				RegisterSerializer("TAGame.Ball_Breakout_TA:LastTeamTouch", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_Breakout_TA:LastTeamTouch"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_Breakout_TA>(struc)->LastTeamTouch); });
				RegisterSerializer("TAGame.VehiclePickup_TA:bNoPickup", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.VehiclePickup_TA:bNoPickup"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::VehiclePickup_TA>(struc)->bNoPickup); });
				RegisterSerializer("TAGame.VehiclePickup_TA:ReplicatedPickupData", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.VehiclePickup_TA:ReplicatedPickupData"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::VehiclePickup_TA>(struc)->ReplicatedPickupData); });
				RegisterSerializer("TAGame.Ball_Haunted_TA:DeactivatedGoalIndex", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_Haunted_TA:DeactivatedGoalIndex"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_Haunted_TA>(struc)->DeactivatedGoalIndex); });
				RegisterSerializer("TAGame.Ball_Haunted_TA:bIsBallBeamed", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_Haunted_TA:bIsBallBeamed"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_Haunted_TA>(struc)->bIsBallBeamed); });
				RegisterSerializer("TAGame.Ball_Haunted_TA:ReplicatedBeamBrokenValue", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_Haunted_TA:ReplicatedBeamBrokenValue"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_Haunted_TA>(struc)->ReplicatedBeamBrokenValue); });
				RegisterSerializer("TAGame.Ball_Haunted_TA:LastTeamTouch", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_Haunted_TA:LastTeamTouch"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_Haunted_TA>(struc)->LastTeamTouch); });
				RegisterSerializer("TAGame.Ball_Haunted_TA:TotalActiveBeams", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.Ball_Haunted_TA:TotalActiveBeams"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::Ball_Haunted_TA>(struc)->TotalActiveBeams); });
				RegisterSerializer("TAGame.GameEvent_TA:ReplicatedRoundCountDownNumber", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_TA:ReplicatedRoundCountDownNumber"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->ReplicatedRoundCountDownNumber); });
				RegisterSerializer("TAGame.GameEvent_TA:ActivatorCar", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_TA:ActivatorCar"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->ActivatorCar); });
				RegisterSerializer("TAGame.GameEvent_TA:ReplicatedGameStateTimeRemaining", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_TA:ReplicatedGameStateTimeRemaining"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->ReplicatedGameStateTimeRemaining); });
				RegisterSerializer("TAGame.GameEvent_TA:ReplicatedStateName", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_TA:ReplicatedStateName"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->ReplicatedStateName); });
				RegisterSerializer("TAGame.GameEvent_TA:MatchTypeClass", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_TA:MatchTypeClass"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->MatchTypeClass); });
				RegisterSerializer("TAGame.GameEvent_TA:BotSkill", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_TA:BotSkill"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->BotSkill); });
				RegisterSerializer("TAGame.GameEvent_TA:bHasLeaveMatchPenalty", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_TA:bHasLeaveMatchPenalty"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->bHasLeaveMatchPenalty); });
				RegisterSerializer("TAGame.GameEvent_TA:bCanVoteToForfeit", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_TA:bCanVoteToForfeit"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->bCanVoteToForfeit); });
				RegisterSerializer("TAGame.GameEvent_TA:bAllowReadyUp", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_TA:bAllowReadyUp"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->bAllowReadyUp); });
				RegisterSerializer("TAGame.GameEvent_TA:GameMode", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_TA:GameMode"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->GameMode); });
				RegisterSerializer("TAGame.GameEvent_TA:ReplicatedStateIndex", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_TA:ReplicatedStateIndex"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->ReplicatedStateIndex); });
				RegisterSerializer("TAGame.GameEvent_Team_TA:MaxTeamSize", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Team_TA:MaxTeamSize"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Team_TA>(struc)->MaxTeamSize); });
				RegisterSerializer("TAGame.GameEvent_Team_TA:bForfeit", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Team_TA:bForfeit"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Team_TA>(struc)->bForfeit); });
				RegisterSerializer("TAGame.GameEvent_Team_TA:bDisableMutingOtherTeam", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Team_TA:bDisableMutingOtherTeam"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Team_TA>(struc)->bDisableMutingOtherTeam); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:bOverTime", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:bOverTime"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->bOverTime); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:GameTime", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:GameTime"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->GameTime); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:MVP", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:MVP"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->MVP); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:MatchWinner", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:MatchWinner"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->MatchWinner); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:GameWinner", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:GameWinner"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->GameWinner); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:SubRulesArchetype", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:SubRulesArchetype"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->SubRulesArchetype); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:SecondsRemaining", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:SecondsRemaining"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->SecondsRemaining); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:RoundNum", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:RoundNum"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->RoundNum); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:SeriesLength", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:SeriesLength"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->SeriesLength); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:ReplicatedMusicStinger", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:ReplicatedMusicStinger"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->ReplicatedMusicStinger); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:bBallHasBeenHit", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:bBallHasBeenHit"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->bBallHasBeenHit); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:bUnlimitedTime", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:bUnlimitedTime"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->bUnlimitedTime); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:ReplicatedStatEvent", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:ReplicatedStatEvent"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->ReplicatedStatEvent); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:bShowIntroScene", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:bShowIntroScene"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->bShowIntroScene); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:bClubMatch", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:bClubMatch"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->bClubMatch); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:ReplicatedScoredOnTeam", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:ReplicatedScoredOnTeam"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->ReplicatedScoredOnTeam); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:bMatchEnded", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:bMatchEnded"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->bMatchEnded); });
				RegisterSerializer("TAGame.GameEvent_Soccar_TA:ReplicatedServerPerformanceState", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_Soccar_TA:ReplicatedServerPerformanceState"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->ReplicatedServerPerformanceState); });
				RegisterSerializer("TAGame.GameEvent_SoccarPrivate_TA:MatchSettings", [](Writer& writer, std::shared_ptr<Engine::Actor>& struc) { writer.String("TAGame.GameEvent_SoccarPrivate_TA:MatchSettings"); Serialize<Writer>(writer, std::static_pointer_cast<CPPRP::TAGame::GameEvent_SoccarPrivate_TA>(struc)->MatchSettings); });
								
			}
	
		};
		
		
	};
};