#pragma once
#include "NetworkData.h"
#include <string>
#include <stdint.h>


#define FIELD(...) 
#define CLASS(...) 
namespace CPPRP
{

	namespace Engine
	{
		struct Object
		{
			virtual ~Object() {} //Makes it polymorphic
		};

		CLASS(Actor, Object);
		struct Actor : public Object
		{
			FIELD(Actor, DrawScale, float, "Engine.Actor:DrawScale");
			float DrawScale;

			FIELD(Actor, bCollideActors, bool, "Engine.Actor:bCollideActors");
			bool bCollideActors;

			FIELD(Actor, bCollideWorld, bool, "Engine.Actor:bCollideWorld");
			bool bCollideWorld;

			FIELD(Actor, bNetOwner, bool, "Engine.Actor:bNetOwner");
			bool bNetOwner;

			FIELD(Actor, Base, struct ActorBase, "Engine.Actor:Base");
			struct ActorBase Base;

			FIELD(Actor, bBlockActors, bool, "Engine.Actor:bBlockActors");
			bool bBlockActors;

			FIELD(Actor, bHidden, bool, "Engine.Actor:bHidden");
			bool bHidden;

			FIELD(Actor, bTearOff, bool, "Engine.Actor:bTearOff");
			bool bTearOff;

			FIELD(Actor, Location, struct Vector3I, "Engine.Actor:Location");
			struct Vector3I Location;

			FIELD(Actor, Rotation, struct Rotator, "Engine.Actor:Rotation");
			struct Rotator Rotation;
		};

		CLASS(Info, Actor);
		struct Info : public Actor
		{

		};

		CLASS(ReplicationInfo, Info)
		struct ReplicationInfo : public Info
		{

		};

		CLASS(GameReplicationInfo, ReplicationInfo);
		struct GameReplicationInfo : public ReplicationInfo
		{
			FIELD(GameReplicationInfo, ServerName, std::string, "Engine.GameReplicationInfo:ServerName");
			std::string ServerName;

			FIELD(GameReplicationInfo, GameClass, struct ObjectTarget, "Engine.GameReplicationInfo:GameClass");
			struct ObjectTarget GameClass;

			FIELD(GameReplicationInfo, bStopCountDown, bool, "Engine.GameReplicationInfo:bStopCountDown");
			bool bStopCountDown;

			FIELD(GameReplicationInfo, bMatchIsOver, bool, "Engine.GameReplicationInfo:bMatchIsOver");
			bool bMatchIsOver;

			FIELD(GameReplicationInfo, bMatchHasBegun, bool, "Engine.GameReplicationInfo:bMatchHasBegun");
			bool bMatchHasBegun;
		};

		CLASS(Pawn, Actor);
		struct Pawn : public Actor
		{
			FIELD(Pawn, PlayerReplicationInfo, struct ActiveActor, "Engine.Pawn:PlayerReplicationInfo");
			struct ActiveActor PlayerReplicationInfo;

			FIELD(Pawn, HealthMax, uint32_t, "Engine.Pawn:HealthMax");
			uint32_t HealthMax;

			FIELD(Pawn, bIsCrouched, bool, "Engine.Pawn:bIsCrouched");
			bool bIsCrouched;

		};


		CLASS(PlayerReplicationInfo, ReplicationInfo);
		struct PlayerReplicationInfo : public ReplicationInfo
		{
			FIELD(PlayerReplicationInfo, Team, struct ActiveActor, "Engine.PlayerReplicationInfo:Team");
			struct ActiveActor Team;

			FIELD(PlayerReplicationInfo, bReadyToPlay, bool, "Engine.PlayerReplicationInfo:bReadyToPlay");
			bool bReadyToPlay;

			FIELD(PlayerReplicationInfo, PlayerName, std::string, "Engine.PlayerReplicationInfo:PlayerName");
			std::string PlayerName;

			FIELD(PlayerReplicationInfo, RemoteUserData, std::string, "Engine.PlayerReplicationInfo:RemoteUserData");
			std::string RemoteUserData;

			FIELD(PlayerReplicationInfo, bWaitingPlayer, bool, "Engine.PlayerReplicationInfo:bWaitingPlayer");
			bool bWaitingPlayer;

			FIELD(PlayerReplicationInfo, Score, uint32_t, "Engine.PlayerReplicationInfo:Score");
			uint32_t Score;

			FIELD(PlayerReplicationInfo, PlayerID, uint32_t, "Engine.PlayerReplicationInfo:PlayerID");
			uint32_t PlayerID;

			FIELD(PlayerReplicationInfo, bBot, bool, "Engine.PlayerReplicationInfo:bBot");
			bool bBot;

			FIELD(PlayerReplicationInfo, bIsSpectator, bool, "Engine.PlayerReplicationInfo:bIsSpectator");
			bool bIsSpectator;

			FIELD(PlayerReplicationInfo, bTimedOut, bool, "Engine.PlayerReplicationInfo:bTimedOut");
			bool bTimedOut;

			FIELD(PlayerReplicationInfo, bAdmin, bool, "Engine.PlayerReplicationInfo:bAdmin");
			bool bAdmin;

			FIELD(PlayerReplicationInfo, bIsInactive, bool, "Engine.PlayerReplicationInfo:bIsInactive");
			bool bIsInactive;

			FIELD(PlayerReplicationInfo, bOnlySpectator, bool, "Engine.PlayerReplicationInfo:bOnlySpectator");
			bool bOnlySpectator;

			FIELD(PlayerReplicationInfo, Ping, unsigned char, "Engine.PlayerReplicationInfo:Ping");
			unsigned char Ping;

