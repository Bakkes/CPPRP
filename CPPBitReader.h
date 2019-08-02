#pragma once
#include <string>
#include <sstream>
#include <stdint.h>
#include <assert.h>

struct Vector3
{
	float x, y, z;

	std::string ToString()
	{
		return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
	}
};

struct Vector3I
{
	int x, y, z;

	explicit operator Vector3() const 
	{
		return Vector3
		{
			static_cast<float>(x),
			static_cast<float>(y),
			static_cast<float>(z)
		};
	};

	std::string ToString()
	{
		return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
	}
};

struct Rotator
{
	int pitch, yaw, roll;

	std::string ToString()
	{
		return std::to_string(pitch) + ", " + std::to_string(yaw) + ", " + std::to_string(roll);
	}
};

struct Quat
{
	float w, x, y, z;

	std::string ToString()
	{
		return std::to_string(w) + ", " + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
	}
};

enum Platform
{
	Platform_Unknown = 0,
	Platform_Steam = 1,
	Platform_PS4 = 2,
	Platform_PS3 = 3,
	Platform_Dingo = 4,
	Platform_QQ = 5,
	Platform_WeGame = 6,
	Platform_NNX = 7,
	Platform_PsyNet = 8,
	Platform_MAX = 9
};

struct UniqueId
{
	uint8_t platform;
	uint8_t playerNumber;
	uint64_t uniqueID;

	std::string ToString() const
	{
		return std::to_string(platform) + "|" + std::to_string(uniqueID) + "|" + std::to_string(playerNumber);
	}

	friend std::ostream& operator<<(std::ostream& os, UniqueId& foo)
	{
		os << foo.ToString();
		return os;
	}

} ;

#define QUAT_NUM_BITS (18)
#define MAX_QUAT_VALUE (0.7071067811865475244f)
#define MAX_QUAT_VALUE_INVERSE (1.0f / MAX_QUAT_VALUE)

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

	return MultiplyDeBruijnBitPosition[(const uint32_t)(v * 0x07C4ACDDU) >> 27];
}



//Attempt at writing a fast bitreader for RL replays
template<typename T>
class CPPBitReader
{
public:
	const T *start;
	const T *data;
	uint32_t size;
	uint32_t t_position;
	uint32_t bit_position;

private:
	template<typename X>
	const X get_bits(uint16_t n)
	{
		constexpr uint8_t SIZE_T = sizeof(T) * 8;
		X result = 0;
		uint8_t bit_pos = 0;
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

			if (bit_position == sizeof(X) * 8)
			{
				bt = *(++data);
				t_position++;
				bit_position = 0;
			}
		}
		return result;
	}
public:

	CPPBitReader(const T * data, uint32_t size)
	{
		this->start = data;
		this->data = data;
		this->size = size;
		this->t_position = 0;
		this->bit_position = 0;
	}

	CPPBitReader()
	{
		this->start = NULL;
		this->data = NULL;
		this->size = 0;
		this->t_position = 0;
		this->bit_position = 0;
	}

	////Maybe remove and force other to be called
	//const bool read_bool()
	//{
	//	return read<bool>();
	//}

	template<typename T>
	const T read()
	{
		return get_bits<T>(sizeof(T) * 8);
	}

	template<typename T>
	const T read(uint16_t customSize)
	{
		return get_bits<T>(customSize);
	}

	template<>
	const bool read()
	{
		return get_bits<uint8_t>(1);
	}

	//Float requires special casting since bit operations aren't allowed
	template<>
	const float read()
	{
		assert(sizeof(float) == sizeof(uint32_t));
		uint32_t value = read<uint32_t>();
		return reinterpret_cast<float&>(value);
	}

	template<>
	const Vector3I read()
	{
		
		const uint32_t num_bits = readBitsMax<int32_t>(22);

		const int32_t bias = 1 << (int)(num_bits + 1);
		const int32_t max = (int)num_bits + 2;

		const int32_t dx = read<int32_t>(max);
		const int32_t dy = read<int32_t>(max);
		const int32_t dz = read<int32_t>(max);
		return { (dx - bias), (dy - bias), (dz - bias) };
	}

	template<>
	const Vector3 read()
	{
		Vector3I v = read<Vector3I>();
		return { v.x / 100.f, v.y / 100.f, v.z / 100.f };
	}

	template<>
	const Rotator read()
	{
		constexpr float conversion = 360.f / 256.f;
		Rotator ret {0};
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
	const Quat read()
	{
		uint8_t largest = read<uint8_t>(2);
		float a = uncompress_quat(read<uint32_t>(QUAT_NUM_BITS));
		float b = uncompress_quat(read<uint32_t>(QUAT_NUM_BITS));
		float c = uncompress_quat(read<uint32_t>(QUAT_NUM_BITS));
		float extra = sqrtf(1.f - (a*a) - (b*b) - (c * c));

		Quat q;
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
	const UniqueId read()
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
			id.uniqueID = read<uint64_t>(40 * 8);
			break;
		case Platform_NNX:
		case Platform_PsyNet:
			id.uniqueID = read<uint64_t>(32 * 8);
			break;
		case Platform_Unknown:
			id.uniqueID = read<uint64_t>(3 * 8);
			printf("Unknown platform found!\n");
			break;
		default:
			printf("Unknown platform %i", id.platform);
			assert(1 == 2);
			break;
		}
		id.playerNumber = read<uint8_t>();
		return id;
	}

	template<>
	const std::string read()
	{
		int32_t length = read<int32_t>();
		const int32_t final_length = length * (length > 0 ? 1 : -2);
		
		std::string str;
		str.resize(final_length-1);
		//Possible optimization (but later)
		//if (reader->bit_position == 0) //hit 94 times
		//{
		//	for (int i = 0; i < length; i++)
		//	{
		//		str[i] = (*reader->data++);
		//		reader->t_position++;
		//	}
		//}
		//else 
		
		for (int32_t i = 0; i < final_length; ++i) //hit 626 times
		{
			str[i] = read<uint8_t>();
		}
		//str[i] = '\0';
		
		return str;
	}

	template<typename T>
	const T readBitsMax(const uint32_t max)
	{
		return get_bits_max<T>(max);
	}

	inline const bool canRead()
	{
		return t_position < size;
	}

	//Not yet implemented
	void skip(uint32_t num)
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
		//read<uint32_t>(num);//Not clean if reading > 32 bits
	}

	const size_t GetBytePosition()
	{
		return t_position * sizeof(T);
	}

	const size_t GetAbsoluteBitPosition()
	{
		return GetBytePosition() * 8 + bit_position;
	}
	
};
