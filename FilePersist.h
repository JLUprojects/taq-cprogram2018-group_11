#pragma once
#include <stdio.h>

class CFilePersist
{
public:
	CFilePersist();
	~CFilePersist();

	static int SaveItem(IN FILE* fp, IN LPCTSTR pszName, IN int nValue);
	static int SaveItem(IN FILE* fp, IN LPCTSTR pszName, IN double fValue);
	static int SaveItem(IN FILE* fp, IN LPCTSTR pszName, IN CString& rstrValue);

	static int LoadItem(IN FILE* fp, IN LPCTSTR pszName, OUT int& rValue);
	static int LoadItem(IN FILE* fp, IN LPCTSTR pszName, IN double& rfValue);
	static int LoadItem(IN FILE* fp, IN LPCTSTR pszName, OUT CString& rValue);
	static int LoadItemLines(IN FILE* fp, IN LPCTSTR pszName, OUT CString& rValue);
};

const int _MAX_LINE = 1024;
void TrimLine(TCHAR* pszLine);