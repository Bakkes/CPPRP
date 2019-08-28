#pragma once
#include <exception>
#include <stdint.h>
#include <string>

namespace CPPRP
{
	struct InvalidVersionException : public std::exception
	{
	protected:
		uint32_t engineVersion;
		uint32_t licenseeVersion;
		uint32_t netVersion;
		std::string errorMsg;
	public:
		InvalidVersionException(const uint32_t engine, const uint32_t licensee, const uint32_t net);
		virtual ~InvalidVersionException();
		const char* what() const throw();
	};
}
