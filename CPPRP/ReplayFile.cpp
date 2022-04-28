#include "ReplayFile.h"
#include <fstream>
#include "./data/GameClasses.h"
#include "./data/NetworkData.h"
#include <set>
#include "./data/ArcheTypes.h"
#include "./generated/ClassExtensions.h"
#include "./exceptions/ParseException.h"
#include "./exceptions/ReplayException.h"
#include "CRC.h"
#include <functional>
#include "NetworkDataParsers.h"
#include "PropertyParser.h"

namespace CPPRP
{
#ifdef PARSELOG_ENABLED
	constexpr bool IncludeParseLog = true;
#else
	constexpr bool IncludeParseLog = false;
#endif
	constexpr uint32_t ParseLogSize = 100;

	ReplayFile::ReplayFile(std::filesystem::path path_) : path(path_)
	{
		
	}

	ReplayFile::ReplayFile(std::vector<char>& fileData)
	{
		this->data = fileData;
	}


	ReplayFile::~ReplayFile()
	{
	}

	const bool ReplayFile::Load()
	{
		if (!std::filesystem::exists(path))
			return false;
		std::ifstream file(path, std::ios::binary | std::ios::ate);

		const std::streamsize size = file.tellg();
		data.resize((size_t)size);
		file.seekg(0, std::ios::beg);

		if (file.bad())
			return false;
		return (bool)file.read(data.data(), size);
	}

	template<typename T>
	T ReadHeaderStruct(std::shared_ptr<CPPBitReader<BitReaderType>>& bitReader)
	{
		return bitReader->read<T>();
	}

	template<>
	KeyFrame ReadHeaderStruct(std::shared_ptr<CPPBitReader<BitReaderType>>& bitReader)
	{
		return KeyFrame {
			bitReader->read<float>(),		//Time
			bitReader->read<uint32_t>(),	//Frame
			bitReader->read<uint32_t>()	//File position
		};
	}

	template<>
	DebugString ReadHeaderStruct(std::shared_ptr<CPPBitReader<BitReaderType>>& bitReader)
	{
		return DebugString {
			bitReader->read<uint32_t>(),	//Time
			bitReader->read<std::string>(),	//Frame
			bitReader->read<std::string>()	//File position
		};
	}

	template<>
	ReplayTick ReadHeaderStruct(std::shared_ptr<CPPBitReader<BitReaderType>>& bitReader)
	{
		return ReplayTick {
			bitReader->read<std::string>(),	//Type
			bitReader->read<uint32_t>()		//Frame	
		};
	}
	template<>
	ClassIndex ReadHeaderStruct(std::shared_ptr<CPPBitReader<BitReaderType>>& bitReader)
	{
		return ClassIndex{
			bitReader->read<std::string>(),	//Class_name
			bitReader->read<uint32_t>()		//Index
		};
	}
	

	template<typename T>
	void ReadVector(std::shared_ptr<CPPBitReader<BitReaderType>>& bitReader, std::vector<T>& inVec)
	{
		const uint32_t vectorCount = bitReader->read<uint32_t>();
		if (vectorCount * sizeof(T) > bitReader->size) throw 0; //TODO: throw proper exception
		inVec.resize(vectorCount);
		for (uint32_t i = 0; i < vectorCount; ++i)
		{
			inVec[i] = ReadHeaderStruct<T>(bitReader);
		}
	}

