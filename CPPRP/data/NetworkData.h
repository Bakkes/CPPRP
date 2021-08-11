#pragma once
#include <stdint.h>
#include <sstream>
#include <vector>
#include "CPPBitReader.h"
#include <variant>

/* 
ParserAttributes:
	Consume true/false -> 
		If on struct -> if false, don't generate parser. Must implement your own in NetworkDataParser.h
		If on member -> needs to be true for non standard types that don't have a read<T> method in CPPBitReader
	NumBits x -> declare if x != sizeof(type) * 8, for example when an uint8_t is used to store but only 3 bits need to be read from stream
	EngineVersion x -> Only parse replay if engine version >= x
	LicenseeVersion x -> Only parse replay if licensee version >= x
	MaxBits x -> instead of reading set number of bits, will call CPPBitReader::get_bits_max(x);
std::vector<T> members always assume it can read a uint8_t member containing the size of vector, then fills it with reading T
std::shared_ptr<T> Parser needs to be specifically implemented to return a shared_ptr.
*/
#define __ParserAttribute__(...)

namespace CPPRP
{

	__ParserAttribute__(Consume, false)
	struct ReplicatedRBState
	{
		Quat rotation;
		Vector3 position;
		Vector3 linear_velocity;
		Vector3 angular_velocity;
		bool sleeping;
	};

	struct LogoData
	{
		bool swap_colors;
		uint32_t logo_id;
	};

	
	struct ActiveActor
	{
		bool active;
		int32_t actor_id;
	};

	struct ObjectTarget
	{
		bool unknown;
		int32_t object_index;
	};

	__ParserAttribute__(Consume, false)
	struct Reservation
	{
		__ParserAttribute__(NumBits, 3)
		uint8_t number;
		OnlineID player_id;
		std::string player_name;
		uint8_t status;
	};

	//	ReservationStatus_None = 0,
	//	ReservationStatus_Reserved = 1,
	//	ReservationStatus_Joining = 2,
	//	ReservationStatus_InGame = 3,
	//	ReservationStatus_MAX = 4

	//Maybe its not version, but size instead? then fill array of size version
	__ParserAttribute__(Consume, false)
	struct ClientLoadout
	{
		uint8_t version;
		uint32_t body;
		uint32_t skin;
		uint32_t wheels;
		uint32_t boost;
		uint32_t antenna;
		uint32_t hat;
		uint32_t unknown2;
		uint32_t unknown3;
		uint32_t engine_audio;
		uint32_t trail;
		uint32_t goal_explosion;
		uint32_t banner;
		uint32_t unknown4;
		uint32_t unknown5;
		uint32_t unknown6;
		uint32_t unknown7;
	};

	struct CameraSettings
	{
		float FOV;
		float height;
		float pitch;
		float distance;
		float stiffness;
		float swivelspeed;
		__ParserAttribute__(EngineVersion, 868)
		__ParserAttribute__(LicenseeVersion, 20)
		float transitionspeed;
	};


	struct ReplicatedPickupData
	{
		bool unknown1; //some kind of flag?
		int32_t actor_id; //instigator
		bool picked_up;
	};

	struct ReplicatedPickupData2
	{
		bool unknown1;
		int32_t actor_id; //instigator
		uint8_t picked_up;
	};

	struct TeamPaint
	{
		uint8_t team_number;
		uint8_t team_color_id;
		uint8_t custom_color_id;
		uint32_t team_finish_id;
		uint32_t custom_finish_id;
	};

	struct ReplicatedDemolish
	{
		bool attacker_flag;
		int32_t attacker_actor_id;
		bool victim_flag;
		int32_t victim_actor_id;
		Vector3 attacker_velocity;
		Vector3 victim_velocity;
	};

	struct ReplicatedDemolish2
	{
		bool custom_demo_flag;
		int32_t custom_demo_id;
		bool attacker_flag;
		int32_t attacker_actor_id;
		bool victim_flag;
		int32_t victim_actor_id;
		Vector3 attacker_velocity;
		Vector3 victim_velocity;
	};

	//Actually extends ReplicatedDemolish but i don't think our preprocessor supports extending
	//And we need to parse in reverse order (?)
	struct DemolishDataGoalExplosion
	{
		bool goal_explosion_owner_flag;
		int32_t goal_explosion_owner;
		bool attacker_flag;
		int32_t attacker_actor_id;
		bool victim_flag;
		int32_t victim_actor_id;
		Vector3 attacker_velocity;
		Vector3 victim_velocity;
		
	};

	struct ReplicatedMusicStringer
	{
		bool unknown1;
		uint32_t object_index;
		uint8_t trigger;
	};

	__ParserAttribute__(Consume, false)
	struct GameMode
	{
		uint8_t gamemode;
	};

	struct ReplicatedStateIndex
	{
		__ParserAttribute__(MaxBits, 140)
		uint32_t value;
	};

	struct PrivateMatchSettings
	{
		std::string mutators;
		uint32_t map_name;
		uint32_t max_player_count;
		std::string game_name;
		std::string password;
		bool is_public;
	};