			FIELD(PlayerReplicationInfo, UniqueId, class std::shared_ptr<struct UniqueId>, "Engine.PlayerReplicationInfo:UniqueId");
			class std::shared_ptr<struct UniqueId> UniqueId;

		};

		CLASS(TeamInfo, ReplicationInfo);
		struct TeamInfo : public ReplicationInfo
		{
			FIELD(TeamInfo, Score, uint32_t, "Engine.TeamInfo:Score");
			uint32_t Score;
		};

		CLASS(WorldInfo, Info);
		struct WorldInfo : public Info
		{
			FIELD(WorldInfo, WorldGravityZ, float, "Engine.WorldInfo:WorldGravityZ");
			float WorldGravityZ;

			FIELD(WorldInfo, TimeDilation, float, "Engine.WorldInfo:TimeDilation");
			float TimeDilation;
		};

	};

	namespace ProjectX
	{
		CLASS(GRI_X, Engine::GameReplicationInfo);
		struct GRI_X : public Engine::GameReplicationInfo
		{
			FIELD(GRI_X, MatchGUID, std::string, "ProjectX.GRI_X:MatchGUID");
			std::string MatchGUID;

			FIELD(GRI_X, ReplicatedGameMutatorIndex, int, "ProjectX.GRI_X:ReplicatedGameMutatorIndex");
			int ReplicatedGameMutatorIndex;

			FIELD(GRI_X, bGameStarted, bool, "ProjectX.GRI_X:bGameStarted");
			bool bGameStarted;

			FIELD(GRI_X, ReplicatedGamePlaylist, uint32_t, "ProjectX.GRI_X:ReplicatedGamePlaylist");
			uint32_t ReplicatedGamePlaylist;

			FIELD(GRI_X, GameServerID, uint64_t, "ProjectX.GRI_X:GameServerID");
			uint64_t GameServerID;

			FIELD(GRI_X, Reservations, struct Reservation, "ProjectX.GRI_X:Reservations");
			struct Reservation Reservations;

		};

		CLASS(Pawn_X, Engine::Pawn);
		struct Pawn_X : public Engine::Pawn
		{

		};

		CLASS(PRI_X, Engine::PlayerReplicationInfo)
		struct PRI_X : public Engine::PlayerReplicationInfo
		{

		};

	};

	namespace TAGame
	{
		CLASS(PRI_TA, ProjectX::PRI_X);
		struct PRI_TA : public ProjectX::PRI_X
		{
			FIELD(PRI_TA, MatchShots, uint32_t, "TAGame.PRI_TA:MatchShots");
			uint32_t MatchShots;

			FIELD(PRI_TA, PersistentCamera, struct ActiveActor, "TAGame.PRI_TA:PersistentCamera");
			struct ActiveActor PersistentCamera;

			FIELD(PRI_TA, SkillTier, struct SkillTier, "TAGame.PRI_TA:SkillTier");
			struct SkillTier SkillTier;

			FIELD(PRI_TA, bUsingBehindView, bool, "TAGame.PRI_TA:bUsingBehindView");
			bool bUsingBehindView;

			FIELD(PRI_TA, MatchAssists, uint32_t, "TAGame.PRI_TA:MatchAssists");
			uint32_t MatchAssists;

			FIELD(PRI_TA, RespawnTimeRemaining, int, "TAGame.PRI_TA:RespawnTimeRemaining");
			int RespawnTimeRemaining;

			FIELD(PRI_TA, bOnlineLoadoutSet, bool, "TAGame.PRI_TA:bOnlineLoadoutSet");
			bool bOnlineLoadoutSet;

			FIELD(PRI_TA, MatchGoals, uint32_t, "TAGame.PRI_TA:MatchGoals");
			uint32_t MatchGoals;

			FIELD(PRI_TA, ReplicatedGameEvent, struct ActiveActor, "TAGame.PRI_TA:ReplicatedGameEvent");
			struct ActiveActor ReplicatedGameEvent;

			FIELD(PRI_TA, TotalXP, uint32_t, "TAGame.PRI_TA:TotalXP");
			uint32_t TotalXP;

			FIELD(PRI_TA, MatchScore, uint32_t, "TAGame.PRI_TA:MatchScore");
			uint32_t MatchScore;

			FIELD(PRI_TA, MatchSaves, uint32_t, "TAGame.PRI_TA:MatchSaves");
			uint32_t MatchSaves;

			FIELD(PRI_TA, Title, uint32_t, "TAGame.PRI_TA:Title");
			uint32_t Title;

			FIELD(PRI_TA, ClubID, uint64_t, "TAGame.PRI_TA:ClubID");
			uint64_t ClubID;

			FIELD(PRI_TA, MaxTimeTillItem, int, "TAGame.PRI_TA:MaxTimeTillItem");
			int MaxTimeTillItem;

			FIELD(PRI_TA, MatchBreakoutDamage, uint32_t, "TAGame.PRI_TA:MatchBreakoutDamage");
			uint32_t MatchBreakoutDamage;

			FIELD(PRI_TA, BotProductName, uint32_t, "TAGame.PRI_TA:BotProductName");
			uint32_t BotProductName;

			FIELD(PRI_TA, bReady, bool, "TAGame.PRI_TA:bReady");
			bool bReady;

			FIELD(PRI_TA, SpectatorShortcut, uint32_t, "TAGame.PRI_TA:SpectatorShortcut");
			uint32_t SpectatorShortcut;

			FIELD(PRI_TA, bUsingSecondaryCamera, bool, "TAGame.PRI_TA:bUsingSecondaryCamera");
			bool bUsingSecondaryCamera;

