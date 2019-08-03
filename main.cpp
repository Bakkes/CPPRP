#include <omp.h>
#include "ReplayFile.h"
#include <iostream>
#include "bench.h"
#include <thread>
#include <vector>
#include <mutex>
std::vector<std::string> failingReplays = {/* "6688", "a128", "c23b","d044", "d236", "f811" */};

std::unordered_map<std::string, std::string> replaysToTest =
{
	{"0008", "a flip time"}
  , {"000b", "nintendo switch"}
  , {"07e9", "a game mode before Neo Tokyo"}
  , {"0ad2", "some Latin-1 text"}
  , {"1205", "rumble mode"}
  , {"160c", "a dedicated server IP"}
  , {"16d5", "new property types"}
  , {"18d6", "an online loadout attribute"}
  , {"1a12", "overtime"}
  , {"1ae4", "a game time"}
  , {"1bc2", "no padding after the frames"}
  , {"1d1d", "a camera pitch"}
  , {"1ec9", "a V1.63 match"}
  , {"1ef9", "a private hoops match"}
  , {"1f37", "splitscreen players"}
  , {"2114", "a match save"}
  , {"2266", "dropshot"}
  , {"22ba", "a vote to forfeit"}
  , {"27b6", "some UTF-16 text"}
  , {"29f5", "frames"}
  , {"2cfe", "a new playstation id"}
  , {"3381", "patch 1.37"}
  , {"372d", "a camera yaw attribute"}
  , {"383e", "older unknown content field"}
  , {"387f", "a frozen attribute"}
  , {"3abd", "rlcs"}
  , {"3ea1", "a custom team name"}
  , {"4126", "a game mode after Neo Tokyo"}
  , {"419a", "a club match"}
  , {"42f0", "reservations after Neo Tokyo"}
  , {"4bc3", "with timed out attribute"}
  , {"504e", "some messages"}
  , {"520e", "no pickup attribute"}
  , {"524f", "quat edge case"}
  , {"52aa", "a match-ending attribute"}
  , {"540d", "a demolish attribute"}
  , {"551c", "private match settings"}
  , {"5a06", "esports items"}
  , {"6210", "different player history key"}
  , {"6320", "a forfeit attribute"}
  , {"6688", "a malformed byte property"}
  , {"6b0d", "patch 1.37"}
  , {"6d1b", "a flip right"}
  , {"6f7c", "a map with numbers"}
  , {"7083", "weird basketball capitalization"}
  , {"7109", "a boost modifier"}
  , {"7256", "special edition"}
  , {"75ce", "primary and secondary titles"}
  , {"7bf6", "an online loadouts attribute"}
  , {"89cb", "remote user data"}
  , {"8ae5", "new painted items"}
  , {"92a6", "with server performance state"}
  , {"946f", "patch 1.43"}
  , {"9704", "a batarang"}
  , {"98e5", "a player using behind view"}
  , {"9eaa", "newer replay without trailing bytes"}
  , {"a09e", "a tournament"}
  , {"a128", "a round count down"}
  , {"a52f", "some more mutators"}
  , {"a558", "extended explosion data"}
  , {"a671", "a waiting player"}
  , {"a676", "new user color"}
  , {"a7f0", "a ready attribute"}
  , {"a9df", "salty shores patch 1.45"}
  , {"aa70", "patch 1.50 - TitleID attribute"}
  , {"afb1", "patch 1.37"}
  , {"b9f9", "a party leader"}
  , {"c14f", "some mutators"}
  , {"c23b", "new psynet id"}
  , {"c837", "a spectator"}
  , {"cc4c", "after Starbase ARC"}
  , {"d044", "hoops mutators"}
  , {"d236", "rlcs s2"}
  , {"d428", "a private hockey match"}
  , {"d52e", "psynet system id"}
  , {"d7fb", "an explosion attribute"}
  , {"db70", "new lag indicator"}
  , {"dcb3", "a pawn type attribute"}
  , {"de56", "a problematic product attribute"}
  , {"e80d", "unlimited time"}
  , {"eae3", "an actor/object ID collision"}
  , {"eae8", "custom team colors"}
  , {"edbb", "remote role"}
  , {"f299", "a location attribute"}
  , {"f7b9", "a hockey game event"}
  , {"f811", "no frames"}
  , {"fdc7", "an MVP"}
};


