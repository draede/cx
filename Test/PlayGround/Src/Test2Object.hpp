
#pragma once


#include "CX/IO/SimpleBuffers/Types.hpp"
#include "Test1Object.hpp"


namespace My
{

namespace Test
{

namespace NameSpace
{

class Test2Object : public CX::IO::SimpleBuffers::IObject
{
public:

	CX::Bool bool_val;
	CX::Int8 int8_val;
	CX::UInt8 uint8_val;
	CX::Int16 int16_val;
	CX::UInt16 uint16_val;
	CX::Int32 int32_val;
	CX::UInt32 uint32_val;
	CX::Int64 int64_val;
	CX::UInt64 uint64_val;
	CX::Float float_val;
	CX::Double double_val;
	CX::String string_val;
	CX::WString wstring_val;
	My::Test::NameSpace::Test1Object obj1_val;
	CX::IO::SimpleBuffers::BoolArray bool_arr;
	CX::IO::SimpleBuffers::Int8Array int8_arr;
	CX::IO::SimpleBuffers::UInt8Array uint8_arr;
	CX::IO::SimpleBuffers::Int16Array int16_arr;
	CX::IO::SimpleBuffers::UInt16Array uint16_arr;
	CX::IO::SimpleBuffers::Int32Array int32_arr;
	CX::IO::SimpleBuffers::UInt32Array uint32_arr;
	CX::IO::SimpleBuffers::Int64Array int64_arr;
	CX::IO::SimpleBuffers::UInt64Array uint64_arr;
	CX::IO::SimpleBuffers::FloatArray float_arr;
	CX::IO::SimpleBuffers::DoubleArray double_arr;
	CX::IO::SimpleBuffers::StringArray string_arr;
	CX::IO::SimpleBuffers::WStringArray wstring_arr;
	My::Test::NameSpace::Test1ObjectArray obj1_arr;

	virtual void Reset()
	{
		bool_val = false;
		int8_val = 0;
		uint8_val = 0;
		int16_val = 0;
		uint16_val = 0;
		int32_val = 0;
		uint32_val = 0;
		int64_val = 0;
		uint64_val = 0;
		float_val = 0.0f;
		double_val = 0.0;
		string_val.clear();
		wstring_val.clear();
		obj1_val.Reset();
		bool_arr.clear();
		int8_arr.clear();
		uint8_arr.clear();
		int16_arr.clear();
		uint16_arr.clear();
		int32_arr.clear();
		uint32_arr.clear();
		int64_arr.clear();
		uint64_arr.clear();
		float_arr.clear();
		double_arr.clear();
		string_arr.clear();
		wstring_arr.clear();
		obj1_arr.clear();
	}

