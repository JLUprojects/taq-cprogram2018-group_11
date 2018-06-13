#include "stdafx.h"
#include "ReadRecord.h"
#include "FilePersist.h"

CReadRecord::CReadRecord(void)
{
	m_strDate = _T("2018-5-18");
	m_iStartPage = 0;
	m_iEndPage = 0;
}

CReadRecord::CReadRecord(CReadRecord& src)
{
	m_strDate = src.m_strDate;
	m_iStartPage = src.m_iStartPage;
	m_iEndPage = src.m_iEndPage;
}

CReadRecord::~CReadRecord(void)
{
}

CReadRecord& CReadRecord::operator = (const CReadRecord& src)
{
	m_strDate = src.m_strDate;
	m_iStartPage = src.m_iStartPage;
	m_iEndPage = src.m_iEndPage;

	return *this;
}

int CReadRecord::Save(IN FILE* fp)
{
	CString strLine;

	CFilePersist::SaveItem(fp, _T("Date"), m_strDate);
	CFilePersist::SaveItem(fp, _T("StartPage"), m_iStartPage);
	CFilePersist::SaveItem(fp, _T("EndPage"), m_iEndPage);

	return 0;
}

int CReadRecord::Load(IN FILE* fp)
{
	int rc = 0;

	do
	{
		rc = CFilePersist::LoadItem(fp, _T("Date"), m_strDate);
		if (rc < 0)
		{
			break;
		}

		rc = CFilePersist::LoadItem(fp, _T("StartPage"), m_iStartPage);
		if (rc < 0)
		{
			break;
		}

		rc = CFilePersist::LoadItem(fp, _T("EndPage"), m_iEndPage);
		if (rc < 0)
		{
			break;
		}
	} while (0);

	return 0;
}


