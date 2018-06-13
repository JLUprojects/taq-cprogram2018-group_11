#include "stdafx.h"
#include "FilePersist.h"

CFilePersist::CFilePersist()
{
}

CFilePersist::~CFilePersist()
{
}

int CFilePersist::SaveItem(IN FILE* fp, IN LPCTSTR pszName, IN int nValue)
{
	int rc = 0;

	_ftprintf_s(fp, _T("[%s]\n%d\n"), pszName, nValue);

	return rc;
}

int CFilePersist::SaveItem(IN FILE* fp, IN LPCTSTR pszName, IN double fValue)
{
	int rc = 0;

	_ftprintf_s(fp, _T("[%s]\n%.2f\n"), pszName, fValue);

	return rc;
}

int CFilePersist::SaveItem(IN FILE* fp, IN LPCTSTR pszName, IN CString& rstrValue)
{
	int rc = 0;

	_ftprintf_s(fp, _T("[%s]\n"), pszName);
	if (_T("") != rstrValue)
	{
		_ftprintf_s(fp, _T("%s"), LPCTSTR(rstrValue));
	}
	_ftprintf_s(fp, _T("\n"));

	return rc;
}

void TrimLine(TCHAR* pszLine)
{
	TCHAR* pb = pszLine;
	while (_T('\0') != *pb)
	{
		pb++;
	}
	pb--;
	while (pb >= pszLine)
	{
		if (_T('\r') == *pb || _T('\n') == *pb
			|| _T('\t') == *pb || _T(' ') == *pb)
		{
			*pb = _T('\0');
			pb--;
		}
		else
		{
			break;
		}
	}
}

int CFilePersist::LoadItem(IN FILE* fp, IN LPCTSTR pszName, OUT int& rValue)
{
	int rc = 0;

	do
	{
		CString strName;
		strName.Format(_T("[%s]"), pszName);

		TCHAR szLine[_MAX_LINE] = _T("");
		if (NULL == _fgetts(szLine, _MAX_LINE, fp))
		{
			rc = -1;
			break;
		}
		TrimLine(szLine);
		if (0 != _tcscmp(LPCTSTR(strName), szLine))
		{
			rc = -2;
			break;
		}

		if (NULL == _fgetts(szLine, _MAX_LINE, fp))
		{
			rc = -3;
			break;
		}
		TrimLine(szLine);

		rValue = _ttoi(szLine);
	} while (0);

	return rc;
}

int CFilePersist::LoadItem(IN FILE* fp, IN LPCTSTR pszName, IN double& rfValue)
{
	int rc = 0;

	do
	{
		CString strName;
		strName.Format(_T("[%s]"), pszName);

		TCHAR szLine[_MAX_LINE] = _T("");
		if (NULL == _fgetts(szLine, _MAX_LINE, fp))
		{
			rc = -1;
			break;
		}
		TrimLine(szLine);
		if (0 != _tcscmp(LPCTSTR(strName), szLine))
		{
			rc = -2;
			break;
		}

		if (NULL == _fgetts(szLine, _MAX_LINE, fp))
		{
			rc = -3;
			break;
		}
		TrimLine(szLine);

		rfValue = _ttof(szLine);
	} while (0);

	return rc;
}

int CFilePersist::LoadItem(IN FILE* fp, IN LPCTSTR pszName, OUT CString& rValue)
{
	int rc = 0;

	do
	{
		CString strName;
		strName.Format(_T("[%s]"), pszName);

		TCHAR szLine[_MAX_LINE] = _T("");
		if (NULL == _fgetts(szLine, _MAX_LINE, fp))
		{
			rc = -1;
			break;
		}
		TrimLine(szLine);
		if (0 != _tcscmp(LPCTSTR(strName), szLine))
		{
			rc = -2;
			break;
		}

		if (NULL == _fgetts(szLine, _MAX_LINE, fp))
		{
			rc = -3;
			break;
		}
		TrimLine(szLine);

		rValue = szLine;
	} while (0);

	return rc;
}

int CFilePersist::LoadItemLines(IN FILE* fp, IN LPCTSTR pszName, OUT CString& rValue)
{
	int rc = 0;

	do
	{
		CString strName;
		strName.Format(_T("[%s]"), pszName);

		TCHAR szLine[_MAX_LINE] = _T("");
		if (NULL == _fgetts(szLine, _MAX_LINE, fp))
		{
			rc = -1;
			break;
		}
		TrimLine(szLine);
		if (0 != _tcscmp(LPCTSTR(strName), szLine))
		{
			rc = -2;
			break;
		}

		rValue = _T("");
		while (NULL != _fgetts(szLine, _MAX_LINE, fp))
		{
			TrimLine(szLine);
			if (_T('\0') == szLine[0])
			{
				break;
			}
			if (_T("") != rValue)
			{
				rValue += _T("\n");
			}
			rValue += szLine;
		}
	} while (0);

	return rc;
}
