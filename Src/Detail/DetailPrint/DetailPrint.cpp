
#include "CX/Detail/DetailPrint/DetailPrint.h"
#include "../../../Contrib/DoubleConversion/Include/double-conversion.h"


namespace CX
{

namespace Detail
{

namespace DetailPrint
{

bool DoubleToString(Double lfValue, Char *szOutput, Size cLen, Size cPrecision)
{
	double_conversion::DoubleToStringConverter	cvt(double_conversion::DoubleToStringConverter::Flags::NO_FLAGS, "#", "*", 'E', 0, 0, 0, 0);
	double_conversion::StringBuilder					sb(szOutput, cLen);

	return cvt.ToFixed(lfValue, cPrecision, &sb);
}

}//namespace DetailPrint

}//namespace Detail

}//namespace CX