			FIELD(PRI_TA, PlayerHistoryValid, bool, "TAGame.PRI_TA:PlayerHistoryValid");
			bool PlayerHistoryValid;

			FIELD(PRI_TA, bIsInSplitScreen, bool, "TAGame.PRI_TA:bIsInSplitScreen");
			bool bIsInSplitScreen;

			FIELD(PRI_TA, bMatchMVP, bool, "TAGame.PRI_TA:bMatchMVP");
			bool bMatchMVP;

			FIELD(PRI_TA, RepStatTitles, struct RepStatTitle, "TAGame.PRI_TA:RepStatTitles");
			struct RepStatTitle RepStatTitles;

			FIELD(PRI_TA, bOnlineLoadoutsSet, bool, "TAGame.PRI_TA:bOnlineLoadoutsSet");
			bool bOnlineLoadoutsSet;

			FIELD(PRI_TA, bUsingItems, bool, "TAGame.PRI_TA:bUsingItems");
			bool bUsingItems;

			FIELD(PRI_TA, PrimaryTitle, struct ReplicatedTitle, "TAGame.PRI_TA:PrimaryTitle");
			struct ReplicatedTitle PrimaryTitle;

			FIELD(PRI_TA, bMatchAdmin, bool, "TAGame.PRI_TA:bMatchAdmin");
			bool bMatchAdmin;

			FIELD(PRI_TA, bBusy, bool, "TAGame.PRI_TA:bBusy");
			bool bBusy;

			FIELD(PRI_TA, bVoteToForfeitDisabled, bool, "TAGame.PRI_TA:bVoteToForfeitDisabled");
			bool bVoteToForfeitDisabled;

			FIELD(PRI_TA, bUsingFreecam, bool, "TAGame.PRI_TA:bUsingFreecam");
			bool bUsingFreecam;

			FIELD(PRI_TA, ClientLoadoutOnline, struct OnlineLoadout, "TAGame.PRI_TA:ClientLoadoutOnline");
			struct OnlineLoadout ClientLoadoutOnline;

			FIELD(PRI_TA, CameraYaw, unsigned char, "TAGame.PRI_TA:CameraYaw");
			unsigned char CameraYaw;

			FIELD(PRI_TA, CameraPitch, unsigned char, "TAGame.PRI_TA:CameraPitch");
			unsigned char CameraPitch;

			FIELD(PRI_TA, PawnType, unsigned char, "TAGame.PRI_TA:PawnType");
			unsigned char PawnType;

			FIELD(PRI_TA, ReplicatedWorstNetQualityBeyondLatency, unsigned char, "TAGame.PRI_TA:ReplicatedWorstNetQualityBeyondLatency");
			unsigned char ReplicatedWorstNetQualityBeyondLatency;

			FIELD(PRI_TA, SteeringSensitivity, float, "TAGame.PRI_TA:SteeringSensitivity");
			float SteeringSensitivity;

			FIELD(PRI_TA, PartyLeader, struct PartyLeader, "TAGame.PRI_TA:PartyLeader");
			struct PartyLeader PartyLeader;

			FIELD(PRI_TA, TimeTillItem, int, "TAGame.PRI_TA:TimeTillItem");
			int TimeTillItem;

			FIELD(PRI_TA, ClientLoadoutsOnline, struct ClientLoadoutsOnline, "TAGame.PRI_TA:ClientLoadoutsOnline");
			struct ClientLoadoutsOnline ClientLoadoutsOnline;

			FIELD(PRI_TA, ClientLoadouts, struct ClientLoadouts, "TAGame.PRI_TA:ClientLoadouts");
			struct ClientLoadouts ClientLoadouts;

			FIELD(PRI_TA, ClientLoadout, struct ClientLoadout, "TAGame.PRI_TA:ClientLoadout");
			struct ClientLoadout ClientLoadout;

			FIELD(PRI_TA, CameraSettings, struct CameraSettings, "TAGame.PRI_TA:CameraSettings");
			struct CameraSettings CameraSettings;

			FIELD(PRI_TA, SecondaryTitle, struct ReplicatedTitle, "TAGame.PRI_TA:SecondaryTitle");
			struct ReplicatedTitle SecondaryTitle;

			FIELD(PRI_TA, PlayerHistoryKey, struct HistoryKey, "TAGame.PRI_TA:PlayerHistoryKey");
			struct HistoryKey PlayerHistoryKey;

		};

		CLASS(RBActor_TA, ProjectX::Pawn_X);
		struct RBActor_TA : public ProjectX::Pawn_X
		{
			FIELD(RBActor_TA, ReplicatedRBState, struct ReplicatedRBState, "TAGame.RBActor_TA:ReplicatedRBState");
			struct ReplicatedRBState ReplicatedRBState;

			FIELD(RBActor_TA, bReplayActor, bool, "TAGame.RBActor_TA:bReplayActor");
			bool bReplayActor;

			FIELD(RBActor_TA, bFrozen, bool, "TAGame.RBActor_TA:bFrozen");
			bool bFrozen;

			FIELD(RBActor_TA, WeldedInfo, struct WeldedInfo, "TAGame.RBActor_TA:WeldedInfo");
			struct WeldedInfo WeldedInfo;

			FIELD(RBActor_TA, bIgnoreSyncing, bool, "TAGame.RBActor_TA:bIgnoreSyncing");
			bool bIgnoreSyncing;

		};

		CLASS(CarComponent_TA, Engine::ReplicationInfo);
		struct CarComponent_TA : public Engine::ReplicationInfo
		{
			FIELD(CarComponent_TA, Vehicle, struct ActiveActor, "TAGame.CarComponent_TA:Vehicle");
			struct ActiveActor Vehicle;

