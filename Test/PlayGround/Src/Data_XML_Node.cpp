/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2021 draede
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

#include "CX/Data/XML/Node.hpp"
#include "CX/Print.hpp"
#include "Tester.hpp"
#include "CX/Print.hpp"


using namespace CX;


#define MYTEST(expr, msg)     if (!(expr)) { Print(stdout, "{1}\n", msg); }


void Data_XML_Node_Test1()
{
	Data::XML::Node node;
	Data::XML::Node node1;
	String          sTmp;
	Status          status;

	MYTEST(node.SetName("filesystem").IsOK(), "ERR1");
	node("type") = "ntfs";
	node("size") = "12345";
	MYTEST(node[0].SetName("drive").IsOK(), "ERR2");
	node[0]("name") = "c:";
	MYTEST(node[0].SetValue("drive_label").IsOK(), "ERR3");
	
	MYTEST(node.SaveToString(sTmp).IsOK(), "ERR4");
	MYTEST(node1.LoadFromString(sTmp).IsOK(), "ERR5");
	MYTEST(node1 == node, "Not Equal");
}

REGISTER_TEST(Data_XML_Node_Test1);

