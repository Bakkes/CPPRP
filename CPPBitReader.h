#pragma once
#include <string>
#include <sstream>
#include <stdint.h>
#include <assert.h>
//#include "ParseException.h"
#include "ReplayFileData.h"
#include "ReplayException.h"
#include <cmath>
#define QUAT_NUM_BITS (18)
#define MAX_QUAT_VALUE (0.7071067811865475244f)
#define MAX_QUAT_VALUE_INVERSE (1.0f / MAX_QUAT_VALUE)

typedef uint32_t BitReaderType;


namespace CPPRP
{

	static inline float uncompress_quat(uint32_t val)
	{
		const int MaxValue = (1 << QUAT_NUM_BITS) - 1;
		float positiveRangedValue = val / (float)MaxValue;
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


	//Attempt at writing a fast bitreader for RL replays
	template<typename T>
	class CPPBitReader
	{
	public:
		const T *start;
		const T *data;
		size_t size; //Should be in bits?
		uint32_t t_position;
		uint32_t bit_position;
		std::shared_ptr<ReplayFileData> owner;

	private:
		template<typename X>
		const X get_bits(uint16_t n)
		{
			if (GetAbsoluteBitPosition() + n > size)
			{
				throw std::runtime_error("Attempted to read beyond buffer");
			}
			constexpr uint16_t SIZE_T = sizeof(T) * 8;
			X result = 0;
			uint16_t bit_pos = 0;
			T bt = *data;
			if (bit_position > 0)
			{
				if (n > (SIZE_T - bit_position)) //n finishes this byte and needs next byte for sure
				{
					bit_pos += SIZE_T - bit_position;
					n -= SIZE_T - bit_position;
					result |= bt >> bit_position;

					bit_position = 0;
					++t_position;
					bt = *(++data);
				}
				else //n doesn't finish this byte
				{
					result |= (bt >> (bit_position)) & ((1 << n) - 1);
					bit_position += n;

					if (bit_position == SIZE_T)
					{
						bt = *(++data);
						++t_position;
						bit_position = 0;
					}

					return result;
				}
			}

			//If we reached this point, we know n > 0
			while (n > (SIZE_T - 1))
			{
				result |= bt << (bit_pos);
				++t_position;
				bt = *(++data);
				n -= SIZE_T;
				bit_pos += SIZE_T;
			}

			if (n > 0)
			{
				result |= ((bt >> (bit_position)) & ((1 << n) - 1)) << bit_pos;
				bit_position += n;

				if (bit_position == SIZE_T)
				{
					bt = *(++data);
					++t_position;
					bit_position = 0;
				}
			}

			return result;
		}

		template<typename X>
		const X get_bits_max(const X maxValue)
		{
			X result = 0;
			T bt = *data;
			const uint8_t max_bits = msbDeBruijn32(maxValue) + 1;

			for (uint8_t i = 0; i < max_bits && (result + (1 << i)) < maxValue; ++i)
			{
				result |= ((bt >> bit_position++) & 1) << i;

				if (bit_position == sizeof(T) * 8)
				{
					bt = *(++data);
					t_position++;
					bit_position = 0;
				}
			}
			return result;
		}
	public:
		CPPBitReader(const T * data, size_t size, std::shared_ptr<ReplayFileData> owner_);
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

		void goback(int32_t num);
		void skip(uint32_t num);

		const size_t GetBytePosition() const noexcept;
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
		assert(sizeof(float) == sizeof(uint32_t));
		uint32_t value = read<uint32_t>();
		return reinterpret_cast<float&>(value);
	}

	template<>
	template<>
	inline const Vector3I CPPBitReader<BitReaderType>::read<Vector3I>()
	{
		const uint32_t maxbits = owner->header.netVersion >= 7 ? 22 : 20;
		const uint32_t num_bits = readBitsMax<uint32_t>(maxbits);

		const int32_t bias = 1 << (int)(num_bits + 1);
		const int32_t max = (int)num_bits + 2;

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

	template<>
	template<>
	inline const Quat CPPBitReader<BitReaderType>::read<Quat>()
	{
		uint8_t largest = read<uint8_t>(2);
		float a = uncompress_quat(read<uint32_t>(QUAT_NUM_BITS));
		float b = uncompress_quat(read<uint32_t>(QUAT_NUM_BITS));
		float c = uncompress_quat(read<uint32_t>(QUAT_NUM_BITS));
		float extra = std::sqrt(1.f - (a*a) - (b*b) - (c * c));

		Quat q = { 0 };
		switch (largest)
		{
		case 0:
			q.w = extra;
			q.x = a;
			q.y = b;
			q.z = c;
			break;
		case 1:
			q.w = a;
			q.x = extra;
			q.y = b;
			q.z = c;
			break;
		case 2:
			q.w = a;
			q.x = b;
			q.y = extra;
			q.z = c;
			break;
		case 3:
		default:
			q.w = a;
			q.x = b;
			q.y = c;
			q.z = extra;
			break;
		};
		return q;
	}

	template<>
	template<>
	inline const UniqueId CPPBitReader<BitReaderType>::read<UniqueId>()
	{
		UniqueId id;
		id.platform = read<uint8_t>();
		switch (id.platform)
		{
		case Platform_Steam:
		case Platform_Dingo:
			id.uniqueID = read<uint64_t>(8 * 8);
			break;
		case Platform_PS4:
			if (owner->header.netVersion >= 1)
			{
				id.uniqueID = read<uint64_t>(40 * 8);
			}
			else
			{
				id.uniqueID = read<uint64_t>(32 * 8);
			}
			break;
		case Platform_Switch:
			id.uniqueID = read<uint64_t>(64);
			id.uniqueID = read<uint64_t>(64);
			id.uniqueID = read<uint64_t>(64);
			id.uniqueID = read<uint64_t>(64);
			break;
		case Platform_PsyNet:
			if (owner->header.engineVersion >= 868 && owner->header.licenseeVersion >= 24 && owner->header.netVersion >= 10)
			{
				id.uniqueID = read<uint64_t>(64);
			}
			else
			{
				id.uniqueID = read<uint64_t>(64);
				id.uniqueID = read<uint64_t>(64);
				id.uniqueID = read<uint64_t>(64);
				id.uniqueID = read<uint64_t>(64);
			}
			break;
		case Platform_Unknown:
			if (owner->header.licenseeVersion >= 18 && owner->header.netVersion == 0)
			{
				return id;
			}
			id.uniqueID = read<uint64_t>(3 * 8);
			//printf("Unknown platform found!\n");
			break;
		default:
			//printf("Unknown platform %i", id.platform);
			//assert(1 == 2);
			break;
		}
		id.playerNumber = read<uint8_t>();
		return id;
	}


	template<>
	template<>
	inline const std::string CPPBitReader<BitReaderType>::read<std::string>()
	{
		const int32_t length = read<int32_t>();
		const int32_t final_length = length * (length > 0 ? 1 : -2);
		if (final_length == 0)
		{
			return "";
		}

		if (final_length > 1024)
		{
			if (owner->header.engineVersion == 0
				&& owner->header.licenseeVersion == 0
				&& owner->header.netVersion == 0)
			{
				throw InvalidVersionException(0,0,0);
			}
			else
			{
				throw std::runtime_error("Got unwanted string length, read value " + std::to_string(length) + ", reading bytes " + std::to_string(final_length) + ". (" + std::to_string(this->bit_position) + ")");
			}
		}

		std::string str;
		str.resize(final_length - 1);

		for (int32_t i = 0; i < final_length; ++i)
		{
			str[i] = read<uint8_t>();
		}

		return str;
	}

	template<typename T>
	template<typename U>
	inline const U CPPBitReader<T>::readBitsMax(const uint32_t max)
	{
		return get_bits_max<U>(max);
	}

	template<typename T>
	inline CPPBitReader<T>::CPPBitReader(const T * data, size_t size, std::shared_ptr<ReplayFileData> owner_)
	{
		this->start = data;
		this->data = data;
		this->size = size;
		this->t_position = 0;
		this->bit_position = 0;
		this->owner = owner_;
	}

	template<typename T>
	inline CPPBitReader<T>::CPPBitReader()
	{
		this->start = NULL;
		this->data = NULL;
		this->size = 0;
		this->t_position = 0;
		this->bit_position = 0;
	}

	template<typename T>
	inline const float CPPBitReader<T>::readFixedCompressedFloat(const int32_t maxValue, int32_t numBits)

	{
		float value = 0;

		int32_t maxBitValue = (1 << (numBits - 1)) - 1;
		int32_t bias = (1 << (numBits - 1));
		int32_t serIntMax = (1 << (numBits - 0));

		int32_t delta = readBitsMax<int32_t>(serIntMax);
		float unscaledValue = static_cast<float>(delta - bias);

		if (maxValue > maxBitValue)
		{
			// We have to scale down, scale needs to be a float:
			float invScale = maxValue / (float)maxBitValue;
			value = unscaledValue * invScale;
		}
		else
		{
			float scale = maxBitValue / (float)maxValue;
			float invScale = 1.0f / (float)scale;

			value = unscaledValue * invScale;
		}

		return value;
	}

	template <typename T>
	inline const bool CPPBitReader<T>::canRead() const noexcept
	{
		return GetAbsoluteBitPosition() < size;
	}

	template <typename T>
	void CPPBitReader<T>::goback(int32_t num)
	{
		constexpr uint32_t SIZE_IN_BITS = (sizeof(T) * 8);

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
		data = start + t_position;
	}

	template<typename T>
	inline void CPPBitReader<T>::skip(uint32_t num)
	{
		constexpr uint32_t SIZE_IN_BITS = (sizeof(T) * 8);
		if (bit_position + num >= SIZE_IN_BITS)
		{
			num -= SIZE_IN_BITS - bit_position;
			bit_position = num % SIZE_IN_BITS;
			t_position += num / SIZE_IN_BITS + 1; // +1 since bit_position + num >= SIZE_IN_BITS
		}
		else
		{
			bit_position += num;
		}
		data = start + t_position;
	}

	template<typename T>
	inline const size_t CPPBitReader<T>::GetBytePosition() const noexcept
	{
		return (t_position * sizeof(T)) + (bit_position / 8);
	}

	template<typename T>
	inline const size_t CPPBitReader<T>::GetAbsoluteBitPosition() const noexcept
	{
		return (t_position * sizeof(T) * 8) + bit_position;
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
	
	
	/*const bool CPPBitReader<BitReaderType>::read<bool>()
	{
		
	}*/
}