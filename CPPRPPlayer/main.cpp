#include "IReplayPlayer.h"
#include <string>
#include <queue>


class Timer
{
private:
	std::chrono::time_point<std::chrono::steady_clock> start;
	std::chrono::time_point<std::chrono::steady_clock> end;
	bool ended = false;
	std::string name;
public:
	Timer(std::string timerName) : name(timerName)
	{
		start = std::chrono::steady_clock::now();
	}

	void Stop()
	{
		end = std::chrono::steady_clock::now();
		ended = true;
	}

	~Timer()
	{
		if (!ended) Stop();
		std::cout << name << " duration in microseconds : "
			<< std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
			<< "\n";

		/*std::cout << "Elapsed time in milliseconds : "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
			<< " ms\n";*/
	}
};

struct FK
{
	float time;
	std::string name;
};

struct GT
{
	float speed;
	std::string name;
};
std::mutex kickoffMutex;
std::vector<FK> kickoffs;
std::vector<GT> fastgoals;
std::vector<GT> slowgoals;
std::mutex queueMutex;
//static std::string fastestKickoffFile = "";
//static float fastestKickoff = 10.f;

void AnalyzeReplay(std::filesystem::path& replayPath)
{
	
	using namespace CPPRP::ReplayPlaying;

	auto replayFile = std::make_shared<CPPRP::ReplayFile>(replayPath);
	replayFile->Load();
	replayFile->DeserializeHeader();
	replayFile->PreprocessTables();

	IReplayPlayer p(replayFile);

	replayFile->Parse();

	//float k = p.GetFastestKickoff();
	//float fastestGoal = p.GetFastestGoal();
	std::lock_guard<std::mutex> mtx(kickoffMutex);
	kickoffs.push_back({p.GetFastestKickoff(), replayPath.string()});
	fastgoals.push_back({ p.GetFastestGoal(), replayPath.string() });
	slowgoals.push_back({ p.GetSlowestGoal(), replayPath.string() });

	//if (k < fastestKickoff)
	//{
	//	fastestKickoff = k;
	//	fastestKickoffFile = replayPath.string();
	//}
	printf("Fastest goal that replay was: %.3f\n", p.GetFastestGoal());
}

int main(int argc, char* argv[])
{

	std::queue<std::filesystem::path> replayFilesToLoad;
	{
		//std::filesystem::path p("C:\\Users\\Bakkes\\Documents\\My Games\\Rocket League\\TAGame\\Demos\\");
		
		std::filesystem::path p;
		if (argc > 1)
			p = std::filesystem::path(argv[1]);
		else
			p = std::filesystem::path("C:\\Users\\Bakkes\\Downloads\\RLCS Season 9-20200723T230751Z-001\\RLCS Season 9\\");
		printf("Reading path %s\n", p.u8string().c_str());
		if (std::filesystem::is_regular_file(p))
		{
			replayFilesToLoad.push(p);
		}
		else
		{
			for (const auto& entry : std::filesystem::recursive_directory_iterator(p))
			{
				if (entry.path().filename().u8string().find(".replay") == std::string::npos)
					continue;
				if (replayFilesToLoad.size() >= 5335345)
					break;
				replayFilesToLoad.push(entry.path());
			}
		}
	}

	auto analyzeLambda = [&replayFilesToLoad]()
	{
		while (true)
		{
			std::filesystem::path replayF;
			{
				std::lock_guard<std::mutex> lockGuard(queueMutex);
				if (replayFilesToLoad.empty())
				{
					break;
				}
				replayF = replayFilesToLoad.front();
				replayFilesToLoad.pop();
			}
			
			try
			{
				AnalyzeReplay(replayF);
			}
			catch (...) {}
		}
	};

	{
		Timer t("Analyze");
		const int bothReplayThreadsCount = 8;
		if (bothReplayThreadsCount == 1)
		{
			analyzeLambda();
		}
		else
		{
			std::vector<std::thread> bothReplayThreads;
			for (size_t i = 0; i < bothReplayThreadsCount; ++i)
			{
				std::thread bothReplayThread = std::thread{
					analyzeLambda
				};
				bothReplayThreads.emplace_back(std::move(bothReplayThread));
			}


			for (auto& t : bothReplayThreads)
			{
				t.join();
			}
		}
	}
	printf("Kickoffs\n");
	std::sort(kickoffs.begin(), kickoffs.end(), [](auto lhs, auto rhs) { return lhs.time < rhs.time; });
	for (int i = 0; i < 5; ++i)
	{
		printf("%s: %.4f\n", kickoffs.at(i).name.c_str(), kickoffs.at(i).time);
	}
	//printf("%s: %.4f", fastestKickoffFile.c_str(), fastestKickoff);
	printf("\n\nFastest goals\n");
	std::sort(fastgoals.begin(), fastgoals.end(), [](auto lhs, auto rhs) { return lhs.speed > rhs.speed; });
	for (int i = 0; i < 5; ++i)
	{
		printf("%s: %.4f km/h\n", fastgoals.at(i).name.c_str(), fastgoals.at(i).speed);
	}

	printf("\n\nSlowest goals\n");
	std::sort(slowgoals.begin(), slowgoals.end(), [](auto lhs, auto rhs) { return lhs.speed < rhs.speed; });
	for (int i = 0; i < 5; ++i)
	{
		printf("%s: %.4f km/h\n", slowgoals.at(i).name.c_str(), slowgoals.at(i).speed);
	}


	return 0;
}