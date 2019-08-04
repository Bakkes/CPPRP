#include <omp.h>
#include "ReplayFile.h"
#include <iostream>
#include "bench.h"
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
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
	//C:/Users/Chris/Documents/My Games/Rocket League/TAGame/Demos/
	//"C:/Users/Chris/Documents/My Games/Rocket League/TAGame/Demos_freeplay/"
	//F:/alpaca/
	//Q:/rocketleaguereplays.com/replay_files/
	{
		int i = 0;
		for (const auto & entry : std::filesystem::directory_iterator("Q:/rocketleaguereplays.com/replay_files/"))
		{
			if (i > 501)
				break;
			i++;
			replays.push_back(entry.path());
		}
	}
	//replays.clear();
	//replays.push_back("C:/Users/Chris/source/repos/CPPRP/CPPRP/replays/d236.replay");

	//const size_t numReplays = replays.size();
	//printf("Attempt to parse %i replays\n", numReplays);
	//uint32_t success = 0;
	//uint32_t fail = 0;
	//{
	//	const char* name = "Alpaca replays";
	//	double start_time = get_time();

	//	std::vector<std::thread> threads;

	//	uint32_t threads_active = 0;
	//	for (auto replayName : replays)
	//	{

	//		//while(threads_active > 400)
	//		//	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	//		threads_active++;
	//		//std::thread t{ [replayName, &success, &fail, &threads_active]() 
	//		{

	//			//auto replayData = replaysToTest[replayName];
	//				//printf("Parsing replay \"%s\"\n", replayName.filename().u8string().c_str());
	//			
	//			try
	//			{
	//				std::shared_ptr<ReplayFile> rf = std::make_shared<ReplayFile>(replayName);
	//				std::string s = replayName.filename().u8string();
	//				//printf("Parsing replay \"%s\"\n", replayName.filename().u8string().c_str());
	//				//printf("Calling load\n");
	//				if (!rf->Load())
	//				{
	//					printf("Error loading replay file");
	//					throw;
	//				}
	//				else {
	//					//printf("Called load");
	//					rf->DeserializeHeader();
	//					rf->FixParents();
	//					struct t
	//					{
	//						uint32_t filepos;
	//						uint32_t framenumber;
	//					};
	//					const auto& kf = rf->replayFile->keyframes;
	//					std::vector<t> positions;
	//					for (size_t i = 0; i < kf.size(); ++i)
	//					{
	//						positions.push_back({ kf[i].filepos, kf[i].frame });
	//					}
	//					positions.push_back({ rf->replayFile->netstream_size * 8, static_cast<uint32_t>(rf->GetProperty<int32_t>("NumFrames")) });
	//					std::vector<std::thread> keyframeThreads;

	//					const size_t posCount = positions.size() - 1;
	//					for (size_t i = 0; i < posCount; ++i)
	//					{
	//						std::thread keyframeThread{ [&positions, i, &rf, s]() {
	//							auto wot1 = positions[i];
	//							auto wot2 = positions[i + 1];
	//							uint32_t frameCount = wot2.framenumber - wot1.framenumber;
	//							if (frameCount < 5)
	//							{
	//								printf("Err\n");
	//							}
	//							rf->Parse(s, wot1.filepos, wot2.filepos, frameCount);
	//						} };
	//						keyframeThreads.emplace_back(std::move(keyframeThread));
	//					}
	//					for (auto & t : keyframeThreads)
	//					{
	//						t.join();
	//					}

	//					//rf->Parse(s);
	//					success++;
	//				}
	//			}
	//			catch (const std::exception &) //e
	//			{
	//				fail++;
	//				//printf("Failed to parse %s (%s)'\n", replayName.filename().u8string().c_str(), e.what());
	//				//std::filesystem::copy(replayName, "F:/alpacafails/");
	//			}
	//			catch (...)
	//			{
	//				fail++;
	//				//printf("Failed to parse %s '\n", replayName.filename().u8string().c_str() );
	//				//std::filesystem::copy(replayName, "F:/alpacafails/");
	//			}
	//			threads_active--;

	//			//printf("Parsed replay \"%s\"\n", replayName.filename().u8string().c_str());
	//			//system("Pause");
	//		} 
	//	//}; threads.emplace_back(std::move(t));

	//		//printf("Parsed\n\n");
	//	}

	//	for (auto & t : threads)
	//	{
	//		//	//iterations++;
	//			//const size_t total = success + fail;
	//			//if (total % 500 == 0)
	//			//{
	//			//	
	//			//	printf("%i/%i (%.2f%%)\n", total, numReplays, ((double)total / (double)(numReplays)) * 100);
	//			//}
	//		t.join();
	//	}
	//	double end_time = get_time();
	//	double elapsed = (end_time - start_time) * 1000.f;
	//	printf("Test %s\n", name);
	//	printf("Attempted to parse %i replays in %.5f ms \n", success + fail, elapsed);
	//	printf("Success: %i, fail: %i (%.2f%%). Average parse time %.5f ms (totaltime/attemptedparsecount)\n", success, fail, ((double)success / (double)(success + fail)) * 100, (elapsed / (double)success));
	//}



	static const size_t numReplays = replays.size();
	printf("Attempt to parse %i replays\n", numReplays);
	std::atomic<uint32_t> success = 0;
	std::atomic<uint32_t> fail = 0;
	{
		const char* name = "Rocketleaguereplays.com replays";
		double start_time = get_time();

		std::vector<std::thread> threads;

		std::atomic<uint32_t> threads_active = 0;
		for (auto replayName : replays)
		{
			
			while(threads_active > 400)
				std::this_thread::sleep_for(std::chrono::milliseconds(3));
			threads_active++;
			//std::thread t{ [replayName, &success, &fail, &threads_active]() {

			//auto replayData = replaysToTest[replayName];
				//printf("Parsing replay \"%s\"\n", replayName.filename().u8string().c_str());
			std::shared_ptr<ReplayFile> rf = std::make_shared<ReplayFile>(replayName);
			try 
			{
				
				std::string s = replayName.filename().u8string();
				//printf("Parsing replay \"%s\"\n", replayName.filename().u8string().c_str());
				//printf("Calling load\n");
				if (!rf->Load())
				{
					printf("Error loading replay file");
					throw;
				}
				else {
					//printf("Called load");
					rf->DeserializeHeader();
					rf->FixParents();
					rf->Parse(s);
					success++;
				}
			}
			catch (const AttributeParseException<uint32_t>& e)
			{
				fail++;
				printf("Failed to parse %s (%s)'\n", replayName.filename().u8string().c_str(), e.what());
				for (size_t i = rf->parseLog.size() - 8; i < rf->parseLog.size(); ++i)
				{
					printf("\t%s\n", rf->parseLog[i].c_str());
				}
			}
			catch (const GeneralParseException<uint32_t>& e)
			{
				fail++;
				printf("Failed to parse %s (%s)'\n", replayName.filename().u8string().c_str(), e.what());
				for (size_t i = rf->parseLog.size() - 8; i < rf->parseLog.size(); ++i)
				{
					printf("\t%s\n", rf->parseLog[i].c_str());
				}
			}
			catch(const std::exception &e) //e
			{
				fail++;
				printf("Failed to parse %s (%s)'\n", replayName.filename().u8string().c_str(), e.what());
				//std::filesystem::copy(replayName, "F:/alpacafails/");
			}
			catch (...)
			{
				fail++;
				printf("Failed to parse %s '\n", replayName.filename().u8string().c_str() );
				//std::filesystem::copy(replayName, "F:/alpacafails/");
			}

			const size_t total = success + fail;
			if (total % 500 == 0)
			{
	
				printf("%i/%i (%.2f%%) status: ", total, numReplays, ((double)total / (double)(numReplays)) * 100);
				printf("success: %i, fail: %i (%.2f%%). \n", (success.load()), fail.load(), ((double)(success.load()) / (double)((success.load()) + fail.load())) * 100);
			}
			threads_active--;
			
				//printf("Parsed replay \"%s\"\n", replayName.filename().u8string().c_str());
				//system("Pause");
			//}  };
			//threads.emplace_back(std::move(t));
			//printf("Parsed\n\n");
		}

		for (auto & t : threads) 
		{
		//	//iterations++;
			//const size_t total = success + fail;
			//if (total % 500 == 0)
			//{
			//	
			//	printf("%i/%i (%.2f%%)\n", total, numReplays, ((double)total / (double)(numReplays)) * 100);
			//}
			t.join();
		}
		double end_time = get_time();
		double elapsed = (end_time - start_time) * 1000.f;
		printf("Test %s\n", name);
		printf("Attempted to parse %i replays in %.5f ms \n", success + fail, elapsed);
		printf("Success: %i, fail: %i (%.2f%%). Average parse time %.5f ms (totaltime/attemptedparsecount)\n", (success.load()), fail.load(), ((double)success.load() / (double)((success.load()) + fail.load())) * 100, (elapsed / (double)success.load()));
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
	system("pause");
	return 0;
}