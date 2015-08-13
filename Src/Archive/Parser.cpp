
#include "CX/Archive/Parser.hpp"
#include "CX/IO/FileInputStream.hpp"
#include "CX/IO/Helper.hpp"
#include "CX/C/ctype.h"


namespace CX
{

namespace Archive
{

Parser::Parser()
{
}

Parser::~Parser()
{
}

Status Parser::ParseFilePath(const String &sFilePath, Object &object)
{
	IO::FileInputStream fis(sFilePath.c_str());

	if (!fis.IsOK())
	{
		return Status(Status_OpenFailed);
	}

	return Parser::Parse(&fis, object);
}

Status Parser::Parse(IO::IInputStream *pInputStream, Object &object)
{
	Vector<Byte>::Type   vectorData;
	Status               status;

	if ((status = IO::Helper::LoadStream(pInputStream, vectorData)).IsNOK())
	{
		return status;
	}

	return Parse((const Char *)&vectorData[0], vectorData.size(), object);
}

Status Parser::Parse(const String &sBuffer, Object &object)
{
	return Parse(sBuffer.c_str(), sBuffer.size(), object);
}

Status Parser::ParseWhiteSpaces(CTX *pCTX)
{
	while (pCTX->cbIndex < pCTX->cbSize && cx_isspace((Byte)*(pCTX->pBuffer + pCTX->cbIndex)))
	{
		if ('\n' == *(pCTX->pBuffer + pCTX->cbIndex))
		{
			pCTX->cColumn = 1;
			pCTX->cLine++;
		}
		else
		{
			pCTX->cColumn++;
		}
		pCTX->cbIndex++;
	}

	return Status();
}

Status Parser::ParseIdentifier(CTX *pCTX, String &sIdentifier)
{
	sIdentifier.clear();
	while (pCTX->cbIndex < pCTX->cbSize)
	{
		if (sIdentifier.empty())
		{
			if (cx_isalpha((Byte)*(pCTX->pBuffer + pCTX->cbIndex)) || '_' == *(pCTX->pBuffer + pCTX->cbIndex))
			{
				sIdentifier += *(pCTX->pBuffer + pCTX->cbIndex);
				pCTX->cColumn++;
			}
			else
			{
				return Status(Status_ParseFailed, "Invalid identifier at line {1}, column {2}", pCTX->cLine, pCTX->cColumn);
			}
		}
		else
		{
			if (cx_isalnum((Byte)*(pCTX->pBuffer + pCTX->cbIndex)) || '_' == *(pCTX->pBuffer + pCTX->cbIndex) || 
			    '.' == *(pCTX->pBuffer + pCTX->cbIndex))
			{
				sIdentifier += *(pCTX->pBuffer + pCTX->cbIndex);
				pCTX->cColumn++;
			}
			else
			{
				break;
			}
		}
		pCTX->cbIndex++;
	}

	return Status();
}

Status Parser::ParsePragma(CTX *pCTX, String &sID, String &sValue)
{
	Status   status;

	if ((status = ParseWhiteSpaces(pCTX)).IsNOK())
	{
		return status;
	}
	if ((status = ParseIdentifier(pCTX, sID)).IsNOK())
	{
		return status;
	}
	if ((status = ParseWhiteSpaces(pCTX)).IsNOK())
	{
		return status;
	}
	sValue.clear();
	while (pCTX->cbIndex < pCTX->cbSize)
	{
		if ('\n' == *(pCTX->pBuffer + pCTX->cbIndex))
		{
			pCTX->cbIndex++;
			pCTX->cColumn = 1;
			pCTX->cLine++;

			break;
		}
		else
		{
			sValue += *(pCTX->pBuffer + pCTX->cbIndex);
		}
		pCTX->cColumn++;
		pCTX->cbIndex++;
	}

	return Status();
}

Status Parser::ParseAlias(CTX *pCTX, String &sType, String &sAlias)
{
	Status   status;

	if ((status = ParseWhiteSpaces(pCTX)).IsNOK())
	{
		return status;
	}
	if ((status = ParseIdentifier(pCTX, sType)).IsNOK())
	{
		return status;
	}
	if ((status = ParseWhiteSpaces(pCTX)).IsNOK())
	{
		return status;
	}
	if ((status = ParseIdentifier(pCTX, sAlias)).IsNOK())
	{
		return status;
	}
	if ((status = ParseWhiteSpaces(pCTX)).IsNOK())
	{
		return status;
	}
	if (pCTX->cbIndex >= pCTX->cbSize)
	{
		return Status(Status_ParseFailed, "Unexpected end of file");
	}
	if (';' != *(pCTX->pBuffer + pCTX->cbIndex))
	{
		return Status(Status_ParseFailed, "Missing ; at line {1}, column {2}", pCTX->cLine, pCTX->cColumn);
	}
	pCTX->cColumn++;
	pCTX->cbIndex++;

	return Status();
}

Status Parser::ParseMember(CTX *pCTX, Member &member, const AliasesMap &mapAliases)
{
	String sKeyType;
	Status status;

	if ((status = ParseWhiteSpaces(pCTX)).IsNOK())
	{
		return status;
	}
	if ((status = ParseIdentifier(pCTX, sKeyType)).IsNOK())
	{
		return status;
	}
	if ((status = ParseWhiteSpaces(pCTX)).IsNOK())
	{
		return status;
	}
	if (pCTX->cbIndex >= pCTX->cbSize)
	{
		return Status(Status_ParseFailed, "Unexpected end of file");
	}
	if ('<' == *(pCTX->pBuffer + pCTX->cbIndex))
	{
		pCTX->cbIndex++;
		pCTX->cColumn++;
		if (sKeyType == "vector")
		{
			if ((status = ParseWhiteSpaces(pCTX)).IsNOK())
			{
				return status;
			}
			if ((status = ParseIdentifier(pCTX, sKeyType)).IsNOK())
			{
				return status;
			}
			if ((status = ParseWhiteSpaces(pCTX)).IsNOK())
			{
				return status;
			}
			if (pCTX->cbIndex >= pCTX->cbSize)
			{
				return Status(Status_ParseFailed, "Unexpected end of file");
			}
			if ('>' != *(pCTX->pBuffer + pCTX->cbIndex))
			{
				return Status(Status_ParseFailed, "Missing > at line {1}, column {2}", pCTX->cLine, pCTX->cColumn);
			}
			pCTX->cbIndex++;
			pCTX->cColumn++;
			UpdateType(sKeyType, mapAliases);
			member.SetMemberType(MemberType_Vector);
			member.SetKeyType(sKeyType);
			member.SetValType("");
		}
		else
		if (sKeyType == "set")
		{
			if ((status = ParseWhiteSpaces(pCTX)).IsNOK())
			{
				return status;
			}
			if ((status = ParseIdentifier(pCTX, sKeyType)).IsNOK())
			{
				return status;
			}
			if ((status = ParseWhiteSpaces(pCTX)).IsNOK())
			{
				return status;
			}
			if (pCTX->cbIndex >= pCTX->cbSize)
			{
				return Status(Status_ParseFailed, "Unexpected end of file");
			}
			if ('>' != *(pCTX->pBuffer + pCTX->cbIndex))
			{
				return Status(Status_ParseFailed, "Missing > at line {1}, column {2}", pCTX->cLine, pCTX->cColumn);
			}
			pCTX->cbIndex++;
			pCTX->cColumn++;
			UpdateType(sKeyType, mapAliases);
			member.SetMemberType(MemberType_Set);
			member.SetKeyType(sKeyType);
			member.SetValType("");
		}
		else
		if (sKeyType == "map")
		{
			if ((status = ParseWhiteSpaces(pCTX)).IsNOK())
			{
				return status;
			}
			if ((status = ParseIdentifier(pCTX, sKeyType)).IsNOK())
			{
				return status;
			}
			if ((status = ParseWhiteSpaces(pCTX)).IsNOK())
			{
				return status;
			}
			if (pCTX->cbIndex >= pCTX->cbSize)
			{
				return Status(Status_ParseFailed, "Unexpected end of file");
			}
			if (',' != *(pCTX->pBuffer + pCTX->cbIndex))
			{
				return Status(Status_ParseFailed, "Missing , at line {1}, column {2}", pCTX->cLine, pCTX->cColumn);
			}
			pCTX->cbIndex++;
			pCTX->cColumn++;
			if ((status = ParseWhiteSpaces(pCTX)).IsNOK())
			{
				return status;
			}

			String sValType;

			if ((status = ParseIdentifier(pCTX, sValType)).IsNOK())
			{
				return status;
			}
			if ((status = ParseWhiteSpaces(pCTX)).IsNOK())
			{
				return status;
			}
			if (pCTX->cbIndex >= pCTX->cbSize)
			{
				return Status(Status_ParseFailed, "Unexpected end of file");
			}
			if ('>' != *(pCTX->pBuffer + pCTX->cbIndex))
			{
				return Status(Status_ParseFailed, "Missing > at line {1}, column {2}", pCTX->cLine, pCTX->cColumn);
			}
			pCTX->cbIndex++;
			pCTX->cColumn++;
			UpdateType(sKeyType, mapAliases);
			UpdateType(sValType, mapAliases);
			member.SetMemberType(MemberType_Map);
			member.SetKeyType(sKeyType);
			member.SetValType(sValType);
		}
		else
		{
			return Status(Status_ParseFailed, "Unexpected type at line {1}, column {2}", pCTX->cLine, pCTX->cColumn);
		}
	}
	else
	{
		UpdateType(sKeyType, mapAliases);
		member.SetMemberType(MemberType_Scalar);
		member.SetKeyType(sKeyType);
		member.SetValType("");
	}
	if ((status = ParseWhiteSpaces(pCTX)).IsNOK())
	{
		return status;
	}

	String sName;

	if ((status = ParseIdentifier(pCTX, sName)).IsNOK())
	{
		return status;
	}
	if ((status = ParseWhiteSpaces(pCTX)).IsNOK())
	{
		return status;
	}
	if (pCTX->cbIndex >= pCTX->cbSize)
	{
		return Status(Status_ParseFailed, "Unexpected end of file");
	}
	if (';' != *(pCTX->pBuffer + pCTX->cbIndex))
	{
		return Status(Status_ParseFailed, "Missing ; at line {1}, column {2}", pCTX->cLine, pCTX->cColumn);
	}
	pCTX->cbIndex++;
	pCTX->cColumn++;

	member.SetName(sName);

	return Status();
}

void Parser::UpdateType(String &sType, const AliasesMap &mapAliases)
{
	AliasesMap::const_iterator iter;

	if (mapAliases.end() != (iter = mapAliases.find(sType)))
	{
		sType = iter->second;
	}
}

void Parser::PopulateAliases(AliasesMap &mapAliases)
{
	mapAliases["bool"]   = "CX.Bool";
	mapAliases["int8"]   = "CX.Int8";
	mapAliases["uint8"]  = "CX.UInt8";
	mapAliases["int16"]  = "CX.Int16";
	mapAliases["uint16"] = "CX.UInt16";
	mapAliases["int32"]  = "CX.Int32";
	mapAliases["uint32"] = "CX.UInt32";
	mapAliases["int64"]  = "CX.Int64";
	mapAliases["uint64"] = "CX.UInt64";
	mapAliases["float"]  = "CX.Float";
	mapAliases["double"] = "CX.Double";
	mapAliases["string"] = "CX.String";
	mapAliases["blob"]   = "CX.BLOB";
}

Status Parser::Parse(const Char *pBuffer, Size cLen, Object &object)
{
	CTX          ctx;
	AliasesMap   mapAliases;
	Status       status;

	ctx.pBuffer = pBuffer;
	ctx.cbIndex = 0;
	ctx.cbSize  = cLen;
	ctx.cLine   = 1;
	ctx.cColumn = 1;

	PopulateAliases(mapAliases);

	//prolog

	while (ctx.cbIndex < ctx.cbSize)
	{
		if ((status = ParseWhiteSpaces(&ctx)).IsNOK())
		{
			return status;
		}

		String sIdentifier;

		if ((status = ParseIdentifier(&ctx, sIdentifier)).IsNOK())
		{
			return status;
		}
		if ("pragma" == sIdentifier)
		{
			String sID;
			String sValue;

			if ((status = ParsePragma(&ctx, sID, sValue)).IsNOK())
			{
				return status;
			}
			object.AddPragma(Object::PRAGMA_LOCATION_PROLOG(), sID, sValue);
		}
		else
		if ("alias" == sIdentifier)
		{
			String sType;
			String sAlias;

			if ((status = ParseAlias(&ctx, sType, sAlias)).IsNOK())
			{
				return status;
			}
			mapAliases[sAlias] = sType;
		}
		else
		if ("object" == sIdentifier)
		{
			break;
		}
		else
		{
			return Status(Status_ParseFailed, "Unexpected token {1} at line {2}, column {3}", sIdentifier, ctx.cLine, ctx.cColumn);
		}
	}

	//object

	if ((status = ParseWhiteSpaces(&ctx)).IsNOK())
	{
		return status;
	}

	String sName;

	if ((status = ParseIdentifier(&ctx, sName)).IsNOK())
	{
		return status;
	}
	UpdateType(sName, mapAliases);
	object.SetName(sName);

	if ((status = ParseWhiteSpaces(&ctx)).IsNOK())
	{
		return status;
	}
	if (ctx.cbIndex >= ctx.cbSize)
	{
		return Status(Status_ParseFailed, "Unexpected end of file");
	}
	if ('{' != *(ctx.pBuffer + ctx.cbIndex))
	{
		return Status(Status_ParseFailed, "Missing ; at line {1}, column {2}", ctx.cLine, ctx.cColumn);
	}
	ctx.cColumn++;
	ctx.cbIndex++;
	while (ctx.cbIndex < ctx.cbSize)
	{
		if ((status = ParseWhiteSpaces(&ctx)).IsNOK())
		{
			return status;
		}
		if (ctx.cbIndex >= ctx.cbSize)
		{
			return Status(Status_ParseFailed, "Unexpected end of file");
		}
		if ('}' == *(ctx.pBuffer + ctx.cbIndex))
		{
			ctx.cColumn++;
			ctx.cbIndex++;

			break;
		}

		Member member;

		if ((status = ParseMember(&ctx, member, mapAliases)).IsNOK())
		{
			return status;
		}
		object.GetMembers().push_back(member);
	}

	//prolog

	while (ctx.cbIndex < ctx.cbSize)
	{
		if ((status = ParseWhiteSpaces(&ctx)).IsNOK())
		{
			return status;
		}

		if (ctx.cbIndex >= ctx.cbSize)
		{
			break;
		}

		String sIdentifier;

		if ((status = ParseIdentifier(&ctx, sIdentifier)).IsNOK())
		{
			return status;
		}
		if ("pragma" == sIdentifier)
		{
			String sID;
			String sValue;

			if ((status = ParsePragma(&ctx, sID, sValue)).IsNOK())
			{
				return status;
			}
			object.AddPragma(Object::PRAGMA_LOCATION_EPILOG(), sID, sValue);
		}
		else
		{
			return Status(Status_ParseFailed, "Unexpected token {1} at line {2}, column {3}", sIdentifier, ctx.cLine, ctx.cColumn);
		}
	}

	return Status();
}

}//namespace Archive

}//namespace CX
