// RecordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ReadingRecord.h"
#include "RecordDlg.h"
#include "afxdialogex.h"


// CRecordDlg dialog

IMPLEMENT_DYNAMIC(CRecordDlg, CDialogEx)

CRecordDlg::CRecordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRecordDlg::IDD, pParent)
	, m_iStartPage(1)
	, m_iEndPage(2)
	, m_bFinished(FALSE)
{

}

CRecordDlg::~CRecordDlg()
{
}

void CRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BOOKS, m_books);
	DDX_Text(pDX, IDC_EDIT_START_PAGE, m_iStartPage);
	DDV_MinMaxInt(pDX, m_iStartPage, 1, 100000);
	DDX_Text(pDX, IDC_EDIT_END_PAGE, m_iEndPage);
	DDV_MinMaxInt(pDX, m_iEndPage, 1, 100000);
	DDX_Check(pDX, IDC_CHECK_FINISHED, m_bFinished);
}


BEGIN_MESSAGE_MAP(CRecordDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRecordDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_BOOKS, &CRecordDlg::OnCbnSelchangeComboBooks)
END_MESSAGE_MAP()


// CRecordDlg message handlers

BOOL CRecordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	LoadBooks();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CRecordDlg::LoadBooks()
{
	LibraryBooks& books = m_pLibrary->GetBooks();
	INT_PTR i, cnt = books.GetSize();
	int index;
	m_books.ResetContent();
	for (i = 0; i < cnt; i++)
	{
		CBook& book = books.GetAt(i);
		index = m_books.AddString(LPCTSTR(book.m_strName));
		m_books.SetItemData(index, book.m_id);
	}
	if (cnt > 0)
	{
		m_books.SetCurSel(0);
		OnCbnSelchangeComboBooks();
	}
}

void CRecordDlg::OnCbnSelchangeComboBooks()
{
	if (!UpdateData())
	{
		return;
	}
		
	COleDateTime dtNow = COleDateTime::GetCurrentTime();
	CString strToday = dtNow.Format(_T("%Y-%m-%d"));

	int iSel = m_books.GetCurSel();
	int iBookId = (int)m_books.GetItemData(iSel);

	CBook* pBook = m_pLibrary->FindBook(iBookId);
	if (NULL != pBook)
	{
		BookRecords& records = pBook->GetRecords();
		INT_PTR cnt = records.GetSize();
		if (cnt > 0)
		{
			const CReadRecord& rr = records.GetAt(cnt - 1);
			if (rr.m_strDate == strToday)
			{
				m_iStartPage = rr.m_iStartPage;
				m_iEndPage = rr.m_iEndPage;
			}
			else
			{
				int lastReadPage = rr.m_iEndPage - rr.m_iStartPage;
				if (lastReadPage <= 0)
				{
					lastReadPage = 1;
				}
				m_iStartPage = rr.m_iEndPage + 1;
				m_iEndPage = m_iStartPage + lastReadPage;
			}
		}
		else
		{
			m_iStartPage = 1;
			m_iEndPage = m_iStartPage + 1;
		}
		UpdateData(FALSE);
	}
}

void CRecordDlg::OnBnClickedOk()
{
	if (!UpdateData())
	{
		return;
	}
	if (m_iEndPage < m_iStartPage)
	{
		MessageBox(_T("End page number is less than start page.\r\nPlease correct it first."), 
			_T("Page Number"), MB_OK | MB_ICONWARNING);
		return;
	}

	COleDateTime dtNow = COleDateTime::GetCurrentTime();
	int iSel = m_books.GetCurSel();
	int iBookId = (int)m_books.GetItemData(iSel);

	CBook* pBook = m_pLibrary->FindBook(iBookId);
	if (NULL != pBook)
	{
		BOOK_READ_STATUS status = m_bFinished ? BOOK_READ_STATUS::Finished : BOOK_READ_STATUS::Reading;
		pBook->SetStatus(status);

		CReadRecord rr;
		rr.m_strDate = dtNow.Format(_T("%Y-%m-%d"));
		rr.m_iStartPage = m_iStartPage;
		rr.m_iEndPage = m_iEndPage;

		pBook->AddRecord(rr);
	}

	UpdateData(FALSE);

	//CDialogEx::OnOK();
}

