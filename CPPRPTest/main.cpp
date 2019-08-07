//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

//#include <omp.h>
#pragma comment(lib, "CPPRP.lib")
#include "../CPPRP/ReplayFile.h"
#include "../CPPRP/ReplayException.h"
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
#undef max



int main(int argc, char *argv[])
{
	std::queue<std::filesystem::path> replayFilesToLoad;
	{
		std::filesystem::path p(argv[1]);
		if (std::filesystem::is_regular_file(p))
		{
			replayFilesToLoad.push(p);
		}
		else 
		{
			for (const auto & entry : std::filesystem::recursive_directory_iterator(argv[1]))
			{
				if (entry.path().filename().u8string().find(".replay") == std::string::npos)
					continue;
				//if (replayFilesToLoad.size() >= 5)
				//	break;
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

	std::atomic<bool> allLoaded = false;
	std::queue<std::shared_ptr<CPPRP::ReplayFile>> replayFileQueue;

	auto parseReplay = [&allLoaded, &queueMutex, &replayFileQueue]() 
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
			}
			if (replayFile != nullptr)
			{
				
				try 
				{
					replayFile->DeserializeHeader();
					replayFile->Parse();
					//printf("Parsed\n");
				}
				catch (...) { printf("err\n"); }
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

	auto loadAndParseReplay = [&queueMutex, &replayFilesToLoad]()
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
				replayFile->DeserializeHeader();
				replayFile->Parse();
				//printf("Parsed\n");
			}
			catch (...) { printf("err\n"); }
			
		}
	};

	if constexpr (true)
	{
		constexpr size_t bothReplayThreadCount = 40;
		std::vector<std::thread> bothReplayThreads;
		for (size_t i = 0; i < bothReplayThreadCount; ++i)
		{
			std::thread bothReplayThread = std::thread{
				loadAndParseReplay
			};
			bothReplayThreads.emplace_back(std::move(bothReplayThread));
		}

		auto start = std::chrono::steady_clock::now();
		for (auto& t : bothReplayThreads)
		{
			t.join();
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
	} 
	else
	{
		constexpr size_t loadReplayThreadCount = 4;
		constexpr size_t parseReplayThreadCount = 0;
		std::vector<std::thread> loadReplayThreads;
		std::vector<std::thread> parseReplayThreads;
		for (size_t i = 0; i < loadReplayThreadCount; ++i)
		{
			std::thread loadReplayThread = std::thread{
				loadReplay
			};
			loadReplayThreads.emplace_back(std::move(loadReplayThread));
		}
		for (size_t i = 0; i < parseReplayThreadCount; i++)
		{
			std::thread parseReplayThread = std::thread{
				parseReplay
			};
			parseReplayThreads.emplace_back(std::move(parseReplayThread));
		}

		auto start = std::chrono::steady_clock::now();
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
			<< " µs" << std::endl;

		std::cout << "Elapsed time in milliseconds : "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
			<< " ms" << std::endl;
	}
	{
		
		/*printf("Test %s\n", name);
		printf("Attempted to parse %i replays in %.5f ms \n", success + fail + corrupt, elapsed);
		printf("Success: %i, fail: %i (%.2f%%) corrupt: %i Average parse time %.5f ms (totaltime/successfulparses)\n", (success.load()), fail.load(), ((double)success.load() / (double)((success.load()) + fail.load())) * 100, corrupt.load(), (elapsed / (double)success.load()));*/
	}
	
	system("pause");
	return 0;
}