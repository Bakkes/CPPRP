#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include <memory>
#include <array>

namespace CPPRP
{
	enum { Crc32Poly = 0x04c11db7 };

	constexpr static inline uint32_t swap(uint32_t x)
	{
#if defined(__GNUC__) || defined(__clang__)
		return __builtin_bswap32(x);
#else
		return (x >> 24) | ((x >> 8) & 0x0000FF00) |
			((x << 8) & 0x00FF0000) | (x << 24);
#endif
	}


	/*
	Generate lookup tables at compile time
	*/
	template<uint32_t SIZE>
	constexpr std::array<std::array<uint32_t, 256>, SIZE> GenerateTable(const uint32_t poly)
	{
		std::array<std::array<uint32_t, 256>, SIZE> crcLookupTable{};

		for (uint32_t cell = 0; cell != 256; ++cell)
		{
			uint32_t crc = cell << 24;
			for (uint32_t repeat = 8; repeat; --repeat)
			{
				crc = (crc & 0x80000000) ? (crc << 1) ^ poly : (crc << 1);
			}
			if constexpr (SIZE == 1) //Special case for 1, don't swap crc. Dont need to do it in 2nd loop since we only fill [0] when SIZE=1
			{
				crcLookupTable[0][cell] = crc;
			}
			else
			{
				crcLookupTable[0][cell] = swap(crc);
			}
		}

		for (uint32_t cell = 0; cell != 256; ++cell)
		{
			uint32_t crc = swap(crcLookupTable[0][cell]);
			for (uint32_t table = 1; table < SIZE; ++table)
			{
				crc = swap(crcLookupTable[0][crc >> 24]) ^ (crc << 8);
				crcLookupTable[table][cell] = swap(crc);
			}
		}
		return crcLookupTable;
	}

	template<typename T>
	static const uint32_t CalculateCRC_SB1(const std::vector<T>& data, const size_t startPosition, const size_t length, uint32_t crc)
	{
		constexpr auto CRCTableSB1 = GenerateTable<1>(Crc32Poly);

		crc = ~crc;
		for (size_t i = startPosition; i < startPosition + length; ++i)
		{
			crc = (crc << 8) ^ CRCTableSB1[0][static_cast<uint8_t>(data[i]) ^ (crc >> 24)];
		}
		return ~crc;
	}

	template<typename T>
	static const uint32_t CalculateCRC_SB8(const std::vector<T>& data, const size_t startPosition, size_t length, uint32_t crc)
	{
		constexpr auto CRCTableSB8 = GenerateTable<8>(Crc32Poly);

		crc = ~swap(crc);

		const uint8_t* __restrict currentData = (uint8_t*)(data.data() + startPosition);

		// Align 4 bytes
		uint32_t preProcessBytes = ((reinterpret_cast<std::uintptr_t>(currentData) + 3) & ~(3)) - reinterpret_cast<std::uintptr_t>(currentData);

		if (length > preProcessBytes)
		{
			length -= preProcessBytes;

			while (preProcessBytes--)
			{
				crc = (crc >> 8) ^ CRCTableSB8[0][*currentData++ ^ (crc & 0xFF)];
			}

			const uint32_t* processBy8 = (const uint32_t*)currentData;
			uint32_t times = length >> 3;//divide by 8
			while (times--)
			{
				uint32_t one = *processBy8++ ^ crc;
				uint32_t two = *processBy8++;
				crc =
					CRCTableSB8[7][one & 0xFF] ^
					CRCTableSB8[6][(one >> 8) & 0xFF] ^
					CRCTableSB8[5][(one >> 16) & 0xFF] ^
					CRCTableSB8[4][one >> 24] ^
					CRCTableSB8[3][two & 0xFF] ^
					CRCTableSB8[2][(two >> 8) & 0xFF] ^
					CRCTableSB8[1][(two >> 16) & 0xFF] ^
					CRCTableSB8[0][two >> 24];
			}
			currentData = (const uint8_t*)processBy8;

			length &= 7; //mod 8
		}

		//process remaining bits
		while (length--)
		{
			crc = (crc >> 8) ^ CRCTableSB8[0][*currentData++ ^ (crc & 0xFF)];
		}

		return swap(~crc);
	};


#if defined(_MSC_VER) || defined(__MINGW32__)
#include <xmmintrin.h>
#ifdef __MINGW32__
#define PREFETCH(location) __builtin_prefetch(location)
#else
#define PREFETCH(location) _mm_prefetch(location, _MM_HINT_T0)
#endif
#else
#ifdef __GNUC__
#define PREFETCH(location) __builtin_prefetch(location)
#else
#define PREFETCH(location) ;
#endif
#endif


