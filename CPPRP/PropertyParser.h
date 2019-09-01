#pragma once
#include <unordered_map>
#include <functional>
#include <memory>
#include "./data/GameClasses.h"

namespace CPPRP
{

	typedef std::function<std::shared_ptr<Engine::Actor>()> createObjectFunc;
	typedef std::function<void(std::shared_ptr<Engine::Actor>&, CPPBitReader<BitReaderType>& br)> parsePropertyFunc;

	static std::unordered_map<std::string, createObjectFunc> createObjectFuncs;
	static std::unordered_map<std::string, parsePropertyFunc> parsePropertyFuncs;

	template<typename T1>
	inline static std::shared_ptr<Engine::Actor> createObject()
	{
		return std::make_shared<T1>();
	}

	template<typename T1>
	inline static void RegisterClass(std::string className)
	{
		createObjectFuncs[className] = &createObject<T1>;
	}

	template<typename T>
	inline static void RegisterField(const std::string& str, T callback)
	{
		parsePropertyFuncs[str] = callback;
	}

#include "./generated/GameClassParser.h"

	static int T = Initializor<int>();
}