			FIELD(CarComponent_TA, ReplicatedActive, unsigned char, "TAGame.CarComponent_TA:ReplicatedActive");
			unsigned char ReplicatedActive;

			FIELD(CarComponent_TA, ReplicatedActivityTime, float, "TAGame.CarComponent_TA:ReplicatedActivityTime");
			float ReplicatedActivityTime;
		};

		CLASS(CarComponent_Jump_TA, CarComponent_TA);
		struct CarComponent_Jump_TA : public CarComponent_TA
		{

		};

		CLASS(CarComponent_DoubleJump_TA, CarComponent_TA);
		struct CarComponent_DoubleJump_TA : public CarComponent_TA
		{

		};

		CLASS(CarComponent_Boost_TA, CarComponent_TA);
		struct CarComponent_Boost_TA : public CarComponent_TA
		{
			FIELD(CarComponent_Boost_TA, RechargeDelay, float, "TAGame.CarComponent_Boost_TA:RechargeDelay");
			float RechargeDelay;

			FIELD(CarComponent_Boost_TA, bUnlimitedBoost, bool, "TAGame.CarComponent_Boost_TA:bUnlimitedBoost");
			bool bUnlimitedBoost;

			FIELD(CarComponent_Boost_TA, UnlimitedBoostRefCount, uint32_t, "TAGame.CarComponent_Boost_TA:UnlimitedBoostRefCount");
			uint32_t UnlimitedBoostRefCount;

			FIELD(CarComponent_Boost_TA, bNoBoost, bool, "TAGame.CarComponent_Boost_TA:bNoBoost");
			bool bNoBoost;

			FIELD(CarComponent_Boost_TA, ReplicatedBoostAmount, unsigned char, "TAGame.CarComponent_Boost_TA:ReplicatedBoostAmount");
			unsigned char ReplicatedBoostAmount;

			FIELD(CarComponent_Boost_TA, RechargeRate, float, "TAGame.CarComponent_Boost_TA:RechargeRate");
			float RechargeRate;

			FIELD(CarComponent_Boost_TA, BoostModifier, float, "TAGame.CarComponent_Boost_TA:BoostModifier");
			float BoostModifier;

		};

		CLASS(CarComponent_Dodge_TA, CarComponent_TA);
		struct CarComponent_Dodge_TA : public CarComponent_TA
		{
			FIELD(CarComponent_Dodge_TA, DodgeTorque, struct Vector3I, "TAGame.CarComponent_Dodge_TA:DodgeTorque");
			struct Vector3I DodgeTorque;

		};


		CLASS(CarComponent_FlipCar_TA, CarComponent_TA);
		struct CarComponent_FlipCar_TA : public CarComponent_TA
		{
			FIELD(CarComponent_FlipCar_TA, bFlipRight, bool, "TAGame.CarComponent_FlipCar_TA:bFlipRight");
			bool bFlipRight;

			FIELD(CarComponent_FlipCar_TA, FlipCarTime, float, "TAGame.CarComponent_FlipCar_TA:FlipCarTime");
			float FlipCarTime;

		};

		CLASS(Ball_TA, RBActor_TA);
		struct Ball_TA : public RBActor_TA
		{
			FIELD(Ball_TA, GameEvent, struct ActiveActor, "TAGame.Ball_TA:GameEvent");
			struct ActiveActor GameEvent;

			FIELD(Ball_TA, ReplicatedPhysMatOverride, struct ObjectTarget, "TAGame.Ball_TA:ReplicatedPhysMatOverride");
			struct ObjectTarget ReplicatedPhysMatOverride;

			FIELD(Ball_TA, ReplicatedBallGravityScale, float, "TAGame.Ball_TA:ReplicatedBallGravityScale");
			float ReplicatedBallGravityScale;

			FIELD(Ball_TA, ReplicatedBallScale, float, "TAGame.Ball_TA:ReplicatedBallScale");
			float ReplicatedBallScale;

			FIELD(Ball_TA, HitTeamNum, unsigned char, "TAGame.Ball_TA:HitTeamNum");
			unsigned char HitTeamNum;

			FIELD(Ball_TA, ReplicatedWorldBounceScale, float, "TAGame.Ball_TA:ReplicatedWorldBounceScale");
			float ReplicatedWorldBounceScale;

			FIELD(Ball_TA, ReplicatedAddedCarBounceScale, float, "TAGame.Ball_TA:ReplicatedAddedCarBounceScale");
			float ReplicatedAddedCarBounceScale;

			FIELD(Ball_TA, ReplicatedExplosionData, struct ReplicatedExplosionData, "TAGame.Ball_TA:ReplicatedExplosionData");
			struct ReplicatedExplosionData ReplicatedExplosionData;

			FIELD(Ball_TA, ReplicatedBallMaxLinearSpeedScale, float, "TAGame.Ball_TA:ReplicatedBallMaxLinearSpeedScale");
			float ReplicatedBallMaxLinearSpeedScale;

			FIELD(Ball_TA, ReplicatedExplosionDataExtended, struct ReplicatedExplosionDataExtended, "TAGame.Ball_TA:ReplicatedExplosionDataExtended");
			struct ReplicatedExplosionDataExtended ReplicatedExplosionDataExtended;

		};

		CLASS(Team_TA, Engine::TeamInfo);
		struct Team_TA : public Engine::TeamInfo
		{
			FIELD(Team_TA, LogoData, struct LogoData, "TAGame.Team_TA:LogoData");
			struct LogoData LogoData;

			FIELD(Team_TA, GameEvent, struct ActiveActor, "TAGame.Team_TA:GameEvent");
			struct ActiveActor GameEvent;

