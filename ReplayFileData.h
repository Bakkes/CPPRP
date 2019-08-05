#pragma once
#include <stdint.h>
#include <any>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <memory>

namespace CPPRP
{
	struct Vector3
	{
		float x, y, z;

		std::string ToString() const
		{
			return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
		}
	};

	struct Vector3I
	{
		int x, y, z;

		explicit operator Vector3() const
		{
			return Vector3
			{
				static_cast<float>(x),
				static_cast<float>(y),
				static_cast<float>(z)
			};
		};

		std::string ToString() const
		{
			return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
		}
	};

	struct Rotator
	{
		int pitch, yaw, roll;

		std::string ToString() const
		{
			return std::to_string(pitch) + ", " + std::to_string(yaw) + ", " + std::to_string(roll);
		}
	};

	struct Quat
	{
		float w, x, y, z;

		std::string ToString() const
		{
			return std::to_string(w) + ", " + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
		}
	};

	enum Platform
	{
		Platform_Unknown = 0,
		Platform_Steam = 1,
		Platform_PS4 = 2,
		Platform_PS3 = 3,
		Platform_Dingo = 4,
		Platform_QQ = 5,
		Platform_Switch = 6, //Is in gamefiles as wegame..
		//Platform_NNX = 7,
		Platform_PsyNet = 7, //is 8 in gamefiles
		Platform_MAX = 9
	};

	struct UniqueId
	{
		uint8_t platform;
		uint8_t playerNumber;
		uint64_t uniqueID;

		std::string ToString() const
		{
			return std::to_string(platform) + "|" + std::to_string(uniqueID) + "|" + std::to_string(playerNumber);
		}

		friend std::ostream& operator<<(std::ostream& os, UniqueId& foo)
		{
			os << foo.ToString();
			return os;
		}

	};

	struct Property
	{
		std::string property_name;
		std::string property_type;
		std::any value;
	};

	struct FileHeader
	{
		uint32_t size;
		uint32_t crc;
		uint32_t engineVersion;
		uint32_t licenseeVersion;
		uint32_t netVersion = 0;
	};

	struct KeyFrame
	{
		float time;
		uint32_t frame;
		uint32_t filepos;
	};

	struct ReplayTick
	{
		std::string type;
		uint32_t frame;
	};

	struct ClassIndex
	{
		std::string class_name;
		uint32_t index;
	};

	struct PropIndexId
	{
		int32_t prop_index;
		int32_t prop_id;
	};

	struct ClassNet
	{
		int32_t index;
		int32_t parent;
		std::shared_ptr<ClassNet> parent_class;
		int32_t id;
		int32_t prop_indexes_size;
		std::vector<PropIndexId> prop_indexes;
		uint16_t max_prop_id;
		std::vector<uint16_t> property_id_cache;
	};

	struct Frame
	{
		float time;
		float delta;
	};

	struct EnumProperty
	{
		std::string type;
		std::string value;
	};

	struct ReplayFileData
	{
		//All header props
		FileHeader header;
		std::string replayType;
		std::unordered_map<std::string, std::shared_ptr<Property>> properties;
		uint32_t body_size;
		uint32_t crc2;

		std::vector<std::string> levels;
		std::vector<KeyFrame> keyframes;

		uint32_t netstream_size;
		const char* netstream_data;

		std::vector<ReplayTick> replayticks;
		std::vector<std::string> replicated_packages;
		std::vector<std::string> objects;
		std::vector<std::string> names;
		std::vector<ClassIndex> class_indices;
		std::vector<std::shared_ptr<ClassNet>> classnets;
	};

	struct ActorState
	{
		std::shared_ptr<ClassNet> classNet;
		uint32_t actor_id;
		uint32_t name_id;
		Vector3 position;
		Rotator rotation;
	};
}