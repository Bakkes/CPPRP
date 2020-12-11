#pragma once
#include <string>
#include <sstream>
#include <stdint.h>
#include <assert.h>
//#include "ParseException.h"
#include "./data/ReplayFileData.h"
//#include "ReplayFile.h"
#include "./exceptions/ReplayException.h"
#include <cmath>
#include <memory>
#include <xmmintrin.h>
//#define USESIMD

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include <locale>
#include <codecvt>
#ifdef _WIN32
#include <Windows.h>
#endif
#ifdef _WIN32
static inline std::string ws2s(const std::wstring& wstr)
{
	if (wstr.empty())
	{
		return std::string();
	}

	int size = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, &wstr[0], wstr.size(), NULL, 0, NULL, NULL);
	std::string ret = std::string(size, 0);
	WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, &wstr[0], wstr.size(), &ret[0], size, NULL, NULL);
	return ret;
}
#else
static inline std::string ws2s(const std::u16string& wstr)
{
	if (wstr.empty())
	{
		return std::string();
	}
	using convert_typeX = std::codecvt_utf8<char16_t>;
	std::wstring_convert<convert_typeX, char16_t> converterX;
	return converterX.to_bytes(wstr);
	//char buf[2048] = { 0 };
	//const ssize_t res = std::wcstombs(buf, wstr.c_str(), 2048);
	//return std::string(buf);


}
#endif
#undef _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#define QUAT_NUM_BITS (18)
#define MAX_QUAT_VALUE (0.7071067811865475244f)
#define MAX_QUAT_VALUE_INVERSE (1.0f / MAX_QUAT_VALUE)

typedef uint32_t BitReaderType;


namespace CPPRP
{

	static inline float uncompress_quat(uint32_t val)
	{
		constexpr float MaxValue = (float)(1 << QUAT_NUM_BITS) - 1;
		float positiveRangedValue = val / MaxValue;
		float rangedValue = (positiveRangedValue - 0.50f) * 2.0f;
		return rangedValue * MAX_QUAT_VALUE;
	}

	//TODO: memoize?
	static inline const uint32_t msbDeBruijn32(uint32_t v)
	{
		static const int MultiplyDeBruijnBitPosition[32] =
		{
			0, 9, 1, 10, 13, 21, 2, 29, 11, 14, 16, 18, 22, 25, 3, 30,
			8, 12, 20, 28, 15, 17, 24, 7, 19, 27, 23, 6, 26, 5, 4, 31
		};

		v |= v >> 1; // first round down to one less than a power of 2
		v |= v >> 2;
		v |= v >> 4;
		v |= v >> 8;
		v |= v >> 16;

		return static_cast<uint32_t>(MultiplyDeBruijnBitPosition[(const uint32_t)(v * 0x07C4ACDDU) >> 27]);
	}
	typedef struct
	{
		const uint8_t* data;
		uint64_t cachedVal;
		uint32_t validBits;
		size_t len;
		size_t bytes_read;

	} bitreader_t;

	static inline void bitreader_load_cache(bitreader_t* b)
	{
		uint64_t val = *(uint64_t*)&b->data[b->bytes_read];

		b->bytes_read += 8;
		b->cachedVal = (val);// (val >> 32) | (val << 32);
		b->validBits = 64;
	}

	static inline void bitreader_init(bitreader_t* b, const uint8_t* data, size_t len)
	{
		b->data = data;
		b->len = len;
		b->bytes_read = 0;
		bitreader_load_cache(b);
	}

