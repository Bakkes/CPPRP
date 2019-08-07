#include "ReplayException.h"
#include <sstream>

namespace CPPRP
{
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