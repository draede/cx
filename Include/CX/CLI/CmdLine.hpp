/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2020 draede - draede [at] outlook [dot] com
 *
 * Released under the MIT License.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */ 
 
#pragma once


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/String.hpp"
#include "CX/Vector.hpp"
#include "CX/Print.hpp"
#include "CX/C/string.h"
#include <sstream>
#include <new>


namespace CX
{

namespace CLI
{

template <typename CHAR_TYPE, typename STRING_TYPE, typename ISTRINGSTREAM_TYPE>
class CmdLineBase
{
public:

	static const STRING_TYPE &DEFAULT_STRING()
	{
		static STRING_TYPE   str;

		return str;
	}

	enum ParamType
	{
		ParamType_Int,         //Int64
		ParamType_Double,      //Double
		ParamType_String,      //String
		ParamType_IntArray,    //[Int64]
		ParamType_DoubleArray, //[Double]
		ParamType_StringArray, //[String]
	};

	typedef Bool (* ValidateFunc)(const CHAR_TYPE *szArg, ParamType nType, const CHAR_TYPE *szName, void *pUsrCtx);

	CmdLineBase()
	{
		m_cMinPositionalParams = 0;
	}

	~CmdLineBase()
	{
		Reset();
	}

	Status Reset()
	{
		Clear();

		m_cMinPositionalParams = 0;
		m_vectorOptions.clear();
		m_vectorParams.clear();
		m_vectorPositionalParams.clear();

		return Status();
	}

	Status Clear()
	{
		for (auto iter = m_vectorOptions.begin(); iter != m_vectorOptions.end(); ++iter)
		{
			iter->bPresent = False;
		}
		for (auto iter = m_vectorParams.begin(); iter != m_vectorParams.end(); ++iter)
		{
			if (ParamType_String == iter->nType)
			{
				if (!iter->vectorParamValues.empty())
				{
					delete iter->vectorParamValues[0].psValue;
				}
			}
			else
			if (ParamType_StringArray == iter->nType)
			{
				for (auto iterStrings = iter->vectorParamValues.begin(); 
				     iterStrings != iter->vectorParamValues.end(); ++iterStrings)
				{
					delete iterStrings->psValue;
				}
			}
			iter->vectorParamValues.clear();
		}

		return Status();
	}

	CmdLineBase &SetMinPositionalParams(Size cMinPositionalParams = 0)
	{
		m_cMinPositionalParams = cMinPositionalParams;

		return *this;
	}

	CmdLineBase &AddOption(const CHAR_TYPE chShortName, const CHAR_TYPE *szLongName, const CHAR_TYPE *szDescription)
	{
		Option   option;

		option.bPresent     = False;
		option.chShortName  = chShortName;
		option.sLongName    = szLongName;
		option.sDescription = szDescription;

		for (auto iter = m_vectorOptions.begin(); iter != m_vectorOptions.end(); ++iter)
		{
			if (szLongName == iter->sLongName)
			{
				*iter = option;

				return *this;
			}
		}

		m_vectorOptions.push_back(option);

		return *this;
	}

	CmdLineBase &AddParam(ParamType nType, const CHAR_TYPE *szName, const CHAR_TYPE *szDescription, 
	                      Bool bRequired = False, ValidateFunc pfnValidate = NULL, void *pUsrCtx = NULL)
	{
		Param   param;

		param.nType        = nType;
		param.sName        = szName;
		param.sDescription = szDescription;
		param.bRequired    = bRequired;
		param.pfnValidate  = pfnValidate;
		param.pUsrCtx      = pUsrCtx;

		for (auto iter = m_vectorParams.begin(); iter != m_vectorParams.end(); ++iter)
		{
			if (szName == iter->sName)
			{
				*iter = param;

				return *this;
			}
		}

		m_vectorParams.push_back(param);

		return *this;
	}

