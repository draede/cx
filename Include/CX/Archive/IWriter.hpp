
#pragma once


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/String.hpp"
#include "CX/IO/IOutputStream.hpp"
#include "CX/Archive/BLOB.hpp"


namespace CX
{

namespace Archive
{

class IWriter
{
public:

	virtual ~IWriter() { }

	virtual Status Begin(IO::IOutputStream *pOutputStream) = 0;

	virtual Status End() = 0;

	virtual Status WriteBool(Bool v, const Char *szName = NULL) = 0;

	virtual Status WriteInt8(Int8 v, const Char *szName = NULL) = 0;

	virtual Status WriteUInt8(UInt8 v, const Char *szName = NULL) = 0;

	virtual Status WriteInt16(Int16 v, const Char *szName = NULL) = 0;

	virtual Status WriteUInt16(UInt16 v, const Char *szName = NULL) = 0;

	virtual Status WriteInt32(Int32 v, const Char *szName = NULL) = 0;

	virtual Status WriteUInt32(UInt32 v, const Char *szName = NULL) = 0;

	virtual Status WriteInt64(Int64 v, const Char *szName = NULL) = 0;

	virtual Status WriteUInt64(UInt64 v, const Char *szName = NULL) = 0;

	virtual Status WriteFloat(Float v, const Char *szName = NULL) = 0;

	virtual Status WriteDouble(Double v, const Char *szName = NULL) = 0;

	virtual Status WriteString(const String &v, const Char *szName = NULL) = 0;

	virtual Status WriteBLOB(const BLOB &v, const Char *szName = NULL) = 0;

	virtual Status BeginObject(const Char *szName = NULL) = 0;

	virtual Status EndObject() = 0;

	virtual Status BeginArray(const Char *szName = NULL) = 0;

	virtual Status EndArray() = 0;

};

}//namespace Archive

}//namespace CX

