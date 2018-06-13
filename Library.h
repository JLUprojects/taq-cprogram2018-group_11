#pragma once

#include "Book.h"

typedef CArray<CBook> LibraryBooks;
class CLibrary
{
public:
	CLibrary(void);
	~CLibrary(void);

public:
	int Save();
	int Load();

	int AddBook(IN CBook& rbook);
	CBook* FindBook(IN int id);
	int RemoveBook(IN int id);

	LibraryBooks& GetBooks();

protected:
	LibraryBooks m_books;
	int GetBookFolder(OUT CString& rstrFolder);
};