	CmdLineBase &AddIntParam(const CHAR_TYPE *szName, const CHAR_TYPE *szDescription, Bool bRequired = False,
	                         ValidateFunc pfnValidate = NULL, void *pUsrCtx = NULL)
	{
		return AddParam(ParamType_Int, szName, szDescription, bRequired, pfnValidate, pUsrCtx);
	}

	CmdLineBase &AddDoubleParam(const CHAR_TYPE *szName, const CHAR_TYPE *szDescription, Bool bRequired = False,
	                            ValidateFunc pfnValidate = NULL, void *pUsrCtx = NULL)
	{
		return AddParam(ParamType_Double, szName, szDescription, bRequired, pfnValidate, pUsrCtx);
	}

	CmdLineBase &AddStringParam(const CHAR_TYPE *szName, const CHAR_TYPE *szDescription, Bool bRequired = False,
	                            ValidateFunc pfnValidate = NULL, void *pUsrCtx = NULL)
	{
		return AddParam(ParamType_String, szName, szDescription, bRequired, pfnValidate, pUsrCtx);
	}

	CmdLineBase &AddIntArrayParam(const CHAR_TYPE *szName, const CHAR_TYPE *szDescription, Bool bRequired = False,
	                              ValidateFunc pfnValidate = NULL, void *pUsrCtx = NULL)
	{
		return AddParam(ParamType_IntArray, szName, szDescription, bRequired, pfnValidate, pUsrCtx);
	}

	CmdLineBase &AddDoubleArrayParam(const CHAR_TYPE *szName, const CHAR_TYPE *szDescription, Bool bRequired = False,
	                                 ValidateFunc pfnValidate = NULL, void *pUsrCtx = NULL)
	{
		return AddParam(ParamType_DoubleArray, szName, szDescription, bRequired, pfnValidate, pUsrCtx);
	}

	CmdLineBase &AddStringArrayParam(const CHAR_TYPE *szName, const CHAR_TYPE *szDescription, Bool bRequired = False,
	                                 ValidateFunc pfnValidate = NULL, void *pUsrCtx = NULL)
	{
		return AddParam(ParamType_StringArray, szName, szDescription, bRequired, pfnValidate, pUsrCtx);
	}

	Status Parse(const CHAR_TYPE *szCmdLine, int cSkipFirstArgs = 1)
	{
		StringsVector      vectorArgs;
		StringPtrsVector   vectorArgPtrs;
		const CHAR_TYPE    *pszPos;
		STRING_TYPE        sArg;
		Bool               bInQuote;
		Bool               bLastWasEscape;

		bInQuote       = False;
		bLastWasEscape = False;
		pszPos         = szCmdLine;
		while (0 != *pszPos)
		{
			if ((CHAR_TYPE)'"' == *pszPos)
			{
				if (!bInQuote)
				{
					bInQuote       = True;
					bLastWasEscape = False;
					pszPos++;
					sArg.clear();
				}
				else
				{
					if (bLastWasEscape)
					{
						sArg += (CHAR_TYPE)'"';
						pszPos++;
						bLastWasEscape = False;
					}
					else
					{
						bInQuote = False;
						pszPos++;
						vectorArgs.push_back(sArg);
						sArg.clear();
					}
				}
			}
			else
			if ((CHAR_TYPE)' ' == *pszPos)
			{
				if (!bInQuote)
				{
					if (!sArg.empty())
					{
						vectorArgs.push_back(sArg);
						sArg.clear();
					}
					pszPos++;
				}
				else
				{
					sArg += *pszPos;
					pszPos++;
				}
			}
			else
			if ((CHAR_TYPE)'\\' == *pszPos)
			{
				if (bLastWasEscape)
				{
					bLastWasEscape = False;
					sArg += *pszPos;
				}
				else
				{
					bLastWasEscape = True;
				}
				pszPos++;
			}
			else
			{
				if (bLastWasEscape)
				{
					sArg += (CHAR_TYPE)'\\';
					bLastWasEscape = False;
				}
				sArg += *pszPos;
				pszPos++;
			}
		}
		if (bInQuote)
		{
			return Status(Status_ParseFailed, "Unterminated string");
		}
		if (!sArg.empty())
		{
			vectorArgs.push_back(sArg);
		}
		for (auto iter = vectorArgs.begin(); iter != vectorArgs.end(); ++iter)
		{
			vectorArgPtrs.push_back((CHAR_TYPE *)iter->c_str());
		}

		return Parse((int)vectorArgPtrs.size(), &vectorArgPtrs[0], cSkipFirstArgs);
	}

