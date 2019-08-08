#pragma once
#include <stdint.h>
#include <string>
#include <filesystem>
#include <any>
#include <vector>
#include <unordered_map>
#include <mutex>

#include "NetworkStreamParser.h"
#include "CPPBitReader.h"
#include "ReplayFileData.h"
namespace CPPRP
{
	enum CrcCheck
	{
		CRC_Header = 0x1,
		CRC_Body = 0x02,
		CRC_Both = CRC_Header | CRC_Body
	};

	struct ActorStateData
	{
		std::shared_ptr<Engine::Actor> actorObject;
		std::shared_ptr<ClassNet> classNet;
		uint32_t nameId{ 0 };
	};

	class ReplayFile
	{
	private:
		std::vector<char> data;
		CPPBitReader<BitReaderType> fullReplayBitReader;
		std::unordered_map<std::string, std::shared_ptr<ClassNet>> classnetMap;
	public:
		std::filesystem::path path;
		std::unordered_map<int, ActorStateData> actorStates;
		std::shared_ptr<ReplayFileData> replayFile;
		NetworkStreamParser networkParser;
		std::vector<std::string> parseLog;
	public:
		ReplayFile(std::filesystem::path path_);
		~ReplayFile();

		const bool Load();
		void DeserializeHeader();
		const bool VerifyCRC(CrcCheck verifyWhat);
		void Parse(const uint32_t startPos = 0, int32_t endPos = -1, const uint32_t frameCount = 0);
	protected:
		void MergeDuplicates();
		void FixParents();

		const bool HasInitialPosition(const std::string& name) const;
		const bool HasRotation(const std::string& name) const;

		const std::pair<const uint32_t, const KeyFrame> GetNearestKeyframe(uint32_t frame) const;
		const bool ParseProperty(const std::shared_ptr<Property>& currentProperty);
		const std::shared_ptr<ClassNet>& GetClassnetByNameWithLookup(const std::string& name) const;
		const uint16_t GetPropertyIndexById(const std::shared_ptr<ClassNet>& cn, const int id) const;
		const uint16_t GetMaxPropertyId(const std::shared_ptr<ClassNet>& cn);
		const uint16_t FindMaxPropertyId(const std::shared_ptr<ClassNet>& cn, uint16_t maxProp) const;

	public:
		const bool HasProperty(const std::string& key) const;

		template<typename T>
		const T GetProperty(const std::string& key);
	};

	template<typename T>
	inline const T ReplayFile::GetProperty(const std::string& key)
	{
		if (replayFile->properties.find(key) == replayFile->properties.end())
		{
			throw PropertyDoesNotExistException(key);
		}
		return std::any_cast<T>(replayFile->properties.at(key)->value);
	}
}