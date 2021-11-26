#pragma once
#include <exception>
#include <string>
#include <sstream>
#include "../CPPBitReader.h"

namespace CPPRP
{
	template<typename T>
	struct ParseException : public std::exception
	{
	protected:
		const CPPBitReader<T>& bitReader;
		explicit ParseException(const CPPBitReader<T>& br);
	public:
		std::string errorMsg;
		const char* what() const throw();
	};

	template<typename T>
	struct GeneralParseException : public ParseException<T>
	{
	protected:
		std::string message;
	public:
		explicit GeneralParseException(const std::string& message, const CPPBitReader<T>& br);
	};

	struct PropertyDoesNotExistException : public std::exception
	{
	protected:
		std::string propertyName;
		std::string errorMsg;
	public:
		explicit PropertyDoesNotExistException(const std::string& propertyName_);
		virtual ~PropertyDoesNotExistException();
		const char* what() const throw();
	};


	template<typename T>
	struct AttributeParseException : public ParseException<T>
	{
	protected:
		std::string failedAttribute;
	public:
		AttributeParseException(const std::string& attribute, const CPPBitReader<T>& br);
	};

	template<typename T>
	inline ParseException<T>::ParseException(const CPPBitReader<T>& br) : bitReader(br)
	{
		std::stringstream ss;
		auto absPos = (bitReader.t_position * sizeof(T) * 8) + bitReader.bit_position;
		ss << "T_POS=" << std::to_string(bitReader.t_position) << ", BIT_POS=" << std::to_string(bitReader.bit_position) << ", ABS=" << absPos << "/" << bitReader.size;
		ss << ". Version(" << std::to_string(bitReader.engineVersion) << ", " << std::to_string(bitReader.licenseeVersion) << ", " << std::to_string(bitReader.netVersion) << ")";
		errorMsg = ss.str();
	}

	template<typename T>
	const char* ParseException<T>::what() const throw()
	{
		return errorMsg.c_str();
	}

	template<typename T>
	inline AttributeParseException<T>::AttributeParseException(const std::string& attribute, const CPPBitReader<T>& br) : ParseException<T>(br), failedAttribute(attribute)
	{
		std::stringstream ss;
		ss << "Failed to parse attribute \"" << failedAttribute << "\". ";
		this->errorMsg = ss.str() + this->errorMsg;
	}

	template<typename T>
	inline GeneralParseException<T>::GeneralParseException(const std::string& message_, const CPPBitReader<T>& br) : ParseException<T>(br), message(message_)
	{
		this->errorMsg = message + " " + this->errorMsg;
	}

}