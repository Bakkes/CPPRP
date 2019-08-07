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
#undef max
std::vector<std::string> failingReplays = {/* "6688", "a128", "c23b","d044", "d236", "f811" */};

int main(int argc, char *argv[])
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	std::vector<std::filesystem::path> replays;
	//C:/Users/Chris/Documents/My Games/Rocket League/TAGame/Demos/
	//"C:/Users/Chris/Documents/My Games/Rocket League/TAGame/Demos_freeplay/"
	//F:/alpaca/
	//Q:/rocketleaguereplays.com/replay_files/
	{
		//21AFD0E344271B02DC9A38BD633727F7.replay
		std::filesystem::path p(argv[1]);
		if (std::filesystem::is_regular_file(p))
		{
			replays.push_back(p);
		}
		else 
		{
			for (const auto & entry : std::filesystem::recursive_directory_iterator(argv[1]))
			{
				if (entry.path().filename().u8string().find(".replay") == std::string::npos)
					continue;
				//if (replays.size() >= 1)
				//	break;
				replays.push_back(entry.path());
			}
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


	std::mutex activeThreadsMutex;
	std::map<uint32_t, bool> activeThreads;

	std::mutex errorLogMutex;
	static const size_t numReplays = replays.size();
	printf("Attempt to parse %i replays\n", numReplays);
	std::atomic<uint32_t> success = 0;
	std::atomic<uint32_t> fail = 0;
	std::atomic<uint32_t> corrupt = 0;
	std::atomic<uint32_t> current = 0;
	{
		const char* name = "Alpaca replays";
		double start_time = get_time();

		std::vector<std::thread> threads;

		std::atomic<uint32_t> threads_active = 0;
		for (auto replayName : replays)
		{

			while (threads_active > 400)
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			//printf("Active: %i\n", threads_active.load());
			threads_active++;
			uint32_t curLoaded = 0;
			{
				std::lock_guard<std::mutex> lock(activeThreadsMutex);
				uint32_t u = current.load();
				activeThreads[u] = true;
				curLoaded = current.load();
				current++;
				//printf("Loaded %i\n", curLoaded);
			}
			std::thread t{ [replayName, &success, &fail, &corrupt, &threads_active, &errorLogMutex, cur = curLoaded, &activeThreads, &activeThreadsMutex]() {

				//auto replayData = replaysToTest[replayName];
					//printf("Parsing replay \"%s\"\n", replayName.filename().u8string().c_str());
				std::shared_ptr<CPPRP::ReplayFile> rf = std::make_shared<CPPRP::ReplayFile>(replayName);
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
						//bool b = rf->VerifyCRC(CPPRP::CRC_Both);
						//printf("Called load");
						rf->DeserializeHeader();

						rf->Parse();
						success++;
						if (rf->GetProperty<std::string>("MatchType").compare("Training") == 0)
						{
							printf("Successfully parsed training replay!\n");
						}
					}
				}
				catch (const CPPRP::InvalidVersionException&)
				{
					corrupt++;
					std::lock_guard<std::mutex> lock(errorLogMutex);
					//printf("InvalidVersion: %s\n", e.what());
				}
				catch (const CPPRP::PropertyDoesNotExistException& e)
				{
					corrupt++;
					std::lock_guard<std::mutex> lock(errorLogMutex);
					printf("PropertyDoesNotExistException: %s\n", e.what());
				}
				catch (const CPPRP::AttributeParseException<uint32_t>& e)
				{
					fail++;
#ifdef _WIN32
					//if (replayName.parent_path().compare("F:/failed/") != 0)
					//std::filesystem::copy_file(replayName, "F:/failed/", std::filesystem::copy_options::overwrite_existing);
#endif
					std::lock_guard<std::mutex> lock(errorLogMutex);
					bool hasProp = rf->HasProperty("MatchType");
					printf("Failed to parse %s replay %s (%s)'\n", !hasProp ? "unknown" : rf->GetProperty<std::string>("MatchType").c_str(), replayName.filename().u8string().c_str(), e.what());
					if (rf->parseLog.size() == 0)
					{
						//printf("No parseLog available, recompile with parselog info\n");
					}
					else
					{
						for (size_t i = std::max(0, (int32_t)rf->parseLog.size() - 8); i < rf->parseLog.size(); ++i)
						{
							printf("\t%s\n", rf->parseLog[i].c_str());
						}
					}
				}
				catch (const CPPRP::GeneralParseException<uint32_t>& e)
				{
					fail++;
#ifdef _WIN32
					//if (replayName.parent_path().compare("F:/failed/") != 0)
					//std::filesystem::copy_file(replayName, "F:/failed/", std::filesystem::copy_options::overwrite_existing);
#endif
					std::lock_guard<std::mutex> lock(errorLogMutex);

					bool hasProp = rf->HasProperty("MatchType");

					printf("Failed to parse %s replay %s (%s)'\n", !hasProp ? "unknown" : rf->GetProperty<std::string>("MatchType").c_str(), replayName.filename().u8string().c_str(), e.what());
					if (rf->parseLog.size() == 0)
					{
						//printf("No parseLog available, recompile with parselog info\n");
					}
					else
					{
						for (size_t i = std::max(0, (int32_t)rf->parseLog.size() - 8); i < rf->parseLog.size(); ++i)
						{
							printf("\t%s\n", rf->parseLog[i].c_str());
						}
					}
				}
				catch (const std::exception &e) //e
				{
					fail++;
#ifdef _WIN32
					//if (replayName.parent_path().compare("F:/failed/") != 0)
					//std::filesystem::copy_file(replayName, "F:/failed/", std::filesystem::copy_options::overwrite_existing);
#endif
					std::lock_guard<std::mutex> lock(errorLogMutex);
					bool hasProp = rf->HasProperty("MatchType");

					printf("Failed to parse %s replay %s (%s)'\n", !hasProp ? "unknown" : rf->GetProperty<std::string>("MatchType").c_str(), replayName.filename().u8string().c_str(), e.what());
					if (rf->parseLog.size() == 0)
					{
						//printf("No parseLog available, recompile with parselog info\n");
					}
					else
					{
						for (size_t i = std::max(0, (int32_t)rf->parseLog.size() - 8); i < rf->parseLog.size(); ++i)
						{
							printf("\t%s\n", rf->parseLog[i].c_str());
						}
					}
				}
				catch (...)
				{
					fail++;
#ifdef _WIN32
					//if (replayName.parent_path().compare("F:/failed/") != 0)
					//std::filesystem::copy_file(replayName, "F:/failed/", std::filesystem::copy_options::overwrite_existing);
#endif
					std::lock_guard<std::mutex> lock(errorLogMutex);
					bool hasProp = rf->HasProperty("MatchType");

					printf("Failed to parse %s replay %s (%s)'\n", !hasProp ? "unknown" : rf->GetProperty<std::string>("MatchType").c_str(), replayName.filename().u8string().c_str(), "");
					if (rf->parseLog.size() == 0)
					{
						//printf("No parseLog available, recompile with parselog info\n");
					}
					else
					{
						for (size_t i = std::max(0, (int32_t)rf->parseLog.size() - 8); i < rf->parseLog.size(); ++i)
						{
							printf("\t%s\n", rf->parseLog[i].c_str());
						}
					}
				}
				{
					//printf("AA%i\n", cur);
					std::lock_guard<std::mutex> lock(activeThreadsMutex);
					activeThreads[cur] = false;
					//printf("set %i\n", activeThreads[cur]);
				}
				const size_t total = success + fail;
				if (total % 500 == 0)
				{
					std::lock_guard<std::mutex> lock(activeThreadsMutex);
					printf("%i/%i (%.2f%%) (%i corrupt) status: ", total, numReplays, (double)total / (double)((numReplays - corrupt.load())) * 100, corrupt.load());
					printf("success: %i, fail: %i (%.2f%%). \n", (success.load()), fail.load(), ((double)(success.load()) / (double)((success.load()) + fail.load())) * 100);
					/*for (unsigned int i = 0; i < total; i++)
					{
						if (activeThreads[i])
						{
							printf("Lowest active is %i\n", i);
							break;
						}
					}*/
				}
				threads_active--;

				//printf("Parsed replay \"%s\"\n", replayName.filename().u8string().c_str());
				//system("Pause");
			} };
			threads.emplace_back(std::move(t));
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
		printf("Attempted to parse %i replays in %.5f ms \n", success + fail + corrupt, elapsed);
		printf("Success: %i, fail: %i (%.2f%%) corrupt: %i Average parse time %.5f ms (totaltime/successfulparses)\n", (success.load()), fail.load(), ((double)success.load() / (double)((success.load()) + fail.load())) * 100, corrupt.load(), (elapsed / (double)success.load()));
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

	/*std::shared_ptr<CPPRP::ReplayFile> rf = std::make_shared<CPPRP::ReplayFile>("3381.replay");
	rf->Load();
	rf->DeserializeHeader();
	BENCHMARK("Parse", rf->Parse(), 100);*/
	system("pause");
	return 0;
}