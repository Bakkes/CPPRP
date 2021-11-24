//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

//#include <omp.h>
#pragma comment(lib, "CPPRP.lib")
#include "../CPPRP/ReplayFile.h"
#include "../CPPRP/exceptions/ReplayException.h"
#include <iostream>
#include "bench.h"
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <algorithm>
#include <map>
#include <filesystem>
#include <queue>
#include <unordered_map>
#include "../CPPRPJSON/OptionsParser.h"
#undef max



int main(int argc, char *argv[])
{
	if constexpr (true) {
		auto replayFile = std::make_shared<CPPRP::ReplayFile>(R"(C:\Users\m4rti\Documents\My Games\Rocket League\TAGame\Demos\48238C814A7BF02F8A219BB9C77C2F6E.replay)");
		replayFile->Load();
		replayFile->DeserializeHeader();
		for (auto it : replayFile->GetProperty<std::vector<std::unordered_map<std::string, std::shared_ptr<CPPRP::Property>>>>("PlayerStats"))
		{
			for (auto it2 : it)
			{
				printf("%s\n", it2.first.c_str());
			}
		}
		std::map<uint32_t, std::unordered_map<uint32_t, uint32_t>> scores;
		struct TestData
		{
			CPPRP::OnlineID id;
			uint32_t match_Score;
		};
		std::map<uint32_t, CPPRP::TAGame::PRI_TA> pris;
		replayFile->updatedCallbacks.push_back([&](const CPPRP::ActorStateData& asd, const std::vector<uint32_t>& props)
			{
				if (auto pri = std::dynamic_pointer_cast<CPPRP::TAGame::PRI_TA>(asd.actorObject))
				{
					pris[asd.actorId] = *pri;
				}
			});
		replayFile->tickables.push_back([&](const CPPRP::Frame frame, const std::unordered_map<uint32_t, CPPRP::ActorStateData>& actorStats)
		{
			for (auto& actor : actorStats)
			{
				auto pri = std::dynamic_pointer_cast<CPPRP::TAGame::PRI_TA>(actor.second.actorObject);
				if (pri)
				{
					scores[frame.frameNumber][actor.first] = pri->MatchScore;
				}
			}
		});
		replayFile->Parse();
		auto replay_name = replayFile->GetProperty<std::string>("ReplayName");
		auto replay_date = replayFile->GetProperty<std::string>("Date");
		auto replay_match_type = replayFile->GetProperty<std::string>("MatchType");
		auto replay_id = replayFile->GetProperty<std::string>("Id");
		auto replay_team_size = replayFile->GetProperty<int>("TeamSize");
		auto replay_map_name = replayFile->GetProperty<std::string>("MapName");
		for(auto& [id, pri]: pris)
		{
			if (pri.Team.active)
			{
				const auto team_object = replayFile->actorStates[pri.Team.actor_id];
				const auto team_archetype = replayFile->replayFile->names[team_object.typeId];
			}
		}
		int fdfsd = 5;
		return 0;
	}
	//printf("hi");
	std::queue<std::filesystem::path> replayFilesToLoad;
	{
		std::filesystem::path p(argv[1]);
		if (std::filesystem::is_regular_file(p))
		{
			if (argc > 2)
			{
				int amnt = std::stoi(argv[2]);
				for(int i = 0; i < amnt; ++i)
					replayFilesToLoad.push(p);
			}
			else
			{
				replayFilesToLoad.push(p);
			}
		}
		else 
		{
			for (const auto & entry : std::filesystem::recursive_directory_iterator(argv[1]))
			{
				if (entry.path().filename().u8string().find(".replay") == std::string::npos)
					continue;
				if (replayFilesToLoad.size() >= 5335345)
					break;
				replayFilesToLoad.push(entry.path());
			}
		}
	}


	
	static const size_t numReplays = replayFilesToLoad.size();
	printf("Attempt to parse %i replays\n", numReplays);
	std::atomic<uint32_t> success = 0;
	std::atomic<uint32_t> fail = 0;
	std::atomic<uint32_t> corrupt = 0;

	std::mutex queueMutex;
	std::mutex filesMutex;

	//td::atomic<uint32_t> 
	std::atomic<bool> allLoaded = false;
	std::queue<std::shared_ptr<CPPRP::ReplayFile>> replayFileQueue;

	auto parseReplay = [&success, &fail, &allLoaded, &queueMutex, &replayFileQueue]() 
	{
		while (true)
		{
			std::shared_ptr<CPPRP::ReplayFile> replayFile{ nullptr };
			{
				std::lock_guard<std::mutex> lockGuard(queueMutex);
				if (!replayFileQueue.empty())
				{
					replayFile = replayFileQueue.front();
					replayFileQueue.pop();
				}
				else if (allLoaded)
				{
					break;
				}
				else
				{
					std::this_thread::sleep_for(std::chrono::microseconds(100));
				}
				
			}
			if (replayFile != nullptr)
			{
				
				try 
				{
					replayFile->VerifyCRC(CPPRP::CRC_Both);
					replayFile->DeserializeHeader();
					replayFile->Parse();
					success++;
					
					//printf("Parsed\n");
				}
				catch (...) { 
					fail++;
					printf("[%i/%i] %s\n", fail.load(), success.load() + fail.load(), replayFile->path.filename().u8string().c_str()); 
				}
			}
		}
	};

	auto loadReplay = [&filesMutex, &queueMutex, &replayFilesToLoad, &replayFileQueue]()
	{
		while (true)
		{
			std::filesystem::path replayName;
			{
				std::lock_guard<std::mutex> fileLockGuard(filesMutex);
				if (!replayFilesToLoad.empty())
				{
					replayName = replayFilesToLoad.front();
					replayFilesToLoad.pop();
				}
				else
				{
					break;
				}
			}

			std::shared_ptr<CPPRP::ReplayFile> rf = std::make_shared<CPPRP::ReplayFile>(replayName);
			if (rf->Load())
			{
				std::lock_guard<std::mutex> lockGuard(queueMutex);
				replayFileQueue.push(std::move(rf));
			}
			else
			{
				printf("Failed loading replay file\n");
			}
		}
	};
	OptionsParser op(argc, argv);
	auto loadAndParseReplay = [&success, &queueMutex, &replayFilesToLoad]()
	{
		while (true)
		{
			std::filesystem::path replayName;
			{
				std::lock_guard<std::mutex> lockGuard(queueMutex);
				if (!replayFilesToLoad.empty())
				{
					replayName = replayFilesToLoad.front();
					replayFilesToLoad.pop();
				}
				else
				{
					break;
				}
			}

			try
			{
				std::shared_ptr<CPPRP::ReplayFile> replayFile = std::make_shared<CPPRP::ReplayFile>(replayName);
				replayFile->Load();
				//replayFile->VerifyCRC(CPPRP::CRC_Both);
				replayFile->DeserializeHeader();
				
				replayFile->Parse();

				// struct t
				// {
				// 	uint32_t filepos;
				// 	uint32_t framenumber;
				// };

				// const auto& kf = replayFile->replayFile->keyframes;
				// std::vector<t> positions;

				
				// for (size_t i = 0; i < kf.size(); ++i)
				// {
				// 	positions.push_back({ kf[i].filepos, kf[i].frame });
				// }
				// t ow = { replayFile->replayFile->netstream_size * 8, static_cast<uint32_t>(replayFile->GetProperty<int32_t>("NumFrames")) };
				// positions.push_back(ow);


				// const size_t posCount = positions.size() - 1;
				
				// for (size_t i = 0; i < posCount; ++i)
				// {
				// 	auto wot1 = positions[i];
				// 	auto wot2 = positions[i + 1];
				// 	uint32_t frameCount = wot2.framenumber - wot1.framenumber;

				// 	replayFile->Parse(wot1.filepos, wot2.filepos, frameCount);
				// }

				
				success++;
				//printf("Parsed\n");
			}
			catch (...) { 
				printf("err\n"); 
			}
			
		}
	};

	const int bothReplayThreadsCount = op.GetIntValue({"both"}, 1);
	const int loadThreads = op.GetIntValue({"loads"}, 0);
	const int parseThreads = op.GetIntValue({"parses"}, 0);
	if (loadThreads == 0 || parseThreads == 0)
	{
		
		//constexpr size_t bothReplayThreadCount = 50;
		printf("Loading&parsing using %i thread(s)\n", bothReplayThreadsCount);
		auto start = std::chrono::steady_clock::now();
		if (bothReplayThreadsCount == 1)
		{
			loadAndParseReplay();
		}
		else
		{
			std::vector<std::thread> bothReplayThreads;
			for (size_t i = 0; i < bothReplayThreadsCount; ++i)
			{
				std::thread bothReplayThread = std::thread{
					loadAndParseReplay
				};
				bothReplayThreads.emplace_back(std::move(bothReplayThread));
			}

			
			for (auto& t : bothReplayThreads)
			{
				t.join();
			}
		}
		
		auto end = std::chrono::steady_clock::now();
		printf("Done parsing\n");
		std::cout << "Elapsed time in nanoseconds : "
			<< std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
			<< " ns" << std::endl;

		std::cout << "Elapsed time in microseconds : "
			<< std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
			<< " µs" << std::endl;

		std::cout << "Elapsed time in milliseconds : "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
			<< " ms" << std::endl;
			float totalMs = (float)std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		std::cout << "Average " << (totalMs/success) << std::endl;
	} 
	else
	{
		printf("Loading threads: %i. Parsing using %i thread(s)\n", loadThreads, parseThreads);
		auto start = std::chrono::steady_clock::now();
		//constexpr size_t loadReplayThreadCount = 4;
		//constexpr size_t parseReplayThreadCount = 6;
		std::vector<std::thread> loadReplayThreads;
		std::vector<std::thread> parseReplayThreads;
		for (size_t i = 0; i < loadThreads; ++i)
		{
			std::thread loadReplayThread = std::thread{
				loadReplay
			};
			loadReplayThreads.emplace_back(std::move(loadReplayThread));
		}
		for (size_t i = 0; i < parseThreads; i++)
		{
			std::thread parseReplayThread = std::thread{
				parseReplay
			};
			parseReplayThreads.emplace_back(std::move(parseReplayThread));
		}

		
		for (auto& t : loadReplayThreads)
		{
			t.join();
		}
		printf("Loaded all files, waiting for parse to end\n");
		allLoaded = true;
		for (auto& t : parseReplayThreads)
		{
			t.join();
		}
		printf("Done parsing\n");
		auto end = std::chrono::steady_clock::now();
		std::cout << "Elapsed time in nanoseconds : "
			<< std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
			<< " ns" << std::endl;

		std::cout << "Elapsed time in microseconds : "
			<< std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
			<< " �s" << std::endl;

		std::cout << "Elapsed time in milliseconds : "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
			<< " ms" << std::endl;
		float totalMs = (float)std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		std::cout << "Average " << (totalMs/success) << std::endl;
		std::cout << "Success: " << success.load() << ", fail: " << fail.load() << std::endl;
	}
	{
		
		/*printf("Test %s\n", name);
		printf("Attempted to parse %i replays in %.5f ms \n", success + fail + corrupt, elapsed);
		printf("Success: %i, fail: %i (%.2f%%) corrupt: %i Average parse time %.5f ms (totaltime/successfulparses)\n", (success.load()), fail.load(), ((double)success.load() / (double)((success.load()) + fail.load())) * 100, corrupt.load(), (elapsed / (double)success.load()));*/
	}
	
	system("pause");
	return 0;
}