#pragma once
#include "CPPBitReader.h"
#include "NetworkData.h"
#include <sstream>
#include "ParseException.h"

namespace CPPRP
{


	template<typename T>
	inline const T Consume(CPPBitReader<BitReaderType>& reader) { return reader.read<T>(); }


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
	inline const std::vector<T> ConsumeVector(CPPBitReader<BitReaderType>& reader) {
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
	inline const std::shared_ptr<ProductAttribute> Consume(CPPBitReader<BitReaderType>& reader) {
		std::shared_ptr<ProductAttribute> prodAttr;
		bool unknown1 = reader.read<bool>();
		uint32_t class_index = reader.read<uint32_t>();
		if (class_index > reader.owner->objects.size())
		{
			throw AttributeParseException<BitReaderType>("ProductAttribute", reader);
		}
		std::string className = reader.owner->objects[class_index];

		if (className.compare("TAGame.ProductAttribute_UserColor_TA") == 0)
		{
			if (reader.licenseeVersion >= 23)
			{
				std::shared_ptr<ProductAttributeUserColorRGB> ucargb = std::make_shared<ProductAttributeUserColorRGB>();

				ucargb->r = reader.read<uint8_t>();
				ucargb->g = reader.read<uint8_t>();
				ucargb->b = reader.read<uint8_t>();
				ucargb->a = reader.read<uint8_t>();
				prodAttr = ucargb;
			}
			else
			{
				std::shared_ptr<ProductAttributeUserColorSingle> ucas = std::make_shared<ProductAttributeUserColorSingle>();

				ucas->has_value = reader.read<bool>();
				if (ucas->has_value)
				{
					ucas->value = reader.read<uint32_t>(31);
				}
				else
				{
					ucas->value = 0;
				}
				prodAttr = ucas;
			}
		}
		else if (className.compare("TAGame.ProductAttribute_Painted_TA") == 0)
		{
			std::shared_ptr<ProductAttributePainted> pad = std::make_shared<ProductAttributePainted>();

			if (reader.engineVersion >= 868 && reader.licenseeVersion >= 18)
			{
				pad->value = reader.read<uint32_t>(31);
			}
			else
			{
				pad->value = reader.readBitsMax<uint32_t>(14);
			}
			prodAttr = pad;
		}
		else if (className.compare("TAGame.ProductAttribute_TeamEdition_TA") == 0)
		{
			std::shared_ptr<ProductAttributeTeamEdition> teamEdition = std::make_shared<ProductAttributeTeamEdition>();
			teamEdition->value = reader.read<uint32_t>(31);
			prodAttr = teamEdition;

		}
		else if(className.compare("TAGame.ProductAttribute_SpecialEdition_TA") == 0)
		{
			std::shared_ptr<ProductAttributeSpecialEdition> specialEdition = std::make_shared<ProductAttributeSpecialEdition>();
			specialEdition->value = reader.read<uint32_t>(31);
			prodAttr = specialEdition;
		}
		else if (className.compare("TAGame.ProductAttribute_TitleID_TA") == 0)
		{
			std::shared_ptr<ProductAttributeTitle> title = std::make_shared<ProductAttributeTitle>();
			title->title = reader.read<std::string>();
			prodAttr = title;
			
		}
		else
		{
			printf("Can not read product attribute %s@@@@\n", className.c_str());
		}
		prodAttr->unknown1 = unknown1;
		prodAttr->class_index = class_index;
		return prodAttr;
	}


	template<>
	inline const ClientLoadout Consume(CPPBitReader<BitReaderType>& reader) {
		ClientLoadout item;
		PREFETCH((char*)(reader.data));
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
	inline const ReplicatedRBState Consume(CPPBitReader<BitReaderType>& reader) {
		ReplicatedRBState item;
		const uint32_t netVersion = reader.netVersion;
		//PREFETCH((char*)(reader.data));
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
	inline const PartyLeader Consume(CPPBitReader<BitReaderType>& reader) {
		PartyLeader item{ 0 };
		uint8_t test = reader.read<uint8_t>();
		if (test != 0)
		{
			reader.goback(8);
			item.id = reader.read<std::shared_ptr<UniqueId>>();
		}
		else
		{

		}
		return item;
	}


	template<>
	inline const GameMode Consume(CPPBitReader<BitReaderType>& reader) {
		GameMode item;
		if (reader.engineVersion >= 868 && reader.licenseeVersion >= 12)
		{
			item.gamemode = reader.read<uint8_t>();
		}
		else
		{
			item.gamemode = reader.read<uint8_t>(2);
		}
		return item;
	}

	template<>
	inline const Reservation Consume(CPPBitReader<BitReaderType>& reader) {
		Reservation item;
		item.unknown = reader.read<uint8_t>(3);
		item.player_id = reader.read<std::shared_ptr<UniqueId>>();
		if (item.player_id->platform == Platform_Unknown && (reader.licenseeVersion < 18 || reader.netVersion != 0))
		{
			
		}
		else
		{
			item.player_name = reader.read<std::string>();
		}

		if (reader.engineVersion >= 868 && reader.licenseeVersion >= 12)
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
	inline const std::string ToString(const std::shared_ptr<UniqueId>& item)
	{
		return "";
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


	ToStringStd(uint16_t);
	ToStringStd(uint32_t);
	ToStringStd(int);

}
#include "generated.h"