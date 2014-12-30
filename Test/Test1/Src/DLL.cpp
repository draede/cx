
#include "DLL.h"


DLL_API void TestVector(CX::Vector<int>::Type *pVectorInts)
{
	pVectorInts->push_back(1);
	pVectorInts->push_back(2);
	pVectorInts->push_back(3);
}

DLL_API void * TestMem(CX::Size cbSize)
{
	return CX::Alloc(cbSize);
}
