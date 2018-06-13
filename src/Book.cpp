#include "stdafx.h"
#include <stdio.h>
#include "Book.h"
#include "FilePersist.h"

CBook::CBook(void)
{
	m_bModified = true;
}

CBook::CBook(CBook& src)
{
	m_id = src.m_id;
	m_strURI = src.m_strURI;
	m_strName = src.m_strName;
	m_strAuthor = src.m_strAuthor;
	m_strAbstract = src.m_strAbstract;
	m_strPublisher = src.m_strPublisher;
	m_strPublishDate = src.m_strPublishDate;
	m_fPrice = src.m_fPrice;
	m_status = src.m_status;
	m_records.Copy(src.m_records);

	m_bModified = src.m_bModified;
}

CBook::~CBook(void)
{
}

CBook& CBook::operator =(const CBook& src)
{
	m_id = src.m_id;
	m_strURI = src.m_strURI;
	m_strName = src.m_strName;
	m_strAuthor = src.m_strAuthor;
	m_strAbstract = src.m_strAbstract;
	m_strPublisher = src.m_strPublisher;
	m_strPublishDate = src.m_strPublishDate;
	m_fPrice = src.m_fPrice;
	m_status = src.m_status;
	m_records.Copy(src.m_records);

	m_bModified = src.m_bModified;

	return *this;
}

int CBook::SetStatus(IN BOOK_READ_STATUS nNewStatus)
{
	if (nNewStatus != m_status)
	{
		m_status = nNewStatus;
		m_bModified = true;
	}

	return 0;
}

int CBook::AddRecord(IN CReadRecord& rRecord)
{
	INT_PTR i, count = m_records.GetSize();
	for (i = 0; i < count; i++)
	{
		CReadRecord& rr = m_records.GetAt(i);
		if (rr.m_strDate == rRecord.m_strDate)
		{
			if (rr.m_iStartPage != rRecord.m_iStartPage
				|| rr.m_iEndPage != rRecord.m_iEndPage)
			{
				rr.m_iStartPage = rRecord.m_iStartPage;
				rr.m_iEndPage = rRecord.m_iEndPage;

				m_bModified = true;
			}
			break;
		}
	}

	if (i >= count)
	{
		m_records.Add(rRecord);

		m_bModified = true;
	}

	return 0;
}

int CBook::Save(IN CString& rstrFolder)
{
	int rc = 0;
	FILE* fp = NULL;

	do
	{
		if (!m_bModified)
		{
			break;
		}

		CString strFileName;
		strFileName.Format(_T("%s\\Book_%d.txt"), LPCTSTR(rstrFolder), m_id);
		errno_t et = _tfopen_s(&fp, LPCTSTR(strFileName), _T("wt,ccs=UTF-16LE"));
		if (0 != et || NULL == fp)
		{
			rc = 1;
			break;
		}

		CFilePersist::SaveItem(fp, _T("Id"), m_id);
		CFilePersist::SaveItem(fp, _T("Name"), m_strName);
		CFilePersist::SaveItem(fp, _T("Author"), m_strAuthor);
		CFilePersist::SaveItem(fp, _T("Publisher"), m_strPublisher);
		CFilePersist::SaveItem(fp, _T("Publish Date"), m_strPublishDate);
		CFilePersist::SaveItem(fp, _T("Price"), m_fPrice);
		CFilePersist::SaveItem(fp, _T("Status"), m_status);
		CFilePersist::SaveItem(fp, _T("Abstract"), m_strAbstract);
		_ftprintf_s(fp, _T("\n"));

		INT_PTR i, count = m_records.GetSize();
		for (i = 0; i < count; i++)
		{
			CReadRecord& rr = m_records.GetAt(i);

			_ftprintf_s(fp, _T("[Record]\n"));
			rr.Save(fp);
			_ftprintf_s(fp, _T("\n"));
		}

		m_bModified = false;
	} while (0);

	if (NULL != fp)
	{
		fclose(fp);
	}

	return rc;
}

int CBook::Load(IN CString& rstrFileName)
{
	int rc = 0;
	FILE* fp = NULL;

	do
	{
		m_bModified = false;
		errno_t et = _tfopen_s(&fp, LPCTSTR(rstrFileName), _T("rt,ccs=UTF-16LE"));
		if (0 != et || NULL == fp)
		{
			rc = 1;
			break;
		}

		if( CFilePersist::LoadItem(fp, _T("Id"), m_id) < 0 ) {rc=-11;break;}
		if (CFilePersist::LoadItem(fp, _T("Name"), m_strName) < 0) { rc = -12; break; }
		if (CFilePersist::LoadItem(fp, _T("Author"), m_strAuthor) < 0) { rc = -13; break; }
		if (CFilePersist::LoadItem(fp, _T("Publisher"), m_strPublisher) < 0) { rc = -14; break; }
		if (CFilePersist::LoadItem(fp, _T("Publish Date"), m_strPublishDate) < 0) { rc = -15; break; }
		if (CFilePersist::LoadItem(fp, _T("Price"), m_fPrice) < 0) { rc = -16; break; }
		int nStatus = 0;
		if (CFilePersist::LoadItem(fp, _T("Status"), nStatus) < 0) { rc = -17; break; }
		m_status = (BOOK_READ_STATUS)nStatus;
		if (CFilePersist::LoadItemLines(fp, _T("Abstract"), m_strAbstract) < 0) { rc = -18; break; }

		m_records.RemoveAll();
		TCHAR szLine[_MAX_LINE] = _T("");
		while (NULL != _fgetts(szLine, _MAX_LINE, fp))
		{
			TrimLine(szLine);
			if (_T('\0') == szLine[0])
			{
				continue;
			}
			if (0 != _tcscmp(_T("[Record]"), szLine))
			{
				rc = -21;
				break;
			}
			CReadRecord rr;
			rc = rr.Load(fp);
			if (rc < 0)
			{
				rc = -22;
				break;
			}
			m_records.Add(rr);
			if (NULL == _fgetts(szLine, _MAX_LINE, fp))
			{
				break;
			}
		}
	} while (0);

	if (NULL != fp)
	{
		fclose(fp);
	}

	return rc;
}

BookRecords& CBook::GetRecords()
{
	return m_records;
}