
#include "CX/CX.h"
#include "CX/Deque.h"
#include "CX/List.h"
#include "CX/Map.h"
#include "CX/Queue.h"
#include "CX/Set.h"
#include "CX/Stack.h"
#include "CX/String.h"
#include "CX/UOMap.h"
#include "CX/UOMultiMap.h"
#include "CX/UOMultiSet.h"
#include "CX/UOSet.h"
#include "CX/Vector.h"

#pragma warning(push)
#pragma warning(disable: 4702)
#include "../../../Contrib/Catch/Include/catch.hpp"
#pragma warning(pop)


using namespace CX;



TEST_CASE("STL container tests", "[CX::STL]")
{
	SECTION("Deque")
	{
		Deque<int>::Type c;
		c.insert(c.begin(), 100);
		REQUIRE(100 == *c.begin());
	}
	SECTION("List")
	{
		List<int>::Type c;
		c.insert(c.begin(), 100);
		REQUIRE(100 == *c.begin());
	}
	SECTION("Map")
	{
		Map<int, int>::Type c;
		c[100] = 100;
		REQUIRE(100 == c[100]);
	}
	SECTION("Queue")
	{
		Queue<int>::Type c;
		c.push(100);
		REQUIRE(100 == c.front());
	}
	SECTION("Set")
	{
		Set<int>::Type c;
		c.insert(100);
		REQUIRE(100 == *c.find(100));
	}
	SECTION("Stack")
	{
		Stack<int>::Type c;
		c.push(100);
		REQUIRE(100 == c.top());
	}
	SECTION("Vector")
	{
		Vector<int>::Type c;
		c.push_back(100);
		REQUIRE(100 == *c.begin());
	}
	SECTION("UOMap")
	{
		UOMap<int, int>::Type c;
		c[100] = 100;
		REQUIRE(100 == c[100]);
	}
	SECTION("UOSet")
	{
		UOSet<int>::Type c;
		c.insert(100);
		REQUIRE(100 == *c.find(100));
	}
	SECTION("UOMultiMap")
	{
		UOMultiMap<int, int>::Type c;
		c.insert(std::pair<int, int>(100, 100));
		REQUIRE(1 == c.count(100));
	}
	SECTION("UOSet")
	{
		UOSet<int>::Type c;
		c.insert(100);
		REQUIRE(1 == c.count(100));
	}
	SECTION("String")
	{
		String x;
		x = "abc";
		REQUIRE(x == String("abc"));
	}
}