	__ParserAttribute__(Consume, false)
	struct ProductAttribute
	{
		uint32_t class_index;
		std::string class_name;
		uint32_t value;
		bool has_value;
		bool unknown1;
		virtual ~ProductAttribute() = default;
	};



	//__ParserAttribute__(Consume, false)
	struct ActorBase
	{
		uint32_t value;
		bool unknown1;
		bool unknown2;
	};

	__ParserAttribute__(Consume, false)
	struct PartyLeader
	{
		OnlineID id;
	};


	__ParserAttribute__(Consume, false)
	struct ProductAttributeUserColorSingle : public ProductAttribute 
	{
		bool hasValue;
		uint32_t value;
	};

	__ParserAttribute__(Consume, false)
	struct ProductAttributeUserColorRGB : public ProductAttribute
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
	};

	__ParserAttribute__(Consume, false)
	struct ProductAttributeTitle : public ProductAttribute
	{
		std::string title;
	};

	__ParserAttribute__(Consume, false)
	struct ProductAttributeSingleValue : public ProductAttribute
	{
		uint32_t value;
	};

	__ParserAttribute__(Consume, false)
	struct ProductAttributePainted : public ProductAttributeSingleValue
	{
		
	};

	__ParserAttribute__(Consume, false)
	struct ProductAttributeTeamEdition : public ProductAttributeSingleValue
	{

	};

	__ParserAttribute__(Consume, false)
	struct ProductAttributeSpecialEdition : public ProductAttributeSingleValue
	{

	};

	using AttributeType = std::variant<ProductAttributeUserColorRGB, ProductAttributeUserColorSingle,
		ProductAttributePainted, ProductAttributeTeamEdition, ProductAttributeTitle,
		ProductAttributeSpecialEdition, ProductAttribute>;

	struct Attributes
	{
		//uint8_t attributes_count; //Is automatically read when consuming vector
		__ParserAttribute__(CallConsume, true)
			std::vector<AttributeType> product_attributes;
	};

	struct OnlineLoadout
	{
		//uint8_t attributes_list_count;//Is automatically read when consuming vector
		__ParserAttribute__(CallToString, true)
		__ParserAttribute__(CallConsume, true)
		std::vector<Attributes> attributes_list;
	};


	struct ClientLoadoutsOnline
	{
		__ParserAttribute__(CallConsume, true)
		OnlineLoadout online_one;

		__ParserAttribute__(CallConsume, true)
		OnlineLoadout online_two;

		bool loadout_set;
		bool is_deprecated;
	};

	struct ClientLoadouts
	{
		__ParserAttribute__(CallConsume, true)
		ClientLoadout loadout_one;

		__ParserAttribute__(CallConsume, true)
		ClientLoadout loadout_two;
	};

	struct ClubColors
	{
		bool team_color_set;
		uint8_t team_color_id;
		bool custom_color_set;
		uint8_t custom_color_id;
	};

	struct WeldedInfo
	{
		bool active;
		int32_t actor_id;
		Vector3 offset;
		float mass;
		Rotator rotation;
	};

	enum EBreakoutDamageState
	{
		DamageState_Start,
		DamageState_Damaged,
		DamageState_Broken,
		DamageState_MAX
	};

	struct DamageState
	{
		uint8_t damage_state;
		bool unknown2;
		int32_t causer_actor_id;
		Vector3 damage_location;
		bool direct_damage;
		bool immediate;
	};

	struct AppliedDamage
	{
		uint8_t id;
		Vector3 position;
		int32_t damage_index;
		int32_t total_damage;
	};

	struct ReplicatedExplosionData
	{
		bool unknown1;
		uint32_t actor_id;
		Vector3 position;
	};

	struct ReplicatedExplosionDataExtended
	{
		bool unknown1;
		int32_t actor_id;
		Vector3 position;
		bool unknown3;
		int32_t secondary_actor_id;
	};

	struct ReplicatedTitle
	{
		bool unknown1;
		bool unknown2;
		uint32_t unknown3;
		uint32_t unknown4;
		uint32_t unknown5;
		uint32_t unknown6;
		uint32_t unknown7;
		bool unknown8;
	};

	struct HistoryKey
	{
		__ParserAttribute__(NumBits, 14)
		uint16_t data;
	};

	struct ReplicatedStatEvent
	{
		bool unknown1;
		int32_t object_id;
	};

	struct RepStatTitle
	{
		bool unknown1;
		std::string name;
		__ParserAttribute__(CallConsume, true)
		ObjectTarget object_target;
		uint32_t value;
	};

	struct SkillTier
	{
		__ParserAttribute__(MaxBits, 500)
		uint32_t tier;
	};

	//Probably not relevant for most, is only used for KActors in workshop replays
	//__ParserAttribute__(Consume, false)
	struct RigidBodyState 
	{
		Vector3 position;
		Vector3 lin_vel;
		Quat quaternion;            
		Vector3 ang_vel;              
		uint32_t flags;         
	};

	__ParserAttribute__(Consume, false)
	struct PickupInfo_TA
	{
		ActiveActor AvailablePickups;
		
		//Not entirely sure what this is yet
		bool unknown;
		bool bItemsArePreview;
		
	};

}
#include "GameClasses.h"