#pragma once
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <initializer_list>
#include "NetworkData.h"
#include "NetworkParsers.h"
#include <any>
#include "rapidjson/writer.h"
#include "rapidjson/filewritestream.h"

typedef std::shared_ptr<void>(*createFunc)(CPPBitReader<uint32_t>& reader, rapidjson::Writer<rapidjson::FileWriteStream>& writer);

template<typename T>
static inline std::shared_ptr<void> createInstance(CPPBitReader<uint32_t>& reader, rapidjson::Writer<rapidjson::FileWriteStream>& writer)
{
	auto consumed = Consume<T>(reader);
	Serialize(writer, consumed);
	//printf("%s\n", ToString(consumed).c_str());
	return std::static_pointer_cast<void>(std::make_shared<T>());
}


class NetworkStreamParser
{
private:
	std::vector<createFunc> parseFunctions;
	std::unordered_map<std::string, uint32_t> functionToIndexMapping;
public:
	NetworkStreamParser();
	~NetworkStreamParser();
	


	template<typename T>
	void RegisterParsers(const std::initializer_list<std::string> props)
	{
		for (auto str : props)
		{
			RegisterParsers<T>(str);
		}
	}

	template<typename T>
	void RegisterParsers(const std::string prop)
	{
		parseFunctions[functionToIndexMapping[prop]] = &createInstance<T>;
	}

	void RegisterParsers(const std::shared_ptr<ReplayFileData>& rfd);
	template<typename Writer>
	void Parse(const uint32_t propertyIdx, CPPBitReader<uint32_t>& br, Writer& writer) const
	{
		auto inst = parseFunctions[propertyIdx](br, writer);
	}
	void Parse(const std::string& name, CPPBitReader<uint32_t>& br);
};

