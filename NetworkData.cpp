#include "NetworkData.h"

void NetworkStreamItemParserRBState::Consume(CPPBitReader<uint32_t>& reader)
{
	this->sleeping = reader.read<bool>();
	this->position = reader.read<Vector3>();
	this->rotation = reader.read<Quat>();
	if (!this->sleeping)
	{
		this->linearVelocity = reader.read<Vector3>();
		this->angularVelocity = reader.read<Vector3>();
	}
	else
	{
		this->linearVelocity = { 0 };
		this->angularVelocity = { 0 };
	}
}

void NetworkStreamItemParserReservation::Consume(CPPBitReader<uint32_t>& reader)
{
	this->unknown = reader.read<uint8_t>(3);
	this->playerId = reader.read<UniqueId>();
	if (this->playerId.platform != Platform_Unknown)
	{
		this->playerName = reader.read<std::string>();
	}
	this->unknown2 = reader.read<uint8_t>();
}

void NetworkStreamItemParserSkillTier::Consume(CPPBitReader<uint32_t>& reader)
{
	this->skillTier = reader.readBitsMax<uint32_t>(500);
}