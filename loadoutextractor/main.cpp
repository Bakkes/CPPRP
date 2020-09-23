
#pragma comment(lib, "CPPRP.lib")
#include "../CPPRP/ReplayFile.h"
#include "../CPPRP/exceptions/ReplayException.h"
#include <string>
#include "bmloadout.h"
#include <iostream>
#include "helper_classes.h"
#include <map>
#include "bmloadout.h"
#include "bakkesmodloadoutlib.h"
#include "../CPPRP/data/NetworkData.h"
std::map<int, std::string> actorNameMap;
std::map<int, BMLoadout> processed;
std::map<int, CPPRP::CameraSettings> cameras;
//std::map<int, int> cameraSettingsMapping;

// void on_playername_set(Frame* frame, ActorState* actor, const char* playername)
// {
// 	//printf("Player %i got name %s\n", actor->actor_id, playername);
// 	actorNameMap[actor->actor_id] = std::string(playername);
// }

// void on_camerasettings_map(Frame* frame, ActorState* actor, ActiveActor* aactor)
// {
// 	//printf("Player %i\n", aactor->actor_id);
// 	//int w = 5;
// 	cameraSettingsMapping[aactor->actor_id] = actor->actor_id;
// }

// void SetCameraSettings(Frame* frame, ActorState* actor, CameraSettings* cameraSettings)
// {
// 	//printf("Player %i got name %s\n", actor->actor_id, playername);
// 	cameras[actor->actor_id] = *cameraSettings;
// }

void SetClientLoadouts(const CPPRP::ActorStateData& actor, const CPPRP::ClientLoadouts& loadouts)
{

	BMLoadout customLoadout = processed[actor.actorId];
	customLoadout.body.blue_is_orange = false; //Two seperate loadouts

	customLoadout.body.blue_loadout[SLOT_BODY].product_id = (uint16_t)loadouts.loadout_one.body;
	customLoadout.body.blue_loadout[SLOT_SKIN].product_id = (uint16_t)loadouts.loadout_one.skin;
	customLoadout.body.blue_loadout[SLOT_WHEELS].product_id = (uint16_t)loadouts.loadout_one.wheels;
	customLoadout.body.blue_loadout[SLOT_BOOST].product_id = (uint16_t)loadouts.loadout_one.boost;
	customLoadout.body.blue_loadout[SLOT_ANTENNA].product_id = (uint16_t)loadouts.loadout_one.antenna;
	customLoadout.body.blue_loadout[SLOT_HAT].product_id = (uint16_t)loadouts.loadout_one.hat;
	customLoadout.body.blue_loadout[SLOT_ENGINE_AUDIO].product_id = (uint16_t)loadouts.loadout_one.engine_audio; 
	customLoadout.body.blue_loadout[SLOT_SUPERSONIC_TRAIL].product_id = (uint16_t)loadouts.loadout_one.trail;
	customLoadout.body.blue_loadout[SLOT_GOALEXPLOSION].product_id = (uint16_t)loadouts.loadout_one.goal_explosion;

	customLoadout.body.orange_loadout[SLOT_BODY].product_id = (uint16_t)loadouts.loadout_two.body;
	customLoadout.body.orange_loadout[SLOT_SKIN].product_id = (uint16_t)loadouts.loadout_two.skin;
	customLoadout.body.orange_loadout[SLOT_WHEELS].product_id = (uint16_t)loadouts.loadout_two.wheels;
	customLoadout.body.orange_loadout[SLOT_BOOST].product_id = (uint16_t)loadouts.loadout_two.boost;
	customLoadout.body.orange_loadout[SLOT_ANTENNA].product_id = (uint16_t)loadouts.loadout_two.antenna;
	customLoadout.body.orange_loadout[SLOT_HAT].product_id = (uint16_t)loadouts.loadout_two.hat;
	customLoadout.body.orange_loadout[SLOT_ENGINE_AUDIO].product_id = (uint16_t)loadouts.loadout_two.engine_audio;
	customLoadout.body.orange_loadout[SLOT_SUPERSONIC_TRAIL].product_id = (uint16_t)loadouts.loadout_two.trail;
	customLoadout.body.orange_loadout[SLOT_GOALEXPLOSION].product_id = (uint16_t)loadouts.loadout_two.goal_explosion;

	processed[actor.actorId] = customLoadout;
	
}

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };

template<class... Ts> overload(Ts...) -> overload<Ts...>;

void SetClientLoadoutsOnline(const CPPRP::ActorStateData& actor, const CPPRP::ClientLoadoutsOnline& loadouts)
{
	if (processed.find(actor.actorId) == processed.end())
		return;
	BMLoadout customLoadout = processed[actor.actorId];
	for (int slot = 0; slot < loadouts.online_one.attributes_list.size(); slot++)
	{
		for (int j = 0; j < loadouts.online_one.attributes_list[slot].product_attributes.size(); j++)
		{
			CPPRP::AttributeType pa = loadouts.online_one.attributes_list[slot].product_attributes[j];
			
			std::visit(
			//overload(
				[&customLoadout, slot](const CPPRP::ProductAttributePainted&& paint)
				{
					customLoadout.body.blue_loadout[slot].paint_index = (uint8_t)(paint.value);
				}
			//)
			,
			pa);
			
            /*std::shared_ptr<CPPRP::ProductAttributePainted> paPainted = std::dynamic_pointer_cast<CPPRP::ProductAttributePainted>(pa);
            if (paPainted)
            {
                customLoadout.body.blue_loadout[slot].paint_index = (uint8_t)(pa->value);
            }*/
		}
	}

    for (int slot = 0; slot < loadouts.online_two.attributes_list.size(); slot++)
	{
		for (int j = 0; j < loadouts.online_two.attributes_list[slot].product_attributes.size(); j++)
		{
			std::shared_ptr<CPPRP::ProductAttribute> pa = loadouts.online_two.attributes_list[slot].product_attributes[j];
            std::shared_ptr<CPPRP::ProductAttributePainted> paPainted = std::dynamic_pointer_cast<CPPRP::ProductAttributePainted>(pa);
            if (paPainted)
            {
                customLoadout.body.orange_loadout[slot].paint_index = (uint8_t)(pa->value);
            }
		}
	}

	processed[actor.actorId] = customLoadout;
}




std::map<int, std::string> carNames = {
	{21, "Backfire"},
	{22, "Breakout"},
	{23, "Octane"},
	{24, "Paladin"},
	{25, "Roadhog"},
	{26, "Gizmo"},
	{27, "Sweet Tooth"},
	{28, "X-Devil"},
	{29, "Hotshot"},
	{30, "Merc"},
	{31, "Venom"},
	{402, "Takumi"},
	{403, "Dominus"},
	{404, "Scarab"},
	{523, "Zippy"},
	{597, "Delorean"},
	{600, "Ripper"},
	{607, "Grog"},
	{625, "Armadillo"},
	{723, "Hogsticker"},
	{803, "'16 Batmobile"},
	{1018, "Dominus GT"},
	{1159, "X-Devil Mk2"},
	{1171, "Masamune"},
	{1172, "Marauder"},
	{1286, "Aftershock"},
	{1295, "Takumi RX-T"},
	{1300, "Roadhog XL"},
	{1317, "Esper"},
	{1416, "Breakout Type-S"},
	{1475, "Proteus"},
	{1478, "Triton"},
	{1533, "Vulcan"},
	{1568, "Octane ZSR"},
	{1603, "Twinmill III"},
	{1623, "Bone Shaker"},
	{1624, "Endo"},
	{1675, "Ice Charger"},
	{1691, "Mantis"},
	{1856, "Jager 619"},
	{1883, "Imperator DT5"},
	{1919, "Centio V17"},
	{1932, "Animus GP"},
	{2070, "Werewolf"},
	{2268, "Dodge Charger R/T"},
	{2269, "Skyline GT-R"},
	{2298, "Samus' Gunship"},
	{2313, "Mario NSR"},
	{2665, "TDK Tumbler"},
	{2666, "'89 Batmobile"},
	{2853, "Twinzer"},
	{2919, "Jurassic Jeep Wrangler"},
	{3031, "Cyclone"},
	{3155, "Maverick"},
	{3156, "Maverick G1"},
	{3157, "Maverick GXT"},
	{3265, "McLaren 570S"},
	{3426, "Diestro"},
	{3451, "Nimbus"},
	{3594, "Artemis G1"},
	{3614, "Artemis"},
	{3622, "Artemis GXT"},
	{3875, "Guardian GXT"},
	{3879, "Guardian"},
	{3880, "Guardian G1"},
};

