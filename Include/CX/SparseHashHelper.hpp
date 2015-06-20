
#pragma once


#include "CX/Platform.hpp"


#ifdef CX_OS_POSIX


#include "CX/String.hpp"
#include <tr1/functional>
#include <bits/hash_bytes.h>


namespace std
{

namespace tr1
{

template <>
struct hash<CX::String>
{
public:
	
	size_t operator()(CX::String s) const
	{
		return std::_Hash_bytes(s.c_str(), s.size() * sizeof(CX::Char), 0xc70f6907UL);
	}

};

template <>
struct hash<CX::WString>
{
public:

	size_t operator()(CX::WString s) const
	{
		return std::_Hash_bytes(s.c_str(), s.size() * sizeof(CX::WChar), 0xc70f6907UL);
	}

};

}//namespace tr1

}//namespace std


#endif