int main()
{
	std::vector<std::filesystem::path> replays;
	for (const auto & entry : std::filesystem::directory_iterator("C:/Users/Chris/Documents/My Games/Rocket League/TAGame/Demos/"))
		replays.push_back(entry.path());

	uint32_t iterations = 0;
	{
		const char* name = "OldReplays test";
		double start_time = get_time();

		std::vector<std::thread> threads;
		for (auto replayName : replays)
		{
			//std::thread t{ [replayName]() {

			//auto replayData = replaysToTest[replayName];
			//printf("Parsing replay \"%s\"\n", replayName.filename().u8string().c_str());
				std::shared_ptr<ReplayFile> rf = std::make_shared<ReplayFile>(replayName);
				
				if (!rf->Load())
				{
					printf("Error loading replay file");
					return 0;
				}

				rf->DeserializeHeader();
				rf->FixParents();
				rf->Parse();
				//printf("Parsed replay \"%s\"\n", replayName.filename().u8string().c_str());
			//},  };
			//threads.emplace_back(std::move(t));
			//printf("Parsed\n\n");
		}

		for (auto & t : threads) 
		{
			iterations++;
			t.join();
		}
		double end_time = get_time();
		double elapsed = (end_time - start_time) * 1000.f;
		printf("[%s] Ran %i iterations in %.5f ms (avg: %.5f ms)\n", name, iterations, elapsed, (elapsed / (double)iterations));
	}
	/*std::shared_ptr<ReplayFile> rf = std::make_shared<ReplayFile>("1BE973D44E656FCC97DCD1A4E9076C36.replay");
	if (!rf->Load())
	{
		printf("Error loading replay file");
		return 0;
	}
	
	rf->DeserializeHeader();
	rf->FixParents();

	const auto& kf = rf->replayFile->keyframes;
	std::vector<uint32_t> positions;
	for (size_t i = 0; i < kf.size(); ++i)
	{
		positions.push_back(kf[i].filepos);
	}
	positions.push_back(rf->replayFile->netstream_size * 8);


	for (size_t i = 0; i < positions.size() - 1; ++i)
	{
		auto wot1 = positions[i];
		auto wot2 = positions[i+1];
		rf->Parse(wot1, wot2);
	}*/

	//{
//		const uint32_t iterations = 1000;
//		{
//			const char* name = "OMP test";
//			double start_time = get_time();
//			for (int aaaa = 0; aaaa < iterations; aaaa++) {
//				const size_t size = positions.size() - 1;
//#pragma omp parallel for
//				for (size_t i = 0; i < size; ++i)
//				{
//					//rf.Parse(positions[i], positions[i + 1]);
//				}
//			}
//			double end_time = get_time();
//			double elapsed = (end_time - start_time) * 1000.f;
//			printf("[%s] Ran %i iterations in %.5f ms (avg: %.5f ms)\n", name, iterations, elapsed, (elapsed / (double)iterations));
//		}
//
//		std::mutex parse_mutex;
//		{
//			const char* name = "threading test";
//			double start_time = get_time();
//			
//			//auto l = 
//
//			//for (int aaaa = 0; aaaa < iterations; aaaa++) 
//			//{
//			//	std::lock_guard<std::mutex> lock(parse_mutex);
//			//	const size_t size = positions.size() - 1;
//			//	std::vector<std::thread> threads;
//			//	for (size_t i = 0; i < size; ++i)
//			//	{
//			//		std::thread t{ [](std::shared_ptr<ReplayFile> rf, uint32_t start, uint32_t end) {
//			//			//rf->Parse(start, end);
//			//		}, rf, positions[i], positions[i + 1] };
//			//		threads.emplace_back(std::move(t));
//			//	}
//
//			//	for (auto & t : threads)
//			//	t.join();
//			//}
//			
//			double end_time = get_time();
//			double elapsed = (end_time - start_time) * 1000.f;
//			printf("[%s] Ran %i iterations in %.5f ms (avg: %.5f ms)\n", name, iterations, elapsed, (elapsed / (double)iterations));
//		}

	//	const uint32_t iterations = 1;
	//	{
	//		const char* name = "threading test2";
	//		double start_time = get_time();

	//		//auto l = 
	//		std::shared_ptr<ReplayFile> rf = std::make_shared<ReplayFile>("1BE973D44E656FCC97DCD1A4E9076C36.replay");
	//		if (!rf->Load())
	//		{
	//			printf("Error loading replay file");
	//			return 0;
	//		}

	//		rf->DeserializeHeader();
	//		rf->FixParents();
	//		std::vector<std::thread> threads;
	//		for (int aaaa = 0; aaaa < iterations; aaaa++)
	//		{
	//			
	//			std::thread t{ [&rf]() {
	//				
	//				rf->Parse();
	//			} };
	//			threads.emplace_back(std::move(t));
	//		}
	//		for (auto & t : threads)
	//			t.join();

	//		double end_time = get_time();
	//		double elapsed = (end_time - start_time) * 1000.f;
	//		printf("[%s] Ran %i iterations in %.5f ms (avg: %.5f ms)\n", name, iterations, elapsed, (elapsed / (double)iterations));
	//	}
	//}
	//BENCHMARK("KeyFrame parse", , 10);

	//BENCHMARK("Parse", rf.Parse(), 10);
	int o = 5;
	system("pause");
	return 0;
}