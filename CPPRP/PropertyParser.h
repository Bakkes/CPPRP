#pragma once
#include <unordered_map>
#include <functional>
#include <memory>
#include "GameClasses.h"

namespace CPPRP
{

	typedef std::function<std::shared_ptr<Engine::Actor>()> createObjectFunc;
	typedef std::function<void(std::shared_ptr<Engine::Actor>&, CPPBitReader<BitReaderType>& br)> parsePropertyFunc;

	static std::unordered_map<std::string, createObjectFunc> createObjectFuncs;
	static std::unordered_map<std::string, parsePropertyFunc> parsePropertyFuncs;

	template<typename T1>
	constexpr inline static std::shared_ptr<Engine::Actor> createObject()
	{
		return std::make_shared<T1>();
	}

	template<typename T1>
	constexpr inline static void RegisterClass(std::string className)
	{
		createObjectFuncs[className] = &createObject<T1>;
	}

	template<typename T>
	constexpr inline static void RegisterField(const std::string& str, T callback)
	{
		parsePropertyFuncs[str] = callback;
	}
	template<typename T>
	constexpr inline static T Initializor()
	{

		RegisterClass<Engine::Actor>("Engine.Actor");
		RegisterField("Engine.Actor:DrawScale", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->DrawScale = Consume<float>(br); });
		RegisterField("Engine.Actor:bCollideActors", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->bCollideActors = Consume<bool>(br); });
		RegisterField("Engine.Actor:bCollideWorld", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->bCollideWorld = Consume<bool>(br); });
		RegisterField("Engine.Actor:bNetOwner", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->bNetOwner = Consume<bool>(br); });
		RegisterField("Engine.Actor:Base", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->Base = Consume<struct ActorBase>(br); });
		RegisterField("Engine.Actor:bBlockActors", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->bBlockActors = Consume<bool>(br); });
		RegisterField("Engine.Actor:bHidden", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->bHidden = Consume<bool>(br); });
		RegisterField("Engine.Actor:bTearOff", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->bTearOff = Consume<bool>(br); });
		RegisterField("Engine.Actor:Location", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->Location = Consume<struct Vector3I>(br); });
		RegisterField("Engine.Actor:Rotation", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->Rotation = Consume<struct Rotator>(br); });

		RegisterClass<Engine::Info>("Engine.Info");

		RegisterClass<Engine::ReplicationInfo>("Engine.ReplicationInfo");

		RegisterClass<Engine::GameReplicationInfo>("Engine.GameReplicationInfo");
		RegisterField("Engine.GameReplicationInfo:ServerName", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::GameReplicationInfo>(struc)->ServerName = Consume<std::string>(br); });
		RegisterField("Engine.GameReplicationInfo:GameClass", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::GameReplicationInfo>(struc)->GameClass = Consume<struct ObjectTarget>(br); });
		RegisterField("Engine.GameReplicationInfo:bStopCountDown", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::GameReplicationInfo>(struc)->bStopCountDown = Consume<bool>(br); });
		RegisterField("Engine.GameReplicationInfo:bMatchIsOver", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::GameReplicationInfo>(struc)->bMatchIsOver = Consume<bool>(br); });
		RegisterField("Engine.GameReplicationInfo:bMatchHasBegun", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::GameReplicationInfo>(struc)->bMatchHasBegun = Consume<bool>(br); });

		RegisterClass<Engine::Pawn>("Engine.Pawn");
		RegisterField("Engine.Pawn:PlayerReplicationInfo", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::Pawn>(struc)->PlayerReplicationInfo = Consume<struct ActiveActor>(br); });
		RegisterField("Engine.Pawn:HealthMax", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::Pawn>(struc)->HealthMax = Consume<uint32_t>(br); });
		RegisterField("Engine.Pawn:bIsCrouched", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::Pawn>(struc)->bIsCrouched = Consume<bool>(br); });

		RegisterClass<Engine::PlayerReplicationInfo>("Engine.PlayerReplicationInfo");
		RegisterField("Engine.PlayerReplicationInfo:Team", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->Team = Consume<struct ActiveActor>(br); });
		RegisterField("Engine.PlayerReplicationInfo:bReadyToPlay", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->bReadyToPlay = Consume<bool>(br); });
		RegisterField("Engine.PlayerReplicationInfo:PlayerName", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->PlayerName = Consume<std::string>(br); });
		RegisterField("Engine.PlayerReplicationInfo:RemoteUserData", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->RemoteUserData = Consume<std::string>(br); });
		RegisterField("Engine.PlayerReplicationInfo:bWaitingPlayer", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->bWaitingPlayer = Consume<bool>(br); });
		RegisterField("Engine.PlayerReplicationInfo:Score", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->Score = Consume<uint32_t>(br); });
		RegisterField("Engine.PlayerReplicationInfo:PlayerID", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->PlayerID = Consume<uint32_t>(br); });
		RegisterField("Engine.PlayerReplicationInfo:bBot", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->bBot = Consume<bool>(br); });
		RegisterField("Engine.PlayerReplicationInfo:bIsSpectator", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->bIsSpectator = Consume<bool>(br); });
		RegisterField("Engine.PlayerReplicationInfo:bTimedOut", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->bTimedOut = Consume<bool>(br); });
		RegisterField("Engine.PlayerReplicationInfo:bAdmin", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->bAdmin = Consume<bool>(br); });
		RegisterField("Engine.PlayerReplicationInfo:bIsInactive", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->bIsInactive = Consume<bool>(br); });
		RegisterField("Engine.PlayerReplicationInfo:bOnlySpectator", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->bOnlySpectator = Consume<bool>(br); });
		RegisterField("Engine.PlayerReplicationInfo:Ping", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->Ping = Consume<unsigned char>(br); });
		RegisterField("Engine.PlayerReplicationInfo:UniqueId", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->UniqueId = Consume<class std::shared_ptr<struct UniqueId>>(br); });

		RegisterClass<Engine::TeamInfo>("Engine.TeamInfo");
		RegisterField("Engine.TeamInfo:Score", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::TeamInfo>(struc)->Score = Consume<uint32_t>(br); });

		RegisterClass<Engine::WorldInfo>("Engine.WorldInfo");
		RegisterField("Engine.WorldInfo:WorldGravityZ", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::WorldInfo>(struc)->WorldGravityZ = Consume<float>(br); });
		RegisterField("Engine.WorldInfo:TimeDilation", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::Engine::WorldInfo>(struc)->TimeDilation = Consume<float>(br); });

		RegisterClass<ProjectX::GRI_X>("ProjectX.GRI_X");
		RegisterField("ProjectX.GRI_X:MatchGUID", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::ProjectX::GRI_X>(struc)->MatchGUID = Consume<std::string>(br); });
		RegisterField("ProjectX.GRI_X:ReplicatedGameMutatorIndex", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::ProjectX::GRI_X>(struc)->ReplicatedGameMutatorIndex = Consume<int>(br); });
		RegisterField("ProjectX.GRI_X:bGameStarted", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::ProjectX::GRI_X>(struc)->bGameStarted = Consume<bool>(br); });
		RegisterField("ProjectX.GRI_X:ReplicatedGamePlaylist", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::ProjectX::GRI_X>(struc)->ReplicatedGamePlaylist = Consume<uint32_t>(br); });
		RegisterField("ProjectX.GRI_X:GameServerID", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::ProjectX::GRI_X>(struc)->GameServerID = Consume<uint64_t>(br); });
		RegisterField("ProjectX.GRI_X:Reservations", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::ProjectX::GRI_X>(struc)->Reservations = Consume<struct Reservation>(br); });

		RegisterClass<ProjectX::NetModeReplicator_X>("ProjectX.NetModeReplicator_X");

		RegisterClass<ProjectX::Pawn_X>("ProjectX.Pawn_X");

		RegisterClass<ProjectX::PRI_X>("ProjectX.PRI_X");

		RegisterClass<TAGame::PRI_TA>("TAGame.PRI_TA");
		RegisterField("TAGame.PRI_TA:MatchShots", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->MatchShots = Consume<uint32_t>(br); });
		RegisterField("TAGame.PRI_TA:PersistentCamera", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->PersistentCamera = Consume<struct ActiveActor>(br); });
		RegisterField("TAGame.PRI_TA:SkillTier", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->SkillTier = Consume<struct SkillTier>(br); });
		RegisterField("TAGame.PRI_TA:bUsingBehindView", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bUsingBehindView = Consume<bool>(br); });
		RegisterField("TAGame.PRI_TA:MatchAssists", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->MatchAssists = Consume<uint32_t>(br); });
		RegisterField("TAGame.PRI_TA:RespawnTimeRemaining", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->RespawnTimeRemaining = Consume<int>(br); });
		RegisterField("TAGame.PRI_TA:bOnlineLoadoutSet", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bOnlineLoadoutSet = Consume<bool>(br); });
		RegisterField("TAGame.PRI_TA:MatchGoals", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->MatchGoals = Consume<uint32_t>(br); });
		RegisterField("TAGame.PRI_TA:ReplicatedGameEvent", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->ReplicatedGameEvent = Consume<struct ActiveActor>(br); });
		RegisterField("TAGame.PRI_TA:TotalXP", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->TotalXP = Consume<uint32_t>(br); });
		RegisterField("TAGame.PRI_TA:MatchScore", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->MatchScore = Consume<uint32_t>(br); });
		RegisterField("TAGame.PRI_TA:MatchSaves", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->MatchSaves = Consume<uint32_t>(br); });
		RegisterField("TAGame.PRI_TA:Title", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->Title = Consume<uint32_t>(br); });
		RegisterField("TAGame.PRI_TA:ClubID", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->ClubID = Consume<uint64_t>(br); });
		RegisterField("TAGame.PRI_TA:MaxTimeTillItem", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->MaxTimeTillItem = Consume<int>(br); });
		RegisterField("TAGame.PRI_TA:MatchBreakoutDamage", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->MatchBreakoutDamage = Consume<uint32_t>(br); });
		RegisterField("TAGame.PRI_TA:BotProductName", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->BotProductName = Consume<uint32_t>(br); });
		RegisterField("TAGame.PRI_TA:bReady", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bReady = Consume<bool>(br); });
		RegisterField("TAGame.PRI_TA:SpectatorShortcut", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->SpectatorShortcut = Consume<uint32_t>(br); });
		RegisterField("TAGame.PRI_TA:bUsingSecondaryCamera", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bUsingSecondaryCamera = Consume<bool>(br); });
		RegisterField("TAGame.PRI_TA:PlayerHistoryValid", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->PlayerHistoryValid = Consume<bool>(br); });
		RegisterField("TAGame.PRI_TA:bIsInSplitScreen", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bIsInSplitScreen = Consume<bool>(br); });
		RegisterField("TAGame.PRI_TA:bMatchMVP", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bMatchMVP = Consume<bool>(br); });
		RegisterField("TAGame.PRI_TA:RepStatTitles", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->RepStatTitles = Consume<struct RepStatTitle>(br); });
		RegisterField("TAGame.PRI_TA:bOnlineLoadoutsSet", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bOnlineLoadoutsSet = Consume<bool>(br); });
		RegisterField("TAGame.PRI_TA:bUsingItems", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bUsingItems = Consume<bool>(br); });
		RegisterField("TAGame.PRI_TA:PrimaryTitle", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->PrimaryTitle = Consume<struct ReplicatedTitle>(br); });
		RegisterField("TAGame.PRI_TA:bMatchAdmin", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bMatchAdmin = Consume<bool>(br); });
		RegisterField("TAGame.PRI_TA:bBusy", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bBusy = Consume<bool>(br); });
		RegisterField("TAGame.PRI_TA:bVoteToForfeitDisabled", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bVoteToForfeitDisabled = Consume<bool>(br); });
		RegisterField("TAGame.PRI_TA:bUsingFreecam", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bUsingFreecam = Consume<bool>(br); });
		RegisterField("TAGame.PRI_TA:ClientLoadoutOnline", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->ClientLoadoutOnline = Consume<struct OnlineLoadout>(br); });
		RegisterField("TAGame.PRI_TA:CameraYaw", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->CameraYaw = Consume<unsigned char>(br); });
		RegisterField("TAGame.PRI_TA:CameraPitch", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->CameraPitch = Consume<unsigned char>(br); });
		RegisterField("TAGame.PRI_TA:PawnType", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->PawnType = Consume<unsigned char>(br); });
		RegisterField("TAGame.PRI_TA:ReplicatedWorstNetQualityBeyondLatency", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->ReplicatedWorstNetQualityBeyondLatency = Consume<unsigned char>(br); });
		RegisterField("TAGame.PRI_TA:SteeringSensitivity", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->SteeringSensitivity = Consume<float>(br); });
		RegisterField("TAGame.PRI_TA:PartyLeader", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->PartyLeader = Consume<struct PartyLeader>(br); });
		RegisterField("TAGame.PRI_TA:TimeTillItem", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->TimeTillItem = Consume<int>(br); });
		RegisterField("TAGame.PRI_TA:ClientLoadoutsOnline", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->ClientLoadoutsOnline = Consume<struct ClientLoadoutsOnline>(br); });
		RegisterField("TAGame.PRI_TA:ClientLoadouts", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->ClientLoadouts = Consume<struct ClientLoadouts>(br); });
		RegisterField("TAGame.PRI_TA:ClientLoadout", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->ClientLoadout = Consume<struct ClientLoadout>(br); });
		RegisterField("TAGame.PRI_TA:CameraSettings", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->CameraSettings = Consume<struct CameraSettings>(br); });
		RegisterField("TAGame.PRI_TA:SecondaryTitle", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->SecondaryTitle = Consume<struct ReplicatedTitle>(br); });
		RegisterField("TAGame.PRI_TA:PlayerHistoryKey", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->PlayerHistoryKey = Consume<struct HistoryKey>(br); });

		RegisterClass<TAGame::RBActor_TA>("TAGame.RBActor_TA");
		RegisterField("TAGame.RBActor_TA:ReplicatedRBState", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::RBActor_TA>(struc)->ReplicatedRBState = Consume<struct ReplicatedRBState>(br); });
		RegisterField("TAGame.RBActor_TA:bReplayActor", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::RBActor_TA>(struc)->bReplayActor = Consume<bool>(br); });
		RegisterField("TAGame.RBActor_TA:bFrozen", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::RBActor_TA>(struc)->bFrozen = Consume<bool>(br); });
		RegisterField("TAGame.RBActor_TA:WeldedInfo", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::RBActor_TA>(struc)->WeldedInfo = Consume<struct WeldedInfo>(br); });
		RegisterField("TAGame.RBActor_TA:bIgnoreSyncing", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::RBActor_TA>(struc)->bIgnoreSyncing = Consume<bool>(br); });

		RegisterClass<TAGame::CarComponent_TA>("TAGame.CarComponent_TA");
		RegisterField("TAGame.CarComponent_TA:Vehicle", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::CarComponent_TA>(struc)->Vehicle = Consume<struct ActiveActor>(br); });
		RegisterField("TAGame.CarComponent_TA:ReplicatedActive", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::CarComponent_TA>(struc)->ReplicatedActive = Consume<unsigned char>(br); });
		RegisterField("TAGame.CarComponent_TA:ReplicatedActivityTime", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::CarComponent_TA>(struc)->ReplicatedActivityTime = Consume<float>(br); });

		RegisterClass<TAGame::CarComponent_Jump_TA>("TAGame.CarComponent_Jump_TA");

		RegisterClass<TAGame::CarComponent_DoubleJump_TA>("TAGame.CarComponent_DoubleJump_TA");

		RegisterClass<TAGame::CarComponent_Boost_TA>("TAGame.CarComponent_Boost_TA");
		RegisterField("TAGame.CarComponent_Boost_TA:RechargeDelay", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::CarComponent_Boost_TA>(struc)->RechargeDelay = Consume<float>(br); });
		RegisterField("TAGame.CarComponent_Boost_TA:bUnlimitedBoost", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::CarComponent_Boost_TA>(struc)->bUnlimitedBoost = Consume<bool>(br); });
		RegisterField("TAGame.CarComponent_Boost_TA:UnlimitedBoostRefCount", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::CarComponent_Boost_TA>(struc)->UnlimitedBoostRefCount = Consume<uint32_t>(br); });
		RegisterField("TAGame.CarComponent_Boost_TA:bNoBoost", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::CarComponent_Boost_TA>(struc)->bNoBoost = Consume<bool>(br); });
		RegisterField("TAGame.CarComponent_Boost_TA:ReplicatedBoostAmount", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::CarComponent_Boost_TA>(struc)->ReplicatedBoostAmount = Consume<unsigned char>(br); });
		RegisterField("TAGame.CarComponent_Boost_TA:RechargeRate", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::CarComponent_Boost_TA>(struc)->RechargeRate = Consume<float>(br); });
		RegisterField("TAGame.CarComponent_Boost_TA:BoostModifier", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::CarComponent_Boost_TA>(struc)->BoostModifier = Consume<float>(br); });

		RegisterClass<TAGame::CarComponent_Dodge_TA>("TAGame.CarComponent_Dodge_TA");
		RegisterField("TAGame.CarComponent_Dodge_TA:DodgeTorque", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::CarComponent_Dodge_TA>(struc)->DodgeTorque = Consume<struct Vector3I>(br); });

		RegisterClass<TAGame::CarComponent_FlipCar_TA>("TAGame.CarComponent_FlipCar_TA");
		RegisterField("TAGame.CarComponent_FlipCar_TA:bFlipRight", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::CarComponent_FlipCar_TA>(struc)->bFlipRight = Consume<bool>(br); });
		RegisterField("TAGame.CarComponent_FlipCar_TA:FlipCarTime", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::CarComponent_FlipCar_TA>(struc)->FlipCarTime = Consume<float>(br); });

		RegisterClass<TAGame::Ball_TA>("TAGame.Ball_TA");
		RegisterField("TAGame.Ball_TA:GameEvent", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->GameEvent = Consume<struct ActiveActor>(br); });
		RegisterField("TAGame.Ball_TA:ReplicatedPhysMatOverride", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->ReplicatedPhysMatOverride = Consume<struct ObjectTarget>(br); });
		RegisterField("TAGame.Ball_TA:ReplicatedBallGravityScale", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->ReplicatedBallGravityScale = Consume<float>(br); });
		RegisterField("TAGame.Ball_TA:ReplicatedBallScale", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->ReplicatedBallScale = Consume<float>(br); });
		RegisterField("TAGame.Ball_TA:HitTeamNum", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->HitTeamNum = Consume<unsigned char>(br); });
		RegisterField("TAGame.Ball_TA:ReplicatedWorldBounceScale", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->ReplicatedWorldBounceScale = Consume<float>(br); });
		RegisterField("TAGame.Ball_TA:ReplicatedAddedCarBounceScale", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->ReplicatedAddedCarBounceScale = Consume<float>(br); });
		RegisterField("TAGame.Ball_TA:ReplicatedExplosionData", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->ReplicatedExplosionData = Consume<struct ReplicatedExplosionData>(br); });
		RegisterField("TAGame.Ball_TA:ReplicatedBallMaxLinearSpeedScale", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->ReplicatedBallMaxLinearSpeedScale = Consume<float>(br); });
		RegisterField("TAGame.Ball_TA:ReplicatedExplosionDataExtended", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->ReplicatedExplosionDataExtended = Consume<struct ReplicatedExplosionDataExtended>(br); });

		RegisterClass<TAGame::Team_TA>("TAGame.Team_TA");
		RegisterField("TAGame.Team_TA:LogoData", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Team_TA>(struc)->LogoData = Consume<struct LogoData>(br); });
		RegisterField("TAGame.Team_TA:GameEvent", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Team_TA>(struc)->GameEvent = Consume<struct ActiveActor>(br); });
		RegisterField("TAGame.Team_TA:CustomTeamName", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Team_TA>(struc)->CustomTeamName = Consume<std::string>(br); });
		RegisterField("TAGame.Team_TA:ClubID", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Team_TA>(struc)->ClubID = Consume<uint64_t>(br); });
		RegisterField("TAGame.Team_TA:ClubColors", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Team_TA>(struc)->ClubColors = Consume<struct ClubColors>(br); });

		RegisterClass<TAGame::Team_Soccar_TA>("TAGame.Team_Soccar_TA");
		RegisterField("TAGame.Team_Soccar_TA:GameScore", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Team_Soccar_TA>(struc)->GameScore = Consume<uint32_t>(br); });

		RegisterClass<TAGame::BreakOutActor_Platform_TA>("TAGame.BreakOutActor_Platform_TA");
		RegisterField("TAGame.BreakOutActor_Platform_TA:DamageState", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::BreakOutActor_Platform_TA>(struc)->DamageState = Consume<struct DamageState>(br); });

		RegisterClass<TAGame::SpecialPickup_TA>("TAGame.SpecialPickup_TA");

		RegisterClass<TAGame::SpecialPickup_Targeted_TA>("TAGame.SpecialPickup_Targeted_TA");
		RegisterField("TAGame.SpecialPickup_Targeted_TA:Targeted", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::SpecialPickup_Targeted_TA>(struc)->Targeted = Consume<struct ActiveActor>(br); });

		RegisterClass<TAGame::SpecialPickup_Tornado_TA>("TAGame.SpecialPickup_Tornado_TA");

		RegisterClass<TAGame::SpecialPickup_HauntedBallBeam_TA>("TAGame.SpecialPickup_HauntedBallBeam_TA");

		RegisterClass<TAGame::SpecialPickup_BallVelcro_TA>("TAGame.SpecialPickup_BallVelcro_TA");
		RegisterField("TAGame.SpecialPickup_BallVelcro_TA:bHit", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::SpecialPickup_BallVelcro_TA>(struc)->bHit = Consume<bool>(br); });
		RegisterField("TAGame.SpecialPickup_BallVelcro_TA:bBroken", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::SpecialPickup_BallVelcro_TA>(struc)->bBroken = Consume<bool>(br); });
		RegisterField("TAGame.SpecialPickup_BallVelcro_TA:AttachTime", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::SpecialPickup_BallVelcro_TA>(struc)->AttachTime = Consume<float>(br); });
		RegisterField("TAGame.SpecialPickup_BallVelcro_TA:BreakTime", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::SpecialPickup_BallVelcro_TA>(struc)->BreakTime = Consume<float>(br); });

		RegisterClass<TAGame::SpecialPickup_Rugby_TA>("TAGame.SpecialPickup_Rugby_TA");
		RegisterField("TAGame.SpecialPickup_Rugby_TA:bBallWelded", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::SpecialPickup_Rugby_TA>(struc)->bBallWelded = Consume<bool>(br); });

		RegisterClass<TAGame::SpecialPickup_BallFreeze_TA>("TAGame.SpecialPickup_BallFreeze_TA");
		RegisterField("TAGame.SpecialPickup_BallFreeze_TA:RepOrigSpeed", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::SpecialPickup_BallFreeze_TA>(struc)->RepOrigSpeed = Consume<float>(br); });

		RegisterClass<TAGame::SpecialPickup_Spring_TA>("TAGame.SpecialPickup_Spring_TA");

		RegisterClass<TAGame::SpecialPickup_BallCarSpring_TA>("TAGame.SpecialPickup_BallCarSpring_TA");

		RegisterClass<TAGame::SpecialPickup_BallGravity_TA>("TAGame.SpecialPickup_BallGravity_TA");

		RegisterClass<TAGame::SpecialPickup_GrapplingHook_TA>("TAGame.SpecialPickup_GrapplingHook_TA");

		RegisterClass<TAGame::SpecialPickup_BallLasso_TA>("TAGame.SpecialPickup_BallLasso_TA");

		RegisterClass<TAGame::SpecialPickup_BoostOverride_TA>("TAGame.SpecialPickup_BoostOverride_TA");

		RegisterClass<TAGame::SpecialPickup_Batarang_TA>("TAGame.SpecialPickup_Batarang_TA");

		RegisterClass<TAGame::SpecialPickup_HitForce_TA>("TAGame.SpecialPickup_HitForce_TA");

		RegisterClass<TAGame::SpecialPickup_Swapper_TA>("TAGame.SpecialPickup_Swapper_TA");

		RegisterClass<TAGame::CrowdManager_TA>("TAGame.CrowdManager_TA");
		RegisterField("TAGame.CrowdManager_TA:GameEvent", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::CrowdManager_TA>(struc)->GameEvent = Consume<struct ActiveActor>(br); });
		RegisterField("TAGame.CrowdManager_TA:ReplicatedGlobalOneShotSound", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::CrowdManager_TA>(struc)->ReplicatedGlobalOneShotSound = Consume<struct ObjectTarget>(br); });

		RegisterClass<TAGame::CrowdActor_TA>("TAGame.CrowdActor_TA");
		RegisterField("TAGame.CrowdActor_TA:GameEvent", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::CrowdActor_TA>(struc)->GameEvent = Consume<struct ActiveActor>(br); });
		RegisterField("TAGame.CrowdActor_TA:ReplicatedOneShotSound", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::CrowdActor_TA>(struc)->ReplicatedOneShotSound = Consume<struct ObjectTarget>(br); });
		RegisterField("TAGame.CrowdActor_TA:ReplicatedRoundCountDownNumber", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::CrowdActor_TA>(struc)->ReplicatedRoundCountDownNumber = Consume<uint32_t>(br); });
		RegisterField("TAGame.CrowdActor_TA:ReplicatedCountDownNumber", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::CrowdActor_TA>(struc)->ReplicatedCountDownNumber = Consume<uint32_t>(br); });
		RegisterField("TAGame.CrowdActor_TA:ModifiedNoise", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::CrowdActor_TA>(struc)->ModifiedNoise = Consume<float>(br); });

		RegisterClass<TAGame::InMapScoreboard_TA>("TAGame.InMapScoreboard_TA");

		RegisterClass<TAGame::Vehicle_TA>("TAGame.Vehicle_TA");
		RegisterField("TAGame.Vehicle_TA:ReplicatedThrottle", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Vehicle_TA>(struc)->ReplicatedThrottle = Consume<unsigned char>(br); });
		RegisterField("TAGame.Vehicle_TA:bReplicatedHandbrake", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Vehicle_TA>(struc)->bReplicatedHandbrake = Consume<bool>(br); });
		RegisterField("TAGame.Vehicle_TA:bDriving", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Vehicle_TA>(struc)->bDriving = Consume<bool>(br); });
		RegisterField("TAGame.Vehicle_TA:ReplicatedSteer", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Vehicle_TA>(struc)->ReplicatedSteer = Consume<unsigned char>(br); });

		RegisterClass<TAGame::Car_TA>("TAGame.Car_TA");
		RegisterField("TAGame.Car_TA:AttachedPickup", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Car_TA>(struc)->AttachedPickup = Consume<struct ActiveActor>(br); });
		RegisterField("TAGame.Car_TA:AddedCarForceMultiplier", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Car_TA>(struc)->AddedCarForceMultiplier = Consume<float>(br); });
		RegisterField("TAGame.Car_TA:ReplicatedCarScale", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Car_TA>(struc)->ReplicatedCarScale = Consume<float>(br); });
		RegisterField("TAGame.Car_TA:AddedBallForceMultiplier", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Car_TA>(struc)->AddedBallForceMultiplier = Consume<float>(br); });
		RegisterField("TAGame.Car_TA:TeamPaint", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Car_TA>(struc)->TeamPaint = Consume<struct TeamPaint>(br); });
		RegisterField("TAGame.Car_TA:ReplicatedDemolish", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Car_TA>(struc)->ReplicatedDemolish = Consume<struct ReplicatedDemolish>(br); });
		RegisterField("TAGame.Car_TA:ClubColors", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Car_TA>(struc)->ClubColors = Consume<struct ClubColors>(br); });

		RegisterClass<TAGame::Car_Season_TA>("TAGame.Car_Season_TA");

		RegisterClass<TAGame::CameraSettingsActor_TA>("TAGame.CameraSettingsActor_TA");
		RegisterField("TAGame.CameraSettingsActor_TA:PRI", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(struc)->PRI = Consume<struct ActiveActor>(br); });
		RegisterField("TAGame.CameraSettingsActor_TA:CameraPitch", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(struc)->CameraPitch = Consume<unsigned char>(br); });
		RegisterField("TAGame.CameraSettingsActor_TA:bMouseCameraToggleEnabled", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(struc)->bMouseCameraToggleEnabled = Consume<bool>(br); });
		RegisterField("TAGame.CameraSettingsActor_TA:bUsingSecondaryCamera", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(struc)->bUsingSecondaryCamera = Consume<bool>(br); });
		RegisterField("TAGame.CameraSettingsActor_TA:bUsingBehindView", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(struc)->bUsingBehindView = Consume<bool>(br); });
		RegisterField("TAGame.CameraSettingsActor_TA:ProfileSettings", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(struc)->ProfileSettings = Consume<struct CameraSettings>(br); });
		RegisterField("TAGame.CameraSettingsActor_TA:bUsingSwivel", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(struc)->bUsingSwivel = Consume<bool>(br); });
		RegisterField("TAGame.CameraSettingsActor_TA:CameraYaw", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(struc)->CameraYaw = Consume<unsigned char>(br); });

		RegisterClass<TAGame::GRI_TA>("TAGame.GRI_TA");
		RegisterField("TAGame.GRI_TA:NewDedicatedServerIP", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GRI_TA>(struc)->NewDedicatedServerIP = Consume<std::string>(br); });

		RegisterClass<TAGame::Ball_Breakout_TA>("TAGame.Ball_Breakout_TA");
		RegisterField("TAGame.Ball_Breakout_TA:DamageIndex", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Ball_Breakout_TA>(struc)->DamageIndex = Consume<uint32_t>(br); });
		RegisterField("TAGame.Ball_Breakout_TA:AppliedDamage", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Ball_Breakout_TA>(struc)->AppliedDamage = Consume<struct AppliedDamage>(br); });
		RegisterField("TAGame.Ball_Breakout_TA:LastTeamTouch", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Ball_Breakout_TA>(struc)->LastTeamTouch = Consume<unsigned char>(br); });

		RegisterClass<TAGame::VehiclePickup_TA>("TAGame.VehiclePickup_TA");
		RegisterField("TAGame.VehiclePickup_TA:bNoPickup", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::VehiclePickup_TA>(struc)->bNoPickup = Consume<bool>(br); });
		RegisterField("TAGame.VehiclePickup_TA:ReplicatedPickupData", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::VehiclePickup_TA>(struc)->ReplicatedPickupData = Consume<struct ReplicatedPickupData>(br); });

		RegisterClass<TAGame::VehiclePickup_Boost_TA>("TAGame.VehiclePickup_Boost_TA");

		RegisterClass<TAGame::Ball_Haunted_TA>("TAGame.Ball_Haunted_TA");
		RegisterField("TAGame.Ball_Haunted_TA:DeactivatedGoalIndex", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Ball_Haunted_TA>(struc)->DeactivatedGoalIndex = Consume<unsigned char>(br); });
		RegisterField("TAGame.Ball_Haunted_TA:bIsBallBeamed", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Ball_Haunted_TA>(struc)->bIsBallBeamed = Consume<bool>(br); });
		RegisterField("TAGame.Ball_Haunted_TA:ReplicatedBeamBrokenValue", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Ball_Haunted_TA>(struc)->ReplicatedBeamBrokenValue = Consume<unsigned char>(br); });
		RegisterField("TAGame.Ball_Haunted_TA:LastTeamTouch", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Ball_Haunted_TA>(struc)->LastTeamTouch = Consume<unsigned char>(br); });
		RegisterField("TAGame.Ball_Haunted_TA:TotalActiveBeams", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::Ball_Haunted_TA>(struc)->TotalActiveBeams = Consume<unsigned char>(br); });

		RegisterClass<TAGame::GameEvent_TA>("TAGame.GameEvent_TA");
		RegisterField("TAGame.GameEvent_TA:ReplicatedRoundCountDownNumber", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->ReplicatedRoundCountDownNumber = Consume<uint32_t>(br); });
		RegisterField("TAGame.GameEvent_TA:ActivatorCar", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->ActivatorCar = Consume<struct ActiveActor>(br); });
		RegisterField("TAGame.GameEvent_TA:ReplicatedGameStateTimeRemaining", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->ReplicatedGameStateTimeRemaining = Consume<uint32_t>(br); });
		RegisterField("TAGame.GameEvent_TA:ReplicatedStateName", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->ReplicatedStateName = Consume<uint32_t>(br); });
		RegisterField("TAGame.GameEvent_TA:MatchTypeClass", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->MatchTypeClass = Consume<struct ObjectTarget>(br); });
		RegisterField("TAGame.GameEvent_TA:BotSkill", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->BotSkill = Consume<uint32_t>(br); });
		RegisterField("TAGame.GameEvent_TA:bHasLeaveMatchPenalty", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->bHasLeaveMatchPenalty = Consume<bool>(br); });
		RegisterField("TAGame.GameEvent_TA:bCanVoteToForfeit", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->bCanVoteToForfeit = Consume<bool>(br); });
		RegisterField("TAGame.GameEvent_TA:bAllowReadyUp", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->bAllowReadyUp = Consume<bool>(br); });
		RegisterField("TAGame.GameEvent_TA:GameMode", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->GameMode = Consume<struct GameMode>(br); });
		RegisterField("TAGame.GameEvent_TA:ReplicatedStateIndex", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->ReplicatedStateIndex = Consume<struct ReplicatedStateIndex>(br); });

		RegisterClass<TAGame::GameEvent_Team_TA>("TAGame.GameEvent_Team_TA");
		RegisterField("TAGame.GameEvent_Team_TA:MaxTeamSize", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_Team_TA>(struc)->MaxTeamSize = Consume<uint32_t>(br); });
		RegisterField("TAGame.GameEvent_Team_TA:bForfeit", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_Team_TA>(struc)->bForfeit = Consume<bool>(br); });
		RegisterField("TAGame.GameEvent_Team_TA:bDisableMutingOtherTeam", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_Team_TA>(struc)->bDisableMutingOtherTeam = Consume<bool>(br); });

		RegisterClass<TAGame::GameEvent_Soccar_TA>("TAGame.GameEvent_Soccar_TA");
		RegisterField("TAGame.GameEvent_Soccar_TA:bOverTime", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->bOverTime = Consume<bool>(br); });
		RegisterField("TAGame.GameEvent_Soccar_TA:GameTime", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->GameTime = Consume<uint32_t>(br); });
		RegisterField("TAGame.GameEvent_Soccar_TA:MVP", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->MVP = Consume<struct ActiveActor>(br); });
		RegisterField("TAGame.GameEvent_Soccar_TA:MatchWinner", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->MatchWinner = Consume<struct ActiveActor>(br); });
		RegisterField("TAGame.GameEvent_Soccar_TA:GameWinner", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->GameWinner = Consume<struct ActiveActor>(br); });
		RegisterField("TAGame.GameEvent_Soccar_TA:SubRulesArchetype", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->SubRulesArchetype = Consume<struct ObjectTarget>(br); });
		RegisterField("TAGame.GameEvent_Soccar_TA:SecondsRemaining", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->SecondsRemaining = Consume<uint32_t>(br); });
		RegisterField("TAGame.GameEvent_Soccar_TA:RoundNum", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->RoundNum = Consume<uint32_t>(br); });
		RegisterField("TAGame.GameEvent_Soccar_TA:SeriesLength", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->SeriesLength = Consume<uint32_t>(br); });
		RegisterField("TAGame.GameEvent_Soccar_TA:ReplicatedMusicStinger", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->ReplicatedMusicStinger = Consume<struct ReplicatedMusicStringer>(br); });
		RegisterField("TAGame.GameEvent_Soccar_TA:bBallHasBeenHit", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->bBallHasBeenHit = Consume<bool>(br); });
		RegisterField("TAGame.GameEvent_Soccar_TA:bUnlimitedTime", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->bUnlimitedTime = Consume<bool>(br); });
		RegisterField("TAGame.GameEvent_Soccar_TA:ReplicatedStatEvent", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->ReplicatedStatEvent = Consume<struct ReplicatedStatEvent>(br); });
		RegisterField("TAGame.GameEvent_Soccar_TA:bShowIntroScene", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->bShowIntroScene = Consume<bool>(br); });
		RegisterField("TAGame.GameEvent_Soccar_TA:bClubMatch", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->bClubMatch = Consume<bool>(br); });
		RegisterField("TAGame.GameEvent_Soccar_TA:ReplicatedScoredOnTeam", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->ReplicatedScoredOnTeam = Consume<unsigned char>(br); });
		RegisterField("TAGame.GameEvent_Soccar_TA:bMatchEnded", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->bMatchEnded = Consume<bool>(br); });
		RegisterField("TAGame.GameEvent_Soccar_TA:ReplicatedServerPerformanceState", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->ReplicatedServerPerformanceState = Consume<unsigned char>(br); });

		RegisterClass<TAGame::GameEvent_Breakout_TA>("TAGame.GameEvent_Breakout_TA");

		RegisterClass<TAGame::GameEvent_Season_TA>("TAGame.GameEvent_Season_TA");

		RegisterClass<TAGame::GameEvent_SoccarPrivate_TA>("TAGame.GameEvent_SoccarPrivate_TA");
		RegisterField("TAGame.GameEvent_SoccarPrivate_TA:MatchSettings", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) { std::static_pointer_cast<CPPRP::TAGame::GameEvent_SoccarPrivate_TA>(struc)->MatchSettings = Consume<struct PrivateMatchSettings>(br); });

		RegisterClass<TAGame::GameEvent_SoccarSplitscreen_TA>("TAGame.GameEvent_SoccarSplitscreen_TA");

		RegisterClass<TAGame::GameEvent_Tutorial_TA>("TAGame.GameEvent_Tutorial_TA");

		RegisterClass<TAGame::HauntedBallTrapTrigger_TA>("TAGame.HauntedBallTrapTrigger_TA");
		return 0;
	}

	static int T = Initializor<int>();
}