	void ReplayFile::DeserializeHeader()
	{
		const size_t dataSizeBits = data.size() * 8;
		replayFile = std::make_shared<ReplayFileData>();
		fullReplayBitReader = std::make_shared<CPPBitReader<BitReaderType>>((const BitReaderType*)data.data(), dataSizeBits, replayFile);

		replayFile->header = {
			fullReplayBitReader->read<uint32_t>(),	//Size
			fullReplayBitReader->read<uint32_t>(),	//CRC
			fullReplayBitReader->read<uint32_t>(),	//engineVersion
			fullReplayBitReader->read<uint32_t>()	//licenseeVersion
		};

		if (replayFile->header.engineVersion >= 868 && replayFile->header.licenseeVersion >= 18)
		{
			replayFile->header.netVersion = fullReplayBitReader->read<uint32_t>();
		}

		//Reconstruct cause we got version info now,  find something better for this
		size_t bitPos = fullReplayBitReader->GetAbsoluteBitPosition();
		fullReplayBitReader = std::make_shared<CPPBitReader<BitReaderType>>((const BitReaderType*)data.data(), dataSizeBits, replayFile);
		fullReplayBitReader->skip(bitPos);

		replayFile->replayType = fullReplayBitReader->read<std::string>(); //Not sure what this is


		while (true) 
		{
			auto baseProperty = std::make_shared<Property>();
			const bool moreToParse = ParseProperty(baseProperty);
			if (!moreToParse)
			{
				break;
			}
			replayFile->properties[baseProperty->property_name] = baseProperty;
		}

		replayFile->body_size = fullReplayBitReader->read<uint32_t>();
		replayFile->crc2 = fullReplayBitReader->read<uint32_t>();

		ReadVector(fullReplayBitReader, replayFile->levels);
		ReadVector(fullReplayBitReader, replayFile->keyframes);

		const uint32_t netstreamCount = static_cast<uint32_t>(fullReplayBitReader->read<int32_t>());
		replayFile->netstream_data = data.data() + fullReplayBitReader->GetAbsoluteBytePosition(); //We know this is always aligned, so valid
		uint32_t test = netstreamCount * 8;
		fullReplayBitReader->skip(test);
		replayFile->netstream_size = netstreamCount;

		if (!fullReplayBitReader->canRead())
		{
			//Replay is corrupt
			const std::string exceptionText = "ReplayFile corrupt. header + netstream_size > filesize";
			throw GeneralParseException(exceptionText, *fullReplayBitReader);
		}

		ReadVector(fullReplayBitReader, replayFile->debugstrings);
		ReadVector(fullReplayBitReader, replayFile->replayticks);
		ReadVector(fullReplayBitReader, replayFile->replicated_packages);
		ReadVector(fullReplayBitReader, replayFile->objects);
		ReadVector(fullReplayBitReader, replayFile->names);
		ReadVector(fullReplayBitReader, replayFile->class_indices);
		

		const uint32_t classNetsCount = fullReplayBitReader->read<uint32_t>();
		replayFile->classnets.resize(classNetsCount);
		for (uint32_t i = 0; i < classNetsCount; ++i)
		{
			ClassNet cn = {
				fullReplayBitReader->read<int32_t>(),		//Index
				fullReplayBitReader->read<int32_t>(),		//Parent
				NULL,							//Parent class, not known yet
				fullReplayBitReader->read<int32_t>(),		//Id
				fullReplayBitReader->read<int32_t>(),		//Prop_indexes_size
				std::vector<PropIndexId>(),		//Empty propindexid array
				0,								//Max_prop_id
				std::vector<uint16_t>()			//Property_id_cache
			};

			const uint32_t newSize = cn.prop_indexes_size;
			cn.prop_indexes.resize(newSize);
			for (uint32_t j = 0; j < newSize; ++j)
			{
				cn.prop_indexes[j] = (
					PropIndexId{
						fullReplayBitReader->read<int32_t>(),	//Prop_index
						fullReplayBitReader->read<int32_t>()	//Prop_id
					});
			}
			std::shared_ptr<ClassNet> classNet = std::make_shared<ClassNet>(cn);
			replayFile->classnets[i] = (classNet);

			//Set parent class if exists
			for (int32_t k = (int32_t)i - 1; k >= 0; --k)
			{
				if (replayFile->classnets[i]->parent == replayFile->classnets[k]->id)
				{
					replayFile->classnets[i]->parent_class = replayFile->classnets[k];
					break;
				}
			}
		}
		if (replayFile->header.netVersion >= 10)
		{
			fullReplayBitReader->read<int32_t>();
		}
		header = replayFile->header;
		this->FixParents();

	}

