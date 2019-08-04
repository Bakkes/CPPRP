#pragma once
#include "CPPBitReader.h"
#include "NetworkData.h"
#include <sstream>
#include "rapidjson/prettywriter.h"

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
	writer.Uint64(item.uniqueID);
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

template<typename T>
inline const T Consume(CPPBitReader<uint32_t>& reader) { return reader.read<T>(); }


template<typename T>
inline const std::string ToString(const T& item) { 
	std::stringstream ss;
	ss << "ERR. ToString not declared for " << typeid(T).name() << "\n";
	return ss.str(); 
}

template<typename T>
inline const std::string ToString(const std::vector<T>& item) {
	std::stringstream ss;
	const size_t size = item.size();
	for (size_t i = 0; i < size; ++i)
	{
		ss << "\t[" << i << "] - " << ToString(item.at(i)) << "\n";
	}
	//ss << "ERR. ToString not declared for " << typeid(T).name() << "\n";
	return ss.str();
}

template<typename T>
inline const std::vector<T> ConsumeVector(CPPBitReader<uint32_t>& reader) {
	uint8_t size = reader.read<uint8_t>();
	std::vector<T> vec;
	vec.resize(size);
	for (uint8_t i = 0; i < size; i++)
	{
		vec[i] = Consume<T>(reader);
	}
	return vec;
}

template<>
inline const ProductAttribute Consume(CPPBitReader<uint32_t>& reader) {
	ProductAttribute item;
	item.unknown1 = reader.read<bool>();
	item.class_index = reader.read<uint32_t>();
	//TODO: read classnames
	std::string className = reader.owner->objects[item.class_index];
	if (className.compare("TAGame.ProductAttribute_UserColor_TA") == 0)
	{
		//TODO: assign this
		//const UserColorAttribute uca = Consume<UserColorAttribute>(reader);
		if (reader.owner->header.licenseeVersion >= 23) 
		{
			UserColorAttribute uca;
			uca.r = reader.read<uint8_t>();
			uca.g = reader.read<uint8_t>();
			uca.b = reader.read<uint8_t>();
			uca.a = reader.read<uint8_t>();
		}
		else
		{
			item.has_value = reader.read<bool>();
			if (item.has_value)
			{
				item.value = reader.read<uint32_t>(31);
			}
		}
	}
	else if (className.compare("TAGame.ProductAttribute_Painted_TA") == 0)
	{
		if (reader.owner->header.engineVersion >= 868 && reader.owner->header.licenseeVersion >= 18)
		{
			reader.read<uint32_t>(31);
		}
		else
		{
			reader.readBitsMax<uint32_t>(14);
		}
	}
	else if (className.compare("TAGame.ProductAttribute_TeamEdition_TA") == 0
		|| className.compare("TAGame.ProductAttribute_SpecialEdition_TA") == 0)
	{
		//TODO: assign
		reader.read<uint32_t>(31); //Only read 31 bits?
	}
	else if (className.compare("TAGame.ProductAttribute_TitleID_TA") == 0)
	{
		reader.read<std::string>();
	}
	else
	{
		printf("Can not read product attribute %s@@@@\n", className.c_str());
	}
	return item;
}


template<>
inline const ClientLoadout Consume(CPPBitReader<uint32_t>& reader) {
	ClientLoadout item;
	item.version = reader.read<uint8_t>();
	item.body = reader.read<uint32_t>();
	item.skin = reader.read<uint32_t>();
	item.wheels = reader.read<uint32_t>();
	item.boost = reader.read<uint32_t>();
	item.antenna = reader.read<uint32_t>();
	item.hat = reader.read<uint32_t>();
	item.unknown2 = reader.read<uint32_t>();
	if (item.version > 10)
	{
		item.unknown3 = reader.read<uint32_t>();
	}
	if (item.version >= 16)
	{
		item.engine_audio = reader.read<uint32_t>();
		item.trail = reader.read<uint32_t>();
		item.goal_explosion = reader.read<uint32_t>();
	}
	if (item.version >= 17)
	{
		item.banner = reader.read<uint32_t>();
	}
	if (item.version >= 19)
	{
		item.unknown4 = reader.read<uint32_t>();
	}

	if (item.version >= 22)
	{
		item.unknown5 = reader.read<uint32_t>();
		item.unknown6 = reader.read<uint32_t>();
		item.unknown7 = reader.read<uint32_t>();
	}
	return item;
}

