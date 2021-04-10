#pragma once
#include "./data/NetworkData.h"
#include <vector>
#include <sstream>
#include "./exceptions/ParseException.h"
#include "CPPBitReader.h"
#include <variant>
/*
File responsible for parsing network data, which are the fields in the game classes
Basically means it should parse all structs defined in data/NetworkData.h.
This file parses all which need special treatment (for example when stuff has changed in a version, and we need to do version checks)
Others are auto generated and written to ./generated/NetworkDataParserGenerated.h
*/

namespace CPPRP
{
	template<typename T>
	inline const T Consume(CPPBitReader<BitReaderType>& reader) { return reader.read<T>(); }



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
	inline const AttributeType Consume(CPPBitReader<BitReaderType>& reader) {
		
		//Make sure this matches attributeNames in the preprocess function
		enum class AttributeTypes : size_t
		{
			UserColor = 0,
			Painted,
			TeamEdition,
			SpecialEdition,
			TitleID,
			MAX
		};
		

		AttributeType att;
		bool unknown1 = reader.read<bool>();
		uint32_t class_index = reader.read<uint32_t>();
		size_t index = std::distance(reader.attributeIDs.begin(),
			std::find(reader.attributeIDs.begin(), reader.attributeIDs.end(), class_index));
		switch((AttributeTypes)index)
		{
			case AttributeTypes::UserColor:
			{
				if (reader.licenseeVersion >= 23)
				{
					auto tmp = ProductAttributeUserColorRGB();
					{
						tmp.r = reader.read<uint8_t>();
						tmp.g = reader.read<uint8_t>();
						tmp.b = reader.read<uint8_t>();
						tmp.a = reader.read<uint8_t>();
						att = tmp;
					}
				}
				else
				{
					auto tmp = ProductAttributeUserColorSingle();

					tmp.has_value = reader.read<bool>();
					if (tmp.has_value)
					{
						tmp.value = reader.read<uint32_t>(31);
					}
					else
					{
						tmp.value = 0;
					}
					att = tmp;
				}
			}
			break;
			case AttributeTypes::Painted:
			{
				auto tmp = ProductAttributePainted();

				if (reader.engineVersion >= 868 && reader.licenseeVersion >= 18)
				{
					tmp.value = reader.read<uint32_t>(31);
				}
				else
				{
					tmp.value = reader.readBitsMax<uint32_t>(14);
				}
				att = tmp;
			}
			break;
			case AttributeTypes::TeamEdition:
			{
				auto tmp = ProductAttributeTeamEdition();
				tmp.value = reader.read<uint32_t>(31);
				att = tmp;
			}
			break;
			case AttributeTypes::SpecialEdition:
			{
				auto tmp = ProductAttributeSpecialEdition();
				tmp.value = reader.read<uint32_t>(31);
				att = tmp;
			}
			break;
			case AttributeTypes::TitleID:
			{
				auto tmp = ProductAttributeTitle();
				tmp.title = reader.read<std::string>();
				att = tmp;
			}
			break;
			default:
			case AttributeTypes::MAX:
			{
				throw AttributeParseException<BitReaderType>("Unable to parse attribute with ID: " + std::to_string(class_index), reader);
			}
			break;
		}
		std::visit(
			[unknown1, class_index](ProductAttribute& base)
			{
				base.unknown1 = unknown1;
				base.class_index = class_index;
			},
			att);
		//std::get<ProductAttribute>(att).unknown1 = unknown1;
		//std::get<ProductAttribute>(att).class_index = class_index;
		return att;
	}


	template<>
	inline const ClientLoadout Consume(CPPBitReader<BitReaderType>& reader) {
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
	inline const ReplicatedRBState Consume(CPPBitReader<BitReaderType>& reader) {
		ReplicatedRBState item;
		const uint32_t netVersion = reader.netVersion;
		//PREFETCH((char*)(reader.data));
		item.sleeping = reader.read<bool>();

		if (netVersion >= 5)
		{
			item.position = reader.read<Vector3>();
			if (netVersion < 7)
			{
				item.position = { item.position.x * 10, item.position.y * 10, item.position.z * 10 };
			}
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
		PartyLeader item;
		uint8_t test = reader.read<uint8_t>();
		if (test != 0)
		{
			reader.goback(8);

			item.id = reader.read<OnlineID>();
		}
		else
		{
			UniqueId ui;
			ui.platform = 0;
			ui.splitscreenID = 0;
			item.id = ui;
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
	inline const PickupInfo_TA Consume(CPPBitReader<BitReaderType>& reader)
	{
		PickupInfo_TA item;

		ActiveActor itemz;
		itemz.active = reader.read<bool>();
		itemz.actor_id = reader.read<int32_t>();
		item.AvailablePickups = itemz;

		item.bItemsArePreview = reader.read<bool>();
		item.unknown = reader.read<bool>();
		
		return item;
	}

	template<>
	inline const Reservation Consume(CPPBitReader<BitReaderType>& reader) {
		Reservation item;
		item.number = reader.read<uint8_t>(3);
		item.player_id = reader.read<OnlineID>();
		
		//Is always an unique ID
		if (reinterpret_cast<UniqueId*>(&item.player_id)->platform == Platform_Unknown && (reader.licenseeVersion <= 18 || reader.netVersion != 0))
		{
		}
		else
		{
			item.player_name = reader.read<std::string>();
		}

		if (reader.engineVersion >= 868 && reader.licenseeVersion >= 12)
		{
			item.status = reader.read<uint8_t>();

		}
		else
		{
			item.status = reader.read<uint8_t>(2);
		}

		return item;
	}
#include "./generated/NetworkDataParsersGenerated.h"
}
