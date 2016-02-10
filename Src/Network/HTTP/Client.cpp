/*
* CX - C++ framework for general purpose development
*
* https://github.com/draede/cx
*
* Copyright (C) 2014 - 2016 draede - draede [at] outlook [dot] com
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

#include "CX/Network/HTTP/Client.hpp"
#include "CX/Print.hpp"
#include "CX/Network/URLParser.hpp"
#include "curl/curl.h"


namespace CX
{

namespace Network
{

namespace HTTP
{

Client::Client()
{
	m_pHandle = NULL;
}

Client::~Client()
{
	Close();
}

Status Client::Open(const Char *szHost, unsigned int nFlags/* = 0*/, UInt16 nPort/* = HTTP_PORT*/)
{
	Close();

	if (NULL == (m_pHandle = curl_easy_init()))
	{
		return Status(Status_OperationFailed, "curl_easy_init failed");
	}
	m_sHost = szHost;
	m_bSSL  = (Flag_SSL == (nFlags & Flag_SSL));
	if (m_bSSL)
	{
		if (0 == nPort)
		{
			nPort = HTTPS_PORT;
		}
	}
	else
	if (0 == nPort)
	{
		nPort = HTTP_PORT;
	}
	m_nPort = nPort;
	if ((nFlags & Flag_PersistentCookies))
	{
#ifdef CX_OS_WINDOWS
		curl_easy_setopt(m_pHandle, CURLOPT_COOKIEJAR, "NUL");
#else
		curl_easy_setopt(m_pHandle, CURLOPT_COOKIEJAR, "/dev/null");
#endif
	}
	if ((nFlags & Flag_Debug))
	{
		curl_easy_setopt(m_pHandle, CURLOPT_VERBOSE, 1);
	}
	else
	{
		curl_easy_setopt(m_pHandle, CURLOPT_VERBOSE, 0);
	}

	return Status();
}

Status Client::Close()
{
	if (NULL != m_pHandle)
	{
		curl_easy_cleanup(m_pHandle);
		m_pHandle = NULL;
	}

	return Status();
}

Bool Client::IsOpened()
{
	return (NULL != m_pHandle);
}

Status Client::SetUserAgent(const Char *szUserAgent)
{
	if (NULL == m_pHandle)
	{
		return Status_NotInitialized;
	}

	CURLcode nCode;

	if (CURLE_OK != (nCode = curl_easy_setopt(m_pHandle, CURLOPT_USERAGENT, szUserAgent)))
	{
		return Status(Status_OperationFailed, "curl_easy_setopt(CURLOPT_USERAGENT) failed with error {1}", (int)nCode);
	}

	return Status();
}

Status Client::SetReferer(const Char *szReferer)
{
	if (NULL == m_pHandle)
	{
		return Status_NotInitialized;
	}

	CURLcode nCode;

	if (CURLE_OK != (nCode = curl_easy_setopt(m_pHandle, CURLOPT_REFERER, szReferer)))
	{
		return Status(Status_OperationFailed, "curl_easy_setopt(CURLOPT_REFERER) failed with error {1}", (int)nCode);
	}

	return Status();
}

