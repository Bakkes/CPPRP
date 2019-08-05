#include "ParseException.h"
namespace CPPRP
{


	PropertyDoesNotExistException::PropertyDoesNotExistException(const std::string propertyName_) : propertyName(propertyName_)
	{
		std::stringstream ss;
		ss << "Property with key \"" << propertyName << "\" does not exist in properties map.";
		errorMsg = ss.str().c_str();
	}

	const char * PropertyDoesNotExistException::what() const throw()
	{

		return errorMsg.c_str();
	}

	InvalidVersionException::InvalidVersionException(const uint32_t engine, const uint32_t licensee, const uint32_t net) : engineVersion(engine), licenseeVersion(licensee), netVersion(net)
	{
		std::stringstream ss;
		ss << "Invalid version (" << engineVersion << ", " << licenseeVersion << ", " << netVersion << ")";
		errorMsg = ss.str();
	}

	const char * InvalidVersionException::what() const throw()
	{
		return errorMsg.c_str();
	}
};