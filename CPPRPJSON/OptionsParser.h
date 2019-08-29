#pragma once
#include <string>
#include <map>

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

			if (!key.empty() && arg + 1 < argc)
			{
				std::string value = argv[arg + 1];
				arg++;
				args[key] = value;
			}
		}
	}

	bool HasKey(std::string& key)
	{
		return args.find(key) != args.end();
	}

	std::string GetValue(std::initializer_list<std::string> optionNames)
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
};
