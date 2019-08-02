#pragma once
#include <stdint.h>
#include <sstream>
#include "reflect.h"
#include "CPPBitReader.h"
#define exp(...) __VA_ARGS__
#define ALLEH_ZEG(_1, _2) _1##_2
#define COMBINE(_1,_2) ALLEH_ZEG(_1, _2)

#define MEMBER_TYPE(type, name) type name;
#define READ_TYPE(type, name) name = reader.read<type>();
#define WRITE_TYPE(type, name) /*this->##name.Write(writer);*/
#define WRITE_TO_SS(type, name) ss << sep << #name" = " << std::to_string(##name);  sep = ", ";

#define MEMBERS_0()
#define MEMBERS_2(a, _1, _2)  a(_1, _2) 
#define MEMBERS_4(a, _1, _2, _3, _4)  MEMBERS_2(a, _1, _2)  MEMBERS_2( a, _3, _4) 
#define MEMBERS_6(a, _1, _2, _3, _4, _5, _6)  MEMBERS_4(a, _1, _2, _3, _4) MEMBERS_2(a, _5, _6) 
#define MEMBERS_8(a, _1, _2, _3, _4, _5, _6, _7, _8)  MEMBERS_6(a, _1, _2, _3, _4, _5, ##_6) MEMBERS_2(a, _7, _8) 
#define MEMBERS_10(a, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10)  MEMBERS_8(a, _1, _2, _3, _4, _5, _6, _7, _8) MEMBERS_2(a, _9, _10) 
#define MEMBERS_12(a, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12)  MEMBERS_10(a, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10) MEMBERS_2(a, _11, _12) 

template<typename T>
class NetworkStreamItemParser
{
public:
	//std::shared_ptr<T> val;
public:
	
	//template<typename T>
	virtual void Consume(CPPBitReader<T>& reader) {}
	virtual std::string ToString() { return ""; }
	//virtual const std::string ToString() = 0;
};


#define _NetworkStreamItem(networkStreamItemName, ...)\
class NetworkStreamItemParser##networkStreamItemName : public NetworkStreamItemParser<uint32_t>\
{\
public:\
exp(COMBINE(COMBINE(MEMBERS_, _PP_COUNT_VA(__VA_ARGS__)),(##MEMBER_TYPE,  __VA_ARGS__)))\
public:\
\
std::string ToString() {\
std::stringstream ss;\
std::string sep = "";\
exp(COMBINE(COMBINE(MEMBERS_, _PP_COUNT_VA(__VA_ARGS__)),(##WRITE_TO_SS,  __VA_ARGS__)))\
return ss.str();\
}\


#define NetworkStreamItem(networkStreamItemName, ...)\
	_NetworkStreamItem(networkStreamItemName, __VA_ARGS__)\
\
\
void Consume(CPPBitReader<uint32_t>& reader)\
{\
exp(COMBINE(COMBINE(MEMBERS_, _PP_COUNT_VA(__VA_ARGS__)),(##READ_TYPE,  __VA_ARGS__)))\
}\
};

#define NetworkStreamItemNoConsume(networkStreamItemName, ...) \
	_NetworkStreamItem(networkStreamItemName, __VA_ARGS__)\
\
\
void Consume(CPPBitReader<uint32_t>& reader);\
};

namespace std
{
	static inline std::string to_string(std::string s) { return s; }
	static inline std::string to_string(Quat q) { return q.ToString(); }
	static inline std::string to_string(Vector3 v) { return v.ToString(); }
	static inline std::string to_string(Vector3I v) { return v.ToString(); }
	static inline std::string to_string(UniqueId v) { return v.ToString(); }
}

NetworkStreamItemNoConsume(RBState, bool, sleeping, Vector3, position, Quat, rotation, Vector3, linearVelocity, Vector3, angularVelocity);

//NetworkStreamItem(RBState, bool, sleeping, Vector3, position, Quat, rotation);
NetworkStreamItem(LogoData, bool, unknown, uint32_t, logoId);
NetworkStreamItem(ActiveActor, bool, active, int32_t, actorId);
NetworkStreamItem(ObjectTarget, bool, active, int32_t, object_index);
NetworkStreamItem(String, std::string, str);

NetworkStreamItem(Bool, bool, value);
NetworkStreamItem(Uint8_t, uint8_t, value);
NetworkStreamItem(Uint32_t, uint32_t, value);
NetworkStreamItem(Uint64_t, uint64_t, value);
NetworkStreamItem(Int32_t, int32_t, value);
NetworkStreamItem(Float, float, value);

NetworkStreamItem(Vector3I, Vector3I, vector);
NetworkStreamItem(UniqueId, UniqueId, id);
NetworkStreamItem(PickupData, bool, unknown1, uint32_t, actorId, bool, pickedUp);



//
//class NetworkStreamItemParserRBState : public NetworkStreamItemParser<uint32_t>
//{
//public:
//	bool sleeping;
//	Vector3 position;
//	Quat rotation;
//	Vector3 linearVelocity;
//	Vector3 angularVelocity;
//public:
//	void Consume(CPPBitReader<uint32_t>& reader) override
//	{
//		//this->val = std::make_shared<ASDF>();
//		this->sleeping = reader.read<bool>();
//		this->position = reader.read<Vector3>();
//		this->rotation = reader.read<Quat>();
//		if (!this->sleeping)
//		{
//			this->linearVelocity = reader.read<Vector3>();
//			this->angularVelocity = reader.read<Vector3>();
//		}
//	}
//
//	std::string ToString()
//	{
//		std::stringstream ss;
//		ss << "sleeping = " << sleeping << ", position = " << position.ToString()
//			<< ", rotation = " << rotation.ToString()
//			<< ", linearVelocity = " << linearVelocity.ToString()
//			<< ", angularVelocity = " << angularVelocity.ToString();
//		return ss.str();
//	}
//};

class NetworkStreamItemParserReservation : public NetworkStreamItemParser<uint32_t>
{
public:
	uint8_t unknown;
	UniqueId playerId;
	std::string playerName;
	uint8_t unknown2;
public:
	void Consume(CPPBitReader<uint32_t>& reader) override
	{
		//this->val = std::make_shared<ASDF>();
		this->unknown = reader.read<uint8_t>(3);
		this->playerId = reader.read<UniqueId>();
		if (this->playerId.platform != Platform_Unknown)
		{
			this->playerName = reader.read<std::string>();
		}
		this->unknown2 = reader.read<uint8_t>();
	}

	std::string ToString()
	{
		std::stringstream ss;
		ss << playerId.ToString() + "|" + playerName;
		return ss.str();
	}
};

class NetworkStreamItemParserSkillTier : public NetworkStreamItemParser<uint32_t>
{
public:
	uint32_t skillTier;
public:
	void Consume(CPPBitReader<uint32_t>& reader) override
	{
		this->skillTier = reader.readBitsMax<uint32_t>(500);
	}

	std::string ToString()
	{
		std::stringstream ss;
		ss << "Skill: " << std::to_string(skillTier);
		return ss.str();
	}
};