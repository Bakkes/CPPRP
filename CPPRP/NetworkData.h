#pragma once
#include <stdint.h>
#include <sstream>
#include <vector>
#include "CPPBitReader.h"

#define __ParserAttribute__(...)

namespace CPPRP
{

	__ParserAttribute__(Consume, false)
	struct ReplicatedRBState
	{
		__ParserAttribute__(CallToString, true)
		Quat rotation;

		__ParserAttribute__(CallToString, true)
		Vector3 position;

		__ParserAttribute__(CallToString, true)
		Vector3 linear_velocity;

		__ParserAttribute__(CallToString, true)
		Vector3 angular_velocity;
		bool sleeping;
	};

	struct LogoData
	{
		bool unknown;
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
		__ParserAttribute__(CallToString, true)
		uint8_t unknown;

		__ParserAttribute__(CallToString, true)
		std::shared_ptr<UniqueId> player_id;
		std::string player_name;

		__ParserAttribute__(CallToString, true)
		uint8_t unknown2;
	};

	__ParserAttribute__(Consume, false)
	struct ClientLoadout
	{
		__ParserAttribute__(CallToString, true)
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
		bool unknown1;
		int32_t actor_id;
		bool picked_up;
	};

	struct TeamPaint
	{
		__ParserAttribute__(CallToString, true)
		uint8_t team_number;

		__ParserAttribute__(CallToString, true)
		uint8_t team_color_id;

		__ParserAttribute__(CallToString, true)
		uint8_t custom_color_id;
		uint32_t team_finish_id;
		uint32_t custom_finish_id;
	};

	struct ReplicatedDemolish
	{
		bool unknown1;
		int32_t attacker_actor_id;
		bool unknown2;
		int32_t victim_actor_id;
		__ParserAttribute__(CallToString, true)
		Vector3 attacker_velocity;
		__ParserAttribute__(CallToString, true)
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
	};



	struct Attributes
	{
		//uint8_t attributes_count; //Is automatically read when consuming vector
		__ParserAttribute__(CallToString, true)
		__ParserAttribute__(CallConsume, true)
		std::vector<std::shared_ptr<ProductAttribute>> product_attributes;
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
		__ParserAttribute__(CallToString, true)
		std::shared_ptr<UniqueId> id;
	};

	struct OnlineLoadout
	{
		//uint8_t attributes_list_count;//Is automatically read when consuming vector
		__ParserAttribute__(CallToString, true)
		__ParserAttribute__(CallConsume, true)
		std::vector<Attributes> attributes_list;
	};


	struct ProductAttributeUserColorSingle : public ProductAttribute 
	{
		bool hasValue;
		uint32_t value;
	};

	struct ProductAttributeUserColorRGB : public ProductAttribute
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
	};

	struct ProductAttributeTitle : public ProductAttribute
	{
		std::string title;
	};

	struct ProductAttributeSingleValue : public ProductAttribute
	{
		uint32_t value;
	};

	struct ProductAttributePainted : public ProductAttributeSingleValue
	{
		
	};

	struct ProductAttributeTeamEdition : public ProductAttributeSingleValue
	{

	};

	struct ProductAttributeSpecialEdition : public ProductAttributeSingleValue
	{

	};

	struct ClientLoadoutsOnline
	{
		__ParserAttribute__(CallToString, true)
		__ParserAttribute__(CallConsume, true)
		OnlineLoadout online_one;

		__ParserAttribute__(CallToString, true)
		__ParserAttribute__(CallConsume, true)
		OnlineLoadout online_two;

		bool loadout_set;
		bool is_deprecated;
	};

	struct ClientLoadouts
	{
		__ParserAttribute__(CallToString, true)
		__ParserAttribute__(CallConsume, true)
		ClientLoadout loadout_one;

		__ParserAttribute__(CallToString, true)
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
		__ParserAttribute__(CallToString, true)
		Vector3 offset;
		float mass;
		__ParserAttribute__(CallToString, true)
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

		__ParserAttribute__(CallToString, true)
		Vector3 damage_location;
		bool direct_damage;
		bool immediate;
	};

	struct AppliedDamage
	{
		uint8_t id;
		__ParserAttribute__(CallToString, true)
		Vector3 position;
		int32_t damage_index;
		int32_t total_damage;
	};

	struct ReplicatedExplosionData
	{
		bool unknown1;
		uint32_t actor_id;
		__ParserAttribute__(CallToString, true)
		Vector3 position;
	};

	struct ReplicatedExplosionDataExtended
	{
		bool unknown1;
		uint32_t actor_id;
		__ParserAttribute__(CallToString, true)
		Vector3 position;
		bool unknown3;
		uint32_t unknown4;
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

		__ParserAttribute__(CallToString, true)
		__ParserAttribute__(CallConsume, true)
		ObjectTarget object_target;
		uint32_t value;
	};

	struct SkillTier
	{
		__ParserAttribute__(MaxBits, 500)
		uint32_t tier;
	};
}
#include "GameClasses.h"