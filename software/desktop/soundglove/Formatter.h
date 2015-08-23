#pragma once

#include "Export.h"
#include <string>
#include <sstream>

class Formatter
{
public:
	SOUNDGLOVE_API Formatter() {}
	SOUNDGLOVE_API ~Formatter() {}
	
	template <typename Type>
	SOUNDGLOVE_API Formatter & operator << (const Type & value)
	{
		m_stream << value;
		return *this;
	}

	SOUNDGLOVE_API std::string str() const        { return m_stream.str(); }
	SOUNDGLOVE_API operator std::string() const   { return m_stream.str(); }

private:
	std::stringstream m_stream;
};

