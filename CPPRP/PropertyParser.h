#pragma once

#include <unordered_map>
#include <functional>
#include <memory>
#include "./data/GameClasses.h"


namespace CPPRP
{
	typedef std::function<std::shared_ptr<Engine::Actor>()> createObjectFunc;
	typedef std::function<void(Engine::Actor*, CPPBitReader<BitReaderType>& br)> parsePropertyFunc;

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

	template<typename T>
	inline static T Initializor()
	{	
#define GAMECLASS(namesp, classn) RegisterClass<namesp::classn>(xstr(namesp) "." xstr(classn));
#define fulln(namesp, classn, propname) xstr(namesp) "." xstr(classn) ":" xstr(propname)
#define GAMEFIELD(namesp, classn, propname, nameoftype) \
	RegisterField(fulln(namesp, classn, propname), [](Engine::Actor* struc, CPPBitReader<BitReaderType>& br) { ((CPPRP::namesp::classn*)(struc))->propname = Consume<nameoftype>(br); })

		#include "./generated/GameClassMacros.h"

#undef GAMECLASS
#undef fulln
#undef GAMEFIELD
		return 0;
	}
	static int T = Initializor<int>();
}