			FIELD(Team_TA, CustomTeamName, std::string, "TAGame.Team_TA:CustomTeamName");
			std::string CustomTeamName;

			FIELD(Team_TA, ClubID, uint64_t, "TAGame.Team_TA:ClubID");
			uint64_t ClubID;

			FIELD(Team_TA, ClubColors, struct ClubColors, "TAGame.Team_TA:ClubColors");
			struct ClubColors ClubColors;

		};

		CLASS(Team_Soccar_TA, Team_TA);
		struct Team_Soccar_TA : public Team_TA
		{
			FIELD(Team_Soccar_TA, GameScore, uint32_t, "TAGame.Team_Soccar_TA:GameScore");
			uint32_t GameScore;

		};

		CLASS(BreakOutActor_Platform_TA, Engine::Actor);
		struct BreakOutActor_Platform_TA : public Engine::Actor
		{
			FIELD(BreakOutActor_Platform_TA, DamageState, struct DamageState, "TAGame.BreakOutActor_Platform_TA:DamageState");
			struct DamageState DamageState;

		};

		CLASS(SpecialPickup_TA, CarComponent_TA);
		struct SpecialPickup_TA : public CarComponent_TA
		{

		};

		CLASS(SpecialPickup_Targeted_TA, SpecialPickup_TA);
		struct SpecialPickup_Targeted_TA : public SpecialPickup_TA
		{
			FIELD(SpecialPickup_Targeted_TA, Targeted, struct ActiveActor, "TAGame.SpecialPickup_Targeted_TA:Targeted");
			struct ActiveActor Targeted;

		};

		CLASS(SpecialPickup_Tornado_TA, SpecialPickup_TA);
		struct SpecialPickup_Tornado_TA : public SpecialPickup_TA
		{

		};

		CLASS(SpecialPickup_HauntedBallBeam_TA, SpecialPickup_TA);
		struct SpecialPickup_HauntedBallBeam_TA : public SpecialPickup_TA
		{

		};


		CLASS(SpecialPickup_BallVelcro_TA, SpecialPickup_TA);
		struct SpecialPickup_BallVelcro_TA : public SpecialPickup_TA
		{
			FIELD(SpecialPickup_BallVelcro_TA, bHit, bool, "TAGame.SpecialPickup_BallVelcro_TA:bHit");
			bool bHit;

			FIELD(SpecialPickup_BallVelcro_TA, bBroken, bool, "TAGame.SpecialPickup_BallVelcro_TA:bBroken");
			bool bBroken;

			FIELD(SpecialPickup_BallVelcro_TA, AttachTime, float, "TAGame.SpecialPickup_BallVelcro_TA:AttachTime");
			float AttachTime;

			FIELD(SpecialPickup_BallVelcro_TA, BreakTime, float, "TAGame.SpecialPickup_BallVelcro_TA:BreakTime");
			float BreakTime;

		};


		CLASS(SpecialPickup_Rugby_TA, SpecialPickup_TA);
		struct SpecialPickup_Rugby_TA : public SpecialPickup_TA
		{
			FIELD(SpecialPickup_Rugby_TA, bBallWelded, bool, "TAGame.SpecialPickup_Rugby_TA:bBallWelded");
			bool bBallWelded;

		};

		CLASS(SpecialPickup_BallFreeze_TA, SpecialPickup_Targeted_TA);
		struct SpecialPickup_BallFreeze_TA : public SpecialPickup_Targeted_TA
		{
			FIELD(SpecialPickup_BallFreeze_TA, RepOrigSpeed, float, "TAGame.SpecialPickup_BallFreeze_TA:RepOrigSpeed");
			float RepOrigSpeed;

		};

		CLASS(SpecialPickup_Spring_TA, SpecialPickup_Targeted_TA);
		struct SpecialPickup_Spring_TA : public SpecialPickup_Targeted_TA
		{

		};

		CLASS(SpecialPickup_BallCarSpring_TA, SpecialPickup_Spring_TA);
		struct SpecialPickup_BallCarSpring_TA : public SpecialPickup_Spring_TA
		{

		};

		CLASS(SpecialPickup_BallGravity_TA, SpecialPickup_TA);
		struct SpecialPickup_BallGravity_TA : public SpecialPickup_TA
		{

		};

		CLASS(SpecialPickup_GrapplingHook_TA, SpecialPickup_Targeted_TA);
		struct SpecialPickup_GrapplingHook_TA : public SpecialPickup_Targeted_TA
		{

		};

		CLASS(SpecialPickup_BallLasso_TA, SpecialPickup_GrapplingHook_TA);
		struct SpecialPickup_BallLasso_TA : public SpecialPickup_GrapplingHook_TA
		{

		};

		CLASS(SpecialPickup_BoostOverride_TA, SpecialPickup_Targeted_TA);
		struct SpecialPickup_BoostOverride_TA : public SpecialPickup_Targeted_TA
		{

		};

		CLASS(SpecialPickup_Batarang_TA, SpecialPickup_BallLasso_TA);
		struct SpecialPickup_Batarang_TA : public SpecialPickup_BallLasso_TA
		{

		};

		CLASS(SpecialPickup_HitForce_TA, SpecialPickup_TA);
		struct SpecialPickup_HitForce_TA : public SpecialPickup_TA
		{

		};

		CLASS(SpecialPickup_Swapper_TA, SpecialPickup_Targeted_TA);
		struct SpecialPickup_Swapper_TA : public SpecialPickup_Targeted_TA
		{

		};

		CLASS(CrowdManager_TA, Engine::ReplicationInfo);
		struct CrowdManager_TA : public Engine::ReplicationInfo
		{
			FIELD(CrowdManager_TA, GameEvent, struct ActiveActor, "TAGame.CrowdManager_TA:GameEvent");
			struct ActiveActor GameEvent;