	const bool ReplayFile::VerifyCRC(CrcCheck verifyWhat)
	{
		if ((verifyWhat & CRC_Both) == 0) return false; //User supplied invalid value, < 0 or >= 4

		const size_t dataSizeBits = data.size() * 8;
		//Replay not loaded, less than 8 bytes
		if (dataSizeBits < sizeof(uint32_t) * 2 * 8)
		{
			return false;
		}
		CPPBitReader<BitReaderType> bitReader((const BitReaderType*)data.data(), 
			dataSizeBits, replayFile, 0, 0, 0);
		const uint32_t headerSize = bitReader.read<uint32_t>();
		const uint32_t headerReadCrc = bitReader.read<uint32_t>();

		//File is lying about its size
		if (bitReader.GetAbsoluteBytePosition() + headerSize > data.size())
		{
			return false;
		}

		constexpr uint32_t CRC_SEED = 0xEFCBF201;
		if (verifyWhat & CRC_Header)
		{
			/*const uint32_t headerCalculatedCRC = CalculateCRC(data,
				static_cast<size_t>(bitReader.GetAbsoluteBytePosition()), 
				static_cast<size_t>(headerSize), CRC_SEED);*/
			const uint32_t headerCalculatedCRC2 = CalculateCRC_SB16(*reinterpret_cast<std::vector<uint8_t>*>(&data),
				static_cast<size_t>(bitReader.GetAbsoluteBytePosition()),
				static_cast<size_t>(headerSize), CRC_SEED);
			//std::cout << "headerCalculatedCRC==headerCalculatedCRC2" << (headerCalculatedCRC == headerCalculatedCRC2 ? "true" : "false") << "\n";
			const bool result = headerCalculatedCRC2 == headerReadCrc;
			//If only verify header, or if already failed here
			if (!(verifyWhat & CRC_Body) || !result)
			{
				return result;
			}
		}
		bitReader.skip(headerSize * 8);

		if (bitReader.GetAbsoluteBytePosition() + 2 > data.size())
		{
			//Won't be able to read body size and crc, so false
			return false;
		}


		const uint32_t bodySize = bitReader.read<uint32_t>();
		const uint32_t bodyReadCrc = bitReader.read<uint32_t>();
		if (bitReader.GetAbsoluteBytePosition() + bodySize > data.size())
		{
			return false;
		}

		/*const uint32_t bodyCalculatedCRC = CalculateCRC(data, 
			static_cast<size_t>(bitReader.GetAbsoluteBytePosition()), 
			static_cast<size_t>(bodySize), CRC_SEED);*/

		//cast is ugly but works, fix later
		const uint32_t bodyCalculatedCRC2 = CalculateCRC_SB16(*reinterpret_cast<std::vector<uint8_t>*>(&data),
			static_cast<size_t>(bitReader.GetAbsoluteBytePosition()),
			static_cast<size_t>(bodySize), CRC_SEED);
		//std::cout << "headerCalculatedCRC==headerCalculatedCRC2" << (bodyCalculatedCRC == bodyCalculatedCRC2 ? "true" : "false") << "\n";

		return bodyReadCrc == bodyCalculatedCRC2;
	}

	void ReplayFile::PreprocessTables()
	{
		const size_t size = replayFile->objects.size();
		for(uint32_t i = 0; i < size; ++i)
		{
			objectToId[replayFile->objects.at(i)] = i;
			//printf("[%i] %s", i, replayFile->objects.at(i).c_str());
		}
	}
	
	class Timer
{
private:
	std::chrono::time_point<std::chrono::steady_clock> start;
	std::chrono::time_point<std::chrono::steady_clock> end;
	bool ended = false;
	std::string name;
public:
	Timer(std::string timerName) : name(timerName)
	{
		start = std::chrono::steady_clock::now();
	}

	void Stop()
	{
		end = std::chrono::steady_clock::now();
		ended = true;
	}

	~Timer()
	{
		if (!ended) Stop();
		std::cout << name << " duration in microseconds : "
			<< std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
			<< "\n";

		/*std::cout << "Elapsed time in milliseconds : "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
			<< " ms\n";*/
	}
};

