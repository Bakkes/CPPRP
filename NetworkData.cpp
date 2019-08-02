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