

#pragma once


#include "CX/Platform.h"
#include "CX/APIDefs.h"
#include "CX/Types.h"
#include "CX/String.h"


#define CX_VER_MAJOR               0
#define CX_VER_MINOR               1
#define CX_VER_REVISION            0
#define CX_VER_BUILD               11

#define CX_VER_NAME                "CX" 
#define CX_VER_COPYRIGHT           "Copyright (C) 2014 - draede"
#define CX_VER_COMPANY             "draede"         

#ifdef CX_DEBUG
	#ifdef CX_64BIT_ARCH
		#define CX_VER_DESCRIPTION         "CX (" CX_BUILDSYS ", dbg, 64b) - "                       \
		                                   "https://github.com/draede/cx"
	#else
		#define CX_VER_DESCRIPTION         "CX (" CX_BUILDSYS ", dbg, 32b) - "                       \
		                                   "https://github.com/draede/cx"
	#endif
#else
	#ifdef CX_64BIT_ARCH
		#define CX_VER_DESCRIPTION         "CX (" CX_BUILDSYS ", opt, 64b) - "                       \
		                                   "https://github.com/draede/cx"
	#else
		#define CX_VER_DESCRIPTION         "CX (" CX_BUILDSYS ", opt, 32b) -"                        \
		                                   " https://github.com/draede/cx"
	#endif
#endif


#define CX_VER_HLP_STRBASE(x)                 #x

#define CX_VER_HLP_STR(x)                    CX_VER_HLP_STRBASE(x)

#define CX_VER_HLP_NUMBER(a, b, c, d)        (a), (b), (c), (d)

#define CX_VER_HLP_STRING(a, b, c, d)        CX_VER_HLP_STR(a) "." CX_VER_HLP_STR(b) "."           \
                                             CX_VER_HLP_STR(c) "." CX_VER_HLP_STR(d)

#define CX_VER_HLP_SIGNATURE(a, b, c, d, n, cp, co, ds)   n " " CX_VER_HLP_STRING(a, b, c, d) ", " \
                                                          cp ", " ds


namespace CX
{

class CX_API Version
{
public:

	Version();

	Version(int nMajor, int nMinor, int nRevision, int nBuild, 
	        const Char *szName, const Char *szCopyright, const Char *szCompany, 
	        const Char *szDescription);

	~Version();

	void SetMajor(int nMajor);

	int GetMajor() const;

	void SetMinor(int nMinor);

	int GetMinor() const;

	void SetRevision(int nRevision);

	int GetRevision() const;

	void SetBuild(int nBuild);

	int GetBuild() const;

	void SetName(const Char *szName);

	const Char *GetName() const;

	void SetCopyright(const Char *szCopyright);

	const Char *GetCopyright() const;

	void SetCompany(const Char *szCompany);

	const Char *GetCompany() const;

	void SetDescription(const Char *szDescription);

	const Char *GetDescription() const;

private:

	int      m_nMajor;
	int      m_nMinor;
	int      m_nRevision;
	int      m_nBuild;

#pragma warning(push)
#pragma warning(disable: 4251)
	String   m_sName;
	String   m_sCopyright;
	String   m_sCompany;
	String   m_sDescription;
#pragma warning(pop)

};

class CX_API CXVersion
{
public:

	static const Version &Get();

private:

	CXVersion();

	~CXVersion();

};

}//namespace CX

