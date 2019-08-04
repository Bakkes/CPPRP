#pragma once
#include <stdint.h>
#include <string>
#include <filesystem>
#include <any>
#include <vector>
#include <unordered_map>
#include "NetworkStreamParser.h"
#include "CPPBitReader.h"
#include "ReplayFileData.h"

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
	std::shared_ptr<ReplayFileData> replayFile;
	NetworkStreamParser networkParser;
public:
	ReplayFile(std::filesystem::path path_);
	~ReplayFile();

	bool Load();
	void DeserializeHeader();
	void MergeDuplicates();
	void FixParents();
	void Parse(std::string fileName, uint32_t startPos = 0, int32_t endPos = -1);
protected:
	const bool HasInitialPosition(const std::string& name) const;
	const bool HasRotation(const std::string& name) const;
	
	const std::pair<const uint32_t, const KeyFrame> GetNearestKeyframe(uint32_t frame) const;
	const bool ParseProperty(const std::shared_ptr<Property>& currentProperty);
	//const std::shared_ptr<ClassNet>& GetClassnetByName(const std::string& name) const;
	const std::shared_ptr<ClassNet>& GetClassnetByNameWithLookup(const std::string& name) const;
	const uint16_t GetPropertyIndexById(const std::shared_ptr<ClassNet>& cn, const int id) const;
	const uint16_t GetMaxPropertyId(const std::shared_ptr<ClassNet>& cn);
	const uint16_t FindMaxPropertyId(const std::shared_ptr<ClassNet>& cn, uint16_t maxProp) const;

	template<typename T>
	const T GetProperty(const std::string& key)
	{
		//auto baseMap = std::any_cast<std::unordered_map<std::string, std::shared_ptr<Property>>>(replayFile.properties->value);
		if (replayFile->properties.find(key) == replayFile->properties.end())
		{
			//assert(1 == 2); //die
		}
		return std::any_cast<T>(replayFile->properties.at(key)->value);
	}
};