	Status Parse(int argc, CHAR_TYPE *argv[], int cSkipFirstArgs = 1)
	{
		Clear();

		Status   status;

		if (0 > cSkipFirstArgs)
		{
			cSkipFirstArgs = 0;
		}
		for (int i = cSkipFirstArgs; i < argc; i++)
		{
			if ((CHAR_TYPE)'-' == argv[i][0])
			{
				if ((CHAR_TYPE)'-' == argv[i][1])
				{
					Bool   bFound;

					bFound = False;
					for (auto iter = m_vectorOptions.begin(); iter != m_vectorOptions.end(); ++iter)
					{
						if (argv[i] + 2 == iter->sLongName)
						{
							iter->bPresent = True;
							bFound         = True;

							break;
						}
					}
					if (!bFound)
					{
						for (auto iter = m_vectorParams.begin(); iter != m_vectorParams.end(); ++iter)
						{
							if (argv[i] + 2 == iter->sName)
							{
								if (i + 1 < argc)
								{
									if ((CHAR_TYPE)'-' == argv[i + 1][0])
									{
										return Status(Status_ParseFailed, "Missing value for parameter '{1}'", iter->sName);
									}
									if (!(status = SetParamValue(*iter, argv[i + 1])))
									{
										return status;
									}
									i++;
								}
								else
								{
									return Status(Status_ParseFailed, "Missing value for parameter '{1}'", iter->sName);
								}

								bFound = True;

								break;
							}
						}
					}
					if (!bFound)
					{
						return Status(Status_ParseFailed, "Unknown option or param '{1}'", argv[i]);
					}
				}
				else
				{
					const CHAR_TYPE   *pszPos;

					pszPos = argv[i] + 1;
					while (0 != *pszPos)
					{
						Bool   bFound;

						bFound = False;
						for (auto iter = m_vectorOptions.begin(); iter != m_vectorOptions.end(); ++iter)
						{
							if (*pszPos == iter->chShortName)
							{
								iter->bPresent = True;
								bFound         = True;

								break;
							}
						}
						if (!bFound)
						{
							return Status(Status_ParseFailed, "Unknown option '{1}'", argv[i]);
						}

						pszPos++;
					}
				}
			}
			else
			{
				m_vectorPositionalParams.push_back(argv[i]);
			}
		}
		if (m_vectorPositionalParams.size() < m_cMinPositionalParams)
		{
			return Status(Status_ParseFailed, "Too few positional arguments");
		}
		for (auto iter = m_vectorParams.begin(); iter != m_vectorParams.end(); ++iter)
		{
			if (iter->bRequired)
			{
				if (iter->vectorParamValues.empty())
				{
					return Status(Status_ParseFailed, "Required param '{1}' is missing", iter->sName);
				}
			}
		}

		return Status();
	}

