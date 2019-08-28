#include "ParseException.h"
namespace CPPRP
{


	PropertyDoesNotExistException::PropertyDoesNotExistException(const std::string& propertyName_) : propertyName(propertyName_)
	{
		std::stringstream ss;
		ss << "Property with key \"" << propertyName << "\" does not exist in properties map.";
		errorMsg = ss.str().c_str();
	}

	PropertyDoesNotExistException::~PropertyDoesNotExistException() {}
	const char * PropertyDoesNotExistException::what() const throw()
	{

		return errorMsg.c_str();
	}

};