	void ReplayFile::FixParents()
	{
		//Timer t("Preprocessing");
		for (uint32_t i = 0; i < replayFile->classnets.size(); ++i)
		{
			const uint32_t index = replayFile->classnets.at(i)->index;
			const std::string objectName = replayFile->objects.at(index);
			if (classnetMap.find(objectName) != classnetMap.end())
			{
				auto newClassnet = replayFile->classnets.at(i);
				auto originalClassnet = classnetMap[objectName];
				//Kind of a cheap hack, just insert map with higher ID properties to start of array so we don't have to find and replace existing ones
				//This way the property index cacher will find these newer properties before the old ones thus making the old ones obsolete
				originalClassnet->prop_indexes.insert(originalClassnet->prop_indexes.begin(), newClassnet->prop_indexes.begin(), newClassnet->prop_indexes.end());
				
			}
			else
			{
				classnetMap[objectName] = replayFile->classnets.at(i);
			}
		}

		for (auto& archetypeMapping : archetypeMap)
		{
			const auto found = classnetMap.find(archetypeMapping.first);
			if (found == classnetMap.end())
			{
				continue;
			}
			std::shared_ptr<ClassNet>& headClassnet = found->second;
			for (auto& archetype : archetypeMapping.second)
			{
				classnetMap[archetype] = headClassnet;
			}
		}

		//TODO: derive this from gameclasses
		for (const auto& [child_name, parent_name] : class_extensions)
		{
			std::shared_ptr<ClassNet> childClass = GetClassnetByNameWithLookup(child_name);
			std::shared_ptr<ClassNet> parentClass = GetClassnetByNameWithLookup(parent_name);
			if (parentClass != nullptr && childClass != nullptr && (childClass->parent_class == nullptr || (childClass->parent_class->index != parentClass->index)))
			{
				childClass->parent_class = parentClass;
			}
		}

		for (const auto& cn : replayFile->classnets)
		{
			uint16_t i = 0;
			uint16_t result = GetPropertyIndexById(cn, i);
			while (result != 0)
			{
				cn->property_id_cache.push_back(result);
				result = GetPropertyIndexById(cn, ++i);
			}
		}

		

		const size_t objectsSize = replayFile->objects.size();
		parseFunctions.resize(objectsSize);
		createFunctions.resize(objectsSize);
		for(size_t i = 0; i < objectsSize; i++)
		{
			const std::string& name = replayFile->objects.at(i);
			auto found = parsePropertyFuncs.find(name);
			if(found != parsePropertyFuncs.end())
			{
				parseFunctions[i] = found->second;
			}

			auto found2 = createObjectFuncs.find(name);
			if(found2 != createObjectFuncs.end())
			{
				createFunctions[i] = found2->second;
			}
		}

		//printf("Ab");
		const std::vector<std::string> position_names = {
			"TAGame.CrowdActor_TA", "TAGame.VehiclePickup_Boost_TA", "TAGame.InMapScoreboard_TA",
			"TAGame.BreakOutActor_Platform_TA", "Engine.WorldInfo", "TAGame.HauntedBallTrapTrigger_TA",
			"Engine.KActor", "TAGame.CrowdManager_TA", "TAGame.PlayerStart_Platform_TA"
		};
		const std::vector<std::string> rotation_names = {
			"TAGame.Ball_TA", "TAGame.Car_TA", "TAGame.Car_KnockOut_TA", "TAGame.Car_Season_TA",
			"TAGame.Ball_Breakout_TA", "TAGame.Ball_Haunted_TA", "TAGame.Ball_God_TA"
		};
		//printf("Preprocess\n");
		for(size_t i = 0; i < objectsSize; i++)
		{
			const std::string& name = replayFile->objects.at(i);
			if(std::find(position_names.begin(), position_names.end(), name) != position_names.end())
			{
				positionIDs.push_back(i);
				//printf("Position %i %s\n", i, name.c_str());
			}
			if(std::find(rotation_names.begin(), rotation_names.end(), name) != rotation_names.end())
			{
				rotationIDs.push_back(i);
				//printf("Rotation %i %s\n", i, name.c_str());
			}

			classnetCache.push_back(GetClassnetByNameWithLookup(name));
			if(classnetCache[i])
				GetMaxPropertyId(classnetCache[i].get());
		}
//printf("Aa");
		const std::vector<std::string> attributeNames = 
		{
			"TAGame.ProductAttribute_UserColor_TA",
			"TAGame.ProductAttribute_Painted_TA",
			"TAGame.ProductAttribute_TeamEdition_TA",
			"TAGame.ProductAttribute_SpecialEdition_TA",
			"TAGame.ProductAttribute_TitleID_TA"
		};
		//printf("A");
		PreprocessTables();
		for(size_t i = 0; i < attributeNames.size(); ++i)
		{
			//printf("B");
			const uint32_t attributeID = objectToId[attributeNames.at(i)];
			attributeIDs.push_back(attributeID);
			//printf("[%i] %s", attributeID, attributeNames.at(i).c_str());
		}
		//attributeIDs



		//printf("Done preprocessing\n");
	}

	std::string ReplayFile::GetParseLog(size_t amount)
	{
		std::stringstream ss;
		ss << "Parse log: ";
		for (size_t i = amount > parseLog.size() ?  0 : parseLog.size() - amount; i < parseLog.size(); i++)
		{
			ss <<"\n\t" + parseLog.at(i);
		}
		return ss.str();
	}

