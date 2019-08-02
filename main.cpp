#include <omp.h>
#include "ReplayFile.h"
#include <iostream>
#include "bench.h"
#include <thread>
#include <vector>
#include <mutex>
int main()
{
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

	{
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

		const uint32_t iterations = 1;
		{
			const char* name = "threading test2";
			double start_time = get_time();

			//auto l = 
			std::shared_ptr<ReplayFile> rf = std::make_shared<ReplayFile>("1BE973D44E656FCC97DCD1A4E9076C36.replay");
			if (!rf->Load())
			{
				printf("Error loading replay file");
				return 0;
			}

			rf->DeserializeHeader();
			rf->FixParents();
			std::vector<std::thread> threads;
			for (int aaaa = 0; aaaa < iterations; aaaa++)
			{
				
				std::thread t{ [&rf]() {
					
					rf->Parse();
				} };
				threads.emplace_back(std::move(t));
			}
			for (auto & t : threads)
				t.join();

			double end_time = get_time();
			double elapsed = (end_time - start_time) * 1000.f;
			printf("[%s] Ran %i iterations in %.5f ms (avg: %.5f ms)\n", name, iterations, elapsed, (elapsed / (double)iterations));
		}
	}
	//BENCHMARK("KeyFrame parse", , 10);

	//BENCHMARK("Parse", rf.Parse(), 10);
	int o = 5;
	system("pause");
	return 0;
}