int main(int argc, char *argv[])
{
    auto replayFile = std::make_shared<CPPRP::ReplayFile>(argv[1]);
    replayFile->Load();
    replayFile->DeserializeHeader();

    replayFile->actorDeleteCallbacks.push_back([&](const CPPRP::ActorStateData& actor)
    {
        std::shared_ptr<CPPRP::TAGame::Car_TA> car = std::dynamic_pointer_cast<CPPRP::TAGame::Car_TA>(actor.actorObject);
        if(car)
        {
            if(replayFile->actorStates.find(car->PlayerReplicationInfo.actor_id) == replayFile->actorStates.end())
            {
                return;
            }
            CPPRP::ActorStateData& priActor = replayFile->actorStates[car->PlayerReplicationInfo.actor_id];

            std::shared_ptr<CPPRP::TAGame::PRI_TA> pri = std::dynamic_pointer_cast<CPPRP::TAGame::PRI_TA>(priActor.actorObject);
            if (pri)
            {
                actorNameMap[priActor.actorId] = pri->PlayerName;
                cameras[priActor.actorId] = pri->CameraSettings;
                
                SetClientLoadouts(priActor, pri->ClientLoadouts);
                SetClientLoadoutsOnline(priActor, pri->ClientLoadoutsOnline);
            }
        }
        else
        {
            std::shared_ptr<CPPRP::TAGame::CameraSettingsActor_TA> camera = std::dynamic_pointer_cast<CPPRP::TAGame::CameraSettingsActor_TA>(actor.actorObject);

            //TAGame.CameraSettingsActor_TA:ProfileSettings
            if(camera)
            {
                
                cameras[camera->PRI.actor_id] = camera->ProfileSettings;
            }
        }
        
        //printf("%s\n", typeid(*actor.actorObject).name());
    });

    
    replayFile->Parse();
	//Engine.PlayerReplicationInfo:PlayerName
	// crp_register_updated_callback("Engine.PlayerReplicationInfo:PlayerName", (callback_updated)&on_playername_set);
	// crp_register_updated_callback("TAGame.PRI_TA:ClientLoadouts", (callback_updated)&on_loadout_set);
	// crp_register_updated_callback("TAGame.PRI_TA:ClientLoadoutsOnline", (callback_updated)&on_onlineloadout_set);
	

	
	// crp_register_updated_callback("TAGame.CameraSettingsActor_TA:PRI", (callback_updated)&on_camerasettings_map);
	// crp_register_updated_callback("TAGame.PRI_TA:CameraSettings", (callback_updated)&on_camerasettings_set);
	// crp_register_updated_callback("TAGame.CameraSettingsActor_TA:ProfileSettings", (callback_updated)&on_camerasettings_set);
	// //

	// crp_parse_replay();
	
	//FILE * pFile;

	//pFile = fopen("loadout.txt", "w");
	for (auto a : processed)
	{
		std::string loadout = save(a.second);
		std::string body = carNames[a.second.body.blue_loadout[SLOT_BODY].product_id].c_str();
        //printf("%i", a.second.body.blue_loadout[SLOT_BODY].product_id);
		printf("%s (%s): %s\n", actorNameMap[a.first].c_str(), body.c_str(),  loadout.c_str());
	}
	printf("\n");
	for (auto a : processed)
	{
		int actual_actor = a.first;
		auto cam = cameras[actual_actor];
		printf("%s: FOV: %i, height: %i, pitch: %i, distance: %i, stiffness: %.1f, swivel: %.1f, transition: %.1f \n", 
			actorNameMap[a.first].c_str(),
			(int)cam.FOV,
            (int) cam.height,
			(int) cam.pitch,
			(int) cam.distance,
			cam.stiffness,
			cam.swivelspeed,
			cam.transitionspeed);
	}
	//fclose(pFile);
	//crp_free_replay();
	//getchar();
	return 0;
}