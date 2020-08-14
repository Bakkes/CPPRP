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
std::vector<FK> kickoffs;

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

	float k = p.GetFastestKickoff();
	kickoffs.push_back({k, replayPath.string()});
	//if (k < fastestKickoff)
	//{
	//	fastestKickoff = k;
	//	fastestKickoffFile = replayPath.string();
	//}
	//printf("Fastest kickoff that replay was: %.3f\n", k);
}

int main()
{

	std::queue<std::filesystem::path> replayFilesToLoad;
	{
		//std::filesystem::path p("C:\\Users\\Bakkes\\Documents\\My Games\\Rocket League\\TAGame\\Demos\\");
		std::filesystem::path p("C:\\Users\\Bakkes\\Downloads\\RLCS Season 9-20200723T230751Z-001\\RLCS Season 9\\");
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
	Timer t("Analyze");
	while (!replayFilesToLoad.empty())
	{
		try
		{
			AnalyzeReplay(replayFilesToLoad.front());
		}
		catch (...) {}
		replayFilesToLoad.pop();
	}
	std::sort(kickoffs.begin(), kickoffs.end(), [](auto lhs, auto rhs) { return lhs.time < rhs.time; });
	for (int i = 0; i < 5; ++i)
	{
		printf("%s: %.4f\n", kickoffs.at(i).name.c_str(), kickoffs.at(i).time);
	}
	//printf("%s: %.4f", fastestKickoffFile.c_str(), fastestKickoff);
	

	return 0;
}