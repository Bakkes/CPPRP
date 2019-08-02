#include "NetworkStreamParser.h"
#include "NetworkData.h"
#include "generated.h"


NetworkStreamParser::NetworkStreamParser()
{
}


NetworkStreamParser::~NetworkStreamParser()
{
}

void NetworkStreamParser::RegisterParsers()
{
	RegisterParsers<ReplicatedRBState>("TAGame.RBActor_TA:ReplicatedRBState");

	RegisterParsers<LogoData>("TAGame.Team_TA:LogoData");

	RegisterParsers<ActiveActor>({ "TAGame.CrowdManager_TA:GameEvent",
		"TAGame.CrowdActor_TA:GameEvent", "TAGame.PRI_TA:PersistentCamera",
		"TAGame.Team_TA:GameEvent", "TAGame.Ball_TA:GameEvent",
		"Engine.Pawn:PlayerReplicationInfo", "TAGame.PRI_TA:ReplicatedGameEvent",
		"TAGame.CarComponent_TA:Vehicle", "TAGame.Car_TA:AttachedPickup",
		"TAGame.SpecialPickup_Targeted_TA:Targeted", "TAGame.CameraSettingsActor_TA:PRI",
		"TAGame.GameEvent_Soccar_TA:MVP", "TAGame.GameEvent_Soccar_TA:MatchWinner",
		"Engine.PlayerReplicationInfo:Team", "TAGame.GameEvent_Soccar_TA:GameWinner" });

	RegisterParsers<ObjectTarget>({"TAGame.CrowdManager_TA:ReplicatedGlobalOneShotSound", 
		"TAGame.CrowdActor_TA:ReplicatedOneShotSound", "TAGame.GameEvent_TA:MatchTypeClass", 
		"Engine.GameReplicationInfo:GameClass", "TAGame.GameEvent_Soccar_TA:SubRulesArchetype", 
		"TAGame.Ball_TA:ReplicatedPhysMatOverride" });

	RegisterParsers<std::string>({"Engine.GameReplicationInfo:ServerName",
		"Engine.PlayerReplicationInfo:PlayerName", "TAGame.Team_TA:CustomTeamName",
		"Engine.PlayerReplicationInfo:RemoteUserData", "TAGame.GRI_TA:NewDedicatedServerIP",
		"ProjectX.GRI_X:MatchGUID" });

	RegisterParsers<uint32_t>({ "TAGame.GameEvent_Soccar_TA:SecondsRemaining", 
		"TAGame.GameEvent_TA:ReplicatedGameStateTimeRemaining", "TAGame.CrowdActor_TA:ReplicatedCountDownNumber", 
		"TAGame.GameEvent_Team_TA:MaxTeamSize", "Engine.PlayerReplicationInfo:PlayerID", 
		"TAGame.PRI_TA:TotalXP", "TAGame.PRI_TA:MatchScore", 
		"TAGame.GameEvent_Soccar_TA:RoundNum", "TAGame.GameEvent_TA:BotSkill", 
		"TAGame.PRI_TA:MatchShots", "TAGame.PRI_TA:MatchSaves", 
		"ProjectX.GRI_X:ReplicatedGamePlaylist", "Engine.TeamInfo:Score", 
		"Engine.PlayerReplicationInfo:Score", "TAGame.PRI_TA:MatchGoals", 
		"TAGame.PRI_TA:MatchAssists", "TAGame.PRI_TA:Title", 
		"TAGame.GameEvent_TA:ReplicatedStateName", "TAGame.Team_Soccar_TA:GameScore", 
		"TAGame.GameEvent_Soccar_TA:GameTime", "TAGame.CarComponent_Boost_TA:UnlimitedBoostRefCount", 
		"TAGame.CrowdActor_TA:ReplicatedRoundCountDownNumber", "TAGame.PRI_TA:MaxTimeTillItem", 
		"TAGame.Ball_Breakout_TA:DamageIndex", "TAGame.PRI_TA:MatchBreakoutDamage", 
		"TAGame.PRI_TA:BotProductName", "TAGame.GameEvent_TA:ReplicatedRoundCountDownNumber", 
		"TAGame.GameEvent_Soccar_TA:SeriesLength", 
		"TAGame.PRI_TA:SpectatorShortcut", "Engine.Pawn:HealthMax" });

	RegisterParsers<bool>({ "Engine.Actor:bCollideWorld", 
		"Engine.Pawn:bIsCrouched", "Engine.PlayerReplicationInfo:bReadyToPlay", 
		"TAGame.Vehicle_TA:bReplicatedHandbrake", "TAGame.Vehicle_TA:bDriving", 
		"Engine.Actor:bNetOwner", "Engine.Actor:bBlockActors", 
		"TAGame.GameEvent_TA:bHasLeaveMatchPenalty", "TAGame.PRI_TA:bUsingBehindView", 
		"TAGame.PRI_TA:bUsingSecondaryCamera", "TAGame.GameEvent_TA:ActivatorCar",
		"TAGame.GameEvent_Soccar_TA:bOverTime", "ProjectX.GRI_X:bGameStarted", 
		"Engine.Actor:bCollideActors", "TAGame.PRI_TA:bReady", 
		"TAGame.RBActor_TA:bFrozen", "Engine.Actor:bHidden", 
		"TAGame.CarComponent_FlipCar_TA:bFlipRight", "Engine.PlayerReplicationInfo:bBot", 
		"Engine.PlayerReplicationInfo:bWaitingPlayer", "TAGame.RBActor_TA:bReplayActor", 
		"TAGame.PRI_TA:bIsInSplitScreen", "Engine.GameReplicationInfo:bMatchIsOver", 
		"TAGame.CarComponent_Boost_TA:bUnlimitedBoost", "Engine.PlayerReplicationInfo:bIsSpectator", 
		"TAGame.GameEvent_Soccar_TA:bBallHasBeenHit", "TAGame.CameraSettingsActor_TA:bUsingSecondaryCamera", 
		"TAGame.CameraSettingsActor_TA:bUsingBehindView", "TAGame.PRI_TA:bOnlineLoadoutSet",
		"TAGame.PRI_TA:bMatchMVP", "TAGame.PRI_TA:bOnlineLoadoutsSet", 
		"TAGame.RBActor_TA:bIgnoreSyncing", "TAGame.SpecialPickup_BallVelcro_TA:bHit", 
		"TAGame.GameEvent_TA:bCanVoteToForfeit", "TAGame.SpecialPickup_BallVelcro_TA:bBroken", 
		"TAGame.GameEvent_Team_TA:bForfeit", "TAGame.PRI_TA:bUsingItems",
		"TAGame.VehiclePickup_TA:bNoPickup", "TAGame.CarComponent_Boost_TA:bNoBoost", 
		"TAGame.PRI_TA:PlayerHistoryValid", "TAGame.GameEvent_Soccar_TA:bUnlimitedTime", 
		"TAGame.GameEvent_Soccar_TA:bClubMatch", "TAGame.GameEvent_Soccar_TA:bMatchEnded", 
		"TAGame.GameEvent_TA:bAllowReadyUp", "Engine.Actor:bTearOff", 
		"Engine.PlayerReplicationInfo:bTimedOut" });

	RegisterParsers<uint64_t>({
		"ProjectX.GRI_X:GameServerID", "TAGame.Team_TA:ClubID", "TAGame.PRI_TA:ClubID"
		});

	RegisterParsers<uint8_t>({
		"Engine.PlayerReplicationInfo:Ping", "TAGame.Vehicle_TA:ReplicatedSteer", 
		"TAGame.Vehicle_TA:ReplicatedThrottle", "TAGame.PRI_TA:CameraYaw", 
		"TAGame.PRI_TA:CameraPitch", "TAGame.Ball_TA:HitTeamNum", 
		"TAGame.GameEvent_Soccar_TA:ReplicatedScoredOnTeam", "TAGame.CarComponent_Boost_TA:ReplicatedBoostAmount", 
		"TAGame.CameraSettingsActor_TA:CameraPitch", "TAGame.CameraSettingsActor_TA:CameraYaw", 
		"TAGame.PRI_TA:PawnType", "TAGame.Ball_Breakout_TA:LastTeamTouch", 
		"TAGame.PRI_TA:ReplicatedWorstNetQualityBeyondLatency", 
		"TAGame.GameEvent_Soccar_TA:ReplicatedServerPerformanceState", 
		"TAGame.CarComponent_TA:ReplicatedActive"
		});

	RegisterParsers<float>({
		"TAGame.CarComponent_FlipCar_TA:FlipCarTime", "TAGame.Ball_TA:ReplicatedBallScale", 
		"TAGame.CarComponent_Boost_TA:RechargeDelay", "TAGame.CarComponent_Boost_TA:RechargeRate", 
		"TAGame.Ball_TA:ReplicatedAddedCarBounceScale", "TAGame.Ball_TA:ReplicatedBallMaxLinearSpeedScale", 
		"TAGame.Ball_TA:ReplicatedWorldBounceScale", "TAGame.CarComponent_Boost_TA:BoostModifier", 
		"Engine.Actor:DrawScale", "TAGame.CrowdActor_TA:ModifiedNoise", 
		"TAGame.CarComponent_TA:ReplicatedActivityTime", "TAGame.SpecialPickup_BallFreeze_TA:RepOrigSpeed", 
		"TAGame.SpecialPickup_BallVelcro_TA:AttachTime", "TAGame.SpecialPickup_BallVelcro_TA:BreakTime", 
		"TAGame.Car_TA:AddedCarForceMultiplier", "TAGame.Car_TA:AddedBallForceMultiplier",
		"TAGame.PRI_TA:SteeringSensitivity", "TAGame.Car_TA:ReplicatedCarScale"
		});

	RegisterParsers<Reservation>("ProjectX.GRI_X:Reservations");

	RegisterParsers<UniqueId>({
		"Engine.PlayerReplicationInfo:UniqueId", "TAGame.PRI_TA:PartyLeader"
		});

	RegisterParsers<int32_t>({
		"ProjectX.GRI_X:ReplicatedGameMutatorIndex", 
		"TAGame.PRI_TA:TimeTillItem"
		});

	RegisterParsers<ReplicatedPickupData>({
		"TAGame.VehiclePickup_TA:ReplicatedPickupData"
		});

	RegisterParsers<SkillTier>({
		"TAGame.PRI_TA:SkillTier"
		});
	RegisterParsers<Vector3I>({
		"Engine.Actor:Location", "TAGame.CarComponent_Dodge_TA:DodgeTorque"
		});

	RegisterParsers<OnlineLoadout>({ "TAGame.PRI_TA:ClientLoadoutOnline" });
	RegisterParsers<ClientLoadoutsOnline>({ "TAGame.PRI_TA:ClientLoadoutsOnline" });
	RegisterParsers<ClientLoadouts>({ "TAGame.PRI_TA:ClientLoadouts" });

	RegisterParsers<CameraSettings>({ 
		"TAGame.PRI_TA:CameraSettings", "TAGame.CameraSettingsActor_TA:ProfileSettings" 
		});

	RegisterParsers<TeamPaint>({
		"TAGame.Car_TA:TeamPaint"
		});

	RegisterParsers<ReplicatedDemolish>({ "TAGame.Car_TA:ReplicatedDemolish" });

	RegisterParsers<ReplicatedMusicStringer>({ "TAGame.GameEvent_Soccar_TA:ReplicatedMusicStinger" });
	RegisterParsers<PrivateMatchSettings>({ "TAGame.GameEvent_SoccarPrivate_TA:MatchSettings" });
	RegisterParsers<ClubColors>({ "TAGame.Team_TA:ClubColors", "TAGame.Car_TA:ClubColors" });
	RegisterParsers<WeldedInfo>({ "TAGame.RBActor_TA:WeldedInfo" });
	RegisterParsers<DamageState>({ "TAGame.BreakOutActor_Platform_TA:DamageState" });
	RegisterParsers<AppliedDamage>({ "TAGame.Ball_Breakout_TA:AppliedDamage" });
	RegisterParsers<ReplicatedExplosionData>({ "TAGame.Ball_TA:ReplicatedExplosionData" });
	RegisterParsers<ReplicatedExplosionDataExtended>({ "TAGame.Ball_TA:ReplicatedExplosionDataExtended" });
	RegisterParsers<ReplicatedTitle>({ "TAGame.PRI_TA:SecondaryTitle", "TAGame.PRI_TA:PrimaryTitle" });
	RegisterParsers<HistoryKey>({ "TAGame.PRI_TA:PlayerHistoryKey" });
	RegisterParsers<ReplicatedStatEvent>({ "TAGame.GameEvent_Soccar_TA:ReplicatedStatEvent" });
	RegisterParsers<RepStatTitle>({ "TAGame.PRI_TA:RepStatTitles" });
	//RegisterParsers<>({  });
	//RegisterParsers<>({  });
}

void NetworkStreamParser::Parse(const std::string & name, CPPBitReader<uint32_t>& br)
{
	if (parseFunctions.find(name) == parseFunctions.end())
	{
		printf("Could not find parser for %s\n", name.c_str());
		return;
	}
	auto inst = parseFunctions[name](br);
	//printf("Result: %s\n", inst->ToString().c_str());
}
