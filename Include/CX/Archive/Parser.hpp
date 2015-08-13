
#pragma once


#include "CX/IO/IInputStream.hpp"
#include "CX/Archive/Object.hpp"


namespace CX
{

namespace Archive
{

class Parser
{
public:

	static Status ParseFilePath(const String &sFilePath, Object &object);

	static Status Parse(IO::IInputStream *pInputStream, Object &object);

	static Status Parse(const String &sBuffer, Object &object);

	static Status Parse(const Char *pBuffer, Size cLen, Object &object);

private:

	typedef Map<String, String>::Type   AliasesMap;

	struct CTX
	{
		const Char *pBuffer;
		Size       cbIndex;
		Size       cbSize;
		Size       cLine;
		Size       cColumn;
	};

	Parser();

	~Parser();

	static Status ParseWhiteSpaces(CTX *pCTX);

	static Status ParseIdentifier(CTX *pCTX, String &sIdentifier);

	static Status ParsePragma(CTX *pCTX, String &sID, String &sValue);

	static Status ParseAlias(CTX *pCTX, String &sType, String &sAlias);

	static Status ParseMember(CTX *pCTX, Member &member, const AliasesMap &mapAliases);

	static void UpdateType(String &sType, const AliasesMap &mapAliases);

	static void PopulateAliases(AliasesMap &mapAliases);

};

}//namespace Archive

}//namespace CX
