// LibraryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ReadingRecord.h"
#include "LibraryDlg.h"
#include "afxdialogex.h"

#include "AddDlg.h"

// CLibraryDlg dialog

IMPLEMENT_DYNAMIC(CLibraryDlg, CDialogEx)

CLibraryDlg::CLibraryDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLibraryDlg::IDD, pParent)
{

}

CLibraryDlg::~CLibraryDlg()
{
}

void CLibraryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BOOKS, m_listCtrlBook);
}


BEGIN_MESSAGE_MAP(CLibraryDlg, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CLibraryDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_ADDBOOK, &CLibraryDlg::OnBnClickedButtonAddbook)
END_MESSAGE_MAP()


// CLibraryDlg message handlers


void CLibraryDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


BOOL CLibraryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_listCtrlBook.SetExtendedStyle
		(m_listCtrlBook.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_listCtrlBook.InsertColumn(0, _T("Order"),LVCFMT_LEFT,64);
	m_listCtrlBook.InsertColumn(1, _T("Name"),LVCFMT_LEFT,160);
	m_listCtrlBook.InsertColumn(2, _T("Start Date"),LVCFMT_LEFT,100);
	m_listCtrlBook.InsertColumn(3, _T("Status"),LVCFMT_LEFT,100);

	ShowBooks();

	return TRUE;  // return TRUE unless you set the focus to a control
}


void CLibraryDlg::OnBnClickedButtonAddbook()
{
	CAddDlg Dlg;

	Dlg.m_pLibrary = m_pLibrary;
	Dlg.DoModal();

	if (Dlg.m_bAdded)
	{
		m_pLibrary->Save();
		ShowBooks();
	}
}

void CLibraryDlg::ShowBooks()
{
	m_listCtrlBook.DeleteAllItems();
	LibraryBooks& books = m_pLibrary->GetBooks();
	INT_PTR i, cnt = books.GetSize();
	int index = 0;
	CString strItem;
	LPCTSTR pszStatus[] = { _T("New"), _T("Reading"), _T("Finished") };
	for (i = 0; i < cnt; i++)
	{
		CBook& book = books.GetAt(i);
		BookRecords& records = book.GetRecords();

		strItem.Format(_T("%d"), i + 1);
		index = m_listCtrlBook.InsertItem(i, LPCTSTR(strItem));
		m_listCtrlBook.SetItemText(index, 1, LPCTSTR(book.m_strName));
		if (records.GetSize() > 0)
		{
			const CReadRecord& rr = records.GetAt(0);
			m_listCtrlBook.SetItemText(index, 2, LPCTSTR(rr.m_strDate));
		}
		m_listCtrlBook.SetItemText(index, 3, pszStatus[book.m_status]);
	}
}