	static inline uint64_t bitreader_read_bits(bitreader_t* b, uint8_t bits)
	{
		uint32_t validBits = b->validBits;
		uint64_t result = 0;


		uint8_t overflow = 0;
		if (bits >= validBits)
		{
			overflow = validBits;
			bits -= validBits;
			

			//Need special case for validBits == 0, since >> 64 is UB, but is a little ugly imo
			//result = validBits == 0 ? 0 : (b->cachedVal >> (64 - validBits));
			result = (b->cachedVal >> (64 - validBits));
			bitreader_load_cache(b);
			validBits = 64;
		}
		validBits -= bits;


		//result |= ((bt >> (bit_position))& ((1ULL << n) - 1)) << bit_pos;
		//(((b->cachedVal) >> 8) & (((1ULL << bits) - 1)))
		result |= (((b->cachedVal) >> (64 - validBits - bits)) & ((1ULL << bits) - 1)) << overflow;// >> (validBits);
		//result |= ((b->cachedVal >> (64 - validBits))& ((1ull << (64 - validBits)) - 1));

		b->validBits = validBits;
		return (result);
	}

	//Attempt at writing a fast bitreader for RL replays
	template<typename T>
	class CPPBitReader
	{
	public:
		bitreader_t b;
		

		const int size;
		const int t_position = 0;
		const int bit_position = 0;

		//Let's store this data in here, saves a call to owner.
		//Use during network stream parsing
		const uint16_t engineVersion;
		const uint8_t licenseeVersion;
		const uint8_t netVersion;

		std::vector<uint32_t> attributeIDs;

	private:
		template<typename X>
		inline const X get_bits(uint16_t n)
		{
#ifndef PARSE_UNSAFE
			if (GetAbsoluteBitPosition() + n > b.len)
			{
				throw std::runtime_error("Attempted to read beyond buffer");
			}
#endif

			uint64_t res = bitreader_read_bits(&b, n);
			return *(X*)(&res);
		}

		template<typename X>
		inline const X get_bits_max(const X maxValue, const uint8_t max_bits)
		{
			X result = 0;

			result = read<X>(max_bits);

			if ((result + (1 << max_bits)) < maxValue)
			{
				result |= ((b.cachedVal >> (64 - b.validBits)) & 1) << max_bits;

				if (b.validBits == 1)
				{
					read<X>(1);
				}
				else
				{
					b.validBits -= 1;
				}
				
				
			}

			return result;
		}

		template<typename X>
		inline const X get_bits_max(const X maxValue)
		{
			return get_bits_max(maxValue, msbDeBruijn32(maxValue));
		}
	public:
		CPPBitReader(const T * data, size_t size, std::shared_ptr<ReplayFileData> owner_);
		CPPBitReader(const T * data, size_t size, std::shared_ptr<ReplayFileData> owner_,
			const uint32_t engineV, const uint32_t licenseeV, const uint32_t netV);
		CPPBitReader(const CPPBitReader& other);
		CPPBitReader();
		

		template<typename U>
		const U read();
		

		template<typename U>
		const U read(uint16_t customSize);

		/*
		Source from this is from the C# replay parser
		*/
		const float readFixedCompressedFloat(const int32_t maxValue, int32_t numBits);

		template<typename U>
		inline const U readBitsMax(const uint32_t max);

		inline const bool canRead() const noexcept;
		inline const bool canRead(int bits) const noexcept;

		void goback(int32_t num);
		void skip(uint32_t num);

		const size_t GetAbsoluteBytePosition() const noexcept;
		const size_t GetAbsoluteBitPosition() const noexcept;
	};

	template<>
	template<>
	inline const bool CPPBitReader<BitReaderType>::read<bool>()
	{
		return get_bits<uint8_t>(1);
	}

	//Float requires special casting since bit operations aren't allowed
	template<>
	template<>
	inline const float CPPBitReader<BitReaderType>::read<float>()
	{
		static_assert(sizeof(float) == sizeof(uint32_t));
		const uint32_t value = read<uint32_t>();
		return reinterpret_cast<const float&>(value);
	}