	virtual void SetupWithSomeValues()
	{
		Reset();
		bool_val = true;
		int8_val = -76;
		uint8_val = 60;
		int16_val = -13938;
		uint16_val = 47380;
		int32_val = -1520684287;
		uint32_val = 4860881;
		int64_val = 7605027460018408644;
		uint64_val = 15908131033522207010;
		float_val = 0.342128f;
		double_val = 0.180525;
		string_val = "rg=vzTFv7wl>L%}-tgU<u@ga3,KzX):gKeKvGuh!fhN{2e[Eeb@{nVEA";
		wstring_val = L"zD{?UV|2=DH!$";
		obj1_val.SetupWithSomeValues();
		int8_arr.push_back(17);
		int8_arr.push_back(24);
		int8_arr.push_back(-58);
		uint8_arr.push_back(238);
		uint8_arr.push_back(99);
		uint8_arr.push_back(141);
		int16_arr.push_back(24420);
		int16_arr.push_back(-20510);
		int16_arr.push_back(-8790);
		int16_arr.push_back(-6564);
		int16_arr.push_back(22423);
		int16_arr.push_back(20711);
		int16_arr.push_back(-11461);
		int16_arr.push_back(25155);
		int16_arr.push_back(20293);
		int16_arr.push_back(-20246);
		uint16_arr.push_back(17335);
		int32_arr.push_back(1854367863);
		int32_arr.push_back(1546297437);
		int32_arr.push_back(-669895169);
		int32_arr.push_back(-2034133410);
		int32_arr.push_back(1169026095);
		int32_arr.push_back(-851237635);
		int32_arr.push_back(-442647294);
		int32_arr.push_back(1158647828);
		int32_arr.push_back(-952488603);
		int32_arr.push_back(-1272244980);
		int32_arr.push_back(1534945582);
		int32_arr.push_back(725098854);
		int32_arr.push_back(193981055);
		uint32_arr.push_back(887904401);
		uint32_arr.push_back(642128002);
		uint32_arr.push_back(431249111);
		uint32_arr.push_back(1970460851);
		uint32_arr.push_back(1112670331);
		uint32_arr.push_back(3705503716);
		uint32_arr.push_back(1681152367);
		uint32_arr.push_back(1133642960);
		uint32_arr.push_back(197090958);
		int64_arr.push_back(7481962392306822546);
		int64_arr.push_back(962401622083955450);
		int64_arr.push_back(-7420189838855115594);
		int64_arr.push_back(7866372933069231266);
		int64_arr.push_back(-2857311654917044708);
		int64_arr.push_back(374072189081915454);
		int64_arr.push_back(6692001381333596330);
		int64_arr.push_back(-244514102260245640);
		int64_arr.push_back(4448842413213428198);
		uint64_arr.push_back(13595318351870493168);
		uint64_arr.push_back(1540131550239531026);
		float_arr.push_back(0.892526f);
		float_arr.push_back(0.828401f);
		double_arr.push_back(0.387467);
		double_arr.push_back(0.654012);
		double_arr.push_back(0.257065);
		double_arr.push_back(0.569768);
		double_arr.push_back(0.235761);
		double_arr.push_back(0.530244);
		double_arr.push_back(0.075887);
		string_arr.push_back("$F<GcZUIf-KG3s*Z,\\&(Ksz}TV1M+i1/(=>j$1pzX4{8p(437P@30");
		string_arr.push_back("]");
		string_arr.push_back("[r;9VKzx/ckL=LDw!'!aMu)]roPoyKlD");
		string_arr.push_back("sP-4ZGk_R@+dSXx-#fS$8B={BV{ZIzN647|Pga7Q9Iqqx\"Wnp0`,Mm\\Q(@");
		string_arr.push_back("CKckGO`<ae%n5@3W.f^QvrkU'S(rZz,0-%CYKR>");
		string_arr.push_back("KQnvm=_]0>Z{El0{(Vs|9,V_P(/F/T%:b7Q$3`.h~\"O=0'[2Ymi66$_)=Y]u==W");
		string_arr.push_back("Z%RwqISN~'~kBz-!|\"NX>3Vr");
		string_arr.push_back("4td|}X4D+XF6u^eFD.$*'5$nwC4GW0K#Fe\"?7r-3!B+)fx>$:\"/9tyc");
		wstring_arr.push_back(L"");
		wstring_arr.push_back(L"#zP}Bc}Fz81{;yb$I&V^JVNd1[w6M8}FLu`Xkn<jufa8E70:U&P)Z}S[Fhv]#@t");
		wstring_arr.push_back(L"7B}eb|?t@7?~DFXUtaps-H&");
		wstring_arr.push_back(L"CDUoLRMNfrg]wD{G3YT[-|o`MRsl(\\B}8m*?Uztxm8Z)WDP\"0|6A~N*a~^");
		wstring_arr.push_back(L"^`#!3{ejUi>>4<_{NW)X&.xl8.TQ;`u./,6Dp,8d9");
		wstring_arr.push_back(L"'?UBF%{9phke;OEiF{:yZ+9hE");
		wstring_arr.push_back(L"yV4O7Y\"fO.1;l:zcno~.z-3,#${aBE__)X\"n~bbxzSOiT[(st");
		wstring_arr.push_back(L"U]`]o&3uu[0.S0KBg0B7~s<elz07;#FZESWk_IqUBz`D8BsZc?7YKLMmMDh}?6");
		wstring_arr.push_back(L"@;RX%`X8\\H(YN<v'i5Cp&N~P?^Nw^{-C@qR<<fA~y,#9<g|8G6BQb&?;yl':V8A");
		wstring_arr.push_back(L"-*4lp<vi1=KScC0;5g70y{81RJ@7WD&julopDo{gDilF>AL`T[L[=ZFU/rADH9;l");
		wstring_arr.push_back(L"(e^jSb]_p1|(hPQZd$2FS]q31M$MvV|H($`|-2?Z-G");
		wstring_arr.push_back(L"KgXiBt@F;'.DxX%fE<tn}I:fDfe/");
		wstring_arr.push_back(L"i~CwH-%LoINV(&)=_Ptl'$$=+[3:Qz,JnY90;r}rRh8$ZTcPo(tQ|MgwiprR+t");
		{
			My::Test::NameSpace::Test1Object v;

			v.SetupWithSomeValues();
			obj1_arr.push_back(v);
		}
		{
			My::Test::NameSpace::Test1Object v;

			v.SetupWithSomeValues();
			obj1_arr.push_back(v);
		}
		{
			My::Test::NameSpace::Test1Object v;

			v.SetupWithSomeValues();
			obj1_arr.push_back(v);
		}
		{
			My::Test::NameSpace::Test1Object v;

			v.SetupWithSomeValues();
			obj1_arr.push_back(v);
		}
		{
			My::Test::NameSpace::Test1Object v;

			v.SetupWithSomeValues();
			obj1_arr.push_back(v);
		}
		{
			My::Test::NameSpace::Test1Object v;

			v.SetupWithSomeValues();
			obj1_arr.push_back(v);
		}
		{
			My::Test::NameSpace::Test1Object v;

			v.SetupWithSomeValues();
			obj1_arr.push_back(v);
		}
		{
			My::Test::NameSpace::Test1Object v;

			v.SetupWithSomeValues();
			obj1_arr.push_back(v);
		}
		{
			My::Test::NameSpace::Test1Object v;

			v.SetupWithSomeValues();
			obj1_arr.push_back(v);
		}
		{
			My::Test::NameSpace::Test1Object v;

			v.SetupWithSomeValues();
			obj1_arr.push_back(v);
		}
		{
			My::Test::NameSpace::Test1Object v;

			v.SetupWithSomeValues();
			obj1_arr.push_back(v);
		}
		{
			My::Test::NameSpace::Test1Object v;

			v.SetupWithSomeValues();
			obj1_arr.push_back(v);
		}
		{
			My::Test::NameSpace::Test1Object v;

			v.SetupWithSomeValues();
			obj1_arr.push_back(v);
		}
	}

