#pragma once
#include <string>
#include <map>
#include <algorithm>

class OptionsParser
{
public:
	std::map<std::string, std::string> args;
	std::string programName = "";
	std::string looseOption = "";
public:
	OptionsParser(int argc, char* argv[])
	{
		programName = std::string(argv[0]);
		for (int arg = 1; arg < argc; arg++)
		{
			std::string key = argv[arg];
			if (key.at(0) == '-')
			{
				if (key.size() >= 2)
				{
					if (key.at(1) == '-')
					{
						key = key.substr(2);
					}
					else
					{
						key = key.substr(1);
					}
				}
				else
				{
					continue;
				}
			}
			else
			{
				looseOption = key;
				continue;
			}

			if (!key.empty())
			{
				if (arg + 1 < argc && argv[arg + 1][0] != '-')
				{
					std::string value = argv[arg + 1];
					arg++;
					args[key] = value;
				}
				else
				{
					args[key] = "true";
				}
			}
		}
	}

	bool HasKey(std::string& key)
	{
		return args.find(key) != args.end();
	}

	std::string GetStringValue(std::initializer_list<std::string> optionNames)
	{
		for (auto it : optionNames)
		{
			if (HasKey(it))
			{
				return args[it];
			}
		}
		return "";
	}

	bool GetBoolValue(std::initializer_list<std::string> optionNames, bool defaultValue)
	{
		for (auto it : optionNames)
		{
			if (HasKey(it))
			{
				std::string val = args[it];
				std::transform(val.begin(), val.end(), val.begin(),
					[](unsigned char c) { return std::tolower(c); });

				return val.compare("1") == 0 || val.compare("true") == 0;
			}
		}
		return defaultValue;
	}

	int GetIntValue(std::initializer_list<std::string> optionNames, bool defaultValue)
	{
		for (auto it : optionNames)
		{
			if (HasKey(it))
			{
				return std::stoi(args[it]);
			}
		}
		return defaultValue;
	}
};
