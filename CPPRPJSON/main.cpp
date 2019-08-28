
#pragma comment(lib, "CPPRP.lib")
#include "../CPPRP/ReplayFile.h"
#include "../CPPRP/exceptions/ReplayException.h"
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
#undef max
static int a = CPPRP::JSON::Test();

struct UpdateData
{
	const CPPRP::ActorStateData asd;
	const std::vector<uint32_t> props;
};
int main(int argc, char* argv[])
{
	if constexpr (true) {
		auto replayFile = std::make_shared<CPPRP::ReplayFile>("J:/74B3720B43DEF8267DD3EB932BED44B5.replay");
		replayFile->Load();
		replayFile->DeserializeHeader();
		replayFile->PreprocessTables();

		std::vector<CPPRP::ActorStateData> createdActorsThisTick;
		std::vector<CPPRP::ActorStateData> deletedActorsThisTick;
		std::vector<UpdateData> updatedActorsThisTick;

		rapidjson::StringBuffer s;
		CPPRP::JSON::Writer writer(s);
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
		
		replayFile->tickables.push_back([&](const CPPRP::Frame frame, const std::unordered_map<int, CPPRP::ActorStateData>& actorStats)
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
				for (auto created : createdActorsThisTick)
				{
					writer.StartObject();
					writer.String("Id");
					writer.Uint(created.actorId);
					
					std::string name = replayFile->replayFile->names.at(created.nameId);
					std::string className = replayFile->replayFile->objects.at(created.classNameId);
					writer.String("Name");
					writer.String(name.c_str());

					writer.String("ClassName");
					writer.String(className.c_str());
					bool hasInitialPosition = replayFile->HasInitialPosition(className);
					writer.String("HasInitialPosition");
					writer.Bool(hasInitialPosition);
					if (hasInitialPosition)
					{
						writer.String("InitialPosition");
						CPPRP::JSON::Serialize<CPPRP::Vector3I>(writer, created.actorObject->Location);
					}

					bool hasInitialRotation = replayFile->HasRotation(className);
					writer.String("HasInitialRotation");
					writer.Bool(hasInitialRotation);
					if (hasInitialRotation)
					{
						writer.String("InitialRotation");
						CPPRP::JSON::Serialize<CPPRP::Rotator>(writer, created.actorObject->Rotation);
					}
					writer.EndObject();
				}
				writer.EndArray();

				writer.String("Deleted");
				writer.StartArray();
				for (auto deleted : deletedActorsThisTick)
				{
					writer.Uint(deleted.actorId);
				}
				writer.EndArray();
				

				writer.String("Updated");
				writer.StartArray();
				for (auto updated : updatedActorsThisTick)
				{
					writer.StartObject();
					writer.String("Id");
					writer.Uint(updated.asd.actorId);
					writer.String("UpdatedProperties");
					writer.StartArray();

					for (auto updatedProp : updated.props)
					{
						writer.StartObject();
						
						std::string objectName = replayFile->replayFile->objects[updatedProp];
						auto a = updated.asd.actorObject;
						CPPRP::JSON::writerFuncs[objectName](writer, a);
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
		replayFile->Parse();
		writer.EndArray();
		int fdfsd = 5;
		std::cout << s.GetString();
		getchar();
	}
}