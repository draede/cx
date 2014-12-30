
#include "CX/Vector.h"


#ifdef DLL_EXPORTS
	#define DLL_API __declspec(dllexport)
#else
	#define DLL_API __declspec(dllimport)
#endif


extern "C" DLL_API void TestVector(CX::Vector<int>::Type *pVectorInts);

extern "C" DLL_API void * TestMem(CX::Size cbSize);