//
//template<>
//inline const GameTime Consume(CPPBitReader<uint32_t>& reader) {
//	static int test = 34;
//	//test++;
//	if (test == 256) system("pause");
//	printf("Parsing gametime with test = %i\n", test);
//	GameTime item;
//	item.value = reader.read<uint32_t>(test);
//	return item;
//}

template<>
inline const ReplicatedRBState Consume(CPPBitReader<uint32_t>& reader) {
	ReplicatedRBState item;
	const uint32_t netVersion = reader.owner->header.netVersion;
	
	item.sleeping = reader.read<bool>();
	
	if (netVersion >= 5)
	{
		item.position = reader.read<Vector3>();
	}
	else
	{
		item.position = static_cast<Vector3>(reader.read<Vector3I>());
	}

	if (netVersion >= 7)
	{
		item.rotation = reader.read<Quat>();
	}
	else
	{
		item.rotation.x = reader.readFixedCompressedFloat(1, 16);
		item.rotation.y = reader.readFixedCompressedFloat(1, 16);
		item.rotation.z = reader.readFixedCompressedFloat(1, 16);
		item.rotation.w = 0;
	}

	if (!item.sleeping)
	{
		item.linear_velocity = reader.read<Vector3>();
		item.angular_velocity = reader.read<Vector3>();
	}
	else
	{
		item.linear_velocity = { 0 };
		item.angular_velocity = { 0 };
	}
	return item;
}

template<>
inline const GameMode Consume(CPPBitReader<uint32_t>& reader) {
	GameMode item;
	if (reader.owner->header.engineVersion >= 868 && reader.owner->header.licenseeVersion >= 12)
	{
		item.gamemode = reader.read<uint8_t>();
	}
	else
	{
		item.gamemode = reader.readBitsMax<uint8_t>(4);
	}
	return item;
}

template<>
inline const Reservation Consume(CPPBitReader<uint32_t>& reader) {
	Reservation item;
	item.unknown = reader.read<uint8_t>(3);
	item.player_id = reader.read<UniqueId>();
	if (item.player_id.platform != Platform_Unknown)
	{
		item.player_name = reader.read<std::string>();
	}

	if (reader.owner->header.engineVersion >= 868 && reader.owner->header.licenseeVersion >= 12)
	{
		item.unknown2 = reader.read<uint8_t>();
		
	}
	else 
	{
		item.unknown2 = reader.read<uint8_t>(2);
	}
	
	return item;
}

template<>
inline const std::string ToString(const UniqueId& item) 
{ 
	return item.ToString();
}

template<>
inline const std::string ToString(const Vector3I& item) { return item.ToString(); }

template<>
inline const std::string ToString(const Vector3& item) { return item.ToString(); }
template<>
inline const std::string ToString(const Quat& item) { return item.ToString(); }
template<>
inline const std::string ToString(const Rotator& item) { return item.ToString(); }
template<>
inline const std::string ToString(const std::string& item) { return item; }
template<>
inline const std::string ToString(const bool& item) { return item ? "true" : "false"; }
template<>
inline const std::string ToString(const uint8_t& item) { return std::to_string((int)item); }
template<>
inline const std::string ToString(const float& item) { return std::to_string(item); }
template<>
inline const std::string ToString(const uint64_t& item) { return std::to_string(item); }


#define ToStringStd(type)\
template<>\
inline const std::string ToString(const type & item) { return std::to_string(item); }


ToStringStd(uint16_t)
ToStringStd(uint32_t)
ToStringStd(int)
#include "generated.h"