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

		struct Actor : public Object
		{
			float DrawScale;
			bool bCollideActors;
			bool bCollideWorld;
			bool bNetOwner;
			struct ActorBase Base;
			bool bBlockActors;
			bool bHidden;
			bool bTearOff;
			struct Vector3I Location;
			struct Rotator Rotation;

			//Fields that aren't used (yet) but could probably be replicated
			uint8_t Physics;
			uint8_t RemoteRole;
			uint8_t Role;
			uint8_t ReplicatedCollisionType;
			ActiveActor Owner;
			bool bHardAttach;
			ActiveActor Instigator;
			Vector3I RelativeLocation;
			Rotator RelativeRotation;
			bool bRootMotionFromInterpCurve;
		};

		struct ReplicatedActor_ORS : public Actor
		{
			ActiveActor ReplicatedOwner;
		};

		struct Info : public Actor
		{

		};

		struct ReplicationInfo : public Info
		{

		};

		struct GameReplicationInfo : public ReplicationInfo
		{
			std::string ServerName;
			struct ObjectTarget GameClass;
			bool bStopCountDown;
			bool bMatchIsOver;
			bool bMatchHasBegun;

			//Unused?
			int RemainingTime;
			int ElapsedTime;
			int RemainingMinute;
			int GoalScore;
			int TimeLimit;
			ActiveActor winner;
		};

		struct Pawn : public Actor
		{
			struct ActiveActor PlayerReplicationInfo;
			uint32_t HealthMax;
			bool bIsCrouched;

			//Controller Controller;
			bool bIsWalking;
			bool bSimulateGravity;
			bool bCanSwatTurn;
			bool bUsedByMatinee;
			bool bRootMotionFromInterpCurve;
			bool bFastAttachedMove;
			uint8_t RemoteViewPitch;

			float GroundSpeed;
			float AirSpeed;
			float AccelRate;
			float JumpZ;
			float AirControl;
			float RootMotionInterpRate;
			float RootMotionInterpCurrentTime;
			Vector3I RootMotionInterpCurveLastValue;
		};


		struct PlayerReplicationInfo : public ReplicationInfo
		{
			struct ActiveActor Team;
			bool bReadyToPlay;
			std::string PlayerName;
			std::string RemoteUserData;
			bool bWaitingPlayer;
			uint32_t Score;
			uint32_t PlayerID;
			bool bBot;
			bool bIsSpectator;
			bool bTimedOut;
			bool bAdmin;
			bool bIsInactive;
			bool bOnlySpectator;
			uint8_t Ping;
			OnlineID UniqueId;

			//Unused
			uint32_t Deaths;
			uint8_t TTSSpeaker;
			bool bOutOfLives;
			bool bFromPreviousLevel;
			//PRIRemoteUserData RemoteUserData

		};

		struct TeamInfo : public ReplicationInfo
		{
			uint32_t Score;

			//Unused
			//std::string TeamName;
			//int TeamIndex;
		};

		struct WorldInfo : public Info
		{
			float WorldGravityZ;
			float TimeDilation;
			bool bHighPriorityLoading;
			ActiveActor Pauser; //Is of type PlayerReplicationInfo

			//MusicTrackStruct ReplicatedMusicTrack; 
		};


		struct DynamicSMActor : public Actor
		{

			//class UStaticMesh* ReplicatedMesh;                                   		// 0x01F0 (0x0004) [0x0000000000002020]              ( CPF_Net | CPF_Transient )
			//class UMaterialInterface* ReplicatedMaterial0;                              		// 0x01F4 (0x0004) [0x0000000000000020]              ( CPF_Net )
			//class UMaterialInterface* ReplicatedMaterial1;                              		// 0x01F8 (0x0004) [0x0000000000000020]              ( CPF_Net )
			//class UMaterialInterface* ReplicatedMaterial2;                              		// 0x01FC (0x0004) [0x0000000000000020]              ( CPF_Net )
			//class UMaterialInterface* ReplicatedMaterial3;                              		// 0x0200 (0x0004) [0x0000000000000020]              ( CPF_Net )

			ObjectTarget ReplicatedMesh;                                   		// 0x01F0 (0x0004) [0x0000000000002020]              ( CPF_Net | CPF_Transient )
			ObjectTarget ReplicatedMaterial0;                              		// 0x01F4 (0x0004) [0x0000000000000020]              ( CPF_Net )
			ObjectTarget ReplicatedMaterial1;                              		// 0x01F8 (0x0004) [0x0000000000000020]              ( CPF_Net )
			ObjectTarget ReplicatedMaterial2;                              		// 0x01FC (0x0004) [0x0000000000000020]              ( CPF_Net )
			ObjectTarget ReplicatedMaterial3;                              		// 0x0200 (0x0004) [0x0000000000000020]              ( CPF_Net )
			bool bForceStaticDecals;                           		// 0x0204 (0x0004) [0x0000000000000020] [0x00000001] ( CPF_Net )
			Vector3 ReplicatedMeshTranslation;                        		// 0x0208 (0x000C) [0x0000000000000020]              ( CPF_Net )
			Rotator ReplicatedMeshRotation;                           		// 0x0214 (0x000C) [0x0000000000000020]              ( CPF_Net )
			Vector3 ReplicatedMeshScale3D;                            		// 0x0220 (0x000C) [0x0000000000000020]              ( CPF_Net )
		};

		//Currently only used in workshop map replays
		struct KActor : public DynamicSMActor
		{
			bool bWakeOnLevelStart;
			RigidBodyState RBState;
			Vector3 ReplicatedDrawScale3D;
		};
	};

	namespace ProjectX
	{
		struct GRI_X : public Engine::GameReplicationInfo
		{
			std::string MatchGUID;
			int ReplicatedGameMutatorIndex;
			bool bGameStarted;
			uint32_t ReplicatedGamePlaylist;
			uint64_t GameServerID;
			struct Reservation Reservations; //is actually array of size 0x8, first 3 bits read is index of array i guess


		};

		struct NetModeReplicator_X : public Engine::ReplicationInfo
		{

		};

		struct Pawn_X : public Engine::Pawn
		{
			
		};

		struct PRI_X : public Engine::PlayerReplicationInfo
		{

		};
	};

	namespace TAGame
	{
		struct PRI_TA : public ProjectX::PRI_X
		{
			uint32_t MatchShots;
			struct ActiveActor PersistentCamera;
			struct SkillTier SkillTier;
			bool bUsingBehindView;
			uint32_t MatchAssists;
			int RespawnTimeRemaining;
			bool bOnlineLoadoutSet;
			uint32_t MatchGoals;
			struct ActiveActor ReplicatedGameEvent;
			uint32_t TotalXP;
			uint32_t MatchScore;
			uint32_t MatchSaves;
			uint32_t Title;
			uint64_t ClubID;
			int MaxTimeTillItem;
			ActiveActor PickupTimer;
			uint32_t MatchBreakoutDamage;
			uint32_t BotProductName;
			uint32_t BotBannerProductID;
			bool bReady;
			uint32_t SpectatorShortcut;
			bool bUsingSecondaryCamera;
			bool PlayerHistoryValid;
			bool bIsInSplitScreen;
			bool bMatchMVP;
			struct RepStatTitle RepStatTitles;
			bool bOnlineLoadoutsSet;
			bool bUsingItems;
			struct ReplicatedTitle PrimaryTitle;
			bool bMatchAdmin;
			bool bBusy;
			bool bVoteToForfeitDisabled;
			bool bUsingFreecam;
			struct OnlineLoadout ClientLoadoutOnline;
			uint8_t CameraYaw;
			uint8_t CameraPitch;
			uint8_t PawnType;
			uint8_t ReplicatedWorstNetQualityBeyondLatency;
			float SteeringSensitivity;
			struct PartyLeader PartyLeader;
			int TimeTillItem;
			struct ClientLoadoutsOnline ClientLoadoutsOnline;
			struct ClientLoadouts ClientLoadouts;
			struct ClientLoadout ClientLoadout;
			struct CameraSettings CameraSettings;
			struct ReplicatedTitle SecondaryTitle;
			struct HistoryKey PlayerHistoryKey;
			

			bool bIsDistracted;

			//Unused
			ActiveActor ReplacingBotPRI;
			//PlayerReplicatedEventInfo_TA               PREI
		};


		struct RBActor_TA : public ProjectX::Pawn_X
		{
			struct ReplicatedRBState ReplicatedRBState;
			bool bReplayActor;
			bool bFrozen;
			struct WeldedInfo WeldedInfo;
			bool bIgnoreSyncing;

			float MaxLinearSpeed;
			float MaxAngularSpeed;
			uint8_t TeleportCounter;
		};

		struct CarComponent_TA : public Engine::ReplicationInfo
		{
			struct ActiveActor Vehicle;
			uint8_t ReplicatedActive;
			float ReplicatedActivityTime;
		};


		struct CarComponent_AirActivate_TA : public CarComponent_TA
		{
			int AirActivateCount;
		};

		struct CarComponent_Jump_TA : public CarComponent_TA
		{

		};

		struct CarComponent_DoubleJump_TA : public CarComponent_AirActivate_TA
		{
			struct Vector3I DoubleJumpImpulse;
		};


		struct CarComponent_Boost_TA : public CarComponent_AirActivate_TA
		{
			float RechargeDelay;
			bool bUnlimitedBoost;
			uint32_t UnlimitedBoostRefCount;
			bool bNoBoost;
			uint8_t ReplicatedBoostAmount;
			float RechargeRate;
			float BoostModifier;

			//unused
			float StartBoostAmount;
			float CurrentBoostAmount;
			bool bRechargeGroundOnly;

		};

		struct CarComponent_Dodge_TA : public CarComponent_AirActivate_TA
		{
			struct Vector3I DodgeTorque;
			struct Vector3I DodgeImpulse;
		};


		struct CarComponent_FlipCar_TA : public CarComponent_TA
		{
			bool bFlipRight;
			float FlipCarTime;
		};

		struct Ball_TA : public RBActor_TA
		{
			struct ActiveActor GameEvent;
			struct ObjectTarget ReplicatedPhysMatOverride;
			float ReplicatedBallGravityScale;
			float ReplicatedBallScale;
			unsigned char HitTeamNum;
			float ReplicatedWorldBounceScale;
			float ReplicatedAddedCarBounceScale;
			struct ReplicatedExplosionData ReplicatedExplosionData;
			float ReplicatedBallMaxLinearSpeedScale;
			struct ReplicatedExplosionDataExtended ReplicatedExplosionDataExtended;
			Vector3I MagnusCoefficient;

			//Unused
			bool bEndOfGameHidden;
			//ObjectTarget ReplicatedBallMesh; //ObjectTarget or active actor, idk
		};

		struct Team_TA : public Engine::TeamInfo
		{
			struct LogoData LogoData;
			struct ActiveActor GameEvent;
			std::string CustomTeamName;
			uint64_t ClubID;
			int32_t Difficulty;
			struct ClubColors ClubColors;
		};

		struct Team_Soccar_TA : public Team_TA
		{
			uint32_t GameScore;
		};

		struct BreakOutActor_Platform_TA : public Engine::Actor
		{
			struct DamageState DamageState;
		};

		struct SpecialPickup_TA : public CarComponent_TA
		{

		};

		struct SpecialPickup_Targeted_TA : public SpecialPickup_TA
		{
			struct ActiveActor Targeted;
		};

		struct SpecialPickup_Tornado_TA : public SpecialPickup_TA
		{

		};

		struct SpecialPickup_HauntedBallBeam_TA : public SpecialPickup_TA
		{

		};


		struct SpecialPickup_BallVelcro_TA : public SpecialPickup_TA
		{
			bool bHit;
			bool bBroken;
			float AttachTime;
			float BreakTime;
		};


		struct SpecialPickup_Rugby_TA : public SpecialPickup_TA
		{
			bool bBallWelded;
		};

		struct SpecialPickup_BallFreeze_TA : public SpecialPickup_Targeted_TA
		{
			float RepOrigSpeed;
		};

		struct SpecialPickup_Spring_TA : public SpecialPickup_Targeted_TA
		{

		};

		struct SpecialPickup_BallCarSpring_TA : public SpecialPickup_Spring_TA
		{

		};

		struct SpecialPickup_BallGravity_TA : public SpecialPickup_TA
		{

		};

		struct SpecialPickup_GrapplingHook_TA : public SpecialPickup_Targeted_TA
		{

		};

		struct SpecialPickup_BallLasso_TA : public SpecialPickup_GrapplingHook_TA
		{

		};

		struct SpecialPickup_BoostOverride_TA : public SpecialPickup_Targeted_TA
		{

		};

		struct SpecialPickup_Batarang_TA : public SpecialPickup_BallLasso_TA
		{

		};

		struct SpecialPickup_HitForce_TA : public SpecialPickup_TA
		{

		};

		struct SpecialPickup_Swapper_TA : public SpecialPickup_Targeted_TA
		{

		};

		struct SpecialPickup_Football_TA : public SpecialPickup_TA
		{
			struct ActiveActor WeldedBall;
		};

		struct CrowdManager_TA : public Engine::ReplicationInfo
		{
			struct ActiveActor GameEvent;
			struct ObjectTarget ReplicatedGlobalOneShotSound;
		};

		struct CrowdActor_TA : public Engine::ReplicationInfo
		{
			struct ActiveActor GameEvent;
			struct ObjectTarget ReplicatedOneShotSound;
			uint32_t ReplicatedRoundCountDownNumber;
			uint32_t ReplicatedCountDownNumber;
			float ModifiedNoise;
		};

		struct InMapScoreboard_TA : public Engine::Actor
		{

		};

		struct Vehicle_TA : public RBActor_TA
		{
			unsigned char ReplicatedThrottle;
			bool bReplicatedHandbrake;
			bool bDriving;
			unsigned char ReplicatedSteer;

			//Unused
			bool bPodiumMode;
		};

		struct Car_TA : public Vehicle_TA
		{
			struct ActiveActor AttachedPickup;
			struct ActiveActor RumblePickups;
			float AddedCarForceMultiplier;
			float ReplicatedCarScale;
			float AddedBallForceMultiplier;
			struct TeamPaint TeamPaint;
			struct ReplicatedDemolish ReplicatedDemolish;
			struct ReplicatedDemolish2 ReplicatedDemolish_CustomFX;
			struct DemolishDataGoalExplosion ReplicatedDemolishGoalExplosion;
			struct ClubColors ClubColors;
		};

		struct Car_Season_TA : public Car_TA
		{

		};

		struct CameraSettingsActor_TA : public Engine::ReplicationInfo
		{
			struct ActiveActor PRI;
			
			bool bMouseCameraToggleEnabled;
			bool bUsingSecondaryCamera;
			bool bUsingBehindView;
			struct CameraSettings ProfileSettings;
			bool bUsingSwivel;
			bool bUsingFreecam;
			bool bHoldMouseCamera;
			bool bResetCamera;
			uint8_t CameraPitch;
			uint8_t CameraYaw;
		};
		
		struct GRI_TA : public ProjectX::GRI_X
		{
			std::string NewDedicatedServerIP;
		};

		struct Ball_Breakout_TA : public Ball_TA
		{
			uint32_t DamageIndex;
			struct AppliedDamage AppliedDamage;
			unsigned char LastTeamTouch;
		};

		struct Ball_God_TA : public Ball_TA
		{
			float TargetSpeed;
		};

		struct VehiclePickup_TA : public Engine::ReplicationInfo
		{
			bool bNoPickup;
			struct ReplicatedPickupData ReplicatedPickupData;
			struct ReplicatedPickupData2 NewReplicatedPickupData;
		};

		struct VehiclePickup_Boost_TA : public VehiclePickup_TA
		{

		};

		struct Ball_Haunted_TA : public Ball_TA
		{
			unsigned char DeactivatedGoalIndex;
			bool bIsBallBeamed;
			unsigned char ReplicatedBeamBrokenValue;
			unsigned char LastTeamTouch;
			unsigned char TotalActiveBeams;
		};


		struct GameEvent_TA : public Engine::ReplicationInfo
		{
			uint32_t ReplicatedRoundCountDownNumber;
			struct ActiveActor ActivatorCar;
			uint32_t ReplicatedGameStateTimeRemaining;
			uint32_t ReplicatedStateName;
			struct ObjectTarget MatchTypeClass;
			float BotSkill; 
			bool bHasLeaveMatchPenalty;
			bool bCanVoteToForfeit;
			bool bAllowReadyUp;
			struct GameMode GameMode;
			struct ReplicatedStateIndex ReplicatedStateIndex; //Might actually just be an uint8_t??
			struct ActiveActor GameOwner; //PRI_TA
			bool bIsBotMatch;
			//struct CustomMatchSettings MatchSettings;
		};

		struct GameEvent_Team_TA : public GameEvent_TA
		{
			uint32_t MaxTeamSize;
			bool bForfeit;
			bool bDisableMutingOtherTeam;
		};



		struct GameEvent_Soccar_TA : public GameEvent_Team_TA
		{
			bool bOverTime;
			uint32_t GameTime;
			struct ActiveActor MVP;
			struct ActiveActor MatchWinner;
			struct ActiveActor GameWinner;
			struct ObjectTarget SubRulesArchetype;
			uint32_t SecondsRemaining;
			uint32_t RoundNum;
			uint32_t SeriesLength;
			struct ReplicatedMusicStringer ReplicatedMusicStinger;
			bool bBallHasBeenHit;
			bool bUnlimitedTime;
			struct ReplicatedStatEvent ReplicatedStatEvent;
			bool bShowIntroScene;
			bool bClubMatch;
			unsigned char ReplicatedScoredOnTeam;
			bool bMatchEnded;
			unsigned char ReplicatedServerPerformanceState;
			uint32_t MaxScore;
		};

		struct GameEvent_Breakout_TA : public GameEvent_Soccar_TA
		{

		};

		struct GameEvent_Football_TA : public GameEvent_Soccar_TA
		{

		};

		struct GameEvent_GodBall_TA : public GameEvent_Soccar_TA
		{

		};

		struct GameEvent_Season_TA : public GameEvent_Soccar_TA
		{

		};

		struct GameEvent_SoccarPrivate_TA : public GameEvent_Soccar_TA
		{
			struct PrivateMatchSettings MatchSettings;
		};

		struct GameEvent_SoccarSplitscreen_TA : public GameEvent_SoccarPrivate_TA
		{

		};

		struct GameEvent_Tutorial_TA : public GameEvent_Soccar_TA
		{

		};
		struct GameEvent_Tutorial_Goalie_TA : public GameEvent_Tutorial_TA
		{

		};

		struct GameEvent_Tutorial_Striker_TA : public GameEvent_Tutorial_TA
		{

		};

		struct GameEvent_GameEditor_TA : public GameEvent_Soccar_TA
		{

		};

		struct GameEditor_Pawn_TA : public ProjectX::Pawn_X
		{

		};

		struct GameEvent_TrainingEditor_TA : public GameEvent_GameEditor_TA
		{

		};

		struct HauntedBallTrapTrigger_TA : public Engine::Actor
		{

		};


		struct MaxTimeWarningData_TA : public Engine::ReplicatedActor_ORS
		{
			uint64_t EndGameEpochTime;
			uint64_t EndGameWarningEpochTime;
		};

		struct RumblePickups_TA : public Engine::Actor
		{
			ActiveActor AttachedPickup;
			int ConcurrentItemCount;
			PickupInfo_TA PickupInfo;
		};

		struct PickupTimer_TA : public CarComponent_TA
		{
			int TimeTillItem;
			int MaxTimeTillItem;
		};

		struct Cannon_TA : public Engine::Actor
		{
			float Pitch;
			uint8_t FireCount;
		};

	};


}