	Status ShowUsage(const Char *szIndent = "") const
	{
		for (auto iter = m_vectorOptions.begin(); iter != m_vectorOptions.end(); ++iter)
		{
			CX::Print(stdout, "{1}-{2}, --{3} : {4}\n", szIndent, iter->chShortName, iter->sLongName, iter->sDescription);
		}
		for (auto iter = m_vectorParams.begin(); iter != m_vectorParams.end(); ++iter)
		{
			switch (iter->nType)
			{
				case ParamType_Int:
				{
					CX::Print(stdout, "{1}--{2} (int) {3} : {4}\n", szIndent, iter->sName, iter->bRequired ? "(REQUIRED)" : "", 
					          iter->sDescription);
				}
				break;
				case ParamType_Double:
				{
					CX::Print(stdout, "{1}--{2} (double) {3} : {4}\n", szIndent, iter->sName, iter->bRequired ? "(REQUIRED)" : "", 
					          iter->sDescription);
				}
				break;
				case ParamType_String:
				{
					CX::Print(stdout, "{1}--{2} (string) {3} : {4}\n", szIndent, iter->sName, iter->bRequired ? "(REQUIRED)" : "", 
					          iter->sDescription);
				}
				break;
				case ParamType_IntArray:
				{
					CX::Print(stdout, "{1}--{2} (int - multiple) {3} : {4}\n", szIndent, iter->sName, 
					          iter->bRequired ? "(REQUIRED)" : "", iter->sDescription);
				}
				break;
				case ParamType_DoubleArray:
				{
					CX::Print(stdout, "{1}--{2} (double - multiple) {3} : {4}\n", szIndent, iter->sName, 
					          iter->bRequired ? "(REQUIRED)" : "", iter->sDescription);
				}
				break;
				case ParamType_StringArray:
				{
					CX::Print(stdout, "{1}--{2} (string - multiple) {3} : {4}\n", szIndent, iter->sName, 
					          iter->bRequired ? "(REQUIRED)" : "", iter->sDescription);
				}
				break;
			}
		}

		return Status();
	}

	Bool GetOption(const CHAR_TYPE chShortName) const
	{
		for (auto iter = m_vectorOptions.begin(); iter != m_vectorOptions.end(); ++iter)
		{
			if (chShortName == iter->chShortName)
			{
				return iter->bPresent;
			}
		}

		return False;
	}

	Bool GetOption(const CHAR_TYPE *szLongName) const
	{
		for (auto iter = m_vectorOptions.begin(); iter != m_vectorOptions.end(); ++iter)
		{
			if (szLongName == iter->sLongName)
			{
				return iter->bPresent;
			}
		}

		return False;
	}

