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

#pragma once


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/String.hpp"
#include "CX/ScopePtr.hpp"
#include "CX/IO/IInputStream.hpp"
#include "CX/IO/IOutputStream.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace Network
{

namespace HTTP
{

class CX_API Client : public IObject
{
public:

	static const UInt16    HTTP_PORT  = 80;
	static const UInt16    HTTPS_PORT = 443;

	enum Flags
	{
		Flag_SSL,
		Flag_PersistentCookies,
		Flag_Debug,
	};

	Client();

	~Client();

	Status Open(const Char *szHost, unsigned int nFlags = 0, UInt16 nPort = 0);

	Status Close();

	Bool IsOpened();

	Status SetUserAgent(const Char *szUserAgent);

	Status SetReferer(const Char *szReferer);

	Status Perform(const Char *szURI, const Char *szVerb, ScopePtr<IO::IInputStream> request, 
	               ScopePtr<IO::IOutputStream> response, int *pnStatusCode = NULL);

	static Status DownloadURL(const Char *szURL, ScopePtr<IO::IOutputStream> response);

private:

	struct ReadUserData
	{
		Client           *pClient;
		IO::IInputStream *pInputStream;
	};

	struct WriteUserData
	{
		Client            *pClient;
		IO::IOutputStream *pOutputStream;
	};

	void   *m_pHandle;
#pragma warning(push)
#pragma warning(disable: 4251)
	String m_sHost;
#pragma warning(pop)
	bool   m_bSSL;
	UInt16 m_nPort;

	static size_t ReadCallback(char *pBuffer, size_t cbItemSize, size_t cItemsCount, void *pUserData); 

	static size_t WriteCallback(char *pBuffer, size_t cbItemSize, size_t cItemsCount, void *pUserData);

};

}//namespace HTTP

}//namespace Network

}//namespace CX