	void ReplayFile::Parse(const uint32_t startPos, int32_t endPos, const uint32_t frameCount)
	{
		/*
		Replay is corrupt, no way we'll parse this correctly
		Parsing header is fine though, so only throw this in parse
		*/
		if (replayFile->header.engineVersion == 0 &&
			replayFile->header.licenseeVersion == 0 &&
			replayFile->header.netVersion == 0)
		{
			throw InvalidVersionException(0, 0, 0);
		}
		if (endPos < 0)
		{
			endPos = replayFile->netstream_size * 8;
		}

		
		CPPBitReader<BitReaderType> networkReader((BitReaderType*)(replayFile->netstream_data), static_cast<size_t>(endPos), replayFile);

		//FILE* fp = fopen(("./json/" + fileName + ".json").c_str(), "wb");

		try
		{
			int first = 0;
			//char writeBuffer[65536 * 5];
			//rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

			//rapidjson::Writer<rapidjson::FileWriteStream> writer;

			networkReader.skip(startPos);

			//Get some const data we're gonna need repeatedly during parsing and store for performance reasons
			const uint32_t numFrames = frameCount > 0 ? frameCount : static_cast<uint32_t>(GetProperty<int32_t>("NumFrames"));

			const int32_t maxChannels = GetProperty<int32_t>("MaxChannels");
			const bool isLan = GetProperty<std::string>("MatchType") == "Lan";

			const size_t namesSize = replayFile->names.size();
			const size_t objectsSize = replayFile->objects.size();

			const uint32_t engineVersion = replayFile->header.engineVersion;
			const uint32_t licenseeVersion = replayFile->header.licenseeVersion;
			const bool parseNameId = engineVersion > 868 || (engineVersion == 868 && licenseeVersion >= 20) || (engineVersion == 868 && licenseeVersion >= 14 && !isLan) || ((engineVersion == 868 && licenseeVersion == 17 && isLan));
			networkReader.attributeIDs = attributeIDs;

			frames.resize(numFrames);
			std::vector<uint32_t> updatedProperties;
			updatedProperties.reserve(100);
			uint32_t currentFrame = 0;
			while (
				#ifndef PARSE_UNSAFE
				networkReader.canRead() && 
				#endif
				currentFrame < numFrames)
			{
				
				Frame& f = frames[currentFrame];
				f.frameNumber = currentFrame;
				f.position = networkReader.GetAbsoluteBitPosition();
				f.time = networkReader.read<float>();
				f.delta = networkReader.read<float>();
				if constexpr (IncludeParseLog)
				{
					parseLog.push_back("New frame " + std::to_string(currentFrame) + " at " + std::to_string(f.time) + ", pos " + std::to_string(f.position));
				}
				#ifndef PARSE_UNSAFE
				if (f.time < 0 || f.delta < 0
					|| (f.time > 0 && f.time < 1E-10)
					|| (f.delta > 0 && f.delta < 1E-10))
				{
					std::string exceptionText = "Frame time incorrect (parser at wrong position)\n" + GetParseLog(ParseLogSize);
					throw GeneralParseException(exceptionText, networkReader);
				}
				#endif

				for (const auto& newFrame : newFrameCallbacks)
				{
					newFrame(f);
				}

				//While there are actors in buffer (this frame)
				while (networkReader.read<bool>())
				{
					const uint32_t actorId = networkReader.readBitsMax<uint32_t>(maxChannels);
					if (networkReader.read<bool>())
					{
						//Is new state
						if (networkReader.read<bool>())
						{

							uint32_t name_id;
							if (parseNameId)
							{
								const uint32_t nameId = networkReader.read<uint32_t>();
								name_id = nameId;
								#ifndef PARSE_UNSAFE
								if (nameId > namesSize)
								{
									throw GeneralParseException("nameId not in replayFile->objects " + std::to_string(nameId) + " > " + std::to_string(namesSize), networkReader);
								}
								#endif
							}
							else
							{
								name_id = 0;
							}
							const bool unknownBool = networkReader.read<bool>();
							const uint32_t typeId = networkReader.read<uint32_t>();

							#ifndef PARSE_UNSAFE
							if (typeId > objectsSize)
							{
								throw GeneralParseException("Typeid not in replayFile->objects " + std::to_string(typeId) + " > " + std::to_string(objectsSize), networkReader);
							}
							#endif

							//const std::string typeName = replayFile->objects.at(typeId);
							//printf("%s:%i\n", typeName.c_str(), typeId);
							auto classNet = classnetCache[typeId];//GetClassnetByNameWithLookup(typeName);

							#ifndef PARSE_UNSAFE
							if (classNet == nullptr)
							{
								const std::string typeName = replayFile->objects.at(typeId);
								throw GeneralParseException("Classnet for " + typeName + " not found", networkReader);
							}
							#endif



							const uint32_t classId = classNet->index;
							
							//auto found = createObjectFuncs.find(className);

							const auto& funcPtr = createFunctions[classId];
							#ifndef PARSE_UNSAFE
							if (funcPtr == nullptr)
							{
								const std::string className = replayFile->objects.at(classId);
								std::cout << "Could not find class " << className << "\n";
								throw GeneralParseException("Could not find class " + className , networkReader);
								return;
							}
							#endif
							std::shared_ptr<Engine::Actor> actorObject = funcPtr();
							//ActorStateData asd =
							if constexpr (IncludeParseLog)
							{
								const std::string typeName = replayFile->objects.at(typeId);
								const std::string className = replayFile->objects.at(classId);
								parseLog.push_back("New actor for " + typeName + ", classname " + className);
							}
							//printf("%s\n", className.c_str());
							if (HasInitialPosition(classId))
							{
								actorObject->Location = networkReader.read<Vector3I>();
								//printf("has pos\n");
							}
							if (HasRotation(classId))
							{
								actorObject->Rotation = networkReader.read<Rotator>();
								//printf("has rot\n");
							}
							//printf("---\n");
							//ActorStateData asd =  { std::move(actorObject), classNet, actorId, name_id, classId };
							//actorStates[actorId] = asd;
							auto [inserted, insert_result] = actorStates.emplace(actorId, ActorStateData{ std::move(actorObject), classNet, actorId, name_id, classId, typeId });
							for(const auto& createdFunc : createdCallbacks)
							{
								createdFunc(inserted->second);
							}
						}
						else //Is existing state
						{
							ActorStateData& actorState = actorStates[actorId];
							updatedProperties.clear();
							//While there's data for this state to be updated
							while (networkReader.read<bool>())
							{
								const uint16_t maxPropId = GetMaxPropertyId(actorState.classNet.get());
								const uint32_t propertyId = networkReader.readBitsMax<uint32_t>(maxPropId + 1);
								const uint32_t propertyIndex = actorState.classNet->property_id_cache[propertyId];

								if constexpr (IncludeParseLog)
								{
									char buff[1024];
									snprintf(buff, sizeof(buff), "Calling parser for %s (%i, %i, %s)", replayFile->objects[propertyIndex].c_str(), propertyIndex, actorId, actorState.nameId >= namesSize ? "unknown" : replayFile->names[actorState.nameId].c_str());
									parseLog.emplace_back(buff);
								}

								 {


									updatedProperties.push_back(propertyIndex);
									const auto& funcPtr = parseFunctions[propertyIndex];
									/*if (b)
									{
										printf("Calling parser for %s (%i, %i, %s)\n", replayFile->objects[propertyIndex].c_str(), propertyIndex, actorId, actorState.nameId >= namesSize ? "unknown" : replayFile->names[actorState.nameId].c_str());
									}*/

#ifndef PARSE_UNSAFE
									if (funcPtr == nullptr)
									{
										const std::string& objName = replayFile->objects[propertyIndex];
										//std::cout << "Property " << objName << " is undefined\n";

										std::string exceptionText = "Property " + objName + " is undefined\n" + GetParseLog(ParseLogSize);
										throw GeneralParseException(exceptionText, networkReader);

									}
#endif
									funcPtr(actorState.actorObject.get(), networkReader);
								}
							}
							for(const auto& updateFunc : updatedCallbacks)
							{
								updateFunc(actorState, updatedProperties);
							}
						}
					}
					else
					{
						ActorStateData& actorState = actorStates[actorId];
						for(const auto& deleteFunc : actorDeleteCallbacks)
						{
							deleteFunc(actorState);
						}
						actorStates.erase(actorId);
					}
				}
				
				for (const auto& tick : tickables)
				{
					tick(f, actorStates);
				}
				currentFrame++;
			}
			if (numFrames != currentFrame)
			{
				throw GeneralParseException("Number of expected frames does not match number of parsed frames. Expected: " + std::to_string(numFrames) + ", parsed: " + std::to_string(currentFrame), networkReader);
			}
			if (networkReader.size - networkReader.GetAbsoluteBitPosition() > 8192)
			{
				//Unsure how big RL buffer sizes are, 8192 seems fair
				throw GeneralParseException("Not enough bytes parsed! Expected ~" + std::to_string(networkReader.size) + ", parsed: " + std::to_string(networkReader.GetAbsoluteBitPosition()) + ". Diff(" + std::to_string(networkReader.size - networkReader.GetAbsoluteBitPosition()) + ")", networkReader);
			}
		}
		catch (...)
		{
			printf("Caught ex\n");
			//Parse(startPos, endPos);
			//fclose(fp);
			throw;
		}

	}

