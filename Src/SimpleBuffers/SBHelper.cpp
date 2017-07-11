
#include "CX/SimpleBuffers/Helper.hpp"
#include "bson.h"
#include "CX/Data/JSON/SAXParser.hpp"
#include "CX/Stack.hpp"
#include "CX/Str/Base64BinStr.hpp"
#include "memorystream.h"
#include "document.h"


namespace CX
{

namespace SimpleBuffers
{

//from Andrei Alexandrescu
static inline Size GetUInt64DigitsCount(UInt64 nValue)
{
	Size cDigitsCount = 1;

	for (;;)
	{
		if (nValue < 10)
		{
			return cDigitsCount;
		}
		if (nValue < 100)
		{
			return cDigitsCount + 1;
		}
		if (nValue < 1000)
		{
			return cDigitsCount + 2;
		}
		if (nValue < 10000)
		{
			return cDigitsCount + 3;
		}
		nValue /= 10000U;
		cDigitsCount += 4;
	}
}

//from Andrei Alexandrescu
static inline void UInt64ToString(UInt64 nValue, Char *szDst)
{
	static const char digits[201] =
		"0001020304050607080910111213141516171819"
		"2021222324252627282930313233343536373839"
		"4041424344454647484950515253545556575859"
		"6061626364656667686970717273747576777879"
		"8081828384858687888990919293949596979899";

	Size const cLen = GetUInt64DigitsCount(nValue);

	Size cNext = cLen - 1;

	while (nValue >= 100)
	{
		const UInt64 i = (nValue % 100) * 2;

		nValue /= 100;
		szDst[cNext] = digits[i + 1];
		szDst[cNext - 1] = digits[i];
		cNext -= 2;
	}
	if (nValue < 10)
	{
		szDst[cNext] = (Char)('0' + nValue);
	}
	else
	{
		UInt64 i = UInt64(nValue) * 2;

		szDst[cNext] = digits[i + 1];
		szDst[cNext - 1] = digits[i];
	}
	szDst[cLen] = 0;
}

Helper::Helper()
{
}

Helper::~Helper()
{
}

Status Helper::AddIndent(Size cIndent, IOutput *pOutput)
{
	static String sIndent(1024, '\t');
	Status        status;

	while (0 < cIndent)
	{
		if (cIndent <= sIndent.size())
		{
			if (!(status = pOutput->Add(sIndent.c_str(), cIndent)))
			{
				return status;
			}
			cIndent = 0;
		}
		else
		{
			if (!(status = pOutput->Add(sIndent.c_str(), sIndent.size())))
			{
				return status;
			}
			cIndent -= sIndent.size();
		}
	}

	return Status();
}

Status Helper::BSON2JSON(const void *pData, Size cbSize, IOutput *pOutput)
{
	bson_t bson;

	if (!bson_init_static(&bson, (const uint8_t *)pData, cbSize))
	{
		return Status_OperationFailed;
	}

	struct State
	{
		Bool        bObject;
		Size        cIndex;
		bson_iter_t iter;
	};

	typedef Stack<State>::Type   StatesStack;

	StatesStack   stackStates;
	State         state;
	bson_type_t   nType;
	Status        status;

	if (!bson_iter_init(&state.iter, &bson))
	{
		return Status_OperationFailed;
	}
	state.bObject = True;
	state.cIndex  = 0;
	stackStates.push(state);
	if (!(status = pOutput->Add("{", 1)))
	{
		return status;
	}
	while (!stackStates.empty())
	{
		if (!bson_iter_next(&stackStates.top().iter))
		{
			if (!(status = pOutput->Add("\n", 1)))
			{
				return status;
			}
			if (!(status = AddIndent(stackStates.size() - 1, pOutput)))
			{
				return status;
			}
			if (stackStates.top().bObject)
			{
				if (!(status = pOutput->Add("}", 1)))
				{
					return status;
				}
			}
			else
			{
				if (!(status = pOutput->Add("]", 1)))
				{
					return status;
				}
			}
			stackStates.pop();

			continue;
		}

		if (0 == stackStates.top().cIndex)
		{
			if (!(status = pOutput->Add("\n", 1)))
			{
				return status;
			}
		}
		else
		{
			if (!(status = pOutput->Add(",\n", 2)))
			{
				return status;
			}
		}
		if (!(status = AddIndent(stackStates.size(), pOutput)))
		{
			return status;
		}
		if (stackStates.top().bObject)
		{
			if (!(status = pOutput->Add("\"", 1)))
			{
				return status;
			}

			String sName;

			Data::JSON::SAXParser::EscapeString(bson_iter_key(&stackStates.top().iter), &sName);

			if (!(status = pOutput->Add(sName.c_str(), sName.size())))
			{
				return status;
			}
			if (!(status = pOutput->Add("\": ", 3)))
			{
				return status;
			}
		}

		nType = bson_iter_type(&stackStates.top().iter);

		if (BSON_TYPE_BOOL == nType)
		{
			if (bson_iter_bool(&stackStates.top().iter))
			{
				if (!(status = pOutput->Add("true", 4)))
				{
					return status;
				}
			}
			else
			{
				if (!(status = pOutput->Add("false", 5)))
				{
					return status;
				}
			}
			stackStates.top().cIndex++;
		}
		else
		if (BSON_TYPE_INT32 == nType)
		{
			String sTmp;

			Print(&sTmp, "{1}", bson_iter_int32(&stackStates.top().iter));
			if (!(status = pOutput->Add(sTmp.c_str(), sTmp.size())))
			{
				return status;
			}
			stackStates.top().cIndex++;
		}
		else
		if (BSON_TYPE_INT64 == nType)
		{
			String sTmp;

			Print(&sTmp, "{1}", bson_iter_int64(&stackStates.top().iter));
			if (!(status = pOutput->Add(sTmp.c_str(), sTmp.size())))
			{
				return status;
			}
			stackStates.top().cIndex++;
		}
		else
		if (BSON_TYPE_DOUBLE == nType)
		{
			String sTmp;

			Print(&sTmp, "{1}", bson_iter_double(&stackStates.top().iter));
			if (!(status = pOutput->Add(sTmp.c_str(), sTmp.size())))
			{
				return status;
			}
			stackStates.top().cIndex++;
		}
		else
		if (BSON_TYPE_UTF8 == nType)
		{
			String sTmp;

			size_t      cLen  = bson_iter_utf8_len_unsafe(&stackStates.top().iter);
			const char *szStr = bson_iter_utf8_unsafe(&stackStates.top().iter, &cLen);

			Data::JSON::SAXParser::EscapeString(szStr, &sTmp);

			if (!(status = pOutput->Add("\"", 1)))
			{
				return status;
			}
			if (!(status = pOutput->Add(sTmp.c_str(), sTmp.size())))
			{
				return status;
			}
			if (!(status = pOutput->Add("\"", 1)))
			{
				return status;
			}
			stackStates.top().cIndex++;
		}
		else
		if (BSON_TYPE_BINARY == nType)
		{
			String sTmp;

			bson_subtype_t nSubtype;
			uint32_t       cbBINSize;
			const uint8_t  *pBINData;

			bson_iter_binary(&stackStates.top().iter, &nSubtype, &cbBINSize, &pBINData);

			Str::Base64BinStr b64;

			b64.ToString(pBINData, cbBINSize, &sTmp);

			if (!(status = pOutput->Add("\"base64://", 10)))
			{
				return status;
			}
			if (!(status = pOutput->Add(sTmp.c_str(), sTmp.size())))
			{
				return status;
			}
			if (!(status = pOutput->Add("\"", 1)))
			{
				return status;
			}
			stackStates.top().cIndex++;
		}
		else
		if (BSON_TYPE_DOCUMENT == nType)
		{
			state.bObject = true;
			state.cIndex  = 0;
			if (!bson_iter_recurse(&stackStates.top().iter, &state.iter))
			{
				return Status_InvalidCall;
			}
			if (stackStates.top().bObject)
			{
				if (!(status = pOutput->Add("\n", 1)))
				{
					return status;
				}
				if (!(status = AddIndent(stackStates.size(), pOutput)))
				{
					return status;
				}
			}
			if (!(status = pOutput->Add("{", 1)))
			{
				return status;
			}
			stackStates.top().cIndex++;
			stackStates.push(state);
		}
		else
		if (BSON_TYPE_ARRAY == nType)
		{
			state.bObject = false;
			state.cIndex  = 0;
			if (!bson_iter_recurse(&stackStates.top().iter, &state.iter))
			{
				return Status_InvalidCall;
			}
			if (stackStates.top().bObject)
			{
				if (!(status = pOutput->Add("\n", 1)))
				{
					return status;
				}
				if (!(status = AddIndent(stackStates.size(), pOutput)))
				{
					return status;
				}
			}
			if (!(status = pOutput->Add("[", 1)))
			{
				return status;
			}
			stackStates.top().cIndex++;
			stackStates.push(state);
		}
	}
	if (!(status = pOutput->Add("\0", 1)))
	{
		return status;
	}

	return Status();
}

Status Helper::JSON2BSON(const void *pData, Size cbSize, IOutput *pOutput)
{
	bson_t bson;
	Status status;

	bson_init(&bson);

	rapidjson::MemoryStream is((const char *)pData, cbSize);
	rapidjson::Document     doc;

	doc.ParseStream(is);

	if (doc.HasParseError())
	{
		return Status_ParseFailed;
	}

	if (!doc.IsObject())
	{
		return Status_InvalidArg;
	}

	struct State
	{
		bson_t                                  *pBSON;
		Size                                    cIndex;
		const rapidjson::Value                  *pValue;
		rapidjson::Value::ConstMemberIterator   iterObject;
		rapidjson::Value::ConstValueIterator    iterArray;
	};

	typedef Stack<State>::Type   StatesStack;

	StatesStack            stackStates;
	const rapidjson::Value *pValue;
	Char                   szKeyTmp[64];
	const Char             *szKey;
	State                  state;

	state.pBSON      = &bson;
	state.pValue     = &doc;
	state.cIndex     = 0;
	state.iterObject = doc.MemberBegin();
	stackStates.push(state);
	while (!stackStates.empty())
	{
		if (stackStates.top().pValue->IsObject())
		{
			if (stackStates.top().pValue->MemberEnd() == stackStates.top().iterObject)
			{
				bson_t *pChild = stackStates.top().pBSON;

				stackStates.pop();

				if (pChild != &bson)
				{
					if (!(bson_append_document_end(stackStates.top().pBSON, pChild)))
					{
						return Status_OperationFailed;
					}
					free(pChild);
				}

				continue;
			}
			else
			{
				szKey  = stackStates.top().iterObject->name.GetString();
				pValue = &stackStates.top().iterObject->value;
				stackStates.top().iterObject++;
			}
		}
		else
		{
			if (stackStates.top().pValue->End() == stackStates.top().iterArray)
			{
				bson_t *pChild = stackStates.top().pBSON;

				stackStates.pop();

				if (pChild != &bson)
				{
					if (!(bson_append_array_end(stackStates.top().pBSON, pChild)))
					{
						return Status_OperationFailed;
					}
					free(pChild);
				}

				continue;
			}
			else
			{
				UInt64ToString(stackStates.top().cIndex, szKeyTmp);
				szKey  = szKeyTmp;
				pValue = &*stackStates.top().iterArray;
				stackStates.top().iterArray++;
			}
		}

		if (pValue->IsBool())
		{
			if (!(bson_append_bool(stackStates.top().pBSON, szKey, -1, pValue->GetBool())))
			{
				return Status_OperationFailed;
			}
			stackStates.top().cIndex++;
		}
		else
		if (pValue->IsInt())
		{
			if (!(bson_append_int32(stackStates.top().pBSON, szKey, -1, pValue->GetInt())))
			{
				return Status_OperationFailed;
			}
			stackStates.top().cIndex++;
		}
		else
		if (pValue->IsInt64())
		{
			if (!(bson_append_int64(stackStates.top().pBSON, szKey, -1, pValue->GetInt64())))
			{
				return Status_OperationFailed;
			}
			stackStates.top().cIndex++;
		}
		else
		if (pValue->IsDouble())
		{
			if (!(bson_append_double(stackStates.top().pBSON, szKey, -1, pValue->GetDouble())))
			{
				return Status_OperationFailed;
			}
			stackStates.top().cIndex++;
		}
		else
		if (pValue->IsString())
		{
			if (0 == strcmp("base64://", pValue->GetString()))
			{
				Str::Base64BinStr b64;
				Size              cLen = pValue->GetStringLength() - 9;
				CX::BLOB          blob;

				if (0 < cLen)
				{
					blob.resize(b64.GetBinSizeFromStrLen(pValue->GetString() + 9, cLen));
					b64.FromString(pValue->GetString() + 9, pValue->GetStringLength() - 9, &blob[0], blob.size());

					if (!(bson_append_binary(stackStates.top().pBSON, szKey, -1, BSON_SUBTYPE_BINARY, 
					                         (const uint8_t *)&blob[0], (uint32_t)blob.size())))
					{
						return Status_OperationFailed;
					}
				}
				else
				{
					if (!(bson_append_binary(stackStates.top().pBSON, szKey, -1, BSON_SUBTYPE_BINARY,
					                         (const uint8_t *)"", (uint32_t)0)))
					{
						return Status_OperationFailed;
					}
				}
			}
			else
			{
				if (!(bson_append_utf8(stackStates.top().pBSON, szKey, -1, pValue->GetString(), pValue->GetStringLength())))
				{
					return Status_OperationFailed;
				}
			}
			stackStates.top().cIndex++;
		}
		else
		if (pValue->IsObject())
		{
			state.cIndex     = 0;
			state.pBSON      = (bson_t *)malloc(sizeof(bson_t));
			state.pValue     = pValue;
			state.iterObject = pValue->MemberBegin();

			if (!bson_append_document_begin(stackStates.top().pBSON, szKey, -1, state.pBSON))
			{
				free(state.pBSON);

				return Status_OperationFailed;
			}

			stackStates.top().cIndex++;

			stackStates.push(state);
		}
		else
		if (pValue->IsArray())
		{
			state.cIndex    = 0;
			state.pBSON     = (bson_t *)malloc(sizeof(bson_t));
			state.pValue    = pValue;
			state.iterArray = pValue->Begin();

			if (!bson_append_array_begin(stackStates.top().pBSON, szKey, -1, state.pBSON))
			{
				free(state.pBSON);

				return Status_OperationFailed;
			}

			stackStates.top().cIndex++;

			stackStates.push(state);
		}
	}

	if (!(status = pOutput->Add(bson_get_data(&bson), bson.len)))
	{
		return status;
	}

	bson_destroy(&bson);

	return Status();
}

}//namespace SimpleBuffers

}//namespace CX