	virtual bool Compare(const CX::IO::SimpleBuffers::IObject *pObj)
	{
		const Test2Object *pObject = dynamic_cast<const Test2Object *>(pObj);
	
		if (NULL == pObject)
		{
			return false;
		}
		if (bool_val != pObject->bool_val)
		{
			return false;
		}
		if (int8_val != pObject->int8_val)
		{
			return false;
		}
		if (uint8_val != pObject->uint8_val)
		{
			return false;
		}
		if (int16_val != pObject->int16_val)
		{
			return false;
		}
		if (uint16_val != pObject->uint16_val)
		{
			return false;
		}
		if (int32_val != pObject->int32_val)
		{
			return false;
		}
		if (uint32_val != pObject->uint32_val)
		{
			return false;
		}
		if (int64_val != pObject->int64_val)
		{
			return false;
		}
		if (uint64_val != pObject->uint64_val)
		{
			return false;
		}
		if (float_val != pObject->float_val)
		{
			return false;
		}
		if (double_val != pObject->double_val)
		{
			return false;
		}
		if (0 != cx_strcmp(string_val.c_str(), pObject->string_val.c_str()))
		{
			return false;
		}
		if (0 != cxw_strcmp(wstring_val.c_str(), pObject->wstring_val.c_str()))
		{
			return false;
		}
		if (!obj1_val.Compare(&pObject->obj1_val))
		{
			return false;
		}
		if (bool_arr.size() != pObject->bool_arr.size())
		{
			return false;
		}
		for (CX::Size i = 0; i < bool_arr.size(); i++)
		{
			if (bool_arr[i] != pObject->bool_arr[i])
			{
				return false;
			}
		}
		if (int8_arr.size() != pObject->int8_arr.size())
		{
			return false;
		}
		for (CX::Size i = 0; i < int8_arr.size(); i++)
		{
			if (int8_arr[i] != pObject->int8_arr[i])
			{
				return false;
			}
		}
		if (uint8_arr.size() != pObject->uint8_arr.size())
		{
			return false;
		}
		for (CX::Size i = 0; i < uint8_arr.size(); i++)
		{
			if (uint8_arr[i] != pObject->uint8_arr[i])
			{
				return false;
			}
		}
		if (int16_arr.size() != pObject->int16_arr.size())
		{
			return false;
		}
		for (CX::Size i = 0; i < int16_arr.size(); i++)
		{
			if (int16_arr[i] != pObject->int16_arr[i])
			{
				return false;
			}
		}
		if (uint16_arr.size() != pObject->uint16_arr.size())
		{
			return false;
		}
		for (CX::Size i = 0; i < uint16_arr.size(); i++)
		{
			if (uint16_arr[i] != pObject->uint16_arr[i])
			{
				return false;
			}
		}
		if (int32_arr.size() != pObject->int32_arr.size())
		{
			return false;
		}
		for (CX::Size i = 0; i < int32_arr.size(); i++)
		{
			if (int32_arr[i] != pObject->int32_arr[i])
			{
				return false;
			}
		}
		if (uint32_arr.size() != pObject->uint32_arr.size())
		{
			return false;
		}
		for (CX::Size i = 0; i < uint32_arr.size(); i++)
		{
			if (uint32_arr[i] != pObject->uint32_arr[i])
			{
				return false;
			}
		}
		if (int64_arr.size() != pObject->int64_arr.size())
		{
			return false;
		}
		for (CX::Size i = 0; i < int64_arr.size(); i++)
		{
			if (int64_arr[i] != pObject->int64_arr[i])
			{
				return false;
			}
		}
		if (uint64_arr.size() != pObject->uint64_arr.size())
		{
			return false;
		}
		for (CX::Size i = 0; i < uint64_arr.size(); i++)
		{
			if (uint64_arr[i] != pObject->uint64_arr[i])
			{
				return false;
			}
		}
		if (float_arr.size() != pObject->float_arr.size())
		{
			return false;
		}
		for (CX::Size i = 0; i < float_arr.size(); i++)
		{
			if (float_arr[i] != pObject->float_arr[i])
			{
				return false;
			}
		}
		if (double_arr.size() != pObject->double_arr.size())
		{
			return false;
		}
		for (CX::Size i = 0; i < double_arr.size(); i++)
		{
			if (double_arr[i] != pObject->double_arr[i])
			{
				return false;
			}
		}
		if (string_arr.size() != pObject->string_arr.size())
		{
			return false;
		}
		for (CX::Size i = 0; i < string_arr.size(); i++)
		{
			if (0 != cx_strcmp(string_arr[i].c_str(), pObject->string_arr[i].c_str()))
			{
				return false;
			}
		}
		if (wstring_arr.size() != pObject->wstring_arr.size())
		{
			return false;
		}
		for (CX::Size i = 0; i < wstring_arr.size(); i++)
		{
			if (0 != cxw_strcmp(wstring_arr[i].c_str(), pObject->wstring_arr[i].c_str()))
			{
				return false;
			}
		}
		if (obj1_arr.size() != pObject->obj1_arr.size())
		{
			return false;
		}
		for (CX::Size i = 0; i < obj1_arr.size(); i++)
		{
			if (!obj1_arr[i].Compare(&pObject->obj1_arr[i]))
			{
				return false;
			}
		}

		return true;
	}