			FIELD(CrowdManager_TA, ReplicatedGlobalOneShotSound, struct ObjectTarget, "TAGame.CrowdManager_TA:ReplicatedGlobalOneShotSound");
			struct ObjectTarget ReplicatedGlobalOneShotSound;

		};

		CLASS(CrowdActor_TA, Engine::ReplicationInfo);
		struct CrowdActor_TA : public Engine::ReplicationInfo
		{
			FIELD(CrowdActor_TA, GameEvent, struct ActiveActor, "TAGame.CrowdActor_TA:GameEvent");
			struct ActiveActor GameEvent;

			FIELD(CrowdActor_TA, ReplicatedOneShotSound, struct ObjectTarget, "TAGame.CrowdActor_TA:ReplicatedOneShotSound");
			struct ObjectTarget ReplicatedOneShotSound;

			FIELD(CrowdActor_TA, ReplicatedRoundCountDownNumber, uint32_t, "TAGame.CrowdActor_TA:ReplicatedRoundCountDownNumber");
			uint32_t ReplicatedRoundCountDownNumber;

			FIELD(CrowdActor_TA, ReplicatedCountDownNumber, uint32_t, "TAGame.CrowdActor_TA:ReplicatedCountDownNumber");
			uint32_t ReplicatedCountDownNumber;

			FIELD(CrowdActor_TA, ModifiedNoise, float, "TAGame.CrowdActor_TA:ModifiedNoise");
			float ModifiedNoise;

		};

		CLASS(InMapScoreboard_TA, Engine::Actor)
		struct InMapScoreboard_TA : public Engine::Actor
		{

		};

		CLASS(Vehicle_TA, RBActor_TA);
		struct Vehicle_TA : public RBActor_TA
		{
			FIELD(Vehicle_TA, ReplicatedThrottle, unsigned char, "TAGame.Vehicle_TA:ReplicatedThrottle");
			unsigned char ReplicatedThrottle;

			FIELD(Vehicle_TA, bReplicatedHandbrake, bool, "TAGame.Vehicle_TA:bReplicatedHandbrake");
			bool bReplicatedHandbrake;

			FIELD(Vehicle_TA, bDriving, bool, "TAGame.Vehicle_TA:bDriving");
			bool bDriving;

			FIELD(Vehicle_TA, ReplicatedSteer, unsigned char, "TAGame.Vehicle_TA:ReplicatedSteer");
			unsigned char ReplicatedSteer;

		};

		CLASS(Car_TA, Vehicle_TA);
		struct Car_TA : public Vehicle_TA
		{
			FIELD(Car_TA, AttachedPickup, struct ActiveActor, "TAGame.Car_TA:AttachedPickup");
			struct ActiveActor AttachedPickup;

			FIELD(Car_TA, AddedCarForceMultiplier, float, "TAGame.Car_TA:AddedCarForceMultiplier");
			float AddedCarForceMultiplier;

			FIELD(Car_TA, ReplicatedCarScale, float, "TAGame.Car_TA:ReplicatedCarScale");
			float ReplicatedCarScale;

			FIELD(Car_TA, AddedBallForceMultiplier, float, "TAGame.Car_TA:AddedBallForceMultiplier");
			float AddedBallForceMultiplier;

			FIELD(Car_TA, TeamPaint, struct TeamPaint, "TAGame.Car_TA:TeamPaint");
			struct TeamPaint TeamPaint;

			FIELD(Car_TA, ReplicatedDemolish, struct ReplicatedDemolish, "TAGame.Car_TA:ReplicatedDemolish");
			struct ReplicatedDemolish ReplicatedDemolish;

			FIELD(Car_TA, ClubColors, struct ClubColors, "TAGame.Car_TA:ClubColors");
			struct ClubColors ClubColors;

		};

		CLASS(Car_Season_TA, Car_TA);
		struct Car_Season_TA : public Car_TA
		{

		};

		CLASS(CameraSettingsActor_TA, Engine::ReplicationInfo);
		struct CameraSettingsActor_TA : public Engine::ReplicationInfo
		{
			FIELD(CameraSettingsActor_TA, PRI, struct ActiveActor, "TAGame.CameraSettingsActor_TA:PRI");
			struct ActiveActor PRI;

			FIELD(CameraSettingsActor_TA, CameraPitch, unsigned char, "TAGame.CameraSettingsActor_TA:CameraPitch");
			unsigned char CameraPitch;

			FIELD(CameraSettingsActor_TA, bMouseCameraToggleEnabled, bool, "TAGame.CameraSettingsActor_TA:bMouseCameraToggleEnabled");
			bool bMouseCameraToggleEnabled;

			FIELD(CameraSettingsActor_TA, bUsingSecondaryCamera, bool, "TAGame.CameraSettingsActor_TA:bUsingSecondaryCamera");
			bool bUsingSecondaryCamera;

			FIELD(CameraSettingsActor_TA, bUsingBehindView, bool, "TAGame.CameraSettingsActor_TA:bUsingBehindView");
			bool bUsingBehindView;

			FIELD(CameraSettingsActor_TA, ProfileSettings, struct CameraSettings, "TAGame.CameraSettingsActor_TA:ProfileSettings");
			struct CameraSettings ProfileSettings;

			FIELD(CameraSettingsActor_TA, bUsingSwivel, bool, "TAGame.CameraSettingsActor_TA:bUsingSwivel");
			bool bUsingSwivel;

			FIELD(CameraSettingsActor_TA, CameraYaw, unsigned char, "TAGame.CameraSettingsActor_TA:CameraYaw");
			unsigned char CameraYaw;

		};
		
