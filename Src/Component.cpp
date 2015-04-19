/* 
 * CX - C++ framework for general purpose development
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

#include "CX/Component.hpp"
#include "CX/Mem.hpp"


namespace CX
{

Component::Component()
{
}

Component::~Component()
{ 
}

bool Component::Destroy(bool bForce/* = false*/)
{
	if (bForce)
	{
		delete(this);

		return true;
	}
	else
	{
		return Release();
	}
}

Size Component::GetRefCount()
{
	return (Size)Sys::Atomic::Load(&m_refcnt);
}

void Component::Init()
{
	Sys::Atomic::Store(&m_refcnt, 1);
}

void Component::Retain()
{
	Sys::Atomic::FetchAdd(&m_refcnt, 1);
}

bool Component::Release()
{
	if (1 == Sys::Atomic::FetchAdd(&m_refcnt, -1))
	{
		delete(this);

		return true;
	}
	else
	{
		return false;
	}
}

}//namespace CX