	virtual CX::Status Read(CX::IO::IDataReader *pReader)
	{
		CX::Bool bValue;
		CX::Int64 nValue;
		CX::Double lfValue;
		CX::String sValue;
		CX::WString wsValue;
		CX::Status status;

		Reset();
		if ((status = pReader->ReadObjectBool("bool_val", &bool_val)).IsNOK())
		{
			return status;
		}
		if ((status = pReader->ReadObjectInt("int8_val", &nValue)).IsNOK())
		{
			return status;
		}
		int8_val = (CX::Int8)nValue;
		if ((status = pReader->ReadObjectInt("uint8_val", &nValue)).IsNOK())
		{
			return status;
		}
		uint8_val = (CX::UInt8)nValue;
		if ((status = pReader->ReadObjectInt("int16_val", &nValue)).IsNOK())
		{
			return status;
		}
		int16_val = (CX::Int16)nValue;
		if ((status = pReader->ReadObjectInt("uint16_val", &nValue)).IsNOK())
		{
			return status;
		}
		uint16_val = (CX::UInt16)nValue;
		if ((status = pReader->ReadObjectInt("int32_val", &nValue)).IsNOK())
		{
			return status;
		}
		int32_val = (CX::Int32)nValue;
		if ((status = pReader->ReadObjectInt("uint32_val", &nValue)).IsNOK())
		{
			return status;
		}
		uint32_val = (CX::UInt32)nValue;
		if ((status = pReader->ReadObjectInt("int64_val", &nValue)).IsNOK())
		{
			return status;
		}
		int64_val = (CX::Int64)nValue;
		if ((status = pReader->ReadObjectInt("uint64_val", &nValue)).IsNOK())
		{
			return status;
		}
		uint64_val = (CX::UInt64)nValue;
		if ((status = pReader->ReadObjectReal("float_val", &lfValue)).IsNOK())
		{
			return status;
		}
		float_val = (CX::Float)lfValue;
		if ((status = pReader->ReadObjectReal("double_val", &lfValue)).IsNOK())
		{
			return status;
		}
		double_val = (CX::Double)lfValue;
		if ((status = pReader->ReadObjectString("string_val", &string_val)).IsNOK())
		{
			return status;
		}
		if ((status = pReader->ReadObjectWString("wstring_val", &wstring_val)).IsNOK())
		{
			return status;
		}
		if ((status = pReader->BeginObjectObject("obj1_val")).IsNOK())
		{
			return status;
		}
		if ((status = obj1_val.Read(pReader)).IsNOK())
		{
			return status;
		}
		if ((status = pReader->EndObjectObject()).IsNOK())
		{
			return status;
		}
		if ((status = pReader->BeginObjectArray("bool_arr")).IsNOK())
		{
			return status;
		}
		for (;;)
		{
			if ((status = pReader->ReadArrayBool(&bValue)).IsNOK())
			{
				if (CX::Status_NoMoreItems == status.GetCode())
				{
					break;
				}
				return status;
			}
			bool_arr.push_back(bValue);
		}
		if ((status = pReader->EndObjectArray()).IsNOK())
		{
			return status;
		}
		if ((status = pReader->BeginObjectArray("int8_arr")).IsNOK())
		{
			return status;
		}
		for (;;)
		{
			if ((status = pReader->ReadArrayInt(&nValue)).IsNOK())
			{
				if (CX::Status_NoMoreItems == status.GetCode())
				{
					break;
				}
				return status;
			}
			int8_arr.push_back((CX::Int8)nValue);
		}
		if ((status = pReader->EndObjectArray()).IsNOK())
		{
			return status;
		}
		if ((status = pReader->BeginObjectArray("uint8_arr")).IsNOK())
		{
			return status;
		}
		for (;;)
		{
			if ((status = pReader->ReadArrayInt(&nValue)).IsNOK())
			{
				if (CX::Status_NoMoreItems == status.GetCode())
				{
					break;
				}
				return status;
			}
			uint8_arr.push_back((CX::UInt8)nValue);
		}
		if ((status = pReader->EndObjectArray()).IsNOK())
		{
			return status;
		}
		if ((status = pReader->BeginObjectArray("int16_arr")).IsNOK())
		{
			return status;
		}
		for (;;)
		{
			if ((status = pReader->ReadArrayInt(&nValue)).IsNOK())
			{
				if (CX::Status_NoMoreItems == status.GetCode())
				{
					break;
				}
				return status;
			}
			int16_arr.push_back((CX::Int16)nValue);
		}
		if ((status = pReader->EndObjectArray()).IsNOK())
		{
			return status;
		}
		if ((status = pReader->BeginObjectArray("uint16_arr")).IsNOK())
		{
			return status;
		}
		for (;;)
		{
			if ((status = pReader->ReadArrayInt(&nValue)).IsNOK())
			{
				if (CX::Status_NoMoreItems == status.GetCode())
				{
					break;
				}
				return status;
			}
			uint16_arr.push_back((CX::UInt16)nValue);
		}
		if ((status = pReader->EndObjectArray()).IsNOK())
		{
			return status;
		}
		if ((status = pReader->BeginObjectArray("int32_arr")).IsNOK())
		{
			return status;
		}
		for (;;)
		{
			if ((status = pReader->ReadArrayInt(&nValue)).IsNOK())
			{
				if (CX::Status_NoMoreItems == status.GetCode())
				{
					break;
				}
				return status;
			}
			int32_arr.push_back((CX::Int32)nValue);
		}
		if ((status = pReader->EndObjectArray()).IsNOK())
		{
			return status;
		}
		if ((status = pReader->BeginObjectArray("uint32_arr")).IsNOK())
		{
			return status;
		}
		for (;;)
		{
			if ((status = pReader->ReadArrayInt(&nValue)).IsNOK())
			{
				if (CX::Status_NoMoreItems == status.GetCode())
				{
					break;
				}
				return status;
			}
			uint32_arr.push_back((CX::UInt32)nValue);
		}
		if ((status = pReader->EndObjectArray()).IsNOK())
		{
			return status;
		}
		if ((status = pReader->BeginObjectArray("int64_arr")).IsNOK())
		{
			return status;
		}
		for (;;)
		{
			if ((status = pReader->ReadArrayInt(&nValue)).IsNOK())
			{
				if (CX::Status_NoMoreItems == status.GetCode())
				{
					break;
				}
				return status;
			}
			int64_arr.push_back((CX::Int64)nValue);
		}
		if ((status = pReader->EndObjectArray()).IsNOK())
		{
			return status;
		}
		if ((status = pReader->BeginObjectArray("uint64_arr")).IsNOK())
		{
			return status;
		}
		for (;;)
		{
			if ((status = pReader->ReadArrayInt(&nValue)).IsNOK())
			{
				if (CX::Status_NoMoreItems == status.GetCode())
				{
					break;
				}
				return status;
			}
			uint64_arr.push_back((CX::UInt64)nValue);
		}
		if ((status = pReader->EndObjectArray()).IsNOK())
		{
			return status;
		}
		if ((status = pReader->BeginObjectArray("float_arr")).IsNOK())
		{
			return status;
		}
		for (;;)
		{
			if ((status = pReader->ReadArrayReal(&lfValue)).IsNOK())
			{
				if (CX::Status_NoMoreItems == status.GetCode())
				{
					break;
				}
				return status;
			}
			float_arr.push_back((CX::Float)lfValue);
		}
		if ((status = pReader->EndObjectArray()).IsNOK())
		{
			return status;
		}
		if ((status = pReader->BeginObjectArray("double_arr")).IsNOK())
		{
			return status;
		}
		for (;;)
		{
			if ((status = pReader->ReadArrayReal(&lfValue)).IsNOK())
			{
				if (CX::Status_NoMoreItems == status.GetCode())
				{
					break;
				}
				return status;
			}
			double_arr.push_back((CX::Double)lfValue);
		}
		if ((status = pReader->EndObjectArray()).IsNOK())
		{
			return status;
		}
		if ((status = pReader->BeginObjectArray("string_arr")).IsNOK())
		{
			return status;
		}
		for (;;)
		{
			if ((status = pReader->ReadArrayString(&sValue)).IsNOK())
			{
				if (CX::Status_NoMoreItems == status.GetCode())
				{
					break;
				}
				return status;
			}
			string_arr.push_back(sValue);
		}
		if ((status = pReader->EndObjectArray()).IsNOK())
		{
			return status;
		}
		if ((status = pReader->BeginObjectArray("wstring_arr")).IsNOK())
		{
			return status;
		}
		for (;;)
		{
			if ((status = pReader->ReadArrayWString(&wsValue)).IsNOK())
			{
				if (CX::Status_NoMoreItems == status.GetCode())
				{
					break;
				}
				return status;
			}
			wstring_arr.push_back(wsValue);
		}
		if ((status = pReader->EndObjectArray()).IsNOK())
		{
			return status;
		}
		if ((status = pReader->BeginObjectArray("obj1_arr")).IsNOK())
		{
			return status;
		}
		for (;;)
		{
			My::Test::NameSpace::Test1Object tmp;

			if ((status = pReader->BeginArrayObject()).IsNOK())
			{
				if (CX::Status_NoMoreItems == status.GetCode())
				{
					break;
				}
				return status;
			}
			if ((status = tmp.Read(pReader)).IsNOK())
			{
				return status;
			}
			obj1_arr.push_back(tmp);
			if ((status = pReader->EndArrayObject()).IsNOK())
			{
				return status;
			}
		}
		if ((status = pReader->EndObjectArray()).IsNOK())
		{
			return status;
		}

		return CX::Status();
	}

