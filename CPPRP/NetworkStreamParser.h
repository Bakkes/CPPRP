#pragma once
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <initializer_list>
#include "NetworkData.h"
#include "NetworkParsers.h"
#include <any>

namespace CPPRP
{
	constexpr bool IncludeParseLog = false;

	typedef std::shared_ptr<void>(*createFunc)(CPPBitReader<BitReaderType>& reader);

	template<typename T>
	static inline std::shared_ptr<void> createInstance(CPPBitReader<BitReaderType>& reader)
	{
		auto consumed = Consume<T>(reader);
		//Serialize(writer, consumed);
		//printf("%s\n", ToString(consumed).c_str());
		return std::static_pointer_cast<void>(std::make_shared<T>(consumed));
	}

	class NetworkStreamParser
	{
	private:
		std::vector<createFunc> parseFunctions;
		std::unordered_map<std::string, size_t> functionToIndexMapping;
	public:
		NetworkStreamParser();
		~NetworkStreamParser();

		template<typename T>
		constexpr void RegisterParsers(const std::initializer_list<std::string> props)
		{
			for (auto str : props)
			{
				RegisterParsers<T>(str);
			}
		}

		template<typename T>
		constexpr void RegisterParsers(const std::string prop)
		{
			parseFunctions[functionToIndexMapping[prop]] = &createInstance<T>;
		}

		void RegisterParsers(const std::shared_ptr<ReplayFileData>& rfd);

		std::shared_ptr<void> Parse(const uint32_t propertyIdx, CPPBitReader<BitReaderType>& br) const;
	};

}