	Int64 GetIntParam(const CHAR_TYPE *szName, Int64 nDefault = 0, Bool *pbFound = NULL) const
	{
		for (auto iter = m_vectorParams.begin(); iter != m_vectorParams.end(); ++iter)
		{
			if (szName == iter->sName)
			{
				if (ParamType_Int == iter->nType)
				{
					if (!iter->vectorParamValues.empty())
					{
						if (NULL != pbFound)
						{
							*pbFound = True;
						}

						return iter->vectorParamValues[0].nValue;
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}
			}
		}
		if (NULL != pbFound)
		{
			*pbFound = False;
		}

		return nDefault;
	}

	Double GetDoubleParam(const CHAR_TYPE *szName, Double lfDefault = 0.0, Bool *pbFound = NULL) const
	{
		for (auto iter = m_vectorParams.begin(); iter != m_vectorParams.end(); ++iter)
		{
			if (szName == iter->sName)
			{
				if (ParamType_Double == iter->nType)
				{
					if (!iter->vectorParamValues.empty())
					{
						if (NULL != pbFound)
						{
							*pbFound = True;
						}

						return iter->vectorParamValues[0].lfValue;
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}
			}
		}
		if (NULL != pbFound)
		{
			*pbFound = False;
		}

		return lfDefault;
	}

	const CHAR_TYPE *GetStringParam(const CHAR_TYPE *szName, const STRING_TYPE &sDefault = DEFAULT_STRING(), 
	                                Bool *pbFound = NULL) const
	{
		for (auto iter = m_vectorParams.begin(); iter != m_vectorParams.end(); ++iter)
		{
			if (szName == iter->sName)
			{
				if (ParamType_String == iter->nType)
				{
					if (!iter->vectorParamValues.empty())
					{
						if (NULL != pbFound)
						{
							*pbFound = True;
						}

						return iter->vectorParamValues[0].psValue->c_str();
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}
			}
		}
		if (NULL != pbFound)
		{
			*pbFound = False;
		}

		return sDefault.c_str();
	}

	Size GetIntArrayParamCount(const CHAR_TYPE *szName)
	{
		for (auto iter = m_vectorParams.begin(); iter != m_vectorParams.end(); ++iter)
		{
			if (szName == iter->sName)
			{
				if (ParamType_IntArray == iter->nType)
				{
					return iter->vectorParamValues.size();
				}
			}
		}

		return 0;
	}

	Int64 GetIntArrayParamItem(const CHAR_TYPE *szName, Size cIndex, Int64 nDefault = 0, Bool *pbFound = NULL) const
	{
		for (auto iter = m_vectorParams.begin(); iter != m_vectorParams.end(); ++iter)
		{
			if (szName == iter->sName)
			{
				if (ParamType_IntArray == iter->nType)
				{
					if (iter->vectorParamValues.size() > cIndex)
					{
						if (NULL != pbFound)
						{
							*pbFound = True;
						}

						return iter->vectorParamValues[cIndex].nValue;
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}
			}
		}
		if (NULL != pbFound)
		{
			*pbFound = False;
		}

		return nDefault;
	}

	Size GetDoubleArrayParamCount(const CHAR_TYPE *szName)
	{
		for (auto iter = m_vectorParams.begin(); iter != m_vectorParams.end(); ++iter)
		{
			if (szName == iter->sName)
			{
				if (ParamType_DoubleArray == iter->nType)
				{
					return iter->vectorParamValues.size();
				}
			}
		}

		return 0;
	}

	Double GetDoubleArrayParamItem(const CHAR_TYPE *szName, Size cIndex, Double lfDefault = 0.0, 
	                               Bool *pbFound = NULL) const
	{
		for (auto iter = m_vectorParams.begin(); iter != m_vectorParams.end(); ++iter)
		{
			if (szName == iter->sName)
			{
				if (ParamType_DoubleArray == iter->nType)
				{
					if (iter->vectorParamValues.size() > cIndex)
					{
						if (NULL != pbFound)
						{
							*pbFound = True;
						}

						return iter->vectorParamValues[cIndex].lfValue;
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}
			}
		}
		if (NULL != pbFound)
		{
			*pbFound = False;
		}

		return lfDefault;
	}

	Size GetStringArrayParamCount(const CHAR_TYPE *szName)
	{
		for (auto iter = m_vectorParams.begin(); iter != m_vectorParams.end(); ++iter)
		{
			if (szName == iter->sName)
			{
				if (ParamType_StringArray == iter->nType)
				{
					return iter->vectorParamValues.size();
				}
			}
		}

		return 0;
	}

	const CHAR_TYPE *GetStringArrayParamItem(const CHAR_TYPE *szName, Size cIndex, 
	                                         const STRING_TYPE &sDefault = DEFAULT_STRING(), 
	                                         Bool *pbFound = NULL) const
	{
		for (auto iter = m_vectorParams.begin(); iter != m_vectorParams.end(); ++iter)
		{
			if (szName == iter->sName)
			{
				if (ParamType_StringArray == iter->nType)
				{
					if (iter->vectorParamValues.size() > cIndex)
					{
						if (NULL != pbFound)
						{
							*pbFound = True;
						}

						return iter->vectorParamValues[cIndex].psValue->c_str();
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}
			}
		}
		if (NULL != pbFound)
		{
			*pbFound = False;
		}

		return sDefault.c_str();
	}

	Size GetPositionalParamsCount() const
	{
		return m_vectorPositionalParams.size();
	}

	const CHAR_TYPE *GetPositionalParam(Size cIndex, const CHAR_TYPE *szDefault = "", Bool *pbFound = NULL) const
	{
		if (m_vectorPositionalParams.size() <= cIndex)
		{
			if (NULL != pbFound)
			{
				*pbFound = False;
			}

			return szDefault;
		}
		if (NULL != pbFound)
		{
			*pbFound = True;
		}

		return m_vectorPositionalParams[cIndex].c_str();
	}

private:

	typedef typename Vector<STRING_TYPE>::Type   StringsVector;
	typedef typename Vector<CHAR_TYPE *>::Type   StringPtrsVector;

	struct Option
	{
		CHAR_TYPE     chShortName;
		STRING_TYPE   sLongName;
		STRING_TYPE   sDescription;
		Bool          bPresent;
	};

	struct ParamValue
	{
		union
		{
			Int64         nValue;
			Double        lfValue;
			STRING_TYPE   *psValue;
		};
	};

	typedef typename Vector<ParamValue>::Type   ParamValuesVector;

	struct Param
	{
		ParamType           nType;
		STRING_TYPE         sName;
		STRING_TYPE         sDescription;
		Bool                bRequired;
		ValidateFunc        pfnValidate;
		void                *pUsrCtx;
		ParamValuesVector   vectorParamValues;
	};

	typedef typename Vector<Option>::Type        OptionsVector;
	typedef typename Vector<Param>::Type         ParamsVector;
	typedef typename Vector<STRING_TYPE>::Type   PositionalParamsVector;

	Size                     m_cMinPositionalParams;
	OptionsVector            m_vectorOptions;
	ParamsVector             m_vectorParams;
	PositionalParamsVector   m_vectorPositionalParams;

	Status SetParamValue(Param &param, const CHAR_TYPE *szValue)
	{
		switch (param.nType)
		{
			case ParamType_Int:
			{
				ParamValue           val;
				ISTRINGSTREAM_TYPE   iss;

				iss.str(szValue);
				iss >> val.nValue;
				if (iss.fail() || !iss.eof())
				{
					return Status(Status_ParseFailed, "Failed to parse '{1}' as int", param.sName);
				}
				if (param.vectorParamValues.empty())
				{
					param.vectorParamValues.push_back(val);
				}
				else
				{
					param.vectorParamValues[0] = val;
				}
			}
			break;
			case ParamType_Double:
			{
				ParamValue           val;
				ISTRINGSTREAM_TYPE   iss;

				iss.str(szValue);
				iss >> val.lfValue;
				if (iss.fail() || !iss.eof())
				{
					return Status(Status_ParseFailed, "Failed to parse '{1}' as double", param.sName);
				}
				if (param.vectorParamValues.empty())
				{
					param.vectorParamValues.push_back(val);
				}
				else
				{
					param.vectorParamValues[0] = val;
				}
			}
			break;
			case ParamType_String:
			{
				ParamValue   val;

				if (NULL != (val.psValue = new (std::nothrow) STRING_TYPE(szValue)))
				{
					if (param.vectorParamValues.empty())
					{
						param.vectorParamValues.push_back(val);
					}
					else
					{
						delete param.vectorParamValues[0].psValue;
						param.vectorParamValues[0] = val;
					}
				}
				else
				{
					return Status(Status_MemAllocFailed, "");
				}
			}
			break;
			case ParamType_IntArray:
			{
				ParamValue           val;
				ISTRINGSTREAM_TYPE   iss;

				iss.str(szValue);
				iss >> val.nValue;
				if (iss.fail() || !iss.eof())
				{
					return Status(Status_ParseFailed, "Failed to parse '{1}' as int", param.sName);
				}
				param.vectorParamValues.push_back(val);
			}
			break;
			case ParamType_DoubleArray:
			{
				ParamValue           val;
				ISTRINGSTREAM_TYPE   iss;

				iss.str(szValue);
				iss >> val.lfValue;
				if (iss.fail() || !iss.eof())
				{
					return Status(Status_ParseFailed, "Failed to parse '{1}' as double", param.sName);
				}
				param.vectorParamValues.push_back(val);
			}
			break;
			case ParamType_StringArray:
			{
				ParamValue   val;

				if (NULL != (val.psValue = new (std::nothrow) STRING_TYPE(szValue)))
				{
					param.vectorParamValues.push_back(val);
				}
				else
				{
					return Status(Status_MemAllocFailed, "");
				}
			}
			break;
		}

		return Status();
	}

};

typedef CmdLineBase<Char, String, std::istringstream>      CmdLine;
typedef CmdLineBase<WChar, WString, std::wistringstream>   CmdLineW;

}//namespace CLI

}//namespace CX
