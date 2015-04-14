/* 
 * CX - C++ framework for general purpose developement
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014-2015 draede - draede [at] outlook [dot] com
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

#pragma once


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/String.hpp"
#include "CX/Vector.hpp"
#include "CX/IObject.hpp"


namespace CX
{

namespace SB
{

class Member : public IObject
{
public:

	enum Type
	{
		Type_Scalar,  //def => name: m_sValType;
		Type_Vector,  //def => name: vector<m_sValType>;
		Type_Set,     //def => name: set<m_sKeyType>;
		Type_Map,     //def => name: map<m_sKeyType, m_sValType>;
		Type_HashSet, //def => name: hashset<m_sKeyType>;
		Type_HashMap, //def => name: hashmap<m_sKeyType, m_sValType>;
	};

	String  m_sName;
	String  m_sAlias;
	Type    m_nType;
	String  m_sKeyType;
	String  m_sValType;

	Member()
	{
	}

	Member(const Char *szName, const Char *szKeyType, Type nType = Type_Scalar, const Char *szValType = "")
	{
		m_sName    = szName;
		m_nType    = nType;
		m_sKeyType = szKeyType;
		m_sValType = szValType;
	}

};

typedef CX::Vector<Member>::Type   MembersVector;

}//namespace SB

}//namespace CX

