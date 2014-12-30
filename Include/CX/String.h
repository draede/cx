
#pragma once


#include "CX/STLAlloc.h"
#include "CX/C/string.h"
#include <string>


namespace CX
{

typedef std::basic_string<Char, std::char_traits<Char>, STLAlloc<Char> >			String;

typedef std::basic_string<WChar, std::char_traits<WChar>, STLAlloc<WChar> >		WString;

struct CaseInsensitiveOrderPolicy
{
	bool operator()(const String &s1, const String &s2) const
	{
		return (0 > cx_stricmp(s1.c_str(), s2.c_str()));
	}
};

struct CaseSensitiveOrderPolicy
{
	bool operator()(const String &s1, const String &s2) const
	{
		return (0 > cx_strcmp(s1.c_str(), s2.c_str()));
	}
};

}//namespace CX