	template<>
	template<>
	inline const Vector3I CPPBitReader<BitReaderType>::read<Vector3I>()
	{
		//PREFETCH((char*)(this->data));
		const uint32_t max_value = 20 + (2 * (netVersion >= 7));
		const uint32_t num_bits = get_bits_max<uint32_t>(max_value, 4); //Saves a debruijn call since its 4 for both 22 and 20

		const int32_t bias = 1 << (int32_t)(num_bits + 1);
		const int16_t max = (int16_t)num_bits + 2;
		if (max < 22) //if it fits in 64 bits, read it all at once (3*21 < 64)
		{
			const uint64_t test = read<uint64_t>(max * 3);
			const uint64_t rightShift = static_cast<uint64_t>(64 - max);

			const int32_t dx = static_cast<int32_t>((test << rightShift) >> rightShift);
			const int32_t dy = static_cast<int32_t>((test << (64UL - max * 2UL)) >> rightShift);
			const int32_t dz = static_cast<int32_t>((test << (64UL - max * 3UL)) >> rightShift);

			return { (dx - bias), (dy - bias), (dz - bias) };
		}

		//happens in 3 out of 10000 replays, so we still need it i guess
		const int32_t dx = read<int32_t>(max);
		const int32_t dy = read<int32_t>(max);
		const int32_t dz = read<int32_t>(max);
		return { (dx - bias), (dy - bias), (dz - bias) };
	}

	

	template<>
	template<>
	inline const Vector3 CPPBitReader<BitReaderType>::read<Vector3>()
	{
		Vector3I v = read<Vector3I>();
		return { v.x / 100.f, v.y / 100.f, v.z / 100.f };
	}

	template<>
	template<>
	inline const Rotator CPPBitReader<BitReaderType>::read<Rotator>()
	{
		constexpr float conversion = 360.f / 256.f;
		Rotator ret{ 0 };
		if (read<bool>())
		{
			ret.pitch = static_cast<int>(read<int8_t>() * conversion);
		}

		if (read<bool>())
		{
			ret.yaw = static_cast<int>(read<int8_t>()  * conversion);
		}

		if (read<bool>())
		{
			ret.roll = static_cast<int>(read<int8_t>()  * conversion);
		}
		return ret;
	}


#ifdef USESIMD

	#include <immintrin.h>
	template<>
	template<>
	inline const Quat CPPBitReader<BitReaderType>::read<Quat>()
	{
		constexpr uint16_t BitsReadForQuat = 2 + (3 * QUAT_NUM_BITS);
		const uint64_t readQuat = read<uint64_t>(BitsReadForQuat);
		const uint8_t largest = readQuat & 0b11; //Read 2 lsb
		
		constexpr uint64_t QuatMask = (1 << QUAT_NUM_BITS) - 1; //(2^QUAT_NUM_BITS) - 1
		
		const __m128 first = _mm_set_ps(0, (readQuat & (QuatMask << 2ULL)) >> 2ULL, (readQuat & (QuatMask << 20ULL)) >> 20ULL, (readQuat & (QuatMask << 38ULL)) >> 38ULL);
		const __m128 second = _mm_set_ps(QuatMask, QuatMask, QuatMask, QuatMask);
		const __m128 minus = _mm_set_ps(0.5f, 0.5f, 0.5f, 0.5f);

		constexpr float TwoTimesMaxQuat = 2.f * MAX_QUAT_VALUE;
		const __m128 timestwo = _mm_set_ps(TwoTimesMaxQuat, TwoTimesMaxQuat, TwoTimesMaxQuat, TwoTimesMaxQuat);
		const __m128 divd = _mm_div_ps(first, second);

		const __m128 mind = _mm_sub_ps(divd, minus);
		const __m128 result = _mm_mul_ps(mind, timestwo);
		const float* res = (float*)&result;
		const float extra = _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ps1(1.f - (res[0] * res[0]) - (res[1] * res[1]) - (res[2] * res[2]))));

		Quat q = { 0 };
		switch (largest)
		{
		case 0:
			q = { extra, res[2], res[1], res[0] };
			break;
		case 1:
			q = { res[2], extra, res[1], res[0] };
			break;
		case 2:
			q = { res[2], res[1], extra, res[0] };
			break;
		case 3:
		default:
			q = { res[2], res[1], res[0], extra };
			break;
		};
		return q;
	}