		CLASS(GRI_TA, ProjectX::GRI_X);
		struct GRI_TA : public ProjectX::GRI_X
		{
			FIELD(GRI_TA, NewDedicatedServerIP, std::string, "TAGame.GRI_TA:NewDedicatedServerIP");
			std::string NewDedicatedServerIP;

		};

		CLASS(Ball_Breakout_TA, Ball_TA);
		struct Ball_Breakout_TA : public Ball_TA
		{
			FIELD(Ball_Breakout_TA, DamageIndex, uint32_t, "TAGame.Ball_Breakout_TA:DamageIndex");
			uint32_t DamageIndex;

			FIELD(Ball_Breakout_TA, AppliedDamage, struct AppliedDamage, "TAGame.Ball_Breakout_TA:AppliedDamage");
			struct AppliedDamage AppliedDamage;

			FIELD(Ball_Breakout_TA, LastTeamTouch, unsigned char, "TAGame.Ball_Breakout_TA:LastTeamTouch");
			unsigned char LastTeamTouch;

		};



		CLASS(VehiclePickup_TA, Engine::ReplicationInfo);
		struct VehiclePickup_TA : public Engine::ReplicationInfo
		{
			FIELD(VehiclePickup_TA, bNoPickup, bool, "TAGame.VehiclePickup_TA:bNoPickup");
			bool bNoPickup;

			FIELD(VehiclePickup_TA, ReplicatedPickupData, struct ReplicatedPickupData, "TAGame.VehiclePickup_TA:ReplicatedPickupData");
			struct ReplicatedPickupData ReplicatedPickupData;

		};

		CLASS(VehiclePickup_Boost_TA, VehiclePickup_TA);
		struct VehiclePickup_Boost_TA : public VehiclePickup_TA
		{

		};

		CLASS(Ball_Haunted_TA, Ball_TA);
		struct Ball_Haunted_TA : public Ball_TA
		{
			FIELD(Ball_Haunted_TA, DeactivatedGoalIndex, unsigned char, "TAGame.Ball_Haunted_TA:DeactivatedGoalIndex");
			unsigned char DeactivatedGoalIndex;

			FIELD(Ball_Haunted_TA, bIsBallBeamed, bool, "TAGame.Ball_Haunted_TA:bIsBallBeamed");
			bool bIsBallBeamed;

			FIELD(Ball_Haunted_TA, ReplicatedBeamBrokenValue, unsigned char, "TAGame.Ball_Haunted_TA:ReplicatedBeamBrokenValue");
			unsigned char ReplicatedBeamBrokenValue;

			FIELD(Ball_Haunted_TA, LastTeamTouch, unsigned char, "TAGame.Ball_Haunted_TA:LastTeamTouch");
			unsigned char LastTeamTouch;

			FIELD(Ball_Haunted_TA, TotalActiveBeams, unsigned char, "TAGame.Ball_Haunted_TA:TotalActiveBeams");
			unsigned char TotalActiveBeams;

		};


		CLASS(GameEvent_TA, Engine::ReplicationInfo);
		struct GameEvent_TA : public Engine::ReplicationInfo
		{
			FIELD(GameEvent_TA, ReplicatedRoundCountDownNumber, uint32_t, "TAGame.GameEvent_TA:ReplicatedRoundCountDownNumber");
			uint32_t ReplicatedRoundCountDownNumber;

			FIELD(GameEvent_TA, ActivatorCar, struct ActiveActor, "TAGame.GameEvent_TA:ActivatorCar");
			struct ActiveActor ActivatorCar;

			FIELD(GameEvent_TA, ReplicatedGameStateTimeRemaining, uint32_t, "TAGame.GameEvent_TA:ReplicatedGameStateTimeRemaining");
			uint32_t ReplicatedGameStateTimeRemaining;

			FIELD(GameEvent_TA, ReplicatedStateName, uint32_t, "TAGame.GameEvent_TA:ReplicatedStateName");
			uint32_t ReplicatedStateName;

			FIELD(GameEvent_TA, MatchTypeClass, struct ObjectTarget, "TAGame.GameEvent_TA:MatchTypeClass");
			struct ObjectTarget MatchTypeClass;

			FIELD(GameEvent_TA, BotSkill, uint32_t, "TAGame.GameEvent_TA:BotSkill");
			uint32_t BotSkill;

			FIELD(GameEvent_TA, bHasLeaveMatchPenalty, bool, "TAGame.GameEvent_TA:bHasLeaveMatchPenalty");
			bool bHasLeaveMatchPenalty;

			FIELD(GameEvent_TA, bCanVoteToForfeit, bool, "TAGame.GameEvent_TA:bCanVoteToForfeit");
			bool bCanVoteToForfeit;

			FIELD(GameEvent_TA, bAllowReadyUp, bool, "TAGame.GameEvent_TA:bAllowReadyUp");
			bool bAllowReadyUp;

			FIELD(GameEvent_TA, GameMode, struct GameMode, "TAGame.GameEvent_TA:GameMode");
			struct GameMode GameMode;

			FIELD(GameEvent_TA, ReplicatedStateIndex, struct ReplicatedStateIndex, "TAGame.GameEvent_TA:ReplicatedStateIndex");
			struct ReplicatedStateIndex ReplicatedStateIndex;

		};

		CLASS(GameEvent_Team_TA, GameEvent_TA);
		struct GameEvent_Team_TA : public GameEvent_TA
		{
			FIELD(GameEvent_Team_TA, MaxTeamSize, uint32_t, "TAGame.GameEvent_Team_TA:MaxTeamSize");
			uint32_t MaxTeamSize;

