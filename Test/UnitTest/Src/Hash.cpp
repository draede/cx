
#include "CX/CX.h"
#include "CX/Hash/CRC32Hash.h"
#include "CX/Hash/MD5Hash.h"
#include "CX/Hash/SHA1Hash.h"
#include "CX/Hash/SHA256Hash.h"
#include "CX/Hash/SHA384Hash.h"
#include "CX/Hash/SHA512Hash.h"
#include "CX/Str/HexString.h"

#pragma warning(push)
#pragma warning(disable: 4702)
#include "../../../Contrib/Catch/Include/catch.hpp"
#pragma warning(pop)


using namespace CX;


template <typename Hash>
const Char *ToHash()
{
	static String   sRes;

	static Char     szTestString[] = "The quick brown fox jumps over the lazy dog";
	static Size     cTestStringLen = cx_strlen(szTestString);
	Hash            hash;
	Byte            res[Hash::SIZE];

	if (hash.Init().IsNOK())
	{
		return "";
	}
	if (hash.Update(szTestString, cTestStringLen).IsNOK())
	{
		return "";
	}
	if (hash.Done(&res).IsNOK())
	{
		return "";
	}
	if (Str::HexString::ToString(res, Hash::SIZE, &sRes, Str::HexString::Lowercase).IsNOK())
	{
		return "";
	}

	return sRes.c_str();
}

template <typename Hash>
const Char *ToHashAcc()
{
	static String   sRes;

	static Char     szTestString[]  = "The quick brown fox jumps over the lazy dog";
	static Size     cTestStringLen  = cx_strlen(szTestString);
	static Size     cTestStringLen1 = cTestStringLen / 2;
	static Size     cTestStringLen2 = cTestStringLen / 2;
	Hash            hash;
	Byte            res[Hash::SIZE];

	if (0 < (cTestStringLen % 2))
	{
		cTestStringLen2++;
	}
	if (hash.Init().IsNOK())
	{
		return "";
	}
	if (hash.Update(szTestString, cTestStringLen1).IsNOK())
	{
		return "";
	}
	if (hash.Update(szTestString + cTestStringLen1, cTestStringLen2).IsNOK())
	{
		return "";
	}
	if (hash.Done(&res).IsNOK())
	{
		return "";
	}
	if (Str::HexString::ToString(res, Hash::SIZE, &sRes, Str::HexString::Lowercase).IsNOK())
	{
		return "";
	}

	return sRes.c_str();
}

TEST_CASE("Hash tests", "[CX::Hash]")
{
	SECTION("CRC32 single pass")
	{
		REQUIRE(String("39a34f41") == ToHash<Hash::CRC32Hash>());
	}
	SECTION("CRC32 accumulate")
	{
		REQUIRE(String("39a34f41") == ToHashAcc<Hash::CRC32Hash>());
	}
	SECTION("MD5 single pass")
	{
		REQUIRE(String("9e107d9d372bb6826bd81d3542a419d6") == ToHash<Hash::MD5Hash>());
	}
	SECTION("MD5 accumulate")
	{
		REQUIRE(String("9e107d9d372bb6826bd81d3542a419d6") == ToHashAcc<Hash::MD5Hash>());
	}
	SECTION("SHA1 single pass")
	{
		REQUIRE(String("2fd4e1c67a2d28fced849ee1bb76e7391b93eb12") == ToHash<Hash::SHA1Hash>());
	}
	SECTION("SHA1 accumulate")
	{
		REQUIRE(String("2fd4e1c67a2d28fced849ee1bb76e7391b93eb12") == ToHashAcc<Hash::SHA1Hash>());
	}
	SECTION("SHA256 single pass")
	{
		REQUIRE(String("d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592") == 
		        ToHash<Hash::SHA256Hash>());
	}
	SECTION("SHA256 accumulate")
	{
		REQUIRE(String("d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592") == 
		        ToHashAcc<Hash::SHA256Hash>());
	}
	SECTION("SHA384 single pass")
	{
		REQUIRE(String("ca737f1014a48f4c0b6dd43cb177b0afd9e5169367544c494011e3317dbf9a509cb1e5dc1e85a"
		               "941bbee3d7f2afbc9b1") == ToHash<Hash::SHA384Hash>());
	}
	SECTION("SHA384 accumulate")
	{
		REQUIRE(String("ca737f1014a48f4c0b6dd43cb177b0afd9e5169367544c494011e3317dbf9a509cb1e5dc1e85a"
		               "941bbee3d7f2afbc9b1") == ToHashAcc<Hash::SHA384Hash>());
	}
	SECTION("SHA512 single pass")
	{
		REQUIRE(String("07e547d9586f6a73f73fbac0435ed76951218fb7d0c8d788a309d785436bbb642e93a252a954f"
		               "23912547d1e8a3b5ed6e1bfd7097821233fa0538f3db854fee6") == 
		        ToHash<Hash::SHA512Hash>());
	}
	SECTION("SHA512 accumulate")
	{
		REQUIRE(String("07e547d9586f6a73f73fbac0435ed76951218fb7d0c8d788a309d785436bbb642e93a252a954f"
		               "23912547d1e8a3b5ed6e1bfd7097821233fa0538f3db854fee6") == 
		        ToHashAcc<Hash::SHA512Hash>());
	}
}

