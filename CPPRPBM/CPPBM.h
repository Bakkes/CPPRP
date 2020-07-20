#pragma once
#undef byte
#include "bakkesmod/plugin/bakkesmodplugin.h"
#pragma comment( lib, "bakkesmod.lib" )
#pragma comment(lib, "CPPRP.lib")
#include "../CPPRP/ReplayFile.h"
#include "../CPPRP/exceptions/ReplayException.h"

struct ReplicationData
{
	CPPRP::ReplicatedRBState rbState;
	int steer;
	bool handbrake;
	int throttle;
	float delta;

};

class CPPBM : public BakkesMod::Plugin::BakkesModPlugin
{
private:
	uint32_t currentFrameIngame = 0;
	uint32_t currentFrame = 0;
	std::chrono::time_point<std::chrono::system_clock> timeApplied;
	std::shared_ptr<CPPRP::ReplayFile> replayFile;
	std::map<uint32_t, std::map<uint32_t, ReplicationData>> locations;
public:
	virtual void onLoad();
	virtual void onUnload();
	void GVCTick(std::string name);
	void OnTick(CarWrapper cw, void * params, std::string funcName);
};

