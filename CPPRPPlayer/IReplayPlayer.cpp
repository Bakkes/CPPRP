#include "IReplayPlayer.h"
namespace CPPRP
{
	namespace ReplayPlaying
	{
		float IReplayPlayer::GetFastestKickoff()
		{
			if (kickoffSpeeds.size() == 0) return 9999.f;
			return *kickoffSpeeds.begin();
		}
		float IReplayPlayer::GetFastestGoal()
		{
			if (scoreSpeeds.size() == 0) return -1.f;
			return *scoreSpeeds.begin();
		}
		float IReplayPlayer::GetSlowestGoal()
		{
			if (scoreSpeeds.size() == 0) return 10000.f;
			return *scoreSpeeds.rbegin();
		}
		IReplayPlayer::IReplayPlayer(std::shared_ptr<CPPRP::ReplayFile> replay) : replay_(replay)
		{
			using namespace std::placeholders;
			
			replay->tickables.push_back(std::bind(&IReplayPlayer::OnTick, this, _1, _2));
			replay->newFrameCallbacks.push_back(std::bind(&IReplayPlayer::OnNewFrame, this, _1));
			replay->createdCallbacks.push_back(std::bind(&IReplayPlayer::OnActorCreated, this, _1));
			replay->updatedCallbacks.push_back(std::bind(&IReplayPlayer::OnActorUpdated, this, _1, _2));
			replay->actorDeleteCallbacks.push_back(std::bind(&IReplayPlayer::OnActorDeleted, this, _1));
		
		
			RegisterActorUpdate<ProjectX::GRI_X>(FIELD(ProjectX::GRI_X::bGameStarted), 
				[](const std::shared_ptr<ProjectX::GRI_X>& actor, std::string& propertyName)
				{
					//printf("Game has started: %s\n", actor->MatchGUID.c_str());

				});

			RegisterActorUpdate<TAGame::GameEvent_Soccar_TA>(FIELD(TAGame::GameEvent_Soccar_TA::SecondsRemaining),
				[](const std::shared_ptr<TAGame::GameEvent_Soccar_TA>& actor, std::string& propertyName)
				{
					//printf("GameTime: %i\n", actor->SecondsRemaining);

				});

			RegisterActorUpdate<TAGame::GameEvent_TA>(FIELD(TAGame::GameEvent_TA::ReplicatedRoundCountDownNumber),
				[&currentGameState_ = currentGameState_](const std::shared_ptr<TAGame::GameEvent_TA>& actor, std::string& propertyName)
				{
					//printf("Countdown: %i\n", actor->ReplicatedRoundCountDownNumber);
					currentGameState_ = actor->ReplicatedRoundCountDownNumber == 0 ? GameState::Playing : GameState::Countdown;
					
				});

			RegisterActorUpdate<TAGame::GameEvent_Soccar_TA>(FIELD(TAGame::GameEvent_Soccar_TA::bBallHasBeenHit),
				[&timeSinceBallHit = timeSinceBallHit, 
				&lastVal = lastVal,
				&kickoffSpeeds = kickoffSpeeds](const std::shared_ptr<TAGame::GameEvent_Soccar_TA>& actor, std::string& propertyName)
				{
					if (actor->bBallHasBeenHit == lastVal)
						return;
					//Time between kickoff and ball hit
					if (!actor->bBallHasBeenHit)
					{
						timeSinceBallHit = 0.f;
					}
					else
					{
						kickoffSpeeds.insert(timeSinceBallHit);
						//printf("Time since ball hit: %.3f\n", timeSinceBallHit);
					}
					lastVal = actor->bBallHasBeenHit;
					//printf("Ball has been hit: %i\n", actor->bBallHasBeenHit);

				});

			RegisterActorUpdate<TAGame::Team_TA>(FIELD(TAGame::Team_TA::CustomTeamName), [](const std::shared_ptr<TAGame::Team_TA>& actor, std::string& propertyName)
				{

					//printf("Score update: %s\n", actor->CustomTeamName.c_str());

				});

			RegisterActorUpdate<TAGame::GameEvent_Soccar_TA>(FIELD(TAGame::GameEvent_Soccar_TA::ReplicatedScoredOnTeam), [&](const std::shared_ptr<TAGame::GameEvent_Soccar_TA>& actor, std::string& propertyName)
				{
					bool hasbol = false;
					//printf("AAAAAAAAA: %i\n", actor->MatchGoals);
					for (const auto& act : replay_->actorStates)
					{
						if (auto c = std::dynamic_pointer_cast<CPPRP::TAGame::Ball_TA>(act.second.actorObject))
						{
							auto lin_vel = c->ReplicatedRBState.linear_velocity;
							float abc = powf(lin_vel.x, 2) + powf(lin_vel.y, 2) + powf(lin_vel.z, 2);
							abc = sqrt(abc);

							if (abc > .001f)
							{
								const float kmh = (abc * 60 * 60) / 100000;
								scoreSpeeds.insert(kmh);
								//printf("Score speed: %.2f\n", abc);
							}
						}
					}
					if (hasbol == false)
					{
						//printf("No bol!!\n");
					}
				});
		}



		void IReplayPlayer::OnTick(const Frame frame, const std::unordered_map<int, ActorStateData>& actorData)
		{
			timeSinceBallHit += frame.delta;
			//printf("[%i] Hit frame: %i\n", currentGameState_, frame.frameNumber);
		}

		void IReplayPlayer::OnNewFrame(const Frame frame)
		{
		}

		void IReplayPlayer::OnActorCreated(const ActorStateData& createdActor)
		{
		}

		void IReplayPlayer::OnActorUpdated(const ActorStateData& updatedActor, const std::vector<uint32_t>& updatedProperties)
		{
			for (auto prop : updatedProperties)
			{
				for (auto& cb : variableUpdates_[prop])
				{
					std::string abc = "";
					cb(updatedActor.actorObject, abc);
				}
			}
		}

		void IReplayPlayer::OnActorDeleted(const ActorStateData& deletedActor)
		{//
			
		}

	}
}
