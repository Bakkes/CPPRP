/*#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>

#pragma comment(lib, "CPPRP.lib")
#include "../CPPRP/ReplayFile.h"
#include "../CPPRP/Exceptions/ReplayException.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include <stdint.h>
#include <string>

template<typename T>
void Serialize(T wot)
{

}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::Engine::Actor>& item)
{
        Serialize<CPPRP::Engine::Object>(writer, item);
        writer.String("DrawScale");
        Serialize<float>(writer, item->DrawScale);
        writer.String("bCollideActors");
        Serialize<bool>(writer, item->bCollideActors);
        writer.String("bCollideWorld");
        Serialize<bool>(writer, item->bCollideWorld);
        writer.String("bNetOwner");
        Serialize<bool>(writer, item->bNetOwner);
        writer.String("Base");
        Serialize<CPPRP::ActorBase>(writer, item->Base);
        writer.String("bBlockActors");
        Serialize<bool>(writer, item->bBlockActors);
        writer.String("bHidden");
        Serialize<bool>(writer, item->bHidden);
        writer.String("bTearOff");
        Serialize<bool>(writer, item->bTearOff);
        writer.String("Location");
        Serialize<CPPRP::Vector3I>(writer, item->Location);
        writer.String("Rotation");
        Serialize<CPPRP::Rotator>(writer, item->Rotation);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::Engine::Info>& item)
{
        Serialize<CPPRP::Engine::Actor>(writer, item);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::Engine::ReplicationInfo>& item)
{
        Serialize<CPPRP::Engine::Info>(writer, item);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::Engine::GameReplicationInfo>& item)
{
        Serialize<CPPRP::Engine::ReplicationInfo>(writer, item);
        writer.String("ServerName");
        Serialize<std::string>(writer, item->ServerName);
        writer.String("GameClass");
        Serialize<CPPRP::ObjectTarget>(writer, item->GameClass);
        writer.String("bStopCountDown");
        Serialize<bool>(writer, item->bStopCountDown);
        writer.String("bMatchIsOver");
        Serialize<bool>(writer, item->bMatchIsOver);
        writer.String("bMatchHasBegun");
        Serialize<bool>(writer, item->bMatchHasBegun);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::Engine::Pawn>& item)
{
        Serialize<CPPRP::Engine::Actor>(writer, item);
        writer.String("PlayerReplicationInfo");
        Serialize<CPPRP::ActiveActor>(writer, item->PlayerReplicationInfo);
        writer.String("HealthMax");
        Serialize<uint32_t>(writer, item->HealthMax);
        writer.String("bIsCrouched");
        Serialize<bool>(writer, item->bIsCrouched);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::Engine::PlayerReplicationInfo>& item)
{
        Serialize<CPPRP::Engine::ReplicationInfo>(writer, item);
        writer.String("Team");
        Serialize<CPPRP::ActiveActor>(writer, item->Team);
        writer.String("bReadyToPlay");
        Serialize<bool>(writer, item->bReadyToPlay);
        writer.String("PlayerName");
        Serialize<std::string>(writer, item->PlayerName);
        writer.String("RemoteUserData");
        Serialize<std::string>(writer, item->RemoteUserData);
        writer.String("bWaitingPlayer");
        Serialize<bool>(writer, item->bWaitingPlayer);
        writer.String("Score");
        Serialize<uint32_t>(writer, item->Score);
        writer.String("PlayerID");
        Serialize<uint32_t>(writer, item->PlayerID);
        writer.String("bBot");
        Serialize<bool>(writer, item->bBot);
        writer.String("bIsSpectator");
        Serialize<bool>(writer, item->bIsSpectator);
        writer.String("bTimedOut");
        Serialize<bool>(writer, item->bTimedOut);
        writer.String("bAdmin");
        Serialize<bool>(writer, item->bAdmin);
        writer.String("bIsInactive");
        Serialize<bool>(writer, item->bIsInactive);
        writer.String("bOnlySpectator");
        Serialize<bool>(writer, item->bOnlySpectator);
        writer.String("Ping");
        Serialize<unsigned char>(writer, item->Ping);
        writer.String("UniqueId");
        Serialize<class std::shared_ptr<CPPRP::UniqueId>>(writer, item->UniqueId);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::Engine::TeamInfo>& item)
{
        Serialize<CPPRP::Engine::ReplicationInfo>(writer, item);
        writer.String("Score");
        Serialize<uint32_t>(writer, item->Score);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::Engine::WorldInfo>& item)
{
        Serialize<CPPRP::Engine::Info>(writer, item);
        writer.String("WorldGravityZ");
        Serialize<float>(writer, item->WorldGravityZ);
        writer.String("TimeDilation");
        Serialize<float>(writer, item->TimeDilation);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::ProjectX::GRI_X>& item)
{
        Serialize<CPPRP::Engine::GameReplicationInfo>(writer, item);
        writer.String("MatchGUID");
        Serialize<std::string>(writer, item->MatchGUID);
        writer.String("ReplicatedGameMutatorIndex");
        Serialize<int>(writer, item->ReplicatedGameMutatorIndex);
        writer.String("bGameStarted");
        Serialize<bool>(writer, item->bGameStarted);
        writer.String("ReplicatedGamePlaylist");
        Serialize<uint32_t>(writer, item->ReplicatedGamePlaylist);
        writer.String("GameServerID");
        Serialize<uint64_t>(writer, item->GameServerID);
        writer.String("Reservations");
        Serialize<CPPRP::Reservation>(writer, item->Reservations);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::ProjectX::Pawn_X>& item)
{
        Serialize<CPPRP::Engine::Pawn>(writer, item);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::ProjectX::PRI_X>& item)
{
        Serialize<CPPRP::Engine::PlayerReplicationInfo>(writer, item);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::PRI_TA>& item)
{
        Serialize<CPPRP::ProjectX::PRI_X>(writer, item);
        writer.String("MatchShots");
        Serialize<uint32_t>(writer, item->MatchShots);
        writer.String("PersistentCamera");
        Serialize<CPPRP::ActiveActor>(writer, item->PersistentCamera);
        writer.String("SkillTier");
        Serialize<CPPRP::SkillTier>(writer, item->SkillTier);
        writer.String("bUsingBehindView");
        Serialize<bool>(writer, item->bUsingBehindView);
        writer.String("MatchAssists");
        Serialize<uint32_t>(writer, item->MatchAssists);
        writer.String("RespawnTimeRemaining");
        Serialize<int>(writer, item->RespawnTimeRemaining);
        writer.String("bOnlineLoadoutSet");
        Serialize<bool>(writer, item->bOnlineLoadoutSet);
        writer.String("MatchGoals");
        Serialize<uint32_t>(writer, item->MatchGoals);
        writer.String("ReplicatedGameEvent");
        Serialize<CPPRP::ActiveActor>(writer, item->ReplicatedGameEvent);
        writer.String("TotalXP");
        Serialize<uint32_t>(writer, item->TotalXP);
        writer.String("MatchScore");
        Serialize<uint32_t>(writer, item->MatchScore);
        writer.String("MatchSaves");
        Serialize<uint32_t>(writer, item->MatchSaves);
        writer.String("Title");
        Serialize<uint32_t>(writer, item->Title);
        writer.String("ClubID");
        Serialize<uint64_t>(writer, item->ClubID);
        writer.String("MaxTimeTillItem");
        Serialize<int>(writer, item->MaxTimeTillItem);
        writer.String("MatchBreakoutDamage");
        Serialize<uint32_t>(writer, item->MatchBreakoutDamage);
        writer.String("BotProductName");
        Serialize<uint32_t>(writer, item->BotProductName);
        writer.String("bReady");
        Serialize<bool>(writer, item->bReady);
        writer.String("SpectatorShortcut");
        Serialize<uint32_t>(writer, item->SpectatorShortcut);
        writer.String("bUsingSecondaryCamera");
        Serialize<bool>(writer, item->bUsingSecondaryCamera);
        writer.String("PlayerHistoryValid");
        Serialize<bool>(writer, item->PlayerHistoryValid);
        writer.String("bIsInSplitScreen");
        Serialize<bool>(writer, item->bIsInSplitScreen);
        writer.String("bMatchMVP");
        Serialize<bool>(writer, item->bMatchMVP);
        writer.String("RepStatTitles");
        Serialize<CPPRP::RepStatTitle>(writer, item->RepStatTitles);
        writer.String("bOnlineLoadoutsSet");
        Serialize<bool>(writer, item->bOnlineLoadoutsSet);
        writer.String("bUsingItems");
        Serialize<bool>(writer, item->bUsingItems);
        writer.String("PrimaryTitle");
        Serialize<CPPRP::ReplicatedTitle>(writer, item->PrimaryTitle);
        writer.String("bMatchAdmin");
        Serialize<bool>(writer, item->bMatchAdmin);
        writer.String("bBusy");
        Serialize<bool>(writer, item->bBusy);
        writer.String("bVoteToForfeitDisabled");
        Serialize<bool>(writer, item->bVoteToForfeitDisabled);
        writer.String("bUsingFreecam");
        Serialize<bool>(writer, item->bUsingFreecam);
        writer.String("ClientLoadoutOnline");
        Serialize<CPPRP::OnlineLoadout>(writer, item->ClientLoadoutOnline);
        writer.String("CameraYaw");
        Serialize<unsigned char>(writer, item->CameraYaw);
        writer.String("CameraPitch");
        Serialize<unsigned char>(writer, item->CameraPitch);
        writer.String("PawnType");
        Serialize<unsigned char>(writer, item->PawnType);
        writer.String("ReplicatedWorstNetQualityBeyondLatency");
        Serialize<unsigned char>(writer, item->ReplicatedWorstNetQualityBeyondLatency);
        writer.String("SteeringSensitivity");
        Serialize<float>(writer, item->SteeringSensitivity);
        writer.String("PartyLeader");
        Serialize<CPPRP::PartyLeader>(writer, item->PartyLeader);
        writer.String("TimeTillItem");
        Serialize<int>(writer, item->TimeTillItem);
        writer.String("ClientLoadoutsOnline");
        Serialize<CPPRP::ClientLoadoutsOnline>(writer, item->ClientLoadoutsOnline);
        writer.String("ClientLoadouts");
        Serialize<CPPRP::ClientLoadouts>(writer, item->ClientLoadouts);
        writer.String("ClientLoadout");
        Serialize<CPPRP::ClientLoadout>(writer, item->ClientLoadout);
        writer.String("CameraSettings");
        Serialize<CPPRP::CameraSettings>(writer, item->CameraSettings);
        writer.String("SecondaryTitle");
        Serialize<CPPRP::ReplicatedTitle>(writer, item->SecondaryTitle);
        writer.String("PlayerHistoryKey");
        Serialize<CPPRP::HistoryKey>(writer, item->PlayerHistoryKey);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::RBActor_TA>& item)
{
        Serialize<CPPRP::ProjectX::Pawn_X>(writer, item);
        writer.String("ReplicatedRBState");
        Serialize<CPPRP::ReplicatedRBState>(writer, item->ReplicatedRBState);
        writer.String("bReplayActor");
        Serialize<bool>(writer, item->bReplayActor);
        writer.String("bFrozen");
        Serialize<bool>(writer, item->bFrozen);
        writer.String("WeldedInfo");
        Serialize<CPPRP::WeldedInfo>(writer, item->WeldedInfo);
        writer.String("bIgnoreSyncing");
        Serialize<bool>(writer, item->bIgnoreSyncing);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::CarComponent_TA>& item)
{
        Serialize<CPPRP::Engine::ReplicationInfo>(writer, item);
        writer.String("Vehicle");
        Serialize<CPPRP::ActiveActor>(writer, item->Vehicle);
        writer.String("ReplicatedActive");
        Serialize<unsigned char>(writer, item->ReplicatedActive);
        writer.String("ReplicatedActivityTime");
        Serialize<float>(writer, item->ReplicatedActivityTime);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::CarComponent_Jump_TA>& item)
{
        Serialize<CPPRP::TAGame::CarComponent_TA>(writer, item);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::CarComponent_DoubleJump_TA>& item)
{
        Serialize<CPPRP::TAGame::CarComponent_TA>(writer, item);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::CarComponent_Boost_TA>& item)
{
        Serialize<CPPRP::TAGame::CarComponent_TA>(writer, item);
        writer.String("RechargeDelay");
        Serialize<float>(writer, item->RechargeDelay);
        writer.String("bUnlimitedBoost");
        Serialize<bool>(writer, item->bUnlimitedBoost);
        writer.String("UnlimitedBoostRefCount");
        Serialize<uint32_t>(writer, item->UnlimitedBoostRefCount);
        writer.String("bNoBoost");
        Serialize<bool>(writer, item->bNoBoost);
        writer.String("ReplicatedBoostAmount");
        Serialize<unsigned char>(writer, item->ReplicatedBoostAmount);
        writer.String("RechargeRate");
        Serialize<float>(writer, item->RechargeRate);
        writer.String("BoostModifier");
        Serialize<float>(writer, item->BoostModifier);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::CarComponent_Dodge_TA>& item)
{
        Serialize<CPPRP::TAGame::CarComponent_TA>(writer, item);
        writer.String("DodgeTorque");
        Serialize<CPPRP::Vector3I>(writer, item->DodgeTorque);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::CarComponent_FlipCar_TA>& item)
{
        Serialize<CPPRP::TAGame::CarComponent_TA>(writer, item);
        writer.String("bFlipRight");
        Serialize<bool>(writer, item->bFlipRight);
        writer.String("FlipCarTime");
        Serialize<float>(writer, item->FlipCarTime);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::Ball_TA>& item)
{
        Serialize<CPPRP::TAGame::RBActor_TA>(writer, item);
        writer.String("GameEvent");
        Serialize<CPPRP::ActiveActor>(writer, item->GameEvent);
        writer.String("ReplicatedPhysMatOverride");
        Serialize<CPPRP::ObjectTarget>(writer, item->ReplicatedPhysMatOverride);
        writer.String("ReplicatedBallGravityScale");
        Serialize<float>(writer, item->ReplicatedBallGravityScale);
        writer.String("ReplicatedBallScale");
        Serialize<float>(writer, item->ReplicatedBallScale);
        writer.String("HitTeamNum");
        Serialize<unsigned char>(writer, item->HitTeamNum);
        writer.String("ReplicatedWorldBounceScale");
        Serialize<float>(writer, item->ReplicatedWorldBounceScale);
        writer.String("ReplicatedAddedCarBounceScale");
        Serialize<float>(writer, item->ReplicatedAddedCarBounceScale);
        writer.String("ReplicatedExplosionData");
        Serialize<CPPRP::ReplicatedExplosionData>(writer, item->ReplicatedExplosionData);
        writer.String("ReplicatedBallMaxLinearSpeedScale");
        Serialize<float>(writer, item->ReplicatedBallMaxLinearSpeedScale);
        writer.String("ReplicatedExplosionDataExtended");
        Serialize<CPPRP::ReplicatedExplosionDataExtended>(writer, item->ReplicatedExplosionDataExtended);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::Team_TA>& item)
{
        Serialize<CPPRP::Engine::TeamInfo>(writer, item);
        writer.String("LogoData");
        Serialize<CPPRP::LogoData>(writer, item->LogoData);
        writer.String("GameEvent");
        Serialize<CPPRP::ActiveActor>(writer, item->GameEvent);
        writer.String("CustomTeamName");
        Serialize<std::string>(writer, item->CustomTeamName);
        writer.String("ClubID");
        Serialize<uint64_t>(writer, item->ClubID);
        writer.String("ClubColors");
        Serialize<CPPRP::ClubColors>(writer, item->ClubColors);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::Team_Soccar_TA>& item)
{
        Serialize<CPPRP::TAGame::Team_TA>(writer, item);
        writer.String("GameScore");
        Serialize<uint32_t>(writer, item->GameScore);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::BreakOutActor_Platform_TA>& item)
{
        Serialize<CPPRP::Engine::Actor>(writer, item);
        writer.String("DamageState");
        Serialize<CPPRP::DamageState>(writer, item->DamageState);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::SpecialPickup_TA>& item)
{
        Serialize<CPPRP::TAGame::CarComponent_TA>(writer, item);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::SpecialPickup_Targeted_TA>& item)
{
        Serialize<CPPRP::TAGame::SpecialPickup_TA>(writer, item);
        writer.String("Targeted");
        Serialize<CPPRP::ActiveActor>(writer, item->Targeted);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::SpecialPickup_Tornado_TA>& item)
{
        Serialize<CPPRP::TAGame::SpecialPickup_TA>(writer, item);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::SpecialPickup_HauntedBallBeam_TA>& item)
{
        Serialize<CPPRP::TAGame::SpecialPickup_TA>(writer, item);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::SpecialPickup_BallVelcro_TA>& item)
{
        Serialize<CPPRP::TAGame::SpecialPickup_TA>(writer, item);
        writer.String("bHit");
        Serialize<bool>(writer, item->bHit);
        writer.String("bBroken");
        Serialize<bool>(writer, item->bBroken);
        writer.String("AttachTime");
        Serialize<float>(writer, item->AttachTime);
        writer.String("BreakTime");
        Serialize<float>(writer, item->BreakTime);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::SpecialPickup_Rugby_TA>& item)
{
        Serialize<CPPRP::TAGame::SpecialPickup_TA>(writer, item);
        writer.String("bBallWelded");
        Serialize<bool>(writer, item->bBallWelded);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::SpecialPickup_BallFreeze_TA>& item)
{
        Serialize<CPPRP::TAGame::SpecialPickup_Targeted_TA>(writer, item);
        writer.String("RepOrigSpeed");
        Serialize<float>(writer, item->RepOrigSpeed);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::SpecialPickup_Spring_TA>& item)
{
        Serialize<CPPRP::TAGame::SpecialPickup_Targeted_TA>(writer, item);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::SpecialPickup_BallCarSpring_TA>& item)
{
        Serialize<CPPRP::TAGame::SpecialPickup_Spring_TA>(writer, item);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::SpecialPickup_BallGravity_TA>& item)
{
        Serialize<CPPRP::TAGame::SpecialPickup_TA>(writer, item);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::SpecialPickup_GrapplingHook_TA>& item)
{
        Serialize<CPPRP::TAGame::SpecialPickup_Targeted_TA>(writer, item);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::SpecialPickup_BallLasso_TA>& item)
{
        Serialize<CPPRP::TAGame::SpecialPickup_GrapplingHook_TA>(writer, item);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::SpecialPickup_BoostOverride_TA>& item)
{
        Serialize<CPPRP::TAGame::SpecialPickup_Targeted_TA>(writer, item);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::SpecialPickup_Batarang_TA>& item)
{
        Serialize<CPPRP::TAGame::SpecialPickup_BallLasso_TA>(writer, item);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::SpecialPickup_HitForce_TA>& item)
{
        Serialize<CPPRP::TAGame::SpecialPickup_TA>(writer, item);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::SpecialPickup_Swapper_TA>& item)
{
        Serialize<CPPRP::TAGame::SpecialPickup_Targeted_TA>(writer, item);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::CrowdManager_TA>& item)
{
        Serialize<CPPRP::Engine::ReplicationInfo>(writer, item);
        writer.String("GameEvent");
        Serialize<CPPRP::ActiveActor>(writer, item->GameEvent);
        writer.String("ReplicatedGlobalOneShotSound");
        Serialize<CPPRP::ObjectTarget>(writer, item->ReplicatedGlobalOneShotSound);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::CrowdActor_TA>& item)
{
        Serialize<CPPRP::Engine::ReplicationInfo>(writer, item);
        writer.String("GameEvent");
        Serialize<CPPRP::ActiveActor>(writer, item->GameEvent);
        writer.String("ReplicatedOneShotSound");
        Serialize<CPPRP::ObjectTarget>(writer, item->ReplicatedOneShotSound);
        writer.String("ReplicatedRoundCountDownNumber");
        Serialize<uint32_t>(writer, item->ReplicatedRoundCountDownNumber);
        writer.String("ReplicatedCountDownNumber");
        Serialize<uint32_t>(writer, item->ReplicatedCountDownNumber);
        writer.String("ModifiedNoise");
        Serialize<float>(writer, item->ModifiedNoise);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::InMapScoreboard_TA>& item)
{
        Serialize<CPPRP::Engine::Actor>(writer, item);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::Vehicle_TA>& item)
{
        Serialize<CPPRP::TAGame::RBActor_TA>(writer, item);
        writer.String("ReplicatedThrottle");
        Serialize<unsigned char>(writer, item->ReplicatedThrottle);
        writer.String("bReplicatedHandbrake");
        Serialize<bool>(writer, item->bReplicatedHandbrake);
        writer.String("bDriving");
        Serialize<bool>(writer, item->bDriving);
        writer.String("ReplicatedSteer");
        Serialize<unsigned char>(writer, item->ReplicatedSteer);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::Car_TA>& item)
{
        Serialize<CPPRP::TAGame::Vehicle_TA>(writer, item);
        writer.String("AttachedPickup");
        Serialize<CPPRP::ActiveActor>(writer, item->AttachedPickup);
        writer.String("AddedCarForceMultiplier");
        Serialize<float>(writer, item->AddedCarForceMultiplier);
        writer.String("ReplicatedCarScale");
        Serialize<float>(writer, item->ReplicatedCarScale);
        writer.String("AddedBallForceMultiplier");
        Serialize<float>(writer, item->AddedBallForceMultiplier);
        writer.String("TeamPaint");
        Serialize<CPPRP::TeamPaint>(writer, item->TeamPaint);
        writer.String("ReplicatedDemolish");
        Serialize<CPPRP::ReplicatedDemolish>(writer, item->ReplicatedDemolish);
        writer.String("ClubColors");
        Serialize<CPPRP::ClubColors>(writer, item->ClubColors);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::Car_Season_TA>& item)
{
        Serialize<CPPRP::TAGame::Car_TA>(writer, item);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::CameraSettingsActor_TA>& item)
{
        Serialize<CPPRP::Engine::ReplicationInfo>(writer, item);
        writer.String("PRI");
        Serialize<CPPRP::ActiveActor>(writer, item->PRI);
        writer.String("CameraPitch");
        Serialize<unsigned char>(writer, item->CameraPitch);
        writer.String("bMouseCameraToggleEnabled");
        Serialize<bool>(writer, item->bMouseCameraToggleEnabled);
        writer.String("bUsingSecondaryCamera");
        Serialize<bool>(writer, item->bUsingSecondaryCamera);
        writer.String("bUsingBehindView");
        Serialize<bool>(writer, item->bUsingBehindView);
        writer.String("ProfileSettings");
        Serialize<CPPRP::CameraSettings>(writer, item->ProfileSettings);
        writer.String("bUsingSwivel");
        Serialize<bool>(writer, item->bUsingSwivel);
        writer.String("CameraYaw");
        Serialize<unsigned char>(writer, item->CameraYaw);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::GRI_TA>& item)
{
        Serialize<CPPRP::ProjectX::GRI_X>(writer, item);
        writer.String("NewDedicatedServerIP");
        Serialize<std::string>(writer, item->NewDedicatedServerIP);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::Ball_Breakout_TA>& item)
{
        Serialize<CPPRP::TAGame::Ball_TA>(writer, item);
        writer.String("DamageIndex");
        Serialize<uint32_t>(writer, item->DamageIndex);
        writer.String("AppliedDamage");
        Serialize<CPPRP::AppliedDamage>(writer, item->AppliedDamage);
        writer.String("LastTeamTouch");
        Serialize<unsigned char>(writer, item->LastTeamTouch);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::VehiclePickup_TA>& item)
{
        Serialize<CPPRP::Engine::ReplicationInfo>(writer, item);
        writer.String("bNoPickup");
        Serialize<bool>(writer, item->bNoPickup);
        writer.String("ReplicatedPickupData");
        Serialize<CPPRP::ReplicatedPickupData>(writer, item->ReplicatedPickupData);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::VehiclePickup_Boost_TA>& item)
{
        Serialize<CPPRP::TAGame::VehiclePickup_TA>(writer, item);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::Ball_Haunted_TA>& item)
{
        Serialize<CPPRP::TAGame::Ball_TA>(writer, item);
        writer.String("DeactivatedGoalIndex");
        Serialize<unsigned char>(writer, item->DeactivatedGoalIndex);
        writer.String("bIsBallBeamed");
        Serialize<bool>(writer, item->bIsBallBeamed);
        writer.String("ReplicatedBeamBrokenValue");
        Serialize<unsigned char>(writer, item->ReplicatedBeamBrokenValue);
        writer.String("LastTeamTouch");
        Serialize<unsigned char>(writer, item->LastTeamTouch);
        writer.String("TotalActiveBeams");
        Serialize<unsigned char>(writer, item->TotalActiveBeams);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::GameEvent_TA>& item)
{
        Serialize<CPPRP::Engine::ReplicationInfo>(writer, item);
        writer.String("ReplicatedRoundCountDownNumber");
        Serialize<uint32_t>(writer, item->ReplicatedRoundCountDownNumber);
        writer.String("ActivatorCar");
        Serialize<CPPRP::ActiveActor>(writer, item->ActivatorCar);
        writer.String("ReplicatedGameStateTimeRemaining");
        Serialize<uint32_t>(writer, item->ReplicatedGameStateTimeRemaining);
        writer.String("ReplicatedStateName");
        Serialize<uint32_t>(writer, item->ReplicatedStateName);
        writer.String("MatchTypeClass");
        Serialize<CPPRP::ObjectTarget>(writer, item->MatchTypeClass);
        writer.String("BotSkill");
        Serialize<uint32_t>(writer, item->BotSkill);
        writer.String("bHasLeaveMatchPenalty");
        Serialize<bool>(writer, item->bHasLeaveMatchPenalty);
        writer.String("bCanVoteToForfeit");
        Serialize<bool>(writer, item->bCanVoteToForfeit);
        writer.String("bAllowReadyUp");
        Serialize<bool>(writer, item->bAllowReadyUp);
        writer.String("GameMode");
        Serialize<CPPRP::GameMode>(writer, item->GameMode);
        writer.String("ReplicatedStateIndex");
        Serialize<CPPRP::ReplicatedStateIndex>(writer, item->ReplicatedStateIndex);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::GameEvent_Team_TA>& item)
{
        Serialize<CPPRP::TAGame::GameEvent_TA>(writer, item);
        writer.String("MaxTeamSize");
        Serialize<uint32_t>(writer, item->MaxTeamSize);
        writer.String("bForfeit");
        Serialize<bool>(writer, item->bForfeit);
        writer.String("bDisableMutingOtherTeam");
        Serialize<bool>(writer, item->bDisableMutingOtherTeam);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::GameEvent_Soccar_TA>& item)
{
        Serialize<CPPRP::TAGame::GameEvent_Team_TA>(writer, item);
        writer.String("bOverTime");
        Serialize<bool>(writer, item->bOverTime);
        writer.String("GameTime");
        Serialize<uint32_t>(writer, item->GameTime);
        writer.String("MVP");
        Serialize<CPPRP::ActiveActor>(writer, item->MVP);
        writer.String("MatchWinner");
        Serialize<CPPRP::ActiveActor>(writer, item->MatchWinner);
        writer.String("GameWinner");
        Serialize<CPPRP::ActiveActor>(writer, item->GameWinner);
        writer.String("SubRulesArchetype");
        Serialize<CPPRP::ObjectTarget>(writer, item->SubRulesArchetype);
        writer.String("SecondsRemaining");
        Serialize<uint32_t>(writer, item->SecondsRemaining);
        writer.String("RoundNum");
        Serialize<uint32_t>(writer, item->RoundNum);
        writer.String("SeriesLength");
        Serialize<uint32_t>(writer, item->SeriesLength);
        writer.String("ReplicatedMusicStinger");
        Serialize<CPPRP::ReplicatedMusicStringer>(writer, item->ReplicatedMusicStinger);
        writer.String("bBallHasBeenHit");
        Serialize<bool>(writer, item->bBallHasBeenHit);
        writer.String("bUnlimitedTime");
        Serialize<bool>(writer, item->bUnlimitedTime);
        writer.String("ReplicatedStatEvent");
        Serialize<CPPRP::ReplicatedStatEvent>(writer, item->ReplicatedStatEvent);
        writer.String("bShowIntroScene");
        Serialize<bool>(writer, item->bShowIntroScene);
        writer.String("bClubMatch");
        Serialize<bool>(writer, item->bClubMatch);
        writer.String("ReplicatedScoredOnTeam");
        Serialize<unsigned char>(writer, item->ReplicatedScoredOnTeam);
        writer.String("bMatchEnded");
        Serialize<bool>(writer, item->bMatchEnded);
        writer.String("ReplicatedServerPerformanceState");
        Serialize<unsigned char>(writer, item->ReplicatedServerPerformanceState);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::GameEvent_Breakout_TA>& item)
{
        Serialize<CPPRP::TAGame::GameEvent_Soccar_TA>(writer, item);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::GameEvent_Season_TA>& item)
{
        Serialize<CPPRP::TAGame::GameEvent_Soccar_TA>(writer, item);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::GameEvent_SoccarPrivate_TA>& item)
{
        Serialize<CPPRP::TAGame::GameEvent_Soccar_TA>(writer, item);
        writer.String("MatchSettings");
        Serialize<CPPRP::PrivateMatchSettings>(writer, item->MatchSettings);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::GameEvent_SoccarSplitscreen_TA>& item)
{
        Serialize<CPPRP::TAGame::GameEvent_SoccarPrivate_TA>(writer, item);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::GameEvent_Tutorial_TA>& item)
{
        Serialize<CPPRP::TAGame::GameEvent_Soccar_TA>(writer, item);
}

template<typename Writer>
inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::TAGame::HauntedBallTrapTrigger_TA>& item)
{
        Serialize<CPPRP::Engine::Actor>(writer, item);
}


template<typename T>
constexpr inline static void RegisterJSONSerializer(char* variable, T func)
{
        std::map<std::string, std::function<void(auto, std::shared_ptr<CPPRP::Engine::Actor>&)>> test;
        test[variable] = &func;
}

constexpr inline static void InitializeSingles()
{

RegisterJSONSerializer("Engine.Actor:DrawScale", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("DrawScale");
                Serialize<float>(std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->DrawScale);
        });
RegisterJSONSerializer("Engine.Actor:bCollideActors", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bCollideActors");
                Serialize<bool>(std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->bCollideActors);
        });
RegisterJSONSerializer("Engine.Actor:bCollideWorld", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bCollideWorld");
                Serialize<bool>(std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->bCollideWorld);
        });
RegisterJSONSerializer("Engine.Actor:bNetOwner", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bNetOwner");
                Serialize<bool>(std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->bNetOwner);
        });
RegisterJSONSerializer("Engine.Actor:Base", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("Base");
                Serialize<CPPRP::ActorBase>(std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->Base);
        });
RegisterJSONSerializer("Engine.Actor:bBlockActors", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bBlockActors");
                Serialize<bool>(std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->bBlockActors);
        });
RegisterJSONSerializer("Engine.Actor:bHidden", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bHidden");
                Serialize<bool>(std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->bHidden);
        });
RegisterJSONSerializer("Engine.Actor:bTearOff", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bTearOff");
                Serialize<bool>(std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->bTearOff);
        });
RegisterJSONSerializer("Engine.Actor:Location", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("Location");
                Serialize<CPPRP::Vector3I>(std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->Location);
        });
RegisterJSONSerializer("Engine.Actor:Rotation", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("Rotation");
                Serialize<CPPRP::Rotator>(std::static_pointer_cast<CPPRP::Engine::Actor>(struc)->Rotation);
        });
RegisterJSONSerializer("Engine.GameReplicationInfo:ServerName", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ServerName");
                Serialize<std::string>(std::static_pointer_cast<CPPRP::Engine::GameReplicationInfo>(struc)->ServerName);
        });
RegisterJSONSerializer("Engine.GameReplicationInfo:GameClass", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("GameClass");
                Serialize<CPPRP::ObjectTarget>(std::static_pointer_cast<CPPRP::Engine::GameReplicationInfo>(struc)->GameClass);
        });
RegisterJSONSerializer("Engine.GameReplicationInfo:bStopCountDown", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bStopCountDown");
                Serialize<bool>(std::static_pointer_cast<CPPRP::Engine::GameReplicationInfo>(struc)->bStopCountDown);
        });
RegisterJSONSerializer("Engine.GameReplicationInfo:bMatchIsOver", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bMatchIsOver");
                Serialize<bool>(std::static_pointer_cast<CPPRP::Engine::GameReplicationInfo>(struc)->bMatchIsOver);
        });
RegisterJSONSerializer("Engine.GameReplicationInfo:bMatchHasBegun", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bMatchHasBegun");
                Serialize<bool>(std::static_pointer_cast<CPPRP::Engine::GameReplicationInfo>(struc)->bMatchHasBegun);
        });
RegisterJSONSerializer("Engine.Pawn:PlayerReplicationInfo", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("PlayerReplicationInfo");
                Serialize<CPPRP::ActiveActor>(std::static_pointer_cast<CPPRP::Engine::Pawn>(struc)->PlayerReplicationInfo);
        });
RegisterJSONSerializer("Engine.Pawn:HealthMax", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("HealthMax");
                Serialize<uint32_t>(std::static_pointer_cast<CPPRP::Engine::Pawn>(struc)->HealthMax);
        });
RegisterJSONSerializer("Engine.Pawn:bIsCrouched", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bIsCrouched");
                Serialize<bool>(std::static_pointer_cast<CPPRP::Engine::Pawn>(struc)->bIsCrouched);
        });
RegisterJSONSerializer("Engine.PlayerReplicationInfo:Team", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("Team");
                Serialize<CPPRP::ActiveActor>(std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->Team);
        });
RegisterJSONSerializer("Engine.PlayerReplicationInfo:bReadyToPlay", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bReadyToPlay");
                Serialize<bool>(std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->bReadyToPlay);
        });
RegisterJSONSerializer("Engine.PlayerReplicationInfo:PlayerName", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("PlayerName");
                Serialize<std::string>(std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->PlayerName);
        });
RegisterJSONSerializer("Engine.PlayerReplicationInfo:RemoteUserData", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("RemoteUserData");
                Serialize<std::string>(std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->RemoteUserData);
        });
RegisterJSONSerializer("Engine.PlayerReplicationInfo:bWaitingPlayer", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bWaitingPlayer");
                Serialize<bool>(std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->bWaitingPlayer);
        });
RegisterJSONSerializer("Engine.PlayerReplicationInfo:Score", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("Score");
                Serialize<uint32_t>(std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->Score);
        });
RegisterJSONSerializer("Engine.PlayerReplicationInfo:PlayerID", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("PlayerID");
                Serialize<uint32_t>(std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->PlayerID);
        });
RegisterJSONSerializer("Engine.PlayerReplicationInfo:bBot", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bBot");
                Serialize<bool>(std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->bBot);
        });
RegisterJSONSerializer("Engine.PlayerReplicationInfo:bIsSpectator", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bIsSpectator");
                Serialize<bool>(std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->bIsSpectator);
        });
RegisterJSONSerializer("Engine.PlayerReplicationInfo:bTimedOut", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bTimedOut");
                Serialize<bool>(std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->bTimedOut);
        });
RegisterJSONSerializer("Engine.PlayerReplicationInfo:bAdmin", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bAdmin");
                Serialize<bool>(std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->bAdmin);
        });
RegisterJSONSerializer("Engine.PlayerReplicationInfo:bIsInactive", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bIsInactive");
                Serialize<bool>(std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->bIsInactive);
        });
RegisterJSONSerializer("Engine.PlayerReplicationInfo:bOnlySpectator", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bOnlySpectator");
                Serialize<bool>(std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->bOnlySpectator);
        });
RegisterJSONSerializer("Engine.PlayerReplicationInfo:Ping", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("Ping");
                Serialize<unsigned char>(std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->Ping);
        });
RegisterJSONSerializer("Engine.PlayerReplicationInfo:UniqueId", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("UniqueId");
                Serialize<class std::shared_ptr<CPPRP::UniqueId>>(std::static_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(struc)->UniqueId);
        });
RegisterJSONSerializer("Engine.TeamInfo:Score", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("Score");
                Serialize<uint32_t>(std::static_pointer_cast<CPPRP::Engine::TeamInfo>(struc)->Score);
        });
RegisterJSONSerializer("Engine.WorldInfo:WorldGravityZ", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("WorldGravityZ");
                Serialize<float>(std::static_pointer_cast<CPPRP::Engine::WorldInfo>(struc)->WorldGravityZ);
        });
RegisterJSONSerializer("Engine.WorldInfo:TimeDilation", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("TimeDilation");
                Serialize<float>(std::static_pointer_cast<CPPRP::Engine::WorldInfo>(struc)->TimeDilation);
        });
RegisterJSONSerializer("ProjectX.GRI_X:MatchGUID", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("MatchGUID");
                Serialize<std::string>(std::static_pointer_cast<CPPRP::ProjectX::GRI_X>(struc)->MatchGUID);
        });
RegisterJSONSerializer("ProjectX.GRI_X:ReplicatedGameMutatorIndex", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedGameMutatorIndex");
                Serialize<int>(std::static_pointer_cast<CPPRP::ProjectX::GRI_X>(struc)->ReplicatedGameMutatorIndex);
        });
RegisterJSONSerializer("ProjectX.GRI_X:bGameStarted", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bGameStarted");
                Serialize<bool>(std::static_pointer_cast<CPPRP::ProjectX::GRI_X>(struc)->bGameStarted);
        });
RegisterJSONSerializer("ProjectX.GRI_X:ReplicatedGamePlaylist", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedGamePlaylist");
                Serialize<uint32_t>(std::static_pointer_cast<CPPRP::ProjectX::GRI_X>(struc)->ReplicatedGamePlaylist);
        });
RegisterJSONSerializer("ProjectX.GRI_X:GameServerID", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("GameServerID");
                Serialize<uint64_t>(std::static_pointer_cast<CPPRP::ProjectX::GRI_X>(struc)->GameServerID);
        });
RegisterJSONSerializer("ProjectX.GRI_X:Reservations", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("Reservations");
                Serialize<CPPRP::Reservation>(std::static_pointer_cast<CPPRP::ProjectX::GRI_X>(struc)->Reservations);
        });
RegisterJSONSerializer("TAGame.PRI_TA:MatchShots", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("MatchShots");
                Serialize<uint32_t>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->MatchShots);
        });
RegisterJSONSerializer("TAGame.PRI_TA:PersistentCamera", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("PersistentCamera");
                Serialize<CPPRP::ActiveActor>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->PersistentCamera);
        });
RegisterJSONSerializer("TAGame.PRI_TA:SkillTier", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("SkillTier");
                Serialize<CPPRP::SkillTier>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->SkillTier);
        });
RegisterJSONSerializer("TAGame.PRI_TA:bUsingBehindView", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bUsingBehindView");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bUsingBehindView);
        });
RegisterJSONSerializer("TAGame.PRI_TA:MatchAssists", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("MatchAssists");
                Serialize<uint32_t>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->MatchAssists);
        });
RegisterJSONSerializer("TAGame.PRI_TA:RespawnTimeRemaining", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("RespawnTimeRemaining");
                Serialize<int>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->RespawnTimeRemaining);
        });
RegisterJSONSerializer("TAGame.PRI_TA:bOnlineLoadoutSet", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bOnlineLoadoutSet");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bOnlineLoadoutSet);
        });
RegisterJSONSerializer("TAGame.PRI_TA:MatchGoals", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("MatchGoals");
                Serialize<uint32_t>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->MatchGoals);
        });
RegisterJSONSerializer("TAGame.PRI_TA:ReplicatedGameEvent", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedGameEvent");
                Serialize<CPPRP::ActiveActor>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->ReplicatedGameEvent);
        });
RegisterJSONSerializer("TAGame.PRI_TA:TotalXP", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("TotalXP");
                Serialize<uint32_t>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->TotalXP);
        });
RegisterJSONSerializer("TAGame.PRI_TA:MatchScore", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("MatchScore");
                Serialize<uint32_t>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->MatchScore);
        });
RegisterJSONSerializer("TAGame.PRI_TA:MatchSaves", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("MatchSaves");
                Serialize<uint32_t>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->MatchSaves);
        });
RegisterJSONSerializer("TAGame.PRI_TA:Title", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("Title");
                Serialize<uint32_t>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->Title);
        });
RegisterJSONSerializer("TAGame.PRI_TA:ClubID", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ClubID");
                Serialize<uint64_t>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->ClubID);
        });
RegisterJSONSerializer("TAGame.PRI_TA:MaxTimeTillItem", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("MaxTimeTillItem");
                Serialize<int>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->MaxTimeTillItem);
        });
RegisterJSONSerializer("TAGame.PRI_TA:MatchBreakoutDamage", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("MatchBreakoutDamage");
                Serialize<uint32_t>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->MatchBreakoutDamage);
        });
RegisterJSONSerializer("TAGame.PRI_TA:BotProductName", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("BotProductName");
                Serialize<uint32_t>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->BotProductName);
        });
RegisterJSONSerializer("TAGame.PRI_TA:bReady", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bReady");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bReady);
        });
RegisterJSONSerializer("TAGame.PRI_TA:SpectatorShortcut", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("SpectatorShortcut");
                Serialize<uint32_t>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->SpectatorShortcut);
        });
RegisterJSONSerializer("TAGame.PRI_TA:bUsingSecondaryCamera", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bUsingSecondaryCamera");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bUsingSecondaryCamera);
        });
RegisterJSONSerializer("TAGame.PRI_TA:PlayerHistoryValid", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("PlayerHistoryValid");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->PlayerHistoryValid);
        });
RegisterJSONSerializer("TAGame.PRI_TA:bIsInSplitScreen", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bIsInSplitScreen");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bIsInSplitScreen);
        });
RegisterJSONSerializer("TAGame.PRI_TA:bMatchMVP", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bMatchMVP");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bMatchMVP);
        });
RegisterJSONSerializer("TAGame.PRI_TA:RepStatTitles", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("RepStatTitles");
                Serialize<CPPRP::RepStatTitle>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->RepStatTitles);
        });
RegisterJSONSerializer("TAGame.PRI_TA:bOnlineLoadoutsSet", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bOnlineLoadoutsSet");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bOnlineLoadoutsSet);
        });
RegisterJSONSerializer("TAGame.PRI_TA:bUsingItems", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bUsingItems");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bUsingItems);
        });
RegisterJSONSerializer("TAGame.PRI_TA:PrimaryTitle", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("PrimaryTitle");
                Serialize<CPPRP::ReplicatedTitle>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->PrimaryTitle);
        });
RegisterJSONSerializer("TAGame.PRI_TA:bMatchAdmin", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bMatchAdmin");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bMatchAdmin);
        });
RegisterJSONSerializer("TAGame.PRI_TA:bBusy", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bBusy");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bBusy);
        });
RegisterJSONSerializer("TAGame.PRI_TA:bVoteToForfeitDisabled", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bVoteToForfeitDisabled");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bVoteToForfeitDisabled);
        });
RegisterJSONSerializer("TAGame.PRI_TA:bUsingFreecam", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bUsingFreecam");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->bUsingFreecam);
        });
RegisterJSONSerializer("TAGame.PRI_TA:ClientLoadoutOnline", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ClientLoadoutOnline");
                Serialize<CPPRP::OnlineLoadout>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->ClientLoadoutOnline);
        });
RegisterJSONSerializer("TAGame.PRI_TA:CameraYaw", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("CameraYaw");
                Serialize<unsigned char>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->CameraYaw);
        });
RegisterJSONSerializer("TAGame.PRI_TA:CameraPitch", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("CameraPitch");
                Serialize<unsigned char>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->CameraPitch);
        });
RegisterJSONSerializer("TAGame.PRI_TA:PawnType", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("PawnType");
                Serialize<unsigned char>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->PawnType);
        });
RegisterJSONSerializer("TAGame.PRI_TA:ReplicatedWorstNetQualityBeyondLatency", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedWorstNetQualityBeyondLatency");
                Serialize<unsigned char>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->ReplicatedWorstNetQualityBeyondLatency);
        });
RegisterJSONSerializer("TAGame.PRI_TA:SteeringSensitivity", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("SteeringSensitivity");
                Serialize<float>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->SteeringSensitivity);
        });
RegisterJSONSerializer("TAGame.PRI_TA:PartyLeader", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("PartyLeader");
                Serialize<CPPRP::PartyLeader>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->PartyLeader);
        });
RegisterJSONSerializer("TAGame.PRI_TA:TimeTillItem", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("TimeTillItem");
                Serialize<int>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->TimeTillItem);
        });
RegisterJSONSerializer("TAGame.PRI_TA:ClientLoadoutsOnline", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ClientLoadoutsOnline");
                Serialize<CPPRP::ClientLoadoutsOnline>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->ClientLoadoutsOnline);
        });
RegisterJSONSerializer("TAGame.PRI_TA:ClientLoadouts", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ClientLoadouts");
                Serialize<CPPRP::ClientLoadouts>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->ClientLoadouts);
        });
RegisterJSONSerializer("TAGame.PRI_TA:ClientLoadout", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ClientLoadout");
                Serialize<CPPRP::ClientLoadout>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->ClientLoadout);
        });
RegisterJSONSerializer("TAGame.PRI_TA:CameraSettings", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("CameraSettings");
                Serialize<CPPRP::CameraSettings>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->CameraSettings);
        });
RegisterJSONSerializer("TAGame.PRI_TA:SecondaryTitle", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("SecondaryTitle");
                Serialize<CPPRP::ReplicatedTitle>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->SecondaryTitle);
        });
RegisterJSONSerializer("TAGame.PRI_TA:PlayerHistoryKey", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("PlayerHistoryKey");
                Serialize<CPPRP::HistoryKey>(std::static_pointer_cast<CPPRP::TAGame::PRI_TA>(struc)->PlayerHistoryKey);
        });
RegisterJSONSerializer("TAGame.RBActor_TA:ReplicatedRBState", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedRBState");
                Serialize<CPPRP::ReplicatedRBState>(std::static_pointer_cast<CPPRP::TAGame::RBActor_TA>(struc)->ReplicatedRBState);
        });
RegisterJSONSerializer("TAGame.RBActor_TA:bReplayActor", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bReplayActor");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::RBActor_TA>(struc)->bReplayActor);
        });
RegisterJSONSerializer("TAGame.RBActor_TA:bFrozen", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bFrozen");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::RBActor_TA>(struc)->bFrozen);
        });
RegisterJSONSerializer("TAGame.RBActor_TA:WeldedInfo", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("WeldedInfo");
                Serialize<CPPRP::WeldedInfo>(std::static_pointer_cast<CPPRP::TAGame::RBActor_TA>(struc)->WeldedInfo);
        });
RegisterJSONSerializer("TAGame.RBActor_TA:bIgnoreSyncing", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bIgnoreSyncing");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::RBActor_TA>(struc)->bIgnoreSyncing);
        });
RegisterJSONSerializer("TAGame.CarComponent_TA:Vehicle", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("Vehicle");
                Serialize<CPPRP::ActiveActor>(std::static_pointer_cast<CPPRP::TAGame::CarComponent_TA>(struc)->Vehicle);
        });
RegisterJSONSerializer("TAGame.CarComponent_TA:ReplicatedActive", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedActive");
                Serialize<unsigned char>(std::static_pointer_cast<CPPRP::TAGame::CarComponent_TA>(struc)->ReplicatedActive);
        });
RegisterJSONSerializer("TAGame.CarComponent_TA:ReplicatedActivityTime", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedActivityTime");
                Serialize<float>(std::static_pointer_cast<CPPRP::TAGame::CarComponent_TA>(struc)->ReplicatedActivityTime);
        });
RegisterJSONSerializer("TAGame.CarComponent_Boost_TA:RechargeDelay", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("RechargeDelay");
                Serialize<float>(std::static_pointer_cast<CPPRP::TAGame::CarComponent_Boost_TA>(struc)->RechargeDelay);
        });
RegisterJSONSerializer("TAGame.CarComponent_Boost_TA:bUnlimitedBoost", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bUnlimitedBoost");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::CarComponent_Boost_TA>(struc)->bUnlimitedBoost);
        });
RegisterJSONSerializer("TAGame.CarComponent_Boost_TA:UnlimitedBoostRefCount", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("UnlimitedBoostRefCount");
                Serialize<uint32_t>(std::static_pointer_cast<CPPRP::TAGame::CarComponent_Boost_TA>(struc)->UnlimitedBoostRefCount);
        });
RegisterJSONSerializer("TAGame.CarComponent_Boost_TA:bNoBoost", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bNoBoost");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::CarComponent_Boost_TA>(struc)->bNoBoost);
        });
RegisterJSONSerializer("TAGame.CarComponent_Boost_TA:ReplicatedBoostAmount", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedBoostAmount");
                Serialize<unsigned char>(std::static_pointer_cast<CPPRP::TAGame::CarComponent_Boost_TA>(struc)->ReplicatedBoostAmount);
        });
RegisterJSONSerializer("TAGame.CarComponent_Boost_TA:RechargeRate", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("RechargeRate");
                Serialize<float>(std::static_pointer_cast<CPPRP::TAGame::CarComponent_Boost_TA>(struc)->RechargeRate);
        });
RegisterJSONSerializer("TAGame.CarComponent_Boost_TA:BoostModifier", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("BoostModifier");
                Serialize<float>(std::static_pointer_cast<CPPRP::TAGame::CarComponent_Boost_TA>(struc)->BoostModifier);
        });
RegisterJSONSerializer("TAGame.CarComponent_Dodge_TA:DodgeTorque", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("DodgeTorque");
                Serialize<CPPRP::Vector3I>(std::static_pointer_cast<CPPRP::TAGame::CarComponent_Dodge_TA>(struc)->DodgeTorque);
        });
RegisterJSONSerializer("TAGame.CarComponent_FlipCar_TA:bFlipRight", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bFlipRight");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::CarComponent_FlipCar_TA>(struc)->bFlipRight);
        });
RegisterJSONSerializer("TAGame.CarComponent_FlipCar_TA:FlipCarTime", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("FlipCarTime");
                Serialize<float>(std::static_pointer_cast<CPPRP::TAGame::CarComponent_FlipCar_TA>(struc)->FlipCarTime);
        });
RegisterJSONSerializer("TAGame.Ball_TA:GameEvent", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("GameEvent");
                Serialize<CPPRP::ActiveActor>(std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->GameEvent);
        });
RegisterJSONSerializer("TAGame.Ball_TA:ReplicatedPhysMatOverride", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedPhysMatOverride");
                Serialize<CPPRP::ObjectTarget>(std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->ReplicatedPhysMatOverride);
        });
RegisterJSONSerializer("TAGame.Ball_TA:ReplicatedBallGravityScale", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedBallGravityScale");
                Serialize<float>(std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->ReplicatedBallGravityScale);
        });
RegisterJSONSerializer("TAGame.Ball_TA:ReplicatedBallScale", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedBallScale");
                Serialize<float>(std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->ReplicatedBallScale);
        });
RegisterJSONSerializer("TAGame.Ball_TA:HitTeamNum", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("HitTeamNum");
                Serialize<unsigned char>(std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->HitTeamNum);
        });
RegisterJSONSerializer("TAGame.Ball_TA:ReplicatedWorldBounceScale", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedWorldBounceScale");
                Serialize<float>(std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->ReplicatedWorldBounceScale);
        });
RegisterJSONSerializer("TAGame.Ball_TA:ReplicatedAddedCarBounceScale", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedAddedCarBounceScale");
                Serialize<float>(std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->ReplicatedAddedCarBounceScale);
        });
RegisterJSONSerializer("TAGame.Ball_TA:ReplicatedExplosionData", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedExplosionData");
                Serialize<CPPRP::ReplicatedExplosionData>(std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->ReplicatedExplosionData);
        });
RegisterJSONSerializer("TAGame.Ball_TA:ReplicatedBallMaxLinearSpeedScale", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedBallMaxLinearSpeedScale");
                Serialize<float>(std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->ReplicatedBallMaxLinearSpeedScale);
        });
RegisterJSONSerializer("TAGame.Ball_TA:ReplicatedExplosionDataExtended", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedExplosionDataExtended");
                Serialize<CPPRP::ReplicatedExplosionDataExtended>(std::static_pointer_cast<CPPRP::TAGame::Ball_TA>(struc)->ReplicatedExplosionDataExtended);
        });
RegisterJSONSerializer("TAGame.Team_TA:LogoData", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("LogoData");
                Serialize<CPPRP::LogoData>(std::static_pointer_cast<CPPRP::TAGame::Team_TA>(struc)->LogoData);
        });
RegisterJSONSerializer("TAGame.Team_TA:GameEvent", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("GameEvent");
                Serialize<CPPRP::ActiveActor>(std::static_pointer_cast<CPPRP::TAGame::Team_TA>(struc)->GameEvent);
        });
RegisterJSONSerializer("TAGame.Team_TA:CustomTeamName", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("CustomTeamName");
                Serialize<std::string>(std::static_pointer_cast<CPPRP::TAGame::Team_TA>(struc)->CustomTeamName);
        });
RegisterJSONSerializer("TAGame.Team_TA:ClubID", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ClubID");
                Serialize<uint64_t>(std::static_pointer_cast<CPPRP::TAGame::Team_TA>(struc)->ClubID);
        });
RegisterJSONSerializer("TAGame.Team_TA:ClubColors", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ClubColors");
                Serialize<CPPRP::ClubColors>(std::static_pointer_cast<CPPRP::TAGame::Team_TA>(struc)->ClubColors);
        });
RegisterJSONSerializer("TAGame.Team_Soccar_TA:GameScore", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("GameScore");
                Serialize<uint32_t>(std::static_pointer_cast<CPPRP::TAGame::Team_Soccar_TA>(struc)->GameScore);
        });
RegisterJSONSerializer("TAGame.BreakOutActor_Platform_TA:DamageState", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("DamageState");
                Serialize<CPPRP::DamageState>(std::static_pointer_cast<CPPRP::TAGame::BreakOutActor_Platform_TA>(struc)->DamageState);
        });
RegisterJSONSerializer("TAGame.SpecialPickup_Targeted_TA:Targeted", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("Targeted");
                Serialize<CPPRP::ActiveActor>(std::static_pointer_cast<CPPRP::TAGame::SpecialPickup_Targeted_TA>(struc)->Targeted);
        });
RegisterJSONSerializer("TAGame.SpecialPickup_BallVelcro_TA:bHit", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bHit");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::SpecialPickup_BallVelcro_TA>(struc)->bHit);
        });
RegisterJSONSerializer("TAGame.SpecialPickup_BallVelcro_TA:bBroken", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bBroken");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::SpecialPickup_BallVelcro_TA>(struc)->bBroken);
        });
RegisterJSONSerializer("TAGame.SpecialPickup_BallVelcro_TA:AttachTime", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("AttachTime");
                Serialize<float>(std::static_pointer_cast<CPPRP::TAGame::SpecialPickup_BallVelcro_TA>(struc)->AttachTime);
        });
RegisterJSONSerializer("TAGame.SpecialPickup_BallVelcro_TA:BreakTime", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("BreakTime");
                Serialize<float>(std::static_pointer_cast<CPPRP::TAGame::SpecialPickup_BallVelcro_TA>(struc)->BreakTime);
        });
RegisterJSONSerializer("TAGame.SpecialPickup_Rugby_TA:bBallWelded", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bBallWelded");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::SpecialPickup_Rugby_TA>(struc)->bBallWelded);
        });
RegisterJSONSerializer("TAGame.SpecialPickup_BallFreeze_TA:RepOrigSpeed", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("RepOrigSpeed");
                Serialize<float>(std::static_pointer_cast<CPPRP::TAGame::SpecialPickup_BallFreeze_TA>(struc)->RepOrigSpeed);
        });
RegisterJSONSerializer("TAGame.CrowdManager_TA:GameEvent", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("GameEvent");
                Serialize<CPPRP::ActiveActor>(std::static_pointer_cast<CPPRP::TAGame::CrowdManager_TA>(struc)->GameEvent);
        });
RegisterJSONSerializer("TAGame.CrowdManager_TA:ReplicatedGlobalOneShotSound", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedGlobalOneShotSound");
                Serialize<CPPRP::ObjectTarget>(std::static_pointer_cast<CPPRP::TAGame::CrowdManager_TA>(struc)->ReplicatedGlobalOneShotSound);
        });
RegisterJSONSerializer("TAGame.CrowdActor_TA:GameEvent", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("GameEvent");
                Serialize<CPPRP::ActiveActor>(std::static_pointer_cast<CPPRP::TAGame::CrowdActor_TA>(struc)->GameEvent);
        });
RegisterJSONSerializer("TAGame.CrowdActor_TA:ReplicatedOneShotSound", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedOneShotSound");
                Serialize<CPPRP::ObjectTarget>(std::static_pointer_cast<CPPRP::TAGame::CrowdActor_TA>(struc)->ReplicatedOneShotSound);
        });
RegisterJSONSerializer("TAGame.CrowdActor_TA:ReplicatedRoundCountDownNumber", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedRoundCountDownNumber");
                Serialize<uint32_t>(std::static_pointer_cast<CPPRP::TAGame::CrowdActor_TA>(struc)->ReplicatedRoundCountDownNumber);
        });
RegisterJSONSerializer("TAGame.CrowdActor_TA:ReplicatedCountDownNumber", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedCountDownNumber");
                Serialize<uint32_t>(std::static_pointer_cast<CPPRP::TAGame::CrowdActor_TA>(struc)->ReplicatedCountDownNumber);
        });
RegisterJSONSerializer("TAGame.CrowdActor_TA:ModifiedNoise", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ModifiedNoise");
                Serialize<float>(std::static_pointer_cast<CPPRP::TAGame::CrowdActor_TA>(struc)->ModifiedNoise);
        });
RegisterJSONSerializer("TAGame.Vehicle_TA:ReplicatedThrottle", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedThrottle");
                Serialize<unsigned char>(std::static_pointer_cast<CPPRP::TAGame::Vehicle_TA>(struc)->ReplicatedThrottle);
        });
RegisterJSONSerializer("TAGame.Vehicle_TA:bReplicatedHandbrake", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bReplicatedHandbrake");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::Vehicle_TA>(struc)->bReplicatedHandbrake);
        });
RegisterJSONSerializer("TAGame.Vehicle_TA:bDriving", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bDriving");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::Vehicle_TA>(struc)->bDriving);
        });
RegisterJSONSerializer("TAGame.Vehicle_TA:ReplicatedSteer", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedSteer");
                Serialize<unsigned char>(std::static_pointer_cast<CPPRP::TAGame::Vehicle_TA>(struc)->ReplicatedSteer);
        });
RegisterJSONSerializer("TAGame.Car_TA:AttachedPickup", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("AttachedPickup");
                Serialize<CPPRP::ActiveActor>(std::static_pointer_cast<CPPRP::TAGame::Car_TA>(struc)->AttachedPickup);
        });
RegisterJSONSerializer("TAGame.Car_TA:AddedCarForceMultiplier", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("AddedCarForceMultiplier");
                Serialize<float>(std::static_pointer_cast<CPPRP::TAGame::Car_TA>(struc)->AddedCarForceMultiplier);
        });
RegisterJSONSerializer("TAGame.Car_TA:ReplicatedCarScale", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedCarScale");
                Serialize<float>(std::static_pointer_cast<CPPRP::TAGame::Car_TA>(struc)->ReplicatedCarScale);
        });
RegisterJSONSerializer("TAGame.Car_TA:AddedBallForceMultiplier", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("AddedBallForceMultiplier");
                Serialize<float>(std::static_pointer_cast<CPPRP::TAGame::Car_TA>(struc)->AddedBallForceMultiplier);
        });
RegisterJSONSerializer("TAGame.Car_TA:TeamPaint", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("TeamPaint");
                Serialize<CPPRP::TeamPaint>(std::static_pointer_cast<CPPRP::TAGame::Car_TA>(struc)->TeamPaint);
        });
RegisterJSONSerializer("TAGame.Car_TA:ReplicatedDemolish", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedDemolish");
                Serialize<CPPRP::ReplicatedDemolish>(std::static_pointer_cast<CPPRP::TAGame::Car_TA>(struc)->ReplicatedDemolish);
        });
RegisterJSONSerializer("TAGame.Car_TA:ClubColors", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ClubColors");
                Serialize<CPPRP::ClubColors>(std::static_pointer_cast<CPPRP::TAGame::Car_TA>(struc)->ClubColors);
        });
RegisterJSONSerializer("TAGame.CameraSettingsActor_TA:PRI", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("PRI");
                Serialize<CPPRP::ActiveActor>(std::static_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(struc)->PRI);
        });
RegisterJSONSerializer("TAGame.CameraSettingsActor_TA:CameraPitch", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("CameraPitch");
                Serialize<unsigned char>(std::static_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(struc)->CameraPitch);
        });
RegisterJSONSerializer("TAGame.CameraSettingsActor_TA:bMouseCameraToggleEnabled", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bMouseCameraToggleEnabled");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(struc)->bMouseCameraToggleEnabled);
        });
RegisterJSONSerializer("TAGame.CameraSettingsActor_TA:bUsingSecondaryCamera", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bUsingSecondaryCamera");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(struc)->bUsingSecondaryCamera);
        });
RegisterJSONSerializer("TAGame.CameraSettingsActor_TA:bUsingBehindView", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bUsingBehindView");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(struc)->bUsingBehindView);
        });
RegisterJSONSerializer("TAGame.CameraSettingsActor_TA:ProfileSettings", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ProfileSettings");
                Serialize<CPPRP::CameraSettings>(std::static_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(struc)->ProfileSettings);
        });
RegisterJSONSerializer("TAGame.CameraSettingsActor_TA:bUsingSwivel", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bUsingSwivel");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(struc)->bUsingSwivel);
        });
RegisterJSONSerializer("TAGame.CameraSettingsActor_TA:CameraYaw", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("CameraYaw");
                Serialize<unsigned char>(std::static_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(struc)->CameraYaw);
        });
RegisterJSONSerializer("TAGame.GRI_TA:NewDedicatedServerIP", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("NewDedicatedServerIP");
                Serialize<std::string>(std::static_pointer_cast<CPPRP::TAGame::GRI_TA>(struc)->NewDedicatedServerIP);
        });
RegisterJSONSerializer("TAGame.Ball_Breakout_TA:DamageIndex", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("DamageIndex");
                Serialize<uint32_t>(std::static_pointer_cast<CPPRP::TAGame::Ball_Breakout_TA>(struc)->DamageIndex);
        });
RegisterJSONSerializer("TAGame.Ball_Breakout_TA:AppliedDamage", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("AppliedDamage");
                Serialize<CPPRP::AppliedDamage>(std::static_pointer_cast<CPPRP::TAGame::Ball_Breakout_TA>(struc)->AppliedDamage);
        });
RegisterJSONSerializer("TAGame.Ball_Breakout_TA:LastTeamTouch", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("LastTeamTouch");
                Serialize<unsigned char>(std::static_pointer_cast<CPPRP::TAGame::Ball_Breakout_TA>(struc)->LastTeamTouch);
        });
RegisterJSONSerializer("TAGame.VehiclePickup_TA:bNoPickup", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bNoPickup");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::VehiclePickup_TA>(struc)->bNoPickup);
        });
RegisterJSONSerializer("TAGame.VehiclePickup_TA:ReplicatedPickupData", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedPickupData");
                Serialize<CPPRP::ReplicatedPickupData>(std::static_pointer_cast<CPPRP::TAGame::VehiclePickup_TA>(struc)->ReplicatedPickupData);
        });
RegisterJSONSerializer("TAGame.Ball_Haunted_TA:DeactivatedGoalIndex", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("DeactivatedGoalIndex");
                Serialize<unsigned char>(std::static_pointer_cast<CPPRP::TAGame::Ball_Haunted_TA>(struc)->DeactivatedGoalIndex);
        });
RegisterJSONSerializer("TAGame.Ball_Haunted_TA:bIsBallBeamed", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bIsBallBeamed");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::Ball_Haunted_TA>(struc)->bIsBallBeamed);
        });
RegisterJSONSerializer("TAGame.Ball_Haunted_TA:ReplicatedBeamBrokenValue", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedBeamBrokenValue");
                Serialize<unsigned char>(std::static_pointer_cast<CPPRP::TAGame::Ball_Haunted_TA>(struc)->ReplicatedBeamBrokenValue);
        });
RegisterJSONSerializer("TAGame.Ball_Haunted_TA:LastTeamTouch", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("LastTeamTouch");
                Serialize<unsigned char>(std::static_pointer_cast<CPPRP::TAGame::Ball_Haunted_TA>(struc)->LastTeamTouch);
        });
RegisterJSONSerializer("TAGame.Ball_Haunted_TA:TotalActiveBeams", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("TotalActiveBeams");
                Serialize<unsigned char>(std::static_pointer_cast<CPPRP::TAGame::Ball_Haunted_TA>(struc)->TotalActiveBeams);
        });
RegisterJSONSerializer("TAGame.GameEvent_TA:ReplicatedRoundCountDownNumber", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedRoundCountDownNumber");
                Serialize<uint32_t>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->ReplicatedRoundCountDownNumber);
        });
RegisterJSONSerializer("TAGame.GameEvent_TA:ActivatorCar", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ActivatorCar");
                Serialize<CPPRP::ActiveActor>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->ActivatorCar);
        });
RegisterJSONSerializer("TAGame.GameEvent_TA:ReplicatedGameStateTimeRemaining", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedGameStateTimeRemaining");
                Serialize<uint32_t>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->ReplicatedGameStateTimeRemaining);
        });
RegisterJSONSerializer("TAGame.GameEvent_TA:ReplicatedStateName", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedStateName");
                Serialize<uint32_t>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->ReplicatedStateName);
        });
RegisterJSONSerializer("TAGame.GameEvent_TA:MatchTypeClass", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("MatchTypeClass");
                Serialize<CPPRP::ObjectTarget>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->MatchTypeClass);
        });
RegisterJSONSerializer("TAGame.GameEvent_TA:BotSkill", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("BotSkill");
                Serialize<uint32_t>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->BotSkill);
        });
RegisterJSONSerializer("TAGame.GameEvent_TA:bHasLeaveMatchPenalty", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bHasLeaveMatchPenalty");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->bHasLeaveMatchPenalty);
        });
RegisterJSONSerializer("TAGame.GameEvent_TA:bCanVoteToForfeit", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bCanVoteToForfeit");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->bCanVoteToForfeit);
        });
RegisterJSONSerializer("TAGame.GameEvent_TA:bAllowReadyUp", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bAllowReadyUp");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->bAllowReadyUp);
        });
RegisterJSONSerializer("TAGame.GameEvent_TA:GameMode", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("GameMode");
                Serialize<CPPRP::GameMode>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->GameMode);
        });
RegisterJSONSerializer("TAGame.GameEvent_TA:ReplicatedStateIndex", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedStateIndex");
                Serialize<CPPRP::ReplicatedStateIndex>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_TA>(struc)->ReplicatedStateIndex);
        });
RegisterJSONSerializer("TAGame.GameEvent_Team_TA:MaxTeamSize", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("MaxTeamSize");
                Serialize<uint32_t>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_Team_TA>(struc)->MaxTeamSize);
        });
RegisterJSONSerializer("TAGame.GameEvent_Team_TA:bForfeit", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bForfeit");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_Team_TA>(struc)->bForfeit);
        });
RegisterJSONSerializer("TAGame.GameEvent_Team_TA:bDisableMutingOtherTeam", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bDisableMutingOtherTeam");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_Team_TA>(struc)->bDisableMutingOtherTeam);
        });
RegisterJSONSerializer("TAGame.GameEvent_Soccar_TA:bOverTime", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bOverTime");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->bOverTime);
        });
RegisterJSONSerializer("TAGame.GameEvent_Soccar_TA:GameTime", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("GameTime");
                Serialize<uint32_t>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->GameTime);
        });
RegisterJSONSerializer("TAGame.GameEvent_Soccar_TA:MVP", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("MVP");
                Serialize<CPPRP::ActiveActor>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->MVP);
        });
RegisterJSONSerializer("TAGame.GameEvent_Soccar_TA:MatchWinner", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("MatchWinner");
                Serialize<CPPRP::ActiveActor>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->MatchWinner);
        });
RegisterJSONSerializer("TAGame.GameEvent_Soccar_TA:GameWinner", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("GameWinner");
                Serialize<CPPRP::ActiveActor>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->GameWinner);
        });
RegisterJSONSerializer("TAGame.GameEvent_Soccar_TA:SubRulesArchetype", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("SubRulesArchetype");
                Serialize<CPPRP::ObjectTarget>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->SubRulesArchetype);
        });
RegisterJSONSerializer("TAGame.GameEvent_Soccar_TA:SecondsRemaining", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("SecondsRemaining");
                Serialize<uint32_t>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->SecondsRemaining);
        });
RegisterJSONSerializer("TAGame.GameEvent_Soccar_TA:RoundNum", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("RoundNum");
                Serialize<uint32_t>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->RoundNum);
        });
RegisterJSONSerializer("TAGame.GameEvent_Soccar_TA:SeriesLength", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("SeriesLength");
                Serialize<uint32_t>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->SeriesLength);
        });
RegisterJSONSerializer("TAGame.GameEvent_Soccar_TA:ReplicatedMusicStinger", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedMusicStinger");
                Serialize<CPPRP::ReplicatedMusicStringer>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->ReplicatedMusicStinger);
        });
RegisterJSONSerializer("TAGame.GameEvent_Soccar_TA:bBallHasBeenHit", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bBallHasBeenHit");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->bBallHasBeenHit);
        });
RegisterJSONSerializer("TAGame.GameEvent_Soccar_TA:bUnlimitedTime", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bUnlimitedTime");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->bUnlimitedTime);
        });
RegisterJSONSerializer("TAGame.GameEvent_Soccar_TA:ReplicatedStatEvent", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedStatEvent");
                Serialize<CPPRP::ReplicatedStatEvent>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->ReplicatedStatEvent);
        });
RegisterJSONSerializer("TAGame.GameEvent_Soccar_TA:bShowIntroScene", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bShowIntroScene");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->bShowIntroScene);
        });
RegisterJSONSerializer("TAGame.GameEvent_Soccar_TA:bClubMatch", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bClubMatch");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->bClubMatch);
        });
RegisterJSONSerializer("TAGame.GameEvent_Soccar_TA:ReplicatedScoredOnTeam", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedScoredOnTeam");
                Serialize<unsigned char>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->ReplicatedScoredOnTeam);
        });
RegisterJSONSerializer("TAGame.GameEvent_Soccar_TA:bMatchEnded", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("bMatchEnded");
                Serialize<bool>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->bMatchEnded);
        });
RegisterJSONSerializer("TAGame.GameEvent_Soccar_TA:ReplicatedServerPerformanceState", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("ReplicatedServerPerformanceState");
                Serialize<unsigned char>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_Soccar_TA>(struc)->ReplicatedServerPerformanceState);
        });
RegisterJSONSerializer("TAGame.GameEvent_SoccarPrivate_TA:MatchSettings", [](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {
                writer.String("MatchSettings");
                Serialize<CPPRP::PrivateMatchSettings>(std::static_pointer_cast<CPPRP::TAGame::GameEvent_SoccarPrivate_TA>(struc)->MatchSettings);
        });

}*/