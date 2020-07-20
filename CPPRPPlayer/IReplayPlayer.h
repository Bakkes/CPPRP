#pragma once
#include <memory>
#pragma comment(lib, "CPPRP.lib")
#include "../CPPRP/ReplayFile.h"
namespace CPPRP
{
	namespace ReplayPlaying
	{
		using namespace CPPRP;

		class IReplayPlayer
		{
		private:
			std::shared_ptr<ReplayFile> replay_;
		public:
			IReplayPlayer(std::shared_ptr<CPPRP::ReplayFile> replay);

			void OnTick(const Frame frame, const std::unordered_map<int, ActorStateData>& actorData);
			void OnNewFrame(const Frame frame);
			void OnActorCreated(const ActorStateData& createdActor);
			void OnActorUpdated(const ActorStateData& updatedActor, const std::vector<uint32_t>& updatedProperties);
			void OnActorDeleted(const ActorStateData& deletedActor);

		};
	}

}