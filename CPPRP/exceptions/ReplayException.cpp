#include "ReplayException.h"
#include <sstream>

namespace CPPRP
{
	InvalidVersionException::InvalidVersionException(const uint32_t engine, const uint32_t licensee, const uint32_t net) : engineVersion(engine), licenseeVersion(licensee), netVersion(net)
	{
		std::stringstream ss;
		ss << "Invalid version (" << std::to_string(engineVersion) << ", " << std::to_string(licenseeVersion) << ", " << std::to_string(netVersion) << ")";
		errorMsg = ss.str();
	}

	InvalidVersionException::~InvalidVersionException() {}

	const char * InvalidVersionException::what() const throw()
	{
		return errorMsg.c_str();
	}
};