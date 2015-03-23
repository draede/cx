
#pragma once


#include "CX/IO/SimpleBuffers/Types.hpp"


namespace My
{

namespace Test
{

namespace NameSpace
{

class Test1Object : public CX::IO::SimpleBuffers::IObject
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
	}

	virtual void SetupWithSomeValues()
	{
		Reset();
		bool_val = true;
		int8_val = 37;
		uint8_val = 154;
		int16_val = 28908;
		uint16_val = 56571;
		int32_val = 429078343;
		uint32_val = 1064296322;
		int64_val = 7155689572760980160;
		uint64_val = 15549531131302430372;
		float_val = 0.636964f;
		double_val = 0.217706;
		string_val = "1\"akC\"skC7)!";
		wstring_val = L"j.ghOj>xOQF6}^Bbk6#!Je\\Z[g9%nbNyhlMv~0A'J3i8GP*Un1X9n'X'JOvTE3GK";
		bool_arr.push_back(false);
		bool_arr.push_back(false);
		bool_arr.push_back(false);
		bool_arr.push_back(true);
		bool_arr.push_back(false);
		bool_arr.push_back(false);
		bool_arr.push_back(true);
		bool_arr.push_back(false);
		bool_arr.push_back(true);
		bool_arr.push_back(false);
		int8_arr.push_back(-37);
		int8_arr.push_back(-68);
		int8_arr.push_back(-93);
		int8_arr.push_back(-102);
		int8_arr.push_back(31);
		int8_arr.push_back(97);
		int8_arr.push_back(31);
		int8_arr.push_back(89);
		int8_arr.push_back(-91);
		uint8_arr.push_back(39);
		uint8_arr.push_back(232);
		uint8_arr.push_back(97);
		uint8_arr.push_back(109);
		uint8_arr.push_back(104);
		uint8_arr.push_back(48);
		uint8_arr.push_back(31);
		uint8_arr.push_back(209);
		uint8_arr.push_back(179);
		uint8_arr.push_back(97);
		uint8_arr.push_back(196);
		uint8_arr.push_back(191);
		uint8_arr.push_back(91);
		uint8_arr.push_back(210);
		uint8_arr.push_back(144);
		uint8_arr.push_back(42);
		int16_arr.push_back(27298);
		int16_arr.push_back(25550);
		int16_arr.push_back(32288);
		int16_arr.push_back(-7950);
		int16_arr.push_back(32426);
		int16_arr.push_back(-15493);
		int16_arr.push_back(-24025);
		int16_arr.push_back(-12974);
		int16_arr.push_back(8099);
		int16_arr.push_back(-26422);
		int16_arr.push_back(-15125);
		int16_arr.push_back(28553);
		uint16_arr.push_back(26943);
		uint16_arr.push_back(6303);
		uint16_arr.push_back(44035);
		uint16_arr.push_back(7231);
		uint16_arr.push_back(16709);
		uint16_arr.push_back(11808);
		uint16_arr.push_back(5293);
		uint32_arr.push_back(317307840);
		uint32_arr.push_back(2320612170);
		uint32_arr.push_back(4192374014);
		uint32_arr.push_back(3357191780);
		uint32_arr.push_back(3364972132);
		uint32_arr.push_back(3224077993);
		uint32_arr.push_back(2926185186);
		uint32_arr.push_back(3779684566);
		uint32_arr.push_back(667343246);
		uint64_arr.push_back(11255548767725194022);
		uint64_arr.push_back(8334678534081577490);
		float_arr.push_back(0.139890f);
		double_arr.push_back(0.073859);
		double_arr.push_back(0.197018);
		double_arr.push_back(0.081132);
		double_arr.push_back(0.799313);
		double_arr.push_back(0.248596);
		double_arr.push_back(0.328050);
		double_arr.push_back(0.615630);
		double_arr.push_back(0.416947);
		double_arr.push_back(0.727105);
		double_arr.push_back(0.985389);
		string_arr.push_back("0Q{9Xn.w$0O<|Hr9yV8$skgRpAw-SL(N");
		string_arr.push_back(".LHp*qGH(jd@ePXy9Yx<~0VY`)8bKac1");
		string_arr.push_back("QgL\"ER.P]e(VT>F{%qHD%gAQ0N[4:[Myk");
		string_arr.push_back("KRp[6'S,Ldc?hN>$40J+_eGp$OX}.Vi");
		string_arr.push_back("BR[kqBr_*TNgFEE9RJ\\By8N$K4o7&hSV\\;:L0<");
		string_arr.push_back(";9Y/j::");
		string_arr.push_back("50NQp?]a1^VOK{.@Nlk*AHD");
		string_arr.push_back("v#9>JJ'G.*Q&YFL");
		string_arr.push_back("47>m*CIU~}N@'G:)HH{bw90QV'oU^^~vQ#?ZUfurP<'_2K6l#6Bc&aehW]H3_aC");
		string_arr.push_back("LP99f9nFuz{`H$gA|m@3ji<-`ktbSlPi#7c:UT7=%dvD%");
		string_arr.push_back("Uh=/Tv@:+f60Hekse]g#^<");
		string_arr.push_back("3x>/6\"Dx_l?;vz+`ck");
		string_arr.push_back("E%?~(p7x>l){xk3*jY,gC&O/!~(%JO&CGl0VPpBq.Gf}N:P\\+");
		wstring_arr.push_back(L"a`Eb");
		wstring_arr.push_back(L"\"27");
		wstring_arr.push_back(L"P\"25oq[?Y'j@{D+~T7*V{0z$?ER&O5#PESe@Jh4?|;k_{yCmMKj[F4V#@^8");
		wstring_arr.push_back(L"X*G8jAZYu}lOL}kd:fka+B(\"o?Z1_C6}IB-S/V-Fc%R!^Lq^+>QAUumt%,N!\\@p");
		wstring_arr.push_back(L"r`aKT{I3$Q*+:L(R?R$$k_\"MnYU\",yIVb");
		wstring_arr.push_back(L"Ejg+Q-pTgL2\"a48S,Y,\"pzfI7(-9i@S5nC'&");
		wstring_arr.push_back(L"w~$<*bD15]Yn");
		wstring_arr.push_back(L"+=h-)qLJ0$h|Ez%@jyKs:pDJ`vB5*e$,5Eoj#mevK2*EX");
		wstring_arr.push_back(L"E'U}_ftH+EzK4|U</`pvgcy-Q+m6d]>'Gkhx:");
		wstring_arr.push_back(L"u)-LPFQN,]?V'JC04BzOK)/LY)di)e");
		wstring_arr.push_back(L"h06kFNW.Zqh~");
	}

	virtual bool Compare(const CX::IO::SimpleBuffers::IObject *pObj)
	{
		const Test1Object *pObject = dynamic_cast<const Test1Object *>(pObj);
	
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

		return CX::Status();
	}

};

typedef CX::Vector<Test1Object>::Type Test1ObjectArray;

}//namespace NameSpace

}//namespace Test

}//namespace My
