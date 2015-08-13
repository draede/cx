
#pragma once


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/String.hpp"
#include "CX/IO/IInputStream.hpp"
#include "CX/Archive/BLOB.hpp"


namespace CX
{

namespace Archive
{

class IReader
{
public:

	virtual ~IReader() { }

	virtual Status Begin(IO::IInputStream *pInputStream) = 0;

	virtual Status End() = 0;

	virtual Status ReadBool(Bool &v, const Char *szName = NULL) = 0;

	virtual Status ReadInt8(Int8 &v, const Char *szName = NULL) = 0;

	virtual Status ReadUInt8(UInt8 &v, const Char *szName = NULL) = 0;

	virtual Status ReadInt16(Int16 &v, const Char *szName = NULL) = 0;

	virtual Status ReadUInt16(UInt16 &v, const Char *szName = NULL) = 0;

	virtual Status ReadInt32(Int32 &v, const Char *szName = NULL) = 0;

	virtual Status ReadUInt32(UInt32 &v, const Char *szName = NULL) = 0;

	virtual Status ReadInt64(Int64 &v, const Char *szName = NULL) = 0;

	virtual Status ReadUInt64(UInt64 &v, const Char *szName = NULL) = 0;

	virtual Status ReadFloat(Float &v, const Char *szName = NULL) = 0;

	virtual Status ReadDouble(Double &v, const Char *szName = NULL) = 0;

	virtual Status ReadString(String &v, const Char *szName = NULL) = 0;

	virtual Status ReadBLOB(BLOB &v, const Char *szName = NULL) = 0;

	virtual Status BeginObject(const Char *szName = NULL) = 0;

	virtual Status EndObject() = 0;

	virtual Status BeginArray(const Char *szName = NULL) = 0;

	virtual Status EndArray() = 0;

};

}//namespace Archive

}//namespace CX

