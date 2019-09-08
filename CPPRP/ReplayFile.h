#pragma once
#include <stdint.h>
#include <string>
#include <filesystem>
#include <any>
#include <map>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <functional>
#include "CPPBitReader.h"
#include "./data/NetworkData.h"
#include "./exceptions/ParseException.h"
#include "./exceptions/ReplayException.h"
#include "NetworkDataParsers.h"
#include "./data/ReplayFileData.h"
#include "PropertyParser.h"

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
		uint32_t actorId{0};
		uint32_t nameId{ 0 };
		uint32_t classNameId{ 0 };
	};

	typedef std::function<void(const Frame, const std::unordered_map<int, ActorStateData>&)> tickable;
	typedef std::function<void(const ActorStateData&)> actorCreated;
	typedef std::function<void(const ActorStateData&, const std::vector<uint32_t>&)> actorUpdated;
	typedef std::function<void(const ActorStateData&)> actorDeleted;
	typedef std::function<void(uint32_t, const ActorStateData&)> propertyUpdated;

	class ReplayFile
	{
	private:
		
		std::shared_ptr<CPPBitReader<BitReaderType>> fullReplayBitReader;
		std::map<std::string, std::shared_ptr<ClassNet>> classnetMap;
	public:
		std::vector<char> data;
		std::filesystem::path path;
		std::vector<Frame> frames;
		std::unordered_map<int, ActorStateData> actorStates;
		std::shared_ptr<ReplayFileData> replayFile;
		
		std::vector<parsePropertyFunc> parseFunctions;
		std::vector<createObjectFunc> createFunctions;
		std::vector<std::string> parseLog;

	public:
		std::vector<tickable> tickables;
		std::vector<actorCreated> createdCallbacks;
		std::vector<actorUpdated> updatedCallbacks;
		std::vector<actorDeleted> actorDeleteCallbacks;

		std::unordered_map<std::string, uint32_t> objectToId;
	public:
		ReplayFile(std::filesystem::path path_);
		~ReplayFile();

		const bool Load();
		void DeserializeHeader();
		const bool VerifyCRC(CrcCheck verifyWhat);
		void Parse(const uint32_t startPos = 0, int32_t endPos = -1, const uint32_t frameCount = 0);
	
		void PreprocessTables();
		std::string GetParseLog(size_t amount);
	protected:
		void MergeDuplicates();
		void FixParents();

		const std::pair<const uint32_t, const KeyFrame> GetNearestKeyframe(uint32_t frame) const;
		const bool ParseProperty(const std::shared_ptr<Property>& currentProperty);
		const std::shared_ptr<ClassNet>& GetClassnetByNameWithLookup(const std::string& name) const;
		const uint16_t GetPropertyIndexById(const std::shared_ptr<ClassNet>& cn, const int id) const;
		const uint16_t GetMaxPropertyId(const std::shared_ptr<ClassNet>& cn);
		const uint16_t FindMaxPropertyId(const std::shared_ptr<ClassNet>& cn, uint16_t maxProp) const;

		
	public:
		const bool HasProperty(const std::string& key) const;
		const bool HasInitialPosition(const std::string& name) const;
		const bool HasRotation(const std::string& name) const;
		template<typename T>
		const T GetProperty(const std::string& key) const;
	};

	template<typename T>
	inline const T ReplayFile::GetProperty(const std::string& key) const
	{
		if (replayFile->properties.find(key) == replayFile->properties.end())
		{
			throw PropertyDoesNotExistException(key);
		}
		return std::any_cast<T>(replayFile->properties.at(key)->value);
	}
}