	const bool ReplayFile::HasInitialPosition(const uint32_t id) const
	{
		return std::find(positionIDs.begin(), positionIDs.end(), id) == positionIDs.end();
		/*
			const uint32_t classId = classNet->index;
							const std::string className = replayFile->objects.at(classId);
							//auto found = createObjectFuncs.find(className);

							const auto& funcPtr = createFunctions[classId];
		*/


		// return !(name.compare("TAGame.CrowdActor_TA") == 0
		// 	|| name.compare("TAGame.VehiclePickup_Boost_TA") == 0
		// 	|| name.compare("TAGame.InMapScoreboard_TA") == 0
		// 	|| name.compare("TAGame.BreakOutActor_Platform_TA") == 0
		// 	|| name.compare("Engine.WorldInfo") == 0
		// 	|| name.compare("TAGame.HauntedBallTrapTrigger_TA") == 0
		// 	|| name.compare("Engine.KActor") == 0
		// 	|| name.compare("TAGame.CrowdManager_TA") == 0);
	}

	const bool ReplayFile::HasRotation(const uint32_t id) const
	{
		return std::find(rotationIDs.begin(), rotationIDs.end(), id) != rotationIDs.end();
		// return name.compare("TAGame.Ball_TA") == 0
		// 	|| name.compare("TAGame.Car_TA") == 0
		// 	|| name.compare("TAGame.Car_Season_TA") == 0
		// 	|| name.compare("TAGame.Ball_Breakout_TA") == 0
		// 	|| name.compare("TAGame.Ball_Haunted_TA") == 0
		// 	|| name.compare("TAGame.Ball_God_TA") == 0;
	}

	const std::pair<const uint32_t, const KeyFrame> ReplayFile::GetNearestKeyframe(uint32_t frame) const
	{
		if (replayFile->keyframes.size() == 0)
		{
			return std::make_pair<const uint32_t, KeyFrame>(0, { 0.f, 0,0 });
		}

		const size_t size = replayFile->keyframes.size();
		size_t currentKeyframeIndex = 0;
		for (currentKeyframeIndex; currentKeyframeIndex < size; ++currentKeyframeIndex)
		{
			if (replayFile->keyframes.at(currentKeyframeIndex).frame > frame)
			{
				break;
			}
		}

		const KeyFrame nearestKeyFrame = replayFile->keyframes.at(currentKeyframeIndex);
		const uint32_t frameNumber = nearestKeyFrame.frame;
		return std::make_pair(frameNumber, nearestKeyFrame);
	}

	const bool ReplayFile::ParseProperty(const std::shared_ptr<Property>& currentProperty)
	{
		currentProperty->property_name = fullReplayBitReader->read<std::string>();
		if (currentProperty->property_name.compare("None") == 0) //We're done parsing this prop
		{
			return false;
		}
		currentProperty->property_type = fullReplayBitReader->read<std::string>();
		const uint32_t propertySize = fullReplayBitReader->read<uint32_t>();
		const uint32_t idk = fullReplayBitReader->read<uint32_t>();

		//Not sure why I'm doing these micro optimizations here, kinda hurts readability and its only like a nanosecond
		switch (currentProperty->property_type[0])
		{
		case 'N':
		{
			if (currentProperty->property_type[1] == 'o') //Type is "None"
			{
				return false;
			}
			else //Type is "Name"
			{
				currentProperty->value = fullReplayBitReader->read<std::string>();
			}
		}
		break;
		case 'I': //IntProperty
		{
			currentProperty->value = fullReplayBitReader->read<int32_t>();
		}
		break;
		case 'S': //StringProperty
		{
			currentProperty->value = fullReplayBitReader->read<std::string>();
		}
		break;
		case 'B':
		{
			if (currentProperty->property_type[1] == 'y') //Type is "ByteProperty"
			{
				EnumProperty ep;
				ep.type = fullReplayBitReader->read<std::string>();
				if (ep.type.compare("OnlinePlatform_Steam") == 0 || ep.type.compare("OnlinePlatform_PS4") == 0) //for some reason if string is this, there's no value.
				{
					ep.value = "";
					//fullReplayBitReader->read<uint32_t>();
				}
				else
				{
					ep.value = fullReplayBitReader->read<std::string>(); //Value
				}
				currentProperty->value = ep;
			}
			else //Type is "BoolProperty", but unlike network data, is stored as entire byte
			{
				if (replayFile->header.engineVersion == 0 &&
					replayFile->header.licenseeVersion == 0 &&
					replayFile->header.netVersion == 0)
				{
					currentProperty->value = fullReplayBitReader->read<uint32_t>();
				}
				else
				{
					currentProperty->value = fullReplayBitReader->read<uint8_t>();
				}

			}
		}
		break;
		case 'Q': //QWordProperty
		{
			currentProperty->value = fullReplayBitReader->read<uint64_t>();
		}
		break;
		case 'F': //FloatProperty
		{
			currentProperty->value = fullReplayBitReader->read<float>();
		}
		break;
		case 'A': //ArrayProperty
		{
			const int32_t count = fullReplayBitReader->read<int32_t>();
			std::vector<std::unordered_map<std::string, std::shared_ptr<Property>>> properties;
			properties.resize(count);

			for (int32_t i = 0; i < count; ++i)
			{
				std::unordered_map<std::string, std::shared_ptr<Property>> props;
				while (true)
				{
					auto baseProperty = std::make_shared<Property>();
					const bool moreToParse = ParseProperty(baseProperty);
					if (!moreToParse)
					{
						break;
					}
					props[baseProperty->property_name] = baseProperty;
				}
				properties[i] = props;
			}
			currentProperty->value = properties;
		}
		break;
		default: //Die
			//assert(1 == 2);
			break;
		}

		return true;
	}

	const std::shared_ptr<ClassNet>& ReplayFile::GetClassnetByNameWithLookup(const std::string & name) const
	{
		static std::shared_ptr<ClassNet> notfound = std::shared_ptr<ClassNet>(nullptr);
		static const std::map<std::string, std::string> classnetNamesLookups = {
			{"CrowdActor_TA", "TAGame.CrowdActor_TA"},
			{"VehiclePickup_Boost_TA", "TAGame.VehiclePickup_Boost_TA"},
			{"CrowdManager_TA", "TAGame.CrowdManager_TA"},
			{"BreakOutActor_Platform_TA", "TAGame.BreakOutActor_Platform_TA"},
			{"WorldInfo", "Engine.WorldInfo"},
			{"Archetypes.Teams.TeamWhite", "TAGame.Team_Soccar_TA"},
			{"PersistentLevel.KActor", "Engine.KActor"},
			{"PlayerStart_Platform_TA", "TAGame.PlayerStart_Platform_TA"},
			{"InMapScoreboard_TA", "TAGame.InMapScoreboard_TA"}
		};
		for (const auto& kv : classnetNamesLookups)
		{
			if (name.find(kv.first) != std::string::npos)
			{
				auto found = classnetMap.find(kv.second); 
				if (found == classnetMap.end()) 
				{
					return notfound; 
				}
				return (*found).second;
			}
		}
		
		auto found = classnetMap.find(name);
		if (found == classnetMap.end())
			return notfound;
		return found->second;
	}

	const uint16_t ReplayFile::GetPropertyIndexById(const std::shared_ptr<ClassNet>& cn, const int id) const
	{
		for (int i = 0; i < cn->prop_indexes_size; i++)
		{
			if (cn->prop_indexes[i].prop_id == id)
			{
				return cn->prop_indexes[i].prop_index;
			}
		}
		if (cn->parent_class)
		{
			const std::shared_ptr<ClassNet>& parentNet = cn->parent_class;
			if (parentNet == NULL) //Is root?
			{
				return cn->index;
			}
			return this->GetPropertyIndexById(parentNet, id);
		}
		return 0;
	}

	const uint16_t ReplayFile::GetMaxPropertyId(ClassNet* cn)
	{
		if (cn == nullptr)
		{
			throw std::runtime_error("ClassNet is nullptr");
		}

		if (cn->max_prop_id == 0)
		{
			cn->max_prop_id = FindMaxPropertyId(cn, 0);
		}
		return cn->max_prop_id;
	}

	const uint16_t ReplayFile::FindMaxPropertyId(const ClassNet* cn, uint16_t maxProp) const
	{
		if (cn == nullptr)
		{
			return maxProp;
		}

		for (int32_t i = 0; i < cn->prop_indexes_size; ++i)
		{
			if (cn->prop_indexes[i].prop_id > maxProp)
			{
				maxProp = cn->prop_indexes[i].prop_id;
			}
		}
		if (cn->parent_class)
		{
			return FindMaxPropertyId(cn->parent_class.get(), maxProp);
		}
		return maxProp;
	}

	const bool ReplayFile::HasProperty(const std::string & key) const
	{
		return replayFile->properties.find(key) != replayFile->properties.end();
	}
}
