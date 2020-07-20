#include "IReplayPlayer.h"
namespace CPPRP
{
	namespace ReplayPlaying
	{
		IReplayPlayer::IReplayPlayer(std::shared_ptr<CPPRP::ReplayFile> replay)
		{
			using namespace std::placeholders;
			
			replay->tickables.push_back(std::bind(&IReplayPlayer::OnTick, this, _1, _2));
			replay->newFrameCallbacks.push_back(std::bind(&IReplayPlayer::OnNewFrame, this, _1));
			replay->createdCallbacks.push_back(std::bind(&IReplayPlayer::OnActorCreated, this, _1));
			replay->updatedCallbacks.push_back(std::bind(&IReplayPlayer::OnActorUpdated, this, _1, _2));
			replay->actorDeleteCallbacks.push_back(std::bind(&IReplayPlayer::OnActorDeleted, this, _1));
		}

		void IReplayPlayer::OnTick(const Frame frame, const std::unordered_map<int, ActorStateData>& actorData)
		{
		}

		void IReplayPlayer::OnNewFrame(const Frame frame)
		{
		}

		void IReplayPlayer::OnActorCreated(const ActorStateData& createdActor)
		{
		}

		void IReplayPlayer::OnActorUpdated(const ActorStateData& updatedActor, const std::vector<uint32_t>& updatedProperties)
		{
		}

		void IReplayPlayer::OnActorDeleted(const ActorStateData& deletedActor)
		{
		}

	}
}
