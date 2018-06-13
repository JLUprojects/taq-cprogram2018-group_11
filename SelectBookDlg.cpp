// SelectBookDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ReadingRecord.h"
#include "SelectBookDlg.h"
#include "afxdialogex.h"


// CSelectBookDlg dialog

IMPLEMENT_DYNAMIC(CSelectBookDlg, CDialogEx)

CSelectBookDlg::CSelectBookDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectBookDlg::IDD, pParent)
{

}

CSelectBookDlg::~CSelectBookDlg()
{
}

void CSelectBookDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BOOKS, m_listBoxBooks);
}


BEGIN_MESSAGE_MAP(CSelectBookDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSelectBookDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSelectBookDlg message handlers


BOOL CSelectBookDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	INT_PTR i, books = m_pBooks->GetSize();
	int idx;
	for (i = 0; i < books; i++)
	{
		CBook& book = m_pBooks->GetAt(i);
		CString strBook;
		strBook.Format(_T("%s | %s | %s"), LPCTSTR(book.m_strName), LPCTSTR(book.m_strAuthor), LPCTSTR(book.m_strPublisher));
		idx = m_listBoxBooks.AddString(strBook);
		m_listBoxBooks.SetItemData(idx, i);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CSelectBookDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	int idx = m_listBoxBooks.GetCurSel();
	if (-1 == idx)
	{
		AfxMessageBox(_T("Please select a book."));
		return;
	}
	int i = m_listBoxBooks.GetItemData(idx);
	m_SelectedBook = m_pBooks->GetAt(i);

	CDialogEx::OnOK();
}
