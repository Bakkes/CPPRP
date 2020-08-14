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
#define DllExport   __declspec( dllexport )

namespace CPPRP
{
	enum CrcCheck
	{
		CRC_Header = 0x1,
		CRC_Body = 0x02,
		CRC_Both = CRC_Header | CRC_Body
	};

	/*
	Struct storing data for actors containing the actor object, along with
	Some IDs to do lookups in tables and the object's classnet.
	*/
	struct ActorStateData
	{
		std::shared_ptr<Engine::Actor> actorObject;
		std::shared_ptr<ClassNet> classNet;
		uint32_t actorId{0};
		uint32_t nameId{ 0 };
		uint32_t classNameId{ 0 };
	};


	/*
	Function definitions for callbacks that can be registered when using CPPRP as a library.
	These functions allow a full "playthrough" of the replay as if it were a live game but at a very fast speed
	*/
	typedef std::function<void(const Frame, const std::unordered_map<int, ActorStateData>&)> tickable;
	typedef std::function<void(const Frame)> onNewFrame;
	typedef std::function<void(const ActorStateData&)> actorCreated;
	typedef std::function<void(const ActorStateData&, const std::vector<uint32_t>&)> actorUpdated;
	typedef std::function<void(const ActorStateData&)> actorDeleted;
	typedef std::function<void(uint32_t, const ActorStateData&)> propertyUpdated;



	class DllExport ReplayFile
	{
	private:
		/*
		Bitreader used when parsing the entire file
		*/
		std::shared_ptr<CPPBitReader<BitReaderType>> fullReplayBitReader;

		/*
		Map to map names to classnets
		*/
		std::map<std::string, std::shared_ptr<ClassNet>> classnetMap;
	public:
		/*
		Raw bytes of loaded replay file
		*/
		std::vector<char> data;

		/*
		Path to the replay file
		*/
		std::filesystem::path path;

		/*
		Parsed frame data of replays (frameNo, time, delta, position)
		*/
		std::vector<Frame> frames;

		/*
		Map of currently active actors, key = actorID
		*/
		std::unordered_map<int, ActorStateData> actorStates;

		/*
		Parsed header data of the replay
		*/
		std::shared_ptr<ReplayFileData> replayFile;

		/*
		Copy of FileHeader property of the replayFileData for quick access
		*/
		FileHeader header;

		/*
		List of functions to parse properties. Vector used for quick lookups as
		parseFunctions[propertyId] points to the right function to parse given propertyId
		See: PropertyParser.h
		*/
		std::vector<parsePropertyFunc> parseFunctions;

		/*
		List of functions to create objects (actors). Calling createFunctions[classId]
		will create an empty object belonging to the type classId is of
		See: PropertyParser.h
		*/
		std::vector<createObjectFunc> createFunctions;

		/*
		Log containing information on what's being parsed. Only filled if IncludeParseLog
		in ReplayFile.cpp is set to true!
		*/
		std::vector<std::string> parseLog;

	public:
		/*
		Callbacks to be called on certain events. Allows for external classes to subscribe 
		to these events
		*/
		std::vector<tickable> tickables;
		std::vector<onNewFrame> newFrameCallbacks;
		std::vector<actorCreated> createdCallbacks;
		std::vector<actorUpdated> updatedCallbacks;
		std::vector<actorDeleted> actorDeleteCallbacks;

		/*
		Precomputed values needed for parsing. Has some small memory overhead but
		greatly improves performance during parsing. Left public to allow external classes
		to do quick lookups as well.
		*/
		std::vector<uint32_t> positionIDs;
		std::vector<uint32_t> rotationIDs;
		std::vector<uint32_t> attributeIDs;
		std::vector<std::shared_ptr<ClassNet>> classnetCache;
		std::unordered_map<std::string, uint32_t> objectToId;
	public:
		ReplayFile(std::filesystem::path path_);
		ReplayFile(std::vector<char>& fileData);
		~ReplayFile();

		/*
		Loads the bytes from disk into memory
		*/
		const bool Load();

		/*
		Deserializes the header of the replay. Load() must be called before calling this function
		*/
		void DeserializeHeader();

		/*
		Verifies if the CRC in the replay matches the actual content of the replay.
		Additionally also checks if the replay file isn't lying about its size.
		Load() must be called before calling this function
		*/
		const bool VerifyCRC(CrcCheck verifyWhat);

		/*
		Parses the replay file. Requires that Load() and DeserializeHeader() are called before calling this.
		Param startPos: The bit position to start parsing at. Should be a position that matches the start of a frame or parsing will not work.
		When not using a value of 0 or a keyframe parsing will most likely fail as parsing heavily relies on earlier created actors!
		Param endPos: The bit position to stop parsing. -1 means parse till the end
		Param frameCount:  Number of frames to parse. 0 means parse all frames.
		*/
		void Parse(const uint32_t startPos = 0, int32_t endPos = -1, const uint32_t frameCount = 0);
	
		/*
		Gets a "stacktrace" of the last x elements of the parse log.
		Only returns a non-empty string when IncludeParseLog in ReplayFile.cpp 
		is set to true!
		*/
		std::string GetParseLog(size_t amount);

	protected:
		/*
		Precomputes values and fills tables to speed up parsing. See implementation of function
		to see which values are being precomputed
		*/
		void PrecomputeValues();

		/*
		Get the nearest keyframe to a frame. Nearing being the last keyframe before the given frame!
		*/
		const std::pair<const uint32_t, const KeyFrame> GetNearestKeyframe(uint32_t frame) const;

		/*
		Parses a header property
		*/
		const bool ParseProperty(const std::shared_ptr<Property>& currentProperty);

		/*
		Function which does an extra lookup for names that have no explicit archetype.
		E.g. VehiclePickup_Boost_TA has suffixes with their ID (e.g. _9) so we cannot do a simple
		table lookup, so we do some manual checks here.
		*/
		const std::shared_ptr<ClassNet>& GetClassnetByNameWithLookup(const std::string& name) const;
		
		/*
		Function to recursively find the prop_index beloning to a certain ID of a classnet.
		Will go through parent classes until it is found. Returns 0 if given id is not present in the classnet.
		*/
		const uint16_t GetPropertyIndexById(const std::shared_ptr<ClassNet>& cn, const int id) const;

		/*
		Get the maximum property id of a classnet. First call per classnet does a call to FindMaxPropertyId
		after which it is cached
		*/
		const uint16_t GetMaxPropertyId(ClassNet* cn);

		/*
		USE GetMaxPropertyId INSTEAD! This function is called inside GetMaxPropertyId function and the result is then cached, saving computation!
		Finds the maximum property id of a classnet. 
		*/
		const uint16_t FindMaxPropertyId(const ClassNet* cn, uint16_t maxProp) const;

		
	public:
		/*
		Returns true if the header map has the given property
		*/
		const bool HasProperty(const std::string& key) const;

		/*
		Returns true if the given objectId has an initial position that should be parsed
		*/
		const bool HasInitialPosition(const uint32_t name) const;

		/*
		Returns true if the given objectId has an initial rotation that should be parsed
		*/
		const bool HasRotation(const uint32_t name) const;

		/*
		Returns the property under given key from the header map casted to T. Will throw
		PropertyDoesNotExistException if the property is not present in the header map.
		Use HasProperty to check if the key is present in the map.
		*/
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