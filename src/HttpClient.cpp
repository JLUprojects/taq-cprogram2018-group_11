#include "stdafx.h"
#include "HttpClient.h"


#pragma comment(lib, "Winhttp.lib")

CHttpClientBase::CHttpClientBase() :m_dwFlags(0), m_hSession(NULL), m_hConnect(NULL)
{
}


CHttpClientBase::~CHttpClientBase()
{
	Disconnect();
}

int CHttpClientBase::ConnectServer(_In_ LPCTSTR pszHost, _In_opt_ USHORT nPort)
{
	int rc = 0;

	do
	{
		if (NULL != m_hSession)
		{
			Disconnect();
			break;
		}

		m_hSession = ::WinHttpOpen(L"Generic CHttpClient/1.0",
			WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
			WINHTTP_NO_PROXY_NAME,
			WINHTTP_NO_PROXY_BYPASS, 0);
		if (NULL == m_hSession)
		{
			_tprintf(_T("Error %u in WinHttpOpen.\n"), GetLastError());
			break;
		}

		LPCTSTR pszName = pszHost;
		if (0 == _tcsnicmp(pszHost, _T("https://"), 8))
		{
			pszName += 8;
			m_dwFlags |= WINHTTP_FLAG_SECURE;
		}
		else if (0 == _tcsnicmp(pszHost, _T("http://"), 7))
		{
			pszName += 7;
		}

		if (0 == nPort)
		{
			if (m_dwFlags & WINHTTP_FLAG_SECURE)
			{
				nPort = INTERNET_DEFAULT_HTTPS_PORT;
			}
			else
			{
				nPort = INTERNET_DEFAULT_HTTP_PORT;
			}
		}

		m_hConnect = ::WinHttpConnect(m_hSession, pszName, nPort, 0);
		if (NULL == m_hConnect)
		{
			_tprintf(_T("Error %u in WinHttpConnect.\n"), GetLastError());
			break;
		}
	} while (0);

	return rc;
}

int CHttpClientBase::Disconnect()
{
	if (NULL != m_hConnect)
	{
		::WinHttpCloseHandle(m_hConnect);
		m_hSession = NULL;
	}
	if (NULL != m_hSession)
	{
		::WinHttpCloseHandle(m_hSession);
		m_hSession = NULL;
	}

	return 0;
}

HINTERNET CHttpClientBase::SendRequest(_In_ LPCTSTR pszAction, _In_ LPCTSTR pszURI, _In_opt_ LPCSTR pszBody, _In_opt_ DWORD cbBody)
{
	HINTERNET hRequest = NULL;
	int rc = 0;

	do
	{
		if (NULL == m_hConnect)
		{
			break;
		}

		hRequest = ::WinHttpOpenRequest(m_hConnect, pszAction, pszURI, NULL, WINHTTP_NO_REFERER,
			WINHTTP_DEFAULT_ACCEPT_TYPES, m_dwFlags);
		if (NULL == hRequest)
		{
			printf("Error %u in WinHttpOpenRequest.\n", GetLastError());
			break;
		}

		LPVOID lpOptional = WINHTTP_NO_REQUEST_DATA;
		DWORD dwOptionalLength = 0;
		if (NULL != pszBody)
		{
			lpOptional = (LPVOID)pszBody;
			dwOptionalLength = cbBody;
			if (0 == cbBody)
			{
				dwOptionalLength = strlen(pszBody);
			}
		}

		BOOL bResults = ::WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, lpOptional, dwOptionalLength, 0, 0);
		if (!bResults)
		{
			printf("Error %u in WinHttpSendRequest.\n", GetLastError());
			rc = -11;
			break;
		}

		bResults = ::WinHttpReceiveResponse(hRequest, NULL);
		if (!bResults)
		{
			printf("Error %u in WinHttpReceiveResponse.\n", GetLastError());
			rc = -12;
			break;
		}
	} while (0);

	if (rc < 0)
	{
		::WinHttpCloseHandle(hRequest);
		hRequest = NULL;
	}

	return hRequest;
}

int CHttpClientBase::ReceiveRespond(_In_ HINTERNET hRequest)
{
	int rc = 0;
	DWORD dwBufSize = 0;
	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	DWORD dwTotal = 0;
	char* pszRecvBuf = NULL;
	BOOL bRC = FALSE;

	do
	{
		// Check for available data.
		dwSize = 0;
		bRC = ::WinHttpQueryDataAvailable(hRequest, &dwSize);
		if (!bRC)
		{
			printf("Error %u in WinHttpQueryDataAvailable.\n", GetLastError());
			rc = -11;
			break;
		}
		if (dwSize <= 0)
		{
			if (dwTotal <= 0)
			{
				printf("Error %u in WinHttpQueryDataAvailable.\n", GetLastError());
				rc = -21;
			}
			break;
		}

		// Allocate space for the buffer.
		if (dwSize >= dwBufSize)
		{
			dwBufSize = (dwSize + 4096) & ~4095;
			pszRecvBuf = (char*)realloc(pszRecvBuf, dwBufSize);
			if (NULL == pszRecvBuf)
			{
				printf("Out of memory. size=%d\n", dwBufSize);
				rc = -12;
				break;
			}
		}

		// Read the Data.
		bRC = ::WinHttpReadData(hRequest, (LPVOID)pszRecvBuf, dwSize, &dwDownloaded);
		if (!bRC)
		{
			printf("Error %u in WinHttpReadData. size=%d\n", GetLastError(), dwSize);
			rc = -13;
			break;
		}
		if (dwSize != dwDownloaded)
		{
			printf("size %d<-->%d\n", dwSize, dwDownloaded);
		}

		*(pszRecvBuf + dwDownloaded) = '\0';
		rc = OnRecieve(pszRecvBuf, dwDownloaded);
		if (rc < 0)
		{
			break;
		}
		dwTotal += dwDownloaded;
	} while (dwSize > 0);

	if (NULL != pszRecvBuf)
	{
		free(pszRecvBuf);
	}

	return rc;
}

//----------------------------------------------------------------------------
CHttpClient::CHttpClient() :m_pstrRespond(NULL)
{
}

CHttpClient::~CHttpClient()
{
}

int CHttpClient::Head(_In_ LPCTSTR pszURI, _Out_ string& rstrRespond)
{
	int rc = 0;
	HINTERNET hRequest = NULL;

	hRequest = SendRequest(L"HEAD", pszURI);
	if (NULL != hRequest)
	{
		rstrRespond = "";
		m_pstrRespond = &rstrRespond;
		rc = ReceiveRespond(hRequest);
		::WinHttpCloseHandle(hRequest);
	}

	return rc;
}

int CHttpClient::Get(_In_ LPCTSTR pszURI, _Out_ string& rstrRespond)
{
	int rc = 0;
	HINTERNET hRequest = NULL;

	hRequest = SendRequest(L"GET", pszURI);
	if (NULL != hRequest)
	{
		rstrRespond = "";
		m_pstrRespond = &rstrRespond;
		rc = ReceiveRespond(hRequest);
		::WinHttpCloseHandle(hRequest);
	}

	return rc;
}

int CHttpClient::Post(_In_ LPCTSTR pszURI, _In_ LPCSTR pszOption, _Out_ string& rstrRespond)
{
	int rc = 0;
	HINTERNET hRequest = NULL;

	hRequest = SendRequest(L"POST", pszURI, pszOption);
	if (NULL != hRequest)
	{
		rstrRespond = "";
		m_pstrRespond = &rstrRespond;
		rc = ReceiveRespond(hRequest);
		::WinHttpCloseHandle(hRequest);
	}

	return rc;
}

int CHttpClient::Put(_In_ LPCTSTR pszURI, _In_ LPCSTR pszOption, _Out_ string& rstrRespond)
{
	int rc = 0;
	HINTERNET hRequest = NULL;

	hRequest = SendRequest(L"PUT", pszURI, pszOption);
	if (NULL != hRequest)
	{
		rstrRespond = "";
		m_pstrRespond = &rstrRespond;
		rc = ReceiveRespond(hRequest);
		::WinHttpCloseHandle(hRequest);
	}

	return rc;
}

int CHttpClient::Delete(_In_ LPCTSTR pszURI, _Out_ string& rstrRespond)
{
	int rc = 0;
	HINTERNET hRequest = NULL;

	hRequest = SendRequest(L"DELETE", pszURI);
	if (NULL != hRequest)
	{
		rstrRespond = "";
		m_pstrRespond = &rstrRespond;
		rc = ReceiveRespond(hRequest);
		::WinHttpCloseHandle(hRequest);
	}

	return rc;
}

int CHttpClient::OnRecieve(_In_ LPCSTR pszData, _In_ DWORD cbData)
{
	*m_pstrRespond += pszData;

	return 0;
}

//----------------------------------------------------------------------------
CHttpDownloadFile::CHttpDownloadFile() :m_fp(NULL)
{
}

CHttpDownloadFile::~CHttpDownloadFile()
{
}

int CHttpDownloadFile::Download(_In_ LPCTSTR pszFileName, _In_ LPCTSTR pszURI)
{
	int rc = 0;

	do
	{
		if (NULL == pszFileName
			|| NULL == pszURI)
		{
			rc = -1;
			break;
		}

		errno_t et = _tfopen_s(&m_fp, pszFileName, _T("wb"));
		if (0 != et || NULL == m_fp)
		{
			rc = -2;
			break;
		}

		HINTERNET hRequest = NULL;

		hRequest = SendRequest(L"GET", pszURI);
		if (NULL == hRequest)
		{
			break;
		}
		rc = ReceiveRespond(hRequest);
		if (rc < 0)
		{
			break;
		}

		::WinHttpCloseHandle(hRequest);
	} while (0);

	if (NULL != m_fp)
	{
		::fclose(m_fp);
		m_fp = NULL;
	}

	return rc;
}

int CHttpDownloadFile::OnRecieve(_In_ LPCSTR pszData, _In_ DWORD cbData)
{
	int rc = 0;

	if (NULL != m_fp)
	{
		fwrite(pszData, 1, cbData, m_fp);
	}

	return rc;
}

//----------------------------------------------------------------------------
int SaveStringToFile(_In_ LPCTSTR pszFileName, _In_ string& rstr)
{
	int rc = 0;
	FILE* fp = NULL;

	do
	{
		if (NULL == pszFileName)
		{
			rc = -1;
			break;
		}

		errno_t et = _tfopen_s(&fp, pszFileName, _T("wb"));
		if (0 != et || NULL == fp)
		{
			rc = -2;
			break;
		}

		fwrite(rstr.c_str(), 1, rstr.length(), fp);
	} while (0);

	if (NULL != fp)
	{
		fclose(fp);
	}

	return rc;
}
