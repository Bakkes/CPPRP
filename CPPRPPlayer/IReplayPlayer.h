#pragma once
#include <memory>
#pragma comment(lib, "CPPRP.lib")
#include "../CPPRP/ReplayFile.h"
#include <set>
namespace CPPRP
{
	namespace ReplayPlaying
	{
		using namespace CPPRP;

		typedef std::function<void(const std::shared_ptr<Engine::Actor>& actor, std::string& propertyName)> ValueUpdate;

#define FIELD(f) Field(&f, #f ).ToString()

		enum class GameState : uint8_t
		{
			Countdown		= 0b1,
			PlayingNormal	= 0b01,
			PlayingOT		= 0b001,
			PostGoal		= 0b0001,
			PostGame		= 0b00001,
			Playing			= PlayingNormal | PlayingOT
		};

		template<typename T>
		struct Field
		{
		protected:
			std::string name_;
		public:
			Field(T a, std::string name)
			{
				
				name[name.find(':')] = '.';
				name = name.erase(name.find(':'), 1);
				name = name.erase(name.find(':'), 1);
				name_ = name;
				//printf("#######%s\n", typeid(T).name());
				//printf("$$$$$$$$%s\n", name.c_str());
				//auto b = &a;
			}

			std::string ToString()
			{
				return name_;
			}
			//auto a = &ProjectX::GRI_X::bGameStarted;
		};
		class IReplayPlayer
		{
		private:
			std::shared_ptr<ReplayFile> replay_;
			std::unordered_map<uint32_t, std::vector<ValueUpdate>> variableUpdates_;

			GameState currentGameState_ = GameState::Countdown;
			float timeSinceBallHit = 0.f;
			bool lastVal = true;
			std::set<float> kickoffSpeeds;
			std::set<float> scoreSpeeds;
		public:

			float GetFastestKickoff();
			float GetFastestGoal();
			float GetSlowestGoal();
			IReplayPlayer(std::shared_ptr<CPPRP::ReplayFile> replay);

			void OnTick(const Frame frame, const std::unordered_map<int, ActorStateData>& actorData);
			void OnNewFrame(const Frame frame);
			void OnActorCreated(const ActorStateData& createdActor);
			void OnActorUpdated(const ActorStateData& updatedActor, const std::vector<uint32_t>& updatedProperties);
			void OnActorDeleted(const ActorStateData& deletedActor);

			template<typename T2, typename T>
			void RegisterActorUpdate(std::string variable, T callback)
			{
				const uint32_t replicatedIndex = replay_->objectToId[variable];
				//printf("%s@@\n", typeid(T2).name());
				variableUpdates_[replicatedIndex].push_back([callback, &variable](const std::shared_ptr<Engine::Actor>& actor, std::string& propertyName)
					{
						callback(std::dynamic_pointer_cast<T2>(actor), variable);// , static_cast<T3*>(newValue));
					});
			};

			template<typename T>
			std::shared_ptr<T> GetActiveActor(ActiveActor& aa)
			{
				return std::dynamic_pointer_cast<T>(replay_->actorStates[aa.actor_id]);
			}

			template<typename T>
			std::shared_ptr<T> GetActiveActor(ObjectTarget& aa)
			{
				return std::dynamic_pointer_cast<T>(replay_->actorStates[aa.object_index]);
			}

		};
	}

}