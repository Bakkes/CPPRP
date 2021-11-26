#pragma once
#pragma comment(lib, "CPPRP.lib")
#include "../CPPRP/ReplayFile.h"
#include "SerializeFunctions.h"
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <algorithm>
#include <map>
#include <filesystem>
#include <queue>
#include <unordered_map>
#include <fstream>
#include "OptionsParser.h"
#include "rapidjson/filewritestream.h"
#undef max


//static int a = serializer.Test();


struct UpdateData
{
	const CPPRP::ActorStateData asd;
	const std::vector<uint32_t> props;
};

template<typename Writer>
void SerializeProp(Writer& writer, const std::string& name, std::shared_ptr<CPPRP::Property>& currentProperty, const bool specialByteProp)
{

	writer.String(name.c_str());
	switch (currentProperty->property_type[0])
	{
		case 'N':
		{
			if (currentProperty->property_type[1] == 'o') //Type is "None"
			{
				writer.Null();
			}
			else //Type is "Name"
			{
				writer.String(std::get<std::string>(currentProperty->value).c_str());
			}
		}
		break;
		case 'I': //IntProperty
		{
			writer.Int(std::get<int32_t>(currentProperty->value));
		}
		break;
		case 'S': //StringProperty
		{
			writer.String(std::get<std::string>(currentProperty->value).c_str());
		}
		break;
		case 'B':
		{
			if (currentProperty->property_type[1] == 'y') //Type is "ByteProperty"
			{
				CPPRP::EnumProperty ep = std::get<CPPRP::EnumProperty>(currentProperty->value);
				writer.StartObject();
				writer.String("Type");
				writer.String(ep.type.c_str());
				writer.String("Value");
				writer.String(ep.value.c_str());
				writer.EndObject();
			}
			else //Type is "BoolProperty", but unlike network data, is stored as entire byte
			{
				if (specialByteProp)
				{
					writer.Int(std::get<uint32_t>(currentProperty->value));
				}
				else
				{
					writer.Int(std::get<uint8_t>(currentProperty->value));
				}

			}
		}
		break;
		case 'Q': //QWordProperty
		{
			writer.String(std::to_string(std::get<uint64_t>(currentProperty->value)));
		}
		break;
		case 'F': //FloatProperty
		{
			writer.Double(std::get<float>(currentProperty->value));
		}
		break;
		case 'A': //ArrayProperty
		{
			auto temp = (std::get<std::vector<std::unordered_map<std::string, std::shared_ptr<CPPRP::Property>>>>(currentProperty->value));

			writer.StartArray();
			for (auto& wot : temp)
			{
				writer.StartObject();
				for (auto& wot2 : wot)
				{
					SerializeProp(writer, wot2.first, wot2.second, specialByteProp);
				}
				writer.EndObject();
			}
			writer.EndArray();
		}
		break;
		default: //Die
			writer.Null();
			//assert(1 == 2);
			break;
	}
}