	//Based on SB8 and crc32_16bytes_prefetch in crc32.cpp from https://create.stephan-brumme.com/crc32
	template<typename T>
	static const uint32_t CalculateCRC_SB16(const std::vector<T>& data, const size_t startPosition, size_t length, uint32_t crc)
	{
		constexpr auto CRCTableSB16 = GenerateTable<16>(Crc32Poly);
		crc = ~swap(crc);

		const uint8_t* __restrict currentData = (uint8_t*)(data.data() + startPosition);

		// Align 8 bytes
		uint32_t preProcessBytes = ((reinterpret_cast<std::uintptr_t>(currentData) + 3) & ~(3)) - reinterpret_cast<std::uintptr_t>(currentData);


		if (length > preProcessBytes)
		{
			length -= preProcessBytes;

			while (preProcessBytes--)
			{
				crc = (crc >> 8) ^ CRCTableSB16[0][*currentData++ ^ (crc & 0xFF)];
			}

			const uint32_t* processBy8 = (const uint32_t*)currentData;
			uint32_t times = length >> 4;//divide by 16
			constexpr size_t unrollSize = 4;
			constexpr size_t bytesAtOnce = 16 * unrollSize;
			constexpr size_t prefetchAhead = 256;
			while (length >= bytesAtOnce)
			{
				PREFETCH(((const char*)processBy8) + prefetchAhead);
				for (size_t unroll = 0; unroll < unrollSize; unroll++) //let compiler unroll this
				{
					uint32_t one = *processBy8++ ^ crc;
					uint32_t two = *processBy8++;
					uint32_t three = *processBy8++;
					uint32_t four = *processBy8++;
					crc =
						CRCTableSB16[0][(four >> 24) & 0xFF] ^
						CRCTableSB16[1][(four >> 16) & 0xFF] ^
						CRCTableSB16[2][(four >> 8) & 0xFF] ^
						CRCTableSB16[3][four & 0xFF] ^
						CRCTableSB16[4][(three >> 24) & 0xFF] ^
						CRCTableSB16[5][(three >> 16) & 0xFF] ^
						CRCTableSB16[6][(three >> 8) & 0xFF] ^
						CRCTableSB16[7][three & 0xFF] ^
						CRCTableSB16[8][(two >> 24) & 0xFF] ^
						CRCTableSB16[9][(two >> 16) & 0xFF] ^
						CRCTableSB16[10][(two >> 8) & 0xFF] ^
						CRCTableSB16[11][two & 0xFF] ^
						CRCTableSB16[12][(one >> 24) & 0xFF] ^
						CRCTableSB16[13][(one >> 16) & 0xFF] ^
						CRCTableSB16[14][(one >> 8) & 0xFF] ^
						CRCTableSB16[15][one & 0xFF];
				}
				length -= bytesAtOnce;
			}
			currentData = (const uint8_t*)processBy8;

			//length &= (16-1); //mod 16 not needed since we substract from length above

		}

		//process remaining bits
		while (length--)
		{
			crc = (crc >> 8) ^ CRCTableSB16[0][*currentData++ ^ (crc & 0xFF)];
		}

		return swap(~crc);
	};
};