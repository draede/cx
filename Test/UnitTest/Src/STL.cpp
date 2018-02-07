/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2018 draede - draede [at] outlook [dot] com
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

#include "CX/Types.hpp"
#include "CX/Deque.hpp"
#include "CX/List.hpp"
#include "CX/Map.hpp"
#include "CX/Queue.hpp"
#include "CX/Set.hpp"
#include "CX/Stack.hpp"
#include "CX/String.hpp"
#include "CX/UOMap.hpp"
#include "CX/UOMultiMap.hpp"
#include "CX/UOMultiSet.hpp"
#include "CX/UOSet.hpp"
#include "CX/Vector.hpp"

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