template<typename Writer>
int SerializeReplay(Writer& writer, const std::shared_ptr<CPPRP::ReplayFile>& replayFile, const bool parseBody)
{
	/*replayFile->Parse();
	return 0;*/
	auto serializer = CPPRP::JSON::Serializer<Writer>();
	std::vector<CPPRP::ActorStateData> createdActorsThisTick;
	std::vector<CPPRP::ActorStateData> deletedActorsThisTick;
	std::vector<UpdateData> updatedActorsThisTick;
	const auto headerData = replayFile->replayFile;
	writer.StartObject();
	writer.String("Replay");
	writer.StartObject();
	writer.String("Header");
	writer.StartObject();

	writer.String("EngineVersion");
	writer.Uint(headerData->header.engineVersion);
	writer.String("LicenseeVersion");
	writer.Uint(headerData->header.licenseeVersion);
	writer.String("NetVersion");
	writer.Uint(headerData->header.netVersion);
	writer.String("CRC");
	writer.Uint(headerData->header.crc);
	writer.String("Size");
	writer.Uint(headerData->header.size);
	writer.String("ReplayType");
	writer.String(headerData->replayType.c_str());

	writer.String("Properties");
	const bool useSpecialByteProp = headerData->header.engineVersion == 0 &&
		headerData->header.licenseeVersion == 0 &&
		headerData->header.netVersion == 0;
	writer.StartObject();
	for (auto prop : headerData->properties)
	{
		SerializeProp(writer, prop.first, prop.second, useSpecialByteProp);
	}
	writer.EndObject();

	writer.String("Keyframes");
	writer.StartArray();

	for (auto kv : headerData->keyframes)
	{
		writer.StartObject();
		writer.String("Time");
		writer.Double(kv.time);
		writer.String("Frame");
		writer.Uint(kv.frame);
		writer.String("Filepos");
		writer.Uint(kv.filepos);
		writer.EndObject();
	}
	writer.EndArray();

	writer.String("ReplayLog");
	writer.StartArray();

	for (auto kv : headerData->debugstrings)
	{
		writer.StartObject();
		writer.String("Frame");
		writer.Int(kv.frame);
		writer.String("Key");
		writer.String(kv.key);
		writer.String("Value");
		writer.String(kv.value);
		writer.EndObject();
	}
	writer.EndArray();


	writer.EndObject();

	writer.String("Body");

	replayFile->createdCallbacks.push_back([&](const CPPRP::ActorStateData& asd)
		{
			createdActorsThisTick.push_back(asd);
		});
	replayFile->actorDeleteCallbacks.push_back([&](const CPPRP::ActorStateData& asd)
		{
			deletedActorsThisTick.push_back(asd);
		});

	replayFile->updatedCallbacks.push_back([&](const CPPRP::ActorStateData& asd, const std::vector<uint32_t>& props)
		{
			updatedActorsThisTick.push_back({ asd, props });
		});

	replayFile->tickables.push_back([&](const CPPRP::Frame frame, const std::unordered_map<uint32_t, CPPRP::ActorStateData>& actorStats)
		{
			writer.StartObject();
			writer.String("Frame");
			writer.Uint(frame.frameNumber);

			writer.String("Time");
			writer.Double(frame.time);

			writer.String("Delta");
			writer.Double(frame.delta);

			writer.String("Created");
			writer.StartArray();
			for (auto& created : createdActorsThisTick)
			{
				writer.StartObject();
				writer.String("Id");
				writer.Uint(created.actorId);

				std::string name = replayFile->replayFile->names.at(created.nameId);
				std::string className = replayFile->replayFile->objects.at(created.classNameId);
				writer.String("Name");
				writer.String(name);

				writer.String("ClassName");
				writer.String(className);
				bool hasInitialPosition = replayFile->HasInitialPosition(created.nameId);
				writer.String("HasInitialPosition");
				writer.Bool(hasInitialPosition);
				if (hasInitialPosition)
				{
					writer.String("InitialPosition");
					CPPRP::JSON::Serialize<Writer>(writer, created.actorObject->Location);
				}

				bool hasInitialRotation = replayFile->HasRotation(created.nameId);
				writer.String("HasInitialRotation");
				writer.Bool(hasInitialRotation);
				if (hasInitialRotation)
				{
					writer.String("InitialRotation");
					CPPRP::JSON::Serialize<Writer>(writer, created.actorObject->Rotation);
				}
				writer.EndObject();
			}
			writer.EndArray();

			writer.String("Deleted");
			writer.StartArray();
			for (auto& deleted : deletedActorsThisTick)
			{
				writer.Uint(deleted.actorId);
			}
			writer.EndArray();


			writer.String("Updated");
			writer.StartArray();
			for (auto& updated : updatedActorsThisTick)
			{
				writer.StartObject();
				writer.String("Id");
				writer.Uint(updated.asd.actorId);
				writer.String("UpdatedProperties");
				writer.StartArray();

				for (auto& updatedProp : updated.props)
				{
					writer.StartObject();

					//std::string objectName = replayFile->replayFile->objects[updatedProp];
					std::shared_ptr<CPPRP::Engine::Actor> a = updated.asd.actorObject;
					serializer.writerFuncs[updatedProp](writer, a);
					writer.EndObject();
				}

				writer.EndArray();
				writer.EndObject();
			}
			writer.EndArray();
			writer.EndObject();
			createdActorsThisTick.clear();
			updatedActorsThisTick.clear();
			deletedActorsThisTick.clear();
		});
	writer.StartArray();
	try
	{
		if (parseBody)
		{
			replayFile->PreprocessTables();
			const size_t objectsSize = replayFile->replayFile->objects.size();
			serializer.writerFuncs.resize(objectsSize);
			for (size_t i = 0; i < objectsSize; i++)
			{
				const std::string& name = replayFile->replayFile->objects.at(i);
				auto found = serializer.writerFuncMap.find(name);
				if (found != serializer.writerFuncMap.end())
				{
					serializer.writerFuncs[i] = found->second;
				}
			}


			replayFile->Parse();
		}
	}
	catch (const CPPRP::InvalidVersionException& e)
	{
		std::cerr << "InvalidVersionException: " << e.what() << " on file " << replayFile->path << "\n";
		return 1;
	}
	catch (const CPPRP::PropertyDoesNotExistException& e)
	{
		std::cerr << "PropertyDoesNotExistException: " << e.what() << " on file " << replayFile->path << "\n";
		return 1;
	}
	catch (const CPPRP::AttributeParseException<uint32_t>& e)
	{
		std::cerr << "AttributeParseException: " << e.what() << " on file " << replayFile->path << "\n";
		return 1;
	}
	catch (const CPPRP::GeneralParseException<uint32_t>& e)
	{
		std::cerr << "GeneralParseException: " << e.what() << " on file " << replayFile->path << "\n";
		return 1;
	}
	catch (const std::exception& e) //e
	{
		std::cerr << "GeneralException: " << e.what() << " on file " << replayFile->path.filename() << "\n";
		return 1;
	}
	catch (...)
	{
		std::cerr << "UnknownException: on file " << replayFile->path.filename() << "\n";
		return 1;
	}
	writer.EndArray();

	writer.EndObject();
	writer.EndObject();
	return 0;
}

template<typename Writer>
int ParseBodyAndSerializeReplay(Writer& writer, const std::shared_ptr<CPPRP::ReplayFile>& replayFile, const bool parseBody, int precision)
{
	if (precision > 0)
	{
		writer.SetMaxDecimalPlaces(precision);
	}
	return SerializeReplay(writer, replayFile, parseBody);
}