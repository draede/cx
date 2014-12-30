
#include "CX/Sys/DynLib.h"
#include "CX/Print.h"
#include <stdio.h>
#include "DLL.h"

class Test
{
public:

	Test()
	{
		printf("Test()\n");
	}

	~Test()
	{
		printf("~Test()\n");
	}

};

typedef void (* TestVectorFunc)(CX::Vector<int>::Type *);

typedef void * (* TestMemFunc)(CX::Size);


int main(int argc, char *argv[])
{
	CX::Print(stdout, "{1} => '{2}'", 100, "a123b");

	char *x = (char *)CX::Alloc(100);

	CX::Free(x);

	Test *pTest = CX::New<Test>();

	CX::Delete(pTest);

	CX::Sys::DynLib	dynlib;
	TestVectorFunc		pfnTestVector;
	TestMemFunc			pfnTestMem;

	if (CXOK(dynlib.Load("dll.dll")))
	{
		if (NULL != (pfnTestVector = (TestVectorFunc)dynlib.GetFunc("TestVector")) &&
				NULL != (pfnTestMem = (TestMemFunc)dynlib.GetFunc("TestMem")))
		{
			CX::Vector<int>::Type vectorInts;

			pfnTestVector(&vectorInts);
			printf("%d\n", vectorInts[0]);
			printf("%d\n", vectorInts[1]);
			printf("%d\n", vectorInts[2]);
			vectorInts.clear();

			void *a  = pfnTestMem(100);
			CX::Free(a);
		}
		dynlib.Unload();
	}

	return 0;
}

