#pragma once
#include <stdio.h>

class CReadRecord
{
public:
	CReadRecord(void);
	CReadRecord(CReadRecord& src);
	~CReadRecord(void);
	CReadRecord& operator =(const CReadRecord& src);

public:
	int Save(IN FILE* fp);
	int Load(IN FILE* fp);

public:
	CString m_strDate;
	int m_iStartPage;
	int m_iEndPage;
};
