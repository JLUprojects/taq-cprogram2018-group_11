#pragma once
#include "ReadRecord.h"

typedef enum _tagBookReadStatus
{
	New = 0,
	Reading,
	Finished
}BOOK_READ_STATUS;
typedef CArray <CReadRecord> BookRecords;
class CBook
{
public:
	CBook(void);
	CBook(CBook& src);
	~CBook(void);
	CBook& operator =(const CBook& src);

public:
	int SetStatus(IN BOOK_READ_STATUS nNewStatus);
	int AddRecord(IN CReadRecord& rRecord);

	int Save(IN CString& rstrFolder);
	int Load(IN CString& rstrFileName);

	BookRecords& GetRecords();

public:
	int m_id;
	CString m_strURI;
	CString m_strName;
	CString m_strAuthor;
	CString m_strAbstract;
	CString m_strPublisher;
	CString m_strPublishDate;
	double m_fPrice;
	BOOK_READ_STATUS m_status;
	BookRecords m_records;

protected:
	bool m_bModified;
};
