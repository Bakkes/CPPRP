#pragma once
#include <string>
#include <vector>
#include <memory>
#include "../data/"

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

template<>
inline const std::string ToString(const std::shared_ptr<UniqueId>& item)
{
	return "ERR";
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