Status Client::Perform(const Char *szURI, const Char *szVerb, ScopePtr<IO::IInputStream> request, 
                       ScopePtr<IO::IOutputStream> response, int *pnStatusCode/* = NULL*/)
{
	if (NULL == m_pHandle)
	{
		return Status_NotInitialized;
	}

	String        sURL;
	ReadUserData  rdata;
	WriteUserData wdata;

	Print(&sURL, "{1}://{2}:{3}{4}", m_bSSL ? "https" : "http", m_sHost, m_nPort, szURI);
	curl_easy_setopt(m_pHandle, CURLOPT_URL, sURL.c_str());
	curl_easy_setopt(m_pHandle, CURLOPT_FOLLOWLOCATION, 1);
	if (0 == cx_stricmp(szVerb, "POST") && request.IsValid())
	{
		curl_easy_setopt(m_pHandle, CURLOPT_POST, 1);

		UInt64 cbSize;
		Status status;

		if (!(status = request->GetSize(&cbSize)))
		{
			return status;
		}
		if ((UInt64)TYPE_SIZE_MAX < cbSize)
		{
			return Status_TooBig;
		}
		curl_easy_setopt(m_pHandle, CURLOPT_POSTFIELDSIZE, (size_t)cbSize);
		curl_easy_setopt(m_pHandle, CURLOPT_READFUNCTION, &Client::ReadCallback);
		rdata.pClient      = this;
		rdata.pInputStream = request.Get();
		curl_easy_setopt(m_pHandle, CURLOPT_READDATA, (void *)&rdata);
	}
	else
	{
		curl_easy_setopt(m_pHandle, CURLOPT_POST, 0);
	}
	curl_easy_setopt(m_pHandle, CURLOPT_WRITEFUNCTION, &Client::WriteCallback);
	wdata.pClient       = this;
	wdata.pOutputStream = response.Get();
	curl_easy_setopt(m_pHandle, CURLOPT_WRITEDATA, (void *)&wdata);

	CURLcode nCode;

	if (CURLE_OK != (nCode = curl_easy_perform(m_pHandle)))
	{
		return Status(Status_OperationFailed, "curl_easy_perform failed with error {1}", (int)nCode);
	}

	if (NULL != pnStatusCode)
	{
		long nStatusCode;

		if (CURLE_OK != (nCode = curl_easy_getinfo(m_pHandle, CURLINFO_RESPONSE_CODE, &nStatusCode)))
		{
			return Status(Status_OperationFailed, "curl_easy_getinfo failed with error {1}", (int)nCode);
		}
		*pnStatusCode = nStatusCode;
	}

	return Status();
}

size_t Client::ReadCallback(char *pBuffer, size_t cbItemSize, size_t cItemsCount, void *pUserData)
{
	ReadUserData *pData    = (ReadUserData *)pUserData;
	Size         cbReqSize = cbItemSize * cItemsCount;
	Size         cbAckSize;
	Status       status;

	if (!(status = pData->pInputStream->Read(pBuffer, cbReqSize, &cbAckSize)))
	{
		return (size_t)CURL_READFUNC_ABORT;
	}

	return cbAckSize;
}

size_t Client::WriteCallback(char *pBuffer, size_t cbItemSize, size_t cItemsCount, void *pUserData)
{
	WriteUserData *pData    = (WriteUserData *)pUserData;
	Size          cbReqSize = cbItemSize * cItemsCount;
	Size          cbAckSize;
	Status        status;

	if (!(status = pData->pOutputStream->Write(pBuffer, cbReqSize, &cbAckSize)))
	{
		cbAckSize = 0;
	}

	return cbAckSize;
}

Status Client::DownloadURL(const Char *szURL, ScopePtr<IO::IOutputStream> response)
{
	Network::HTTP::Client client;
	int                   nStatusCode;
	String                sProtocol;
	String                sHost;
	int                   nPort;
	String                sPath;
	String                sQuery;
	String                sURI;
	bool                  bSSL;
	Status                status;

	if (!(status = URLParser::Parse(szURL, sProtocol, sHost, nPort, sPath, sQuery)))
	{
		return status;
	}
	if (0 == cx_stricmp(sProtocol.c_str(), "http"))
	{
		bSSL = false;
		if (0 >= nPort)
		{
			nPort = HTTP_PORT;
		}
	}
	else
	if (0 == cx_stricmp(sProtocol.c_str(), "https"))
	{
		bSSL = true;
		if (0 >= nPort)
		{
			nPort = HTTPS_PORT;
		}
	}
	else
	{
		return Status(Status_InvalidArg, "Invalid protocol");
	}
	sURI = "/";
	sURI += sPath;
	if (!sQuery.empty())
	{
		sURI += "?";
		sURI += sQuery;
	}
	if (!(status = client.Open(sHost.c_str(), bSSL, (UInt16)nPort)))
	{
		return status;
	}
	client.SetUserAgent("Mozilla/5.0 (Windows NT 10.0; WOW64; rv:40.0) Gecko/20100101 Firefox/40.0");
	if (!(status = client.Perform(sURI.c_str(), "GET", NULL, response, &nStatusCode)))
	{
		return status;
	}
	if (200 != nStatusCode)
	{
		return Status(Status_OperationFailed, "HTTP operation failed with status code {1}", nStatusCode);
	}

	return Status();
}

}//namespace HTTP

}//namespace Network

}//namespace CX
