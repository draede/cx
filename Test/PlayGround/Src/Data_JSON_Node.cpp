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

#include "CX/Data/JSON/Node.hpp"
#include "CX/Print.hpp"
#include "Tester.hpp"
#include "CX/Print.hpp"


using namespace CX;


#define MYTEST(expr, msg)     if (!(expr)) { Print(stdout, "{1}\n", msg); }


void Data_JSON_Node_Test1()
{
	Data::JSON::Node node;
	Data::JSON::Node *pNode;
	Status           status;

	typedef Data::JSON::Node::Type JSONType;

	MYTEST(node.IsNull(), "Null expected");
	MYTEST(node.SetArray().IsOK() && node.IsArray() && 0 == node.GetItemsCount(), "SetArray failed");
	MYTEST(node.SetObject().IsOK() && node.IsObject() && 0 == node.GetMembersCount(), "SetObject failed");
	MYTEST(node.SetString("123").IsOK() && "123" == node.GetString(), "SetString failed");
	MYTEST(node.SetDouble(123.123).IsOK() && 123.123 == node.GetDouble(), "SetDouble failed");
	MYTEST(node.SetInt(123).IsOK() && 123 == node.GetInt(), "SetInt failed");
	MYTEST(node.SetBool(true).IsOK() && true == node.GetBool(), "SetBool failed");
	MYTEST(node.SetNull().IsOK() && node.IsNull(), "SetNull failed");

	node = "123"; MYTEST(node == String("123"), "SetString failed");
	node = 123.123; MYTEST(node == 123.123, "SetDouble failed");
	node = (Int64)123; MYTEST(node == (Int64)123, "SetInt failed");
	node = true; MYTEST(node == true, "SetBool failed");

	MYTEST(node.SetType(JSONType::Type_Array).IsOK() && node.IsArray() && 0 == node.GetItemsCount(), "SetArray (type) failed");
	MYTEST(node.SetType(JSONType::Type_Object).IsOK() && node.IsObject() && 0 == node.GetMembersCount(), "SetObject (type) failed");
	MYTEST(node.SetType(JSONType::Type_String).IsOK() && node.STRING_DEFAULT() == node.GetString(), "SetString (type) failed");
	MYTEST(node.SetType(JSONType::Type_Double).IsOK() && node.DOUBLE_DEFAULT() == node.GetDouble(), "SetDouble (type) failed");
	MYTEST(node.SetType(JSONType::Type_Int).IsOK() && node.INT_DEFAULT == node.GetInt(), "SetInt (type) failed");
	MYTEST(node.SetType(JSONType::Type_Bool).IsOK() && node.BOOL_DEFAULT == node.GetBool(), "SetBool (type) failed");
	MYTEST(node.SetType(JSONType::Type_Null).IsOK() && node.IsNull(), "SetNull (type) failed");

	MYTEST(node.SetArray().IsOK() && node.IsArray() && 0 == node.GetItemsCount(), "SetArray failed");
	pNode = node.AddItem(); MYTEST(NULL != pNode && pNode->IsNull() && 1 == node.GetItemsCount(), "AddItem failed");
	pNode->SetInt(100); MYTEST(node[0] == pNode->GetInt(), "Set operator failed");
	pNode = node.AddItem(); MYTEST(NULL != pNode && pNode->IsNull() && 2 == node.GetItemsCount(), "AddItem failed");
	pNode = node.AddItem(); MYTEST(NULL != pNode && pNode->IsNull() && 3 == node.GetItemsCount(), "AddItem failed");
	pNode = node.InsertItem(0); MYTEST(NULL != pNode && pNode->IsNull() && 4 == node.GetItemsCount(), "AddItem failed");
	pNode = node.InsertItem(4); MYTEST(NULL != pNode && pNode->IsNull() && 5 == node.GetItemsCount(), "AddItem failed");
	pNode = node.InsertItem(100); MYTEST(NULL != pNode && pNode->IsNull() && 6 == node.GetItemsCount(), "AddItem failed");
	MYTEST(node.RemoveItem(0).IsOK() && 5 == node.GetItemsCount(), "RemoveItem failed");
	MYTEST(node.RemoveAllItems().IsOK() && 0 == node.GetItemsCount(), "RemoveAllItems failed");

	MYTEST(node.SetObject().IsOK() && node.IsObject() && 0 == node.GetMembersCount(), "SetObject failed");
	pNode = node.AddMember("1"); MYTEST(NULL != pNode && pNode->IsNull() && 1 == node.GetMembersCount(), "AddMember failed");
	pNode->SetInt(100); MYTEST(node[String("1")] == pNode->GetInt(), "Set operator failed");
	pNode = node.AddMember("2"); MYTEST(NULL != pNode && pNode->IsNull() && 2 == node.GetMembersCount(), "AddMember failed");
	pNode->SetInt(200); MYTEST(node["2"] == pNode->GetInt(), "Set operator failed");
	pNode = node.AddMember("3"); MYTEST(NULL != pNode && pNode->IsNull() && 3 == node.GetMembersCount(), "AddMember failed");

	String sTmp;

	status = node.SaveToString(sTmp);

	Data::JSON::Node nodex;

	status = nodex.LoadFromString(sTmp);

	bool x = (node == nodex);

	x;

	MYTEST(node.RemoveMember("1").IsOK() && 2 == node.GetMembersCount(), "RemoveMember failed");
	MYTEST(node.RemoveAllMembers().IsOK() && 0 == node.GetMembersCount(), "RemoveAllMembers failed");
}

REGISTER_TEST(Data_JSON_Node_Test1);

