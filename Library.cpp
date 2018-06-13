#include "stdafx.h"
#include "Library.h"


CLibrary::CLibrary(void)
{
}


CLibrary::~CLibrary(void)
{
}

int CLibrary::GetBookFolder(OUT CString& rstrFolder)
{
	int rc = 0;

	do
	{
		rstrFolder = _T("");

		TCHAR szPath[MAX_PATH] = _T("");
		if (!::GetCurrentDirectory(MAX_PATH, szPath))
		{
			rc = -1;
			break;
		}

		rstrFolder = szPath;
		rstrFolder += _T("\\Books");

		::CreateDirectory(LPCTSTR(rstrFolder), NULL);
	} while (0);

	return rc;
}

int CLibrary::Save()
{
	int rc = 0;

	do
	{
		CString strFolder;
		rc = GetBookFolder(strFolder);
		if (rc < 0)
		{
			rc = -1;
			break;
		}

		INT_PTR i, cnt = m_books.GetSize();
		for (i = 0; i < cnt; i++)
		{
			CBook& book = m_books.GetAt(i);
			book.Save(strFolder);
		}
	} while (0);

	return rc;
}

int CLibrary::Load()
{
	int rc = 0;

	do
	{
		m_books.RemoveAll();

		CString strFolder;
		rc = GetBookFolder(strFolder);
		if (rc < 0)
		{
			rc = -1;
			break;
		}
		strFolder += _T("\\");

		CFileFind ff;
		CString strFile = strFolder + _T("Book_*.txt");
		BOOL bContinue = ff.FindFile(LPCTSTR(strFile));
		while (bContinue)
		{
			bContinue = ff.FindNextFile();
			strFile = ff.GetFilePath();
			CBook book;
			rc = book.Load(strFile);
			if (rc >= 0)
			{
				m_books.Add(book);
			}
		}
	} while (0);

	return rc;
}

int CLibrary::AddBook(IN CBook& rbook)
{
	int rc = 0;

	bool bUpdated = false;

	// get book id
	int id = 0;
	INT_PTR i, cnt = m_books.GetSize();
	for (i = 0; i < cnt; i++)
	{
		CBook& book = m_books.GetAt(i);
		if (book.m_id >= id)
		{
			id = book.m_id + 1;
		}
		if (book.m_strName == rbook.m_strName)
		{
			book = rbook;
			bUpdated = true;
			break;
		}
	}

	// add as a new book
	if (!bUpdated)
	{
		rbook.m_id = id;
		m_books.Add(rbook);
	}

	return 0;
}

CBook* CLibrary::FindBook(IN int id)
{
	CBook* pBook = NULL;
	INT_PTR i, cnt = m_books.GetSize();
	for (i = 0; i < cnt; i++)
	{
		CBook& book = m_books.GetAt(i);
		if (book.m_id == id)
		{
			pBook = &book;
			break;
		}
	}

	return pBook;
}

int CLibrary::RemoveBook(IN int id)
{
	INT_PTR i, cnt = m_books.GetSize();
	for (i = 0; i < cnt; i++)
	{
		CBook& book = m_books.GetAt(i);
		if (book.m_id == id)
		{
			m_books.RemoveAt(i);
			break;
		}
	}

	return 0;
}

LibraryBooks& CLibrary::GetBooks()
{
	return m_books;
}