	virtual CX::Status Write(CX::IO::IDataWriter *pWriter) const
	{
		CX::Status status;

		if ((status = pWriter->WriteObjectBool("bool_val", bool_val)).IsNOK())
		{
			return status;
		}
		if ((status = pWriter->WriteObjectInt("int8_val", (CX::Int64)int8_val)).IsNOK())
		{
			return status;
		}
		if ((status = pWriter->WriteObjectInt("uint8_val", (CX::Int64)uint8_val)).IsNOK())
		{
			return status;
		}
		if ((status = pWriter->WriteObjectInt("int16_val", (CX::Int64)int16_val)).IsNOK())
		{
			return status;
		}
		if ((status = pWriter->WriteObjectInt("uint16_val", (CX::Int64)uint16_val)).IsNOK())
		{
			return status;
		}
		if ((status = pWriter->WriteObjectInt("int32_val", (CX::Int64)int32_val)).IsNOK())
		{
			return status;
		}
		if ((status = pWriter->WriteObjectInt("uint32_val", (CX::Int64)uint32_val)).IsNOK())
		{
			return status;
		}
		if ((status = pWriter->WriteObjectInt("int64_val", (CX::Int64)int64_val)).IsNOK())
		{
			return status;
		}
		if ((status = pWriter->WriteObjectInt("uint64_val", (CX::Int64)uint64_val)).IsNOK())
		{
			return status;
		}
		if ((status = pWriter->WriteObjectReal("float_val", (CX::Double)float_val)).IsNOK())
		{
			return status;
		}
		if ((status = pWriter->WriteObjectReal("double_val", (CX::Double)double_val)).IsNOK())
		{
			return status;
		}
		if ((status = pWriter->WriteObjectString("string_val", string_val.c_str())).IsNOK())
		{
			return status;
		}
		if ((status = pWriter->WriteObjectWString("wstring_val", wstring_val.c_str())).IsNOK())
		{
			return status;
		}
		if ((status = pWriter->BeginObjectObject("obj1_val")).IsNOK())
		{
			return status;
		}
		if ((status = obj1_val.Write(pWriter)).IsNOK())
		{
			return status;
		}
		if ((status = pWriter->EndObjectObject()).IsNOK())
		{
			return status;
		}
		if ((status = pWriter->BeginObjectArray("bool_arr")).IsNOK())
		{
			return status;
		}
		for (CX::IO::SimpleBuffers::BoolArray::const_iterator iter = bool_arr.begin(); iter != bool_arr.end(); ++iter)
		{
			if ((status = pWriter->WriteArrayBool(*iter)).IsNOK())
			{
				return status;
			}
		}
		if ((status = pWriter->EndObjectArray()).IsNOK())
		{
			return status;
		}
		if ((status = pWriter->BeginObjectArray("int8_arr")).IsNOK())
		{
			return status;
		}
		for (CX::IO::SimpleBuffers::Int8Array::const_iterator iter = int8_arr.begin(); iter != int8_arr.end(); ++iter)
		{
			if ((status = pWriter->WriteArrayInt((CX::Int64)*iter)).IsNOK())
			{
				return status;
			}
		}
		if ((status = pWriter->EndObjectArray()).IsNOK())
		{
			return status;
		}
		if ((status = pWriter->BeginObjectArray("uint8_arr")).IsNOK())
		{
			return status;
		}
		for (CX::IO::SimpleBuffers::UInt8Array::const_iterator iter = uint8_arr.begin(); iter != uint8_arr.end(); ++iter)
		{
			if ((status = pWriter->WriteArrayInt((CX::Int64)*iter)).IsNOK())
			{
				return status;
			}
		}
		if ((status = pWriter->EndObjectArray()).IsNOK())
		{
			return status;
		}
		if ((status = pWriter->BeginObjectArray("int16_arr")).IsNOK())
		{
			return status;
		}
		for (CX::IO::SimpleBuffers::Int16Array::const_iterator iter = int16_arr.begin(); iter != int16_arr.end(); ++iter)
		{
			if ((status = pWriter->WriteArrayInt((CX::Int64)*iter)).IsNOK())
			{
				return status;
			}
		}
		if ((status = pWriter->EndObjectArray()).IsNOK())
		{
			return status;
		}
		if ((status = pWriter->BeginObjectArray("uint16_arr")).IsNOK())
		{
			return status;
		}
		for (CX::IO::SimpleBuffers::UInt16Array::const_iterator iter = uint16_arr.begin(); iter != uint16_arr.end(); ++iter)
		{
			if ((status = pWriter->WriteArrayInt((CX::Int64)*iter)).IsNOK())
			{
				return status;
			}
		}
		if ((status = pWriter->EndObjectArray()).IsNOK())
		{
			return status;
		}
		if ((status = pWriter->BeginObjectArray("int32_arr")).IsNOK())
		{
			return status;
		}
		for (CX::IO::SimpleBuffers::Int32Array::const_iterator iter = int32_arr.begin(); iter != int32_arr.end(); ++iter)
		{
			if ((status = pWriter->WriteArrayInt((CX::Int64)*iter)).IsNOK())
			{
				return status;
			}
		}
		if ((status = pWriter->EndObjectArray()).IsNOK())
		{
			return status;
		}
		if ((status = pWriter->BeginObjectArray("uint32_arr")).IsNOK())
		{
			return status;
		}
		for (CX::IO::SimpleBuffers::UInt32Array::const_iterator iter = uint32_arr.begin(); iter != uint32_arr.end(); ++iter)
		{
			if ((status = pWriter->WriteArrayInt((CX::Int64)*iter)).IsNOK())
			{
				return status;
			}
		}
		if ((status = pWriter->EndObjectArray()).IsNOK())
		{
			return status;
		}
		if ((status = pWriter->BeginObjectArray("int64_arr")).IsNOK())
		{
			return status;
		}
		for (CX::IO::SimpleBuffers::Int64Array::const_iterator iter = int64_arr.begin(); iter != int64_arr.end(); ++iter)
		{
			if ((status = pWriter->WriteArrayInt((CX::Int64)*iter)).IsNOK())
			{
				return status;
			}
		}
		if ((status = pWriter->EndObjectArray()).IsNOK())
		{
			return status;
		}
		if ((status = pWriter->BeginObjectArray("uint64_arr")).IsNOK())
		{
			return status;
		}
		for (CX::IO::SimpleBuffers::UInt64Array::const_iterator iter = uint64_arr.begin(); iter != uint64_arr.end(); ++iter)
		{
			if ((status = pWriter->WriteArrayInt((CX::Int64)*iter)).IsNOK())
			{
				return status;
			}
		}
		if ((status = pWriter->EndObjectArray()).IsNOK())
		{
			return status;
		}
		if ((status = pWriter->BeginObjectArray("float_arr")).IsNOK())
		{
			return status;
		}
		for (CX::IO::SimpleBuffers::FloatArray::const_iterator iter = float_arr.begin(); iter != float_arr.end(); ++iter)
		{
			if ((status = pWriter->WriteArrayReal((CX::Double)*iter)).IsNOK())
			{
				return status;
			}
		}
		if ((status = pWriter->EndObjectArray()).IsNOK())
		{
			return status;
		}
		if ((status = pWriter->BeginObjectArray("double_arr")).IsNOK())
		{
			return status;
		}
		for (CX::IO::SimpleBuffers::DoubleArray::const_iterator iter = double_arr.begin(); iter != double_arr.end(); ++iter)
		{
			if ((status = pWriter->WriteArrayReal((CX::Double)*iter)).IsNOK())
			{
				return status;
			}
		}
		if ((status = pWriter->EndObjectArray()).IsNOK())
		{
			return status;
		}
		if ((status = pWriter->BeginObjectArray("string_arr")).IsNOK())
		{
			return status;
		}
		for (CX::IO::SimpleBuffers::StringArray::const_iterator iter = string_arr.begin(); iter != string_arr.end(); ++iter)
		{
			if ((status = pWriter->WriteArrayString((*iter).c_str())).IsNOK())
			{
				return status;
			}
		}
		if ((status = pWriter->EndObjectArray()).IsNOK())
		{
			return status;
		}
		if ((status = pWriter->BeginObjectArray("wstring_arr")).IsNOK())
		{
			return status;
		}
		for (CX::IO::SimpleBuffers::WStringArray::const_iterator iter = wstring_arr.begin(); iter != wstring_arr.end(); ++iter)
		{
			if ((status = pWriter->WriteArrayWString((*iter).c_str())).IsNOK())
			{
				return status;
			}
		}
		if ((status = pWriter->EndObjectArray()).IsNOK())
		{
			return status;
		}
		if ((status = pWriter->BeginObjectArray("obj1_arr")).IsNOK())
		{
			return status;
		}
		for (My::Test::NameSpace::Test1ObjectArray::const_iterator iter = obj1_arr.begin(); iter != obj1_arr.end(); ++iter)
		{
			if ((status = pWriter->BeginArrayObject()).IsNOK())
			{
				return status;
			}
			if ((status = (*iter).Write(pWriter)).IsNOK())
			{
				return status;
			}
			if ((status = pWriter->EndArrayObject()).IsNOK())
			{
				return status;
			}
		}
		if ((status = pWriter->EndObjectArray()).IsNOK())
		{
			return status;
		}

		return CX::Status();
	}

};

typedef CX::Vector<Test2Object>::Type Test2ObjectArray;

}//namespace NameSpace

}//namespace Test

}//namespace My

