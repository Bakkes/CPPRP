#pragma once
#include "../CPPRP/NetworkData.h"

namespace CPPRP
{
	namespace JSON
	{

		template<typename Writer, typename T>
		inline const void Serialize(Writer& writer, const T& item)
		{
			std::stringstream ss;
			ss << "unsupported " << typeid(T).name();
			std::string o = ss.str();
			printf("%s\n", o.c_str());
			//assert(1 == 2);
			writer.String(o.c_str(), o.size());
		}

		template<typename Writer>
		inline const void Serialize(Writer& writer, const UniqueId& item)
		{
			writer.StartObject();
			writer.String("platform");
			writer.Uint(item.platform);
			writer.String("playernumber");
			writer.Uint(item.playerNumber);
			writer.String("uniqueid");
			//writer.Uint64(item.uniqueID);
			writer.EndObject();
		}

		template<typename Writer>
		inline const void Serialize(Writer& writer, const uint64_t& item)
		{
			writer.Uint64(item);
		}

		template<typename Writer>
		inline const void Serialize(Writer& writer, const int64_t& item)
		{
			writer.Int64(item);
		}

		template<typename Writer>
		inline const void Serialize(Writer& writer, const uint8_t& item)
		{
			writer.Uint(item);
		}

		template<typename Writer>
		inline const void Serialize(Writer& writer, const float& item)
		{
			writer.Double(item);
		}

		template<typename Writer>
		inline const void Serialize(Writer& writer, const std::string& item)
		{
			writer.String(item.c_str(), item.size());
		}

		template<typename Writer>
		inline const void Serialize(Writer& writer, const Vector3& item)
		{
			writer.StartObject();
			writer.String("X");
			writer.Double(item.x);
			writer.String("Y");
			writer.Double(item.y);
			writer.String("Z");
			writer.Double(item.z);
			writer.EndObject();
		}

		template<typename Writer>
		inline const void Serialize(Writer& writer, const Vector3I& item)
		{
			writer.StartObject();
			writer.String("X");
			writer.Int(item.x);
			writer.String("Y");
			writer.Int(item.y);
			writer.String("Z");
			writer.Int(item.z);
			writer.EndObject();
		}

		template<typename Writer>
		inline const void Serialize(Writer& writer, const struct Quat& item)
		{
			writer.StartObject();
			writer.String("X");
			writer.Double(item.x);
			writer.String("Y");
			writer.Double(item.y);
			writer.String("Z");
			writer.Double(item.z);
			writer.String("W");
			writer.Double(item.w);
			writer.EndObject();
		}

		template<typename Writer>
		inline const void Serialize(Writer& writer, const Rotator& item)
		{
			writer.StartObject();
			writer.String("Pitch");
			writer.Int(item.pitch);
			writer.String("Yaw");
			writer.Int(item.yaw);
			writer.String("Roll");
			writer.Int(item.roll);
			writer.EndObject();
		}


		template<typename Writer, typename T>
		inline const void Serialize(Writer& writer, const std::vector<T>& item) {
			writer.StartArray();
			const size_t size = item.size();
			for (size_t i = 0; i < size; ++i)
			{
				Serialize(writer, item.at(i));
			}
			writer.EndArray();
		}

		template<typename Writer>
		inline const void Serialize(Writer& writer, const bool& item)
		{
			writer.Bool(item);
		}

		template<typename Writer>
		inline const void Serialize(Writer& writer, uint8_t& item)
		{
			writer.Uint(item);
		}

		template<typename Writer>
		inline const void Serialize(Writer& writer, const uint16_t& item)
		{
			writer.Uint(item);
		}

		template<typename Writer>
		inline const void Serialize(Writer& writer, const uint32_t& item)
		{
			writer.Uint(item);
		}

		template<typename Writer>
		inline const void Serialize(Writer& writer, const int8_t& item)
		{
			writer.Int(item);
		}

		template<typename Writer>
		inline const void Serialize(Writer& writer, const int16_t& item)
		{
			writer.Int(item);
		}

		template<typename Writer>
		inline const void Serialize(Writer& writer, const int32_t& item)
		{
			writer.Int(item);
		}
	};
};