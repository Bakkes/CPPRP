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
#ifdef _WIN32
#define DllExport   __declspec( dllexport )
#else
#define DllExport  
#endif
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
		uint32_t typeId{ 0 }; // Used for checking archetypes
	};



	typedef std::function<void(const Frame&, const std::unordered_map<decltype(ActorStateData::actorId), ActorStateData>&)> tickable;
	typedef std::function<void(const Frame&)> onNewFrame;
	typedef std::function<void(const ActorStateData&)> actorCreated;
	typedef std::function<void(const ActorStateData&, const std::vector<uint32_t>&)> actorUpdated;
	typedef std::function<void(const ActorStateData&)> actorDeleted;
	typedef std::function<void(uint32_t, const ActorStateData&)> propertyUpdated;

	class DllExport ReplayFile
	{
	private:
		
		std::shared_ptr<CPPBitReader<BitReaderType>> fullReplayBitReader;
		std::map<std::string, std::shared_ptr<ClassNet>> classnetMap;
	public:
		std::vector<char> data;
		std::filesystem::path path;
		std::vector<Frame> frames;
		std::unordered_map<decltype(ActorStateData::actorId), ActorStateData> actorStates;
		std::shared_ptr<ReplayFileData> replayFile;
		FileHeader header;
		std::vector<parsePropertyFunc> parseFunctions;
		std::vector<createObjectFunc> createFunctions;
		std::vector<std::string> parseLog;

	public:
		std::vector<tickable> tickables;
		std::vector<onNewFrame> newFrameCallbacks;
		std::vector<actorCreated> createdCallbacks;
		std::vector<actorUpdated> updatedCallbacks;
		std::vector<actorDeleted> actorDeleteCallbacks;

		std::vector<uint32_t> positionIDs;
		std::vector<uint32_t> rotationIDs;
		std::vector<uint32_t> attributeIDs;
		std::vector<std::shared_ptr<ClassNet>> classnetCache;
		std::unordered_map<std::string, uint32_t> objectToId;
	public:
		ReplayFile(std::filesystem::path path_);
		ReplayFile(std::vector<char>& fileData);
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
		const uint16_t GetMaxPropertyId(ClassNet* cn);
		const uint16_t FindMaxPropertyId(const ClassNet* cn, uint16_t maxProp) const;

		
	public:
		const bool HasProperty(const std::string& key) const;
		const bool HasInitialPosition(const uint32_t name) const;
		const bool HasRotation(const uint32_t name) const;
		template<typename T>
		const T GetProperty(const std::string& key) const;

		template<typename T>
		const std::shared_ptr<T> GetActiveActor(const ActiveActor& key) const;
	};

	template<typename T>
	inline const T ReplayFile::GetProperty(const std::string& key) const
	{
		if (auto it = replayFile->properties.find(key); it != replayFile->properties.end())
		{
			auto& value = it->second->value;
			return std::get<T>(value);
			// could throw a custom "bad type exception" after checking or just use the std::bad_variant_access std::get throws 
			//if (std::holds_alternative<T>(value))
			//{
			//	
			//}
		}
		throw PropertyDoesNotExistException(key);

	}
	template<typename T>
	inline const std::shared_ptr<T> ReplayFile::GetActiveActor(const ActiveActor& key) const
	{
		if (auto found = actorStates.find(key.actor_id); found != actorStates.end())
		{
			return std::dynamic_pointer_cast<T>(found->second.actorObject);
		}
		return nullptr;
	}
}