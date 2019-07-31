#pragma once
#include <stdint.h>
#include <string>
#include <filesystem>
#include <any>
#include <vector>
#include <unordered_map>

#include "CPPBitReader.h"
typedef struct
{
	std::string property_name;
	std::string property_type;
	std::any value;
} Property;

struct FileHeader
{
	uint32_t size;
	uint32_t crc;
	uint32_t version1;
	uint32_t version2;
} ;

typedef struct
{
	float time;
	uint32_t frame;
	uint32_t filepos;
} KeyFrame;

typedef struct
{
	std::string type;
	uint32_t frame;
} ReplayTick;

typedef struct
{
	std::string class_name;
	uint32_t index;
} ClassIndex;

typedef struct
{
	int32_t prop_index;
	int32_t prop_id;
} PropIndexId;

typedef struct ClassNet
{
	int32_t index;
	int32_t parent;
	std::shared_ptr<ClassNet> parent_class;
	int32_t id;
	int32_t prop_indexes_size;
	std::vector<PropIndexId> prop_indexes;
	uint16_t max_prop_id;
	std::vector<uint16_t> property_id_cache;
} ClassNet;

typedef struct
{
	float time;
	float delta;
} Frame;

struct EnumProperty
{
	std::string type;
	std::string value;
};

struct ReplayFileData
{
	//All header props
	FileHeader header;
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

typedef struct
{
	std::shared_ptr<ClassNet> classNet;
	uint32_t actor_id;
	uint32_t name_id;
	Vector3 position;
	Rotator rotation;
} ActorState;

class ReplayFile
{
private:
	std::filesystem::path path;
	std::vector<char> data;
	CPPBitReader<uint32_t> fullReplayBitReader;

	bool classNetMapCached= false;
	std::unordered_map<std::string, std::shared_ptr<ClassNet>> classnetMap;
public:
	std::unordered_map<int, ActorState> actorStates;
	ReplayFileData replayFile;
public:
	ReplayFile(std::filesystem::path path_);
	~ReplayFile();

	bool Load();
	void DeserializeHeader();
	void FixParents();
	void Parse();
protected:
	const bool HasInitialPosition(const std::string& name) const;
	const bool HasRotation(const std::string& name) const;
	

	const bool ParseProperty(const std::shared_ptr<Property>& currentProperty);
	const std::shared_ptr<ClassNet>& GetClassnetByName(const std::string& name);
	const std::shared_ptr<ClassNet>& GetClassnetByNameWithLookup(const std::string& name);
	const uint16_t GetPropertyIndexById(const std::shared_ptr<ClassNet>& cn, const int id) const;
	const uint16_t GetMaxPropertyId(const std::shared_ptr<ClassNet>& cn);
	const uint16_t FindMaxPropertyId(const std::shared_ptr<ClassNet>& cn, uint16_t maxProp) const;

	template<typename T>
	const T GetProperty(const std::string& key)
	{
		//auto baseMap = std::any_cast<std::unordered_map<std::string, std::shared_ptr<Property>>>(replayFile.properties->value);
		if (replayFile.properties.find(key) == replayFile.properties.end())
		{
			assert(1 == 2); //die
		}
		return std::any_cast<T>(replayFile.properties.at(key)->value);
	}
};

