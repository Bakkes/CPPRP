
#include "CPPRPJSON.h"

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

int main(int argc, char* argv[])
{
	OptionsParser op(argc, argv);
	std::string inputFile = op.looseOption;
	if (inputFile.size() == 0)
	{
		inputFile = op.GetStringValue({ "i", "input" });
	}
	if (inputFile.size() == 0)
	{
		std::cerr << "No input file given! Pass path to replay file as default argument via -i or --input\n";
		return 1;
	}
	if (!std::filesystem::exists(inputFile))
	{
		std::cerr << "Failed to open file " << inputFile << "\n";
		return 1;
	}

	auto replayFile = std::make_shared<CPPRP::ReplayFile>(inputFile);
	if (!replayFile->Load())
	{
		std::cerr << "Cannot open file, it exists but cannot open? " << inputFile << "\n";
		return 1;
	}
	{
		//std::cout << "File size: " << replayFile->data.size() << " bytes\n";
	}
	{
		//Timer crcTimer("CRC");
		int crc = op.GetIntValue({ "crc", "verify" }, 0);
		if (crc < 0 || crc > CPPRP::CrcCheck::CRC_Both)
		{
			std::cerr << "Invalid value given for crc check (0 = no check, 1 = verify header, 2 = verify body, 3 = verify both)\n";
			return 1;
		}

		if (crc != 0 && !replayFile->VerifyCRC((CPPRP::CrcCheck)crc))
		{
			std::cerr << "CRC check failed! Replay file " << inputFile << " is probably corrupt or has been tampered with!";
			return 1;
		}
	}
	
	//Timer t("Time including header deserialization");
	try
	{
		replayFile->DeserializeHeader();
	}
	catch (CPPRP::GeneralParseException<BitReaderType>& gpe)
	{
		std::cerr << "DeserializeHeader threw exception: " << gpe.errorMsg << "\n";
		return 1;
	}
	const bool parseBody = !op.GetBoolValue({ "ho", "header" }, false);
	const bool doDryRun = op.GetBoolValue({ "dry" }, false);

	//Timer t2("Time without header deserialization");
	if (doDryRun)
	{
		if (parseBody)
		{
			replayFile->Parse();
		}
	}
	else
	{
		rapidjson::StringBuffer s(0, 20000000); //Allocate 20mb
		/*FILE* fp = NULL;
		fopen_s(&fp, "test.json", "wb");
		char writeBuffer[65536];
		rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));*/
		const int precision = op.GetIntValue({ "p", "precision" }, 0);

		bool result = 0;

		if (false) {}
#ifdef CPPRP_PRETTYSUPPORT
		else if (const bool writePretty = op.GetBoolValue({ "pretty", "prettify" }, false); writePretty)
		{
			auto writer = rapidjson::PrettyWriter<rapidjson::StringBuffer>(s);
			result = ParseBodyAndSerializeReplay(writer, replayFile, parseBody, precision);
		}
#endif
		else
		{
			auto writer = rapidjson::Writer<rapidjson::StringBuffer>(s);
			result = ParseBodyAndSerializeReplay(writer, replayFile, parseBody, precision);
		}


		if (result != 0) //we got an error
		{
			return result;
		}
		std::string outJsonString = s.GetString();
		std::string outFile = op.GetStringValue({ "o", "output" });
		if (outFile.size() > 0)
		{
			std::ofstream outFileStream(outFile);
			outFileStream << outJsonString;
		}
		if ((outFile.size() > 0 && op.GetBoolValue({ "stdout", "print" }, false)) || (outFile.size() == 0 && op.GetBoolValue({ "stdout", "print" }, true)))
		{
			std::cout << outJsonString;
		}
	}
	//t.Stop();
	//t2.Stop();
}