#else
	template<>
	template<>
	inline const Quat CPPBitReader<BitReaderType>::read<Quat>()
	{
		const uint8_t largest = read<uint8_t>(2);
		const float a = uncompress_quat(read<uint32_t>(QUAT_NUM_BITS));
		const float b = uncompress_quat(read<uint32_t>(QUAT_NUM_BITS));
		const float c = uncompress_quat(read<uint32_t>(QUAT_NUM_BITS));
		float extra = std::sqrt(1.f - (a * a) - (b * b) - (c * c));

		if (std::isnan(extra))
		{
			extra = .0f;
		}

		Quat q = { 0 };
		switch (largest)
		{
		case 0:
			q = { extra, a, b, c };
			break;
		case 1:
			q = { a, extra, b, c };
			break;
		case 2:
			q = { a, b, extra, c };
			break;
		case 3:
		default:
			q = { a, b, c, extra };
			break;
		};
		return q;
	}
#endif


	template<>
	template<>
	inline const std::string CPPBitReader<BitReaderType>::read<std::string>()
	{
		const int32_t length = read<int32_t>();
		const size_t final_length = static_cast<size_t>(length) * (length > 0 ? 1 : -2);
		if (final_length == 0)
		{
			return "";
		}

		

#ifndef PARSE_UNSAFE
		if (final_length > 1024)
		{
			if (engineVersion == 0
				&& licenseeVersion == 0
				&& netVersion == 0)
			{
				throw InvalidVersionException(0, 0, 0);
			}
			else
			{
				throw std::runtime_error("Got unwanted string length, read value " + std::to_string(length) + ", reading bytes " + std::to_string(final_length) + ". (" + std::to_string(this->bit_position) + ")");
			}
		}
#endif
		if (length < 0)
		{
			char test[2048];
			for (size_t i = 0; i < final_length; ++i)
			{
				test[i] = read<uint8_t>();
			}
#ifdef _WIN32
			std::wstring test_ws(reinterpret_cast<wchar_t*>(test));
#else
			std::u16string test_ws(reinterpret_cast<char16_t*>(test));
#endif
			return ws2s(test_ws);
		}

		std::string str;

		if (b.validBits % 8 == 0)
		{
			const char* text = ((char*)&b.data[b.bytes_read - ((b.validBits) / 8)]);
			str = std::string(text);
			skip(final_length * 8);
		}
		else
		{
			str.resize(final_length - 1);
			int todo = final_length;
			while (todo > 7)
			{
				*reinterpret_cast<uint64_t*>(&str[final_length - todo]) = read<uint64_t>();
				todo -= 8;
			}
			if (todo > 3)
			{
				*reinterpret_cast<uint64_t*>(&str[final_length - todo]) = read<uint32_t>();
				todo -= 4;
			}

			for (size_t i = final_length - todo; i < final_length; ++i)
			{
				str[i] = read<uint8_t>();
			}
		}

		return str;
	}
	
	template<>
	template<>
	inline const OnlineID CPPBitReader<BitReaderType>::read<OnlineID>()
	{
		OnlineID uniqueId;
		
		const uint8_t platform = read<uint8_t>();
		switch (platform)
		{
		case Platform_Steam:
		{
			auto tmp = SteamID();
			tmp.steamID = read<uint64_t>(sizeof(uint64_t) * 8);
			uniqueId = tmp;
			break;
		}
		case Platform_Dingo:
		{
			auto tmp = XBoxID();
			tmp.xboxID = read<uint64_t>(sizeof(uint64_t) * 8);
			uniqueId = tmp;
			break;
		}
		case Platform_QQ:
		{
			auto tmp = QQID();
			tmp.qqID = read<uint64_t>(sizeof(uint64_t) * 8);
			uniqueId = tmp;
			break;
		}
		case Platform_PS4:
		{
			auto tmp = PS4ID();

			#define PSY4_MAX_NAME_LENGTH 16
			char playerNameTemp[PSY4_MAX_NAME_LENGTH];

			//Psynet names are always length 16, so optimize by interpreting 128 bits
			*reinterpret_cast<uint64_t*>(&playerNameTemp) = read<uint64_t>();
			*reinterpret_cast<uint64_t*>(&playerNameTemp[8]) = read<uint64_t>();
			/*for (uint32_t i = 0; i < PSY4_MAX_NAME_LENGTH; ++i)
			{
				playerNameTemp[i] = read<char>();
			}*/

			tmp.playerName = std::string(playerNameTemp, PSY4_MAX_NAME_LENGTH);
			tmp.unknown1 = read<uint64_t>();
			if (netVersion >= 1)
			{
				tmp.unknown2 = read<uint64_t>();
			}
			tmp.psId = read<uint64_t>();
			uniqueId = tmp;
			break;
		}
		case Platform_Switch:
		{
			auto tmp = SwitchID();
			tmp.a = read<uint64_t>(64);
			tmp.b = read<uint64_t>(64);
			tmp.c = read<uint64_t>(64);
			tmp.d = read<uint64_t>(64);
			uniqueId = tmp;
		}
			break;
		case Platform_PsyNet:
		{
			auto tmp = PsyNetID();
			if (engineVersion >= 868 && licenseeVersion >= 24 && netVersion >= 10)
			{
				tmp.a = read<uint64_t>(64);
			}
			else
			{
				tmp.a = read<uint64_t>(64);
				tmp.b = read<uint64_t>(64);
				tmp.c = read<uint64_t>(64);
				tmp.d = read<uint64_t>(64);
			}
			uniqueId = tmp;
		}
			break;
		case Platform_Epic:
		{
			auto epicID = EpicID();
			epicID.epicId = read<std::string>();
			uniqueId = epicID;
		}
		break;
		case Platform_Unknown:
		{
			auto tmp = UnknownId();
			if (licenseeVersion > 18 && netVersion == 0)
			{
				tmp.unknown = 0;
			}
			else
			{

				tmp.unknown = read<uint32_t>(3 * 8);
			}
			uniqueId = tmp;
		}
			//printf("Unknown platform found!\n");
			break;
		default:
			//printf("Unknown platform %i", id.platform);
			//assert(1 == 2);
			break;
		}
		uint8_t splitscreenID = read<uint8_t>();
		std::visit(
			[platform, splitscreenID](UniqueId& base)
			{
				base.platform = platform;
				base.splitscreenID = splitscreenID;
			},
			uniqueId);
		
		return uniqueId;
	}


	template<typename T>
	template<typename U>
	inline const U CPPBitReader<T>::readBitsMax(const uint32_t max)
	{
		return get_bits_max<U>(max);
	}

	template<typename T>
	inline CPPBitReader<T>::CPPBitReader(const T * data, size_t sizee, std::shared_ptr<ReplayFileData> owner_) 
	: engineVersion(owner_->header.engineVersion), licenseeVersion(owner_->header.licenseeVersion), 
	netVersion(owner_->header.netVersion), size(sizee)
	{
		bitreader_init(&b, (uint8_t*)data, sizee);
	}

	template<typename T>
	inline CPPBitReader<T>::CPPBitReader(const T * data, size_t sizee, std::shared_ptr<ReplayFileData> owner_, 
		const uint32_t engineV, const uint32_t licenseeV, const uint32_t netV) : engineVersion(engineV), 
		licenseeVersion(licenseeV), netVersion(netV), size(sizee)
	{
		bitreader_init(&b, (uint8_t*)data, sizee);
	}

	template<typename T>
	inline CPPBitReader<T>::CPPBitReader(const CPPBitReader& other) 
	: engineVersion(other.engineVersion), licenseeVersion(other.licenseeVersion), netVersion(other.netVersion), size(other.size)
	{
		bitreader_init(&b, other.b.data, other.b.len);
	}

	template<typename T>
	inline CPPBitReader<T>::CPPBitReader() : engineVersion(0), licenseeVersion(0), netVersion(0)
	{

	}

	template<typename T>
	inline const float CPPBitReader<T>::readFixedCompressedFloat(const int32_t maxValue, const int32_t numBits)
	{
		const int32_t maxBitValue = (1 << (numBits - 1)) - 1;
		const int32_t bias = (1 << (numBits - 1));
		const int32_t serIntMax = (1 << (numBits - 0));

		const int32_t delta = readBitsMax<int32_t>(serIntMax);
		const float unscaledValue = static_cast<float>(delta - bias);

		if (maxValue > maxBitValue)
		{
			// We have to scale down, scale needs to be a float:
			const float invScale = maxValue / (float)maxBitValue;
			return unscaledValue * invScale;
		}
		
		const float scale = maxBitValue / (float)maxValue;
		const float invScale = 1.0f / (float)scale;

		return unscaledValue * invScale;
	}

	template <typename T>
	inline const bool CPPBitReader<T>::canRead() const noexcept
	{
		return GetAbsoluteBitPosition() < b.len;
	}

	template <typename T>
	inline const bool CPPBitReader<T>::canRead(int bits) const noexcept
	{
		return GetAbsoluteBitPosition() + bits < b.len;
	}

	template <typename T>
	void CPPBitReader<T>::goback(int32_t num)
	{
		if (b.validBits + num > 64)
		{
			//num -= b.validBits;
			int old = b.validBits;
			if (old + num > 64)
				int dfsdf = 5;
			//num -= b.validBits;
			b.validBits = 0;

			b.bytes_read -= ((num / 64) + 2) * 8;
			bitreader_load_cache(&b);
			b.validBits = ((num + old) % 64);
			//b.validBits = 64 - (num % 64);
		}
		else
		{
			b.validBits += num; //no need to reset cache
		}
		/*constexpr uint32_t SIZE_IN_BITS = (sizeof(T) * 8);

		if (static_cast<int32_t>(bit_position) - num < 0)
		{
			num -= bit_position;
			bit_position = SIZE_IN_BITS - (num % SIZE_IN_BITS);
			t_position -= (abs(num)) / SIZE_IN_BITS + 1;
		}
		else
		{
			bit_position -= num;
		}
		data = start + t_position;*/
	}

	template<typename T>
	inline void CPPBitReader<T>::skip(uint32_t num)
	{
		/*for(int i = 0; i < num; ++i)
			read<uint64_t>(1);*/
			//b.bytes_read += (num / 64) * 8;

			//bitreader_t br2 = b;
			//uint32_t num2 = num;
		if (b.validBits <= num)
		{

			num -= b.validBits;
			b.validBits = 0;
			b.bytes_read += ((num / 64)) * 8;
			bitreader_load_cache(&b);
			b.validBits = (64 - (num % 64));
		}
		else
		{
			b.validBits -= num;
		}
		
	}

	template<typename T>
	const size_t CPPBitReader<T>::GetAbsoluteBytePosition() const noexcept
	{
		return (b.bytes_read) - (b.validBits / 8);
	}

	template<typename T>
	const size_t CPPBitReader<T>::GetAbsoluteBitPosition() const noexcept
	{
		return (b.bytes_read * 8) - (b.validBits);
	};

	template<typename T>
	template<typename U>
	inline const U CPPBitReader<T>::read()
	{
		return get_bits<U>(sizeof(U) * 8);
	}

	template<typename T>
	template<typename U>
	inline const U CPPBitReader<T>::read(uint16_t customSize)
	{
		return get_bits<U>(customSize);
	}
}
