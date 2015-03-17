
#pragma once


#include "Struct.hpp"
#include "CX/IO/IInputStream.hpp"


namespace CX
{

namespace SB
{


class Parser
{
public:

	Parser();

	Status BeginParse();

	Status EndParse();

	Status Parse(const Char *szFilePath);

	Status Parse(IO::IInputStream *pInputStream);

	Status Parse(const void *pBuffer, Size cbSize);

	void ClearStructs()
	{
		m_mapStructs.clear();
	}

	const StructsMap &GetStructs()
	{
		return m_mapStructs;
	}

private:

	static const Size  MAX_IMPORTS_DEPTH = 16;

	static const Char    *RESERVED_KEYWORDS[];
	static const CX_Size RESERVED_KEYWORDS_COUNT;

	enum State
	{
		State_None,
		State_BeforeDeclaration,
		State_InDeclaration,
		State_BeforeStructName,
		State_InStructName,
		State_AfterStructName,
		State_BeforeStructStart,
		State_BeforeName,
		State_InName,
		State_AfterName,
		State_BeforeColon,
		State_BeforeSemicolon,
		State_BeforeType,
		State_InType,
		State_AfterType,
		State_BeforeArrayBegin,
		State_BeforeArrayEnd,
		State_AfterStruct,
		State_BeforeImport,
		State_InImport,
		State_AfterImport,
		State_BeforeComment,
		State_InSLComment,
		State_InMLComment,
		State_BeforeMLCommentEnd,
	};

	Size m_cLine;
	Size m_cColumn;

	String     m_sPath;
	State      m_nState;
	State      m_nSavedState;
	const Char *m_pPos;
	const Char *m_pEnd;
	String     m_sToken;
	bool       m_bIsArray;
	Struct     m_struct;
	Field      m_field;
	StructsMap m_mapStructs;
	Size       m_cImportDepth;

	Status WalkWhiteSpaces();

	Status WalkToken();

	bool IsReservedKeyword(const Char *szStr);

	void GetFilePath(const Char *szRefPath, const Char *szOrigPath, String *psPath);

	Status BeginParseInternal(Size cImportDepth);

};


}//namespace SB

}//namespace CX

