#pragma once
#include <vector>
#include <stdint.h>
#include "base64.h"
#include <cstring>
/*
Binary writer which optimizes for bit size. Programmer has to specify the number of LSB bits to store
Made by Bakkes
*/
template<typename A>
class BitBinaryWriter
{
public:
	const int type_size = sizeof(A) * 8;
	int current_bit = 0;
	A* buffer;
	BitBinaryWriter(int bufferSize)
	{
		buffer = (A*)calloc(bufferSize, type_size);
	}

	~BitBinaryWriter()
	{
		delete buffer;
	}

	template<typename T>
	void WriteBits(T t, int useLSBcount = sizeof(T))
	{
		T t2 = t;
		for (int i = 0; i < useLSBcount; i++)
		{
			if (t2 & 1) {
				buffer[current_bit / type_size] |= (1 << (current_bit % type_size));
			}
			else
			{
				buffer[current_bit / type_size] &= ~(1 << (current_bit % type_size));
			}
			current_bit++;
			t2 >>= 1;
		}
	}

	void WriteBool(bool b)
	{
		WriteBits(b ? 1 : 0, 1); //Use ternary to ensure last bit contains true false
	}

	uint8_t CalculateCRC(int startByte, int endByte)
	{
		uint8_t crc = 0b11111111;
		for (int i = startByte; i < endByte; i++)
		{
			crc = crc ^ buffer[i];
		}
		return crc;
	}

	std::string ToHex()
	{
		return base64_encode(buffer, current_bit / type_size + 1);
	}
};

template<typename A>
class BitBinaryReader
{
public:
	const int type_size = sizeof(A) * 8;
	int current_bit = 0;
	A* buffer;

	BitBinaryReader(std::string hexString)
	{
		std::vector<BYTE> decodedVector = base64_decode_bytearr(hexString);
		//std::string decoded = base64_decode(hexString);
		//const char* data = decoded.c_str();
		buffer = (A*)malloc(decodedVector.size());
		memcpy(buffer, &decodedVector[0], decodedVector.size());
	}

	BitBinaryReader(A* inBuf)
	{
		buffer = inBuf;
	}

	~BitBinaryReader()
	{
		delete buffer;
	}

	template<typename T>
	T ReadBits(int useLSBcount = sizeof(T))
	{
		T t = 0;
		for (int i = 0; i < useLSBcount; i++)
		{
			t |= ((buffer[current_bit / type_size] >> (current_bit % type_size)) & 1) << i;
			current_bit++;
		}
		return t;
	}
	uint8_t CalculateCRC(int startByte, int endByte)
	{
		uint8_t crc = 0b11111111;
		for (int i = startByte; i < endByte; i++)
		{
			crc = crc ^ buffer[i];
		}
		return crc;
	}
	bool VerifyCRC(uint8_t crc, int startByte, int endByte)
	{
		return (crc ^ CalculateCRC(startByte, endByte)) == 0;
	}

	bool ReadBool()
	{
		return ReadBits<bool>(1) & 1; //Use ternary to ensure last bit contains true false
	}
};