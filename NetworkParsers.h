#pragma once
#include "CPPBitReader.h"
#include "NetworkData.h"
#include <sstream>
template<typename T>
inline const T Consume(CPPBitReader<uint32_t>& reader) { return reader.read<T>(); }

template<typename T>
inline const std::string ToString(T& item) { return "ERR"; }

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
		UserColorAttribute item;
		item.r = reader.read<uint8_t>();
		item.g = reader.read<uint8_t>();
		item.b = reader.read<uint8_t>();
		item.a = reader.read<uint8_t>();
	}
	else if (className.compare("TAGame.ProductAttribute_TeamEdition_TA") == 0
		|| className.compare("TAGame.ProductAttribute_Painted_TA") == 0
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

template<>
inline const ReplicatedRBState Consume(CPPBitReader<uint32_t>& reader) {
	ReplicatedRBState item;
	item.sleeping = reader.read<bool>();
	item.position = reader.read<Vector3>();
	item.rotation = reader.read<Quat>();

	if (!item.sleeping)
	{
		item.linear_velocity = reader.read<Vector3>();
		item.angular_velocity = reader.read<Vector3>();
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
	item.unknown2 = reader.read<uint8_t>();
	return item;
}

#include "generated.h"