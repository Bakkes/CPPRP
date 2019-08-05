#include "ParseException.h"

PropertyDoesNotExistException::PropertyDoesNotExistException(const std::string propertyName_) : propertyName(propertyName_)
{
}

const char * PropertyDoesNotExistException::what() const throw()
{
	std::stringstream ss;
	ss << "Property with key \"" << propertyName << "\" does not exist in properties map.";
	return ss.str().c_str();
}

InvalidVersionException::InvalidVersionException(const uint32_t engine, const uint32_t licensee, const uint32_t net) : engineVersion(engine), licenseeVersion(licensee), netVersion(net)
{
}

const char * InvalidVersionException::what() const throw()
{
	std::stringstream ss;
	ss << "Invalid version (" << engineVersion << ", " << licenseeVersion << ", " << netVersion << ")";
	return ss.str().c_str();
}