			FIELD(GameEvent_Team_TA, bForfeit, bool, "TAGame.GameEvent_Team_TA:bForfeit");
			bool bForfeit;

			FIELD(GameEvent_Team_TA, bDisableMutingOtherTeam, bool, "TAGame.GameEvent_Team_TA:bDisableMutingOtherTeam");
			bool bDisableMutingOtherTeam;

		};



		CLASS(GameEvent_Soccar_TA, GameEvent_Team_TA);
		struct GameEvent_Soccar_TA : public GameEvent_Team_TA
		{
			FIELD(GameEvent_Soccar_TA, bOverTime, bool, "TAGame.GameEvent_Soccar_TA:bOverTime");
			bool bOverTime;

			FIELD(GameEvent_Soccar_TA, GameTime, uint32_t, "TAGame.GameEvent_Soccar_TA:GameTime");
			uint32_t GameTime;

			FIELD(GameEvent_Soccar_TA, MVP, struct ActiveActor, "TAGame.GameEvent_Soccar_TA:MVP");
			struct ActiveActor MVP;

			FIELD(GameEvent_Soccar_TA, MatchWinner, struct ActiveActor, "TAGame.GameEvent_Soccar_TA:MatchWinner");
			struct ActiveActor MatchWinner;

			FIELD(GameEvent_Soccar_TA, GameWinner, struct ActiveActor, "TAGame.GameEvent_Soccar_TA:GameWinner");
			struct ActiveActor GameWinner;

			FIELD(GameEvent_Soccar_TA, SubRulesArchetype, struct ObjectTarget, "TAGame.GameEvent_Soccar_TA:SubRulesArchetype");
			struct ObjectTarget SubRulesArchetype;

			FIELD(GameEvent_Soccar_TA, SecondsRemaining, uint32_t, "TAGame.GameEvent_Soccar_TA:SecondsRemaining");
			uint32_t SecondsRemaining;

			FIELD(GameEvent_Soccar_TA, RoundNum, uint32_t, "TAGame.GameEvent_Soccar_TA:RoundNum");
			uint32_t RoundNum;

			FIELD(GameEvent_Soccar_TA, SeriesLength, uint32_t, "TAGame.GameEvent_Soccar_TA:SeriesLength");
			uint32_t SeriesLength;

			FIELD(GameEvent_Soccar_TA, ReplicatedMusicStinger, struct ReplicatedMusicStringer, "TAGame.GameEvent_Soccar_TA:ReplicatedMusicStinger");
			struct ReplicatedMusicStringer ReplicatedMusicStinger;

			FIELD(GameEvent_Soccar_TA, bBallHasBeenHit, bool, "TAGame.GameEvent_Soccar_TA:bBallHasBeenHit");
			bool bBallHasBeenHit;

			FIELD(GameEvent_Soccar_TA, bUnlimitedTime, bool, "TAGame.GameEvent_Soccar_TA:bUnlimitedTime");
			bool bUnlimitedTime;

			FIELD(GameEvent_Soccar_TA, ReplicatedStatEvent, struct ReplicatedStatEvent, "TAGame.GameEvent_Soccar_TA:ReplicatedStatEvent");
			struct ReplicatedStatEvent ReplicatedStatEvent;

			FIELD(GameEvent_Soccar_TA, bShowIntroScene, bool, "TAGame.GameEvent_Soccar_TA:bShowIntroScene");
			bool bShowIntroScene;

			FIELD(GameEvent_Soccar_TA, bClubMatch, bool, "TAGame.GameEvent_Soccar_TA:bClubMatch");
			bool bClubMatch;

			FIELD(GameEvent_Soccar_TA, ReplicatedScoredOnTeam, unsigned char, "TAGame.GameEvent_Soccar_TA:ReplicatedScoredOnTeam");
			unsigned char ReplicatedScoredOnTeam;

			FIELD(GameEvent_Soccar_TA, bMatchEnded, bool, "TAGame.GameEvent_Soccar_TA:bMatchEnded");
			bool bMatchEnded;

			FIELD(GameEvent_Soccar_TA, ReplicatedServerPerformanceState, unsigned char, "TAGame.GameEvent_Soccar_TA:ReplicatedServerPerformanceState");
			unsigned char ReplicatedServerPerformanceState;

		};

		CLASS(GameEvent_Breakout_TA, GameEvent_Soccar_TA);
		struct GameEvent_Breakout_TA : public GameEvent_Soccar_TA
		{

		};

		CLASS(GameEvent_Season_TA, GameEvent_Soccar_TA);
		struct GameEvent_Season_TA : public GameEvent_Soccar_TA
		{

		};

		CLASS(GameEvent_SoccarPrivate_TA, GameEvent_Soccar_TA);
		struct GameEvent_SoccarPrivate_TA : public GameEvent_Soccar_TA
		{
			FIELD(GameEvent_SoccarPrivate_TA, MatchSettings, struct PrivateMatchSettings, "TAGame.GameEvent_SoccarPrivate_TA:MatchSettings");
			struct PrivateMatchSettings MatchSettings;

		};

		CLASS(GameEvent_SoccarSplitscreen_TA, GameEvent_SoccarPrivate_TA);
		struct GameEvent_SoccarSplitscreen_TA : public GameEvent_SoccarPrivate_TA
		{

		};

		CLASS(GameEvent_Tutorial_TA, GameEvent_Soccar_TA);
		struct GameEvent_Tutorial_TA : public GameEvent_Soccar_TA
		{

		};

		CLASS(HauntedBallTrapTrigger_TA, Engine::Actor);
		struct HauntedBallTrapTrigger_TA : public Engine::Actor
		{

		};
	};


}