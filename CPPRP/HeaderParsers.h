#pragma once
#include "./data/GameClasses.h"
namespace CPPRP
{
	template<typename T>
	T ReadHeaderStruct(std::shared_ptr<CPPBitReader<BitReaderType>>& bitReader)
	{
		return bitReader->read<T>();
	}

	template<>
	KeyFrame ReadHeaderStruct(std::shared_ptr<CPPBitReader<BitReaderType>>& bitReader)
	{
		return KeyFrame{
			bitReader->read<float>(),		//Time
			bitReader->read<uint32_t>(),	//Frame
			bitReader->read<uint32_t>()	//File position
		};
	}

	template<>
	DebugString ReadHeaderStruct(std::shared_ptr<CPPBitReader<BitReaderType>>& bitReader)
	{
		return DebugString{
			bitReader->read<uint32_t>(),	//Time
			bitReader->read<std::string>(),	//Frame
			bitReader->read<std::string>()	//File position
		};
	}

	template<>
	ReplayTick ReadHeaderStruct(std::shared_ptr<CPPBitReader<BitReaderType>>& bitReader)
	{
		return ReplayTick{
			bitReader->read<std::string>(),	//Type
			bitReader->read<uint32_t>()		//Frame	
		};
	}
	template<>
	ClassIndex ReadHeaderStruct(std::shared_ptr<CPPBitReader<BitReaderType>>& bitReader)
	{
		return ClassIndex{
			bitReader->read<std::string>(),	//Class_name
			bitReader->read<uint32_t>()		//Index
		};
	}


	template<typename T>
	void ReadVector(std::shared_ptr<CPPBitReader<BitReaderType>>& bitReader, std::vector<T>& inVec)
	{
		const uint32_t vectorCount = bitReader->read<uint32_t>();
		if (vectorCount * sizeof(T) > bitReader->size) throw 0; //TODO: throw proper exception
		inVec.resize(vectorCount);
		for (uint32_t i = 0; i < vectorCount; ++i)
		{
			inVec[i] = ReadHeaderStruct<T>(bitReader);
		}
	}
	
}