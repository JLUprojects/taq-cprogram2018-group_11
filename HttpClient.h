#pragma once

#include <Windows.h>
#include <Winhttp.h>
#include <string>
using namespace std;

//NOTE: 
// Class CHttpClientBase, CHttpClient, and CHttpDownloadFile are not thread safe.
// Please don't use an instance in different threads.
class CHttpClientBase
{
public:
	CHttpClientBase();
	virtual ~CHttpClientBase();

public:
	int ConnectServer(_In_ LPCTSTR pszHost, _In_opt_ USHORT nPort = 0);
	int Disconnect();

protected:
	DWORD m_dwFlags; // WINHTTP_FLAG_SECURE, WINHTTP_FLAG_REFRESH
	HINTERNET m_hSession;
	HINTERNET m_hConnect;

protected:
	HINTERNET SendRequest(_In_ LPCTSTR pszAction, _In_ LPCTSTR pszURI, _In_opt_ LPCSTR pszBody = NULL, _In_opt_ DWORD cbBody = 0);
	virtual int OnRecieve(LPCSTR pszData,DWORD cbData) = 0;
	int ReceiveRespond(_In_ HINTERNET hRequest);
};

class CHttpClient :public CHttpClientBase
{
public:
	CHttpClient();
	virtual ~CHttpClient();

public:
	int Head(_In_ LPCTSTR pszURI, _Out_ string& rstrRespond);
	int Get(_In_ LPCTSTR pszURI, _Out_ string& rstrRespond);
	int Post(_In_ LPCTSTR pszURI, _In_ LPCSTR pszOption, _Out_ string& rstrRespond);
	int Put(_In_ LPCTSTR pszURI, _In_ LPCSTR pszOption, _Out_ string& rstrRespond);
	int Delete(_In_ LPCTSTR pszURI, _Out_ string& rstrRespond);

protected:
	string* m_pstrRespond;
	virtual int OnRecieve(_In_ LPCSTR pszData, _In_ DWORD cbData);
};

class CHttpDownloadFile:public CHttpClientBase
{
public:
	CHttpDownloadFile();
	virtual ~CHttpDownloadFile();

public:
	int Download(_In_ LPCTSTR pszFileName, _In_ LPCTSTR pszURI);

protected:
	FILE* m_fp;
	virtual int OnRecieve(_In_ LPCSTR pszData, _In_ DWORD cbData);
};

int SaveStringToFile(_In_ LPCTSTR pszFileName, _In_ string& rstr);
