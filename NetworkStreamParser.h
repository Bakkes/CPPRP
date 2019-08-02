#pragma once
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <initializer_list>
#include "NetworkData.h"
typedef std::shared_ptr<NetworkStreamItemParser<uint32_t>>(*createFunc)();

template<typename T>
std::shared_ptr<NetworkStreamItemParser<uint32_t>> createInstance()
{
	return std::static_pointer_cast<NetworkStreamItemParser<uint32_t>>(std::make_shared<T>());
}

class NetworkStreamParser
{
private:
	std::unordered_map<std::string, createFunc> parseFunctions;
public:
	NetworkStreamParser();
	~NetworkStreamParser();
	


	template<typename T>
	void RegisterParsers(const std::initializer_list<std::string> props)
	{
		for (auto str : props)
		{
			parseFunctions[str] = &createInstance<T>;
		}
	}

	template<typename T>
	void RegisterParsers(const std::string prop)
	{
		parseFunctions[prop] = &createInstance<T>;
	}

	void RegisterParsers();
	void Parse(const std::string& name, CPPBitReader<uint32_t>& br);
};

