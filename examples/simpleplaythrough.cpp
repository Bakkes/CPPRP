
#include "../CPPRP/ReplayFile.h"
#include "../CPPRP/ReplayException.h"
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <algorithm>
#include <map>
#include <filesystem>
#include <queue>
#include <unordered_map>
#undef max



int main(int argc, char *argv[])
{
    auto replayFile = std::make_shared<CPPRP::ReplayFile>("/windows2/alpaca/2F6924754EEC41B468694D9537E4C1D7.replay");
    replayFile->Load();
    replayFile->DeserializeHeader();
    for (auto it : replayFile->GetProperty<std::vector<std::unordered_map<std::string, std::shared_ptr<CPPRP::Property>>>>("PlayerStats"))
    {
        for (auto it2 : it)
        {
            //printf("%s\n", it2.first.c_str());
        }
    }

    std::vector<CPPRP::TAGame::Car_TA> cars;
    replayFile->createdCallbacks.push_back([&](const CPPRP::ActorStateData& asd)
    {
        //std::cout << "New actor created " << typeid(*asd.actorObject).name() << "\n";
    });

    std::map<uint32_t, std::unordered_map<uint32_t, CPPRP::Vector3>> locations;
    replayFile->tickables.push_back([&](const uint32_t frameNumber, const std::unordered_map<int, CPPRP::ActorStateData>& actorStats)
    {
        for (auto& actor : actorStats)
        {
            std::shared_ptr<CPPRP::TAGame::Car_TA> car = std::dynamic_pointer_cast<CPPRP::TAGame::Car_TA>(actor.second.actorObject);
            if (car)
            {
                //printf("%i\n", actor.first);
                locations[frameNumber][actor.first] = car->ReplicatedRBState.position;
            }
        }
    });

    replayFile->updatedCallbacks.push_back([&](const CPPRP::ActorStateData& actor, const std::vector<uint32_t>& updatedProperties)
    {
        
        std::shared_ptr<CPPRP::Engine::PlayerReplicationInfo> gameEvent = std::dynamic_pointer_cast<CPPRP::Engine::PlayerReplicationInfo>(actor.actorObject);
        if(gameEvent)
        {
            const uint32_t replicatedIndex = replayFile->objectToId["Engine.PlayerReplicationInfo:PlayerName"];
           // std::string wot = "";
            // for(auto prop : updatedProperties)
            // {
            //     wot += std::to_string(prop) + ", ";
            // }
            //printf("Test: (%i) -> (%s): %i - ", replicatedIndex, wot.c_str(), (int)(std::find(updatedProperties.begin(), updatedProperties.end(), replicatedIndex) != updatedProperties.end()));
            if((std::find(updatedProperties.begin(), updatedProperties.end(), replicatedIndex) != updatedProperties.end()))
            {
                printf("SkillTier %s\n", gameEvent->PlayerName.c_str());
                //printf("%s\n", gameEvent->DodgeTorque.ToString().c_str());
                //if(gameEvent->ReplicatedStatEvent.object_id >= 0)
                //printf("New winner %s\n", replayFile->replayFile->objects[gameEvent->ReplicatedStatEvent.object_id].c_str());
                //printf("New state name [%i] %s\n", gameEvent->ReplicatedStateName, replayFile->replayFile->names[gameEvent->ReplicatedStateName].c_str());
            }
        }
    });
    replayFile->PreprocessTables();
    replayFile->Parse();
    int fdfsd = 5;


	
	system("pause");
	return 0;
}