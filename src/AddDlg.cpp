// AddDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ReadingRecord.h"
#include "AddDlg.h"
#include "afxdialogex.h"

#include "SelectBookDlg.h"
#include "SearchBookWeb.h"
// CAddDlg dialog

IMPLEMENT_DYNAMIC(CAddDlg, CDialogEx)

CAddDlg::CAddDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddDlg::IDD, pParent)
	, m_strName(_T(""))
	, m_strAuthor(_T(""))
	, m_strPublisher(_T(""))
	, m_fPrice(0.0)
	, m_strAbstract(_T(""))
	, m_strPublishDate(_T(""))
{
	m_bAdded = false;
}

CAddDlg::~CAddDlg()
{
}

void CAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_STATUS, m_comboStatus);
	DDX_Text(pDX, IDC_EDIT1, m_strName);
	DDV_MaxChars(pDX, m_strName, 250);
	DDX_Text(pDX, IDC_EDIT_AUTHOR, m_strAuthor);
	DDX_Text(pDX, IDC_EDIT_PUBLISHER, m_strPublisher);
	DDX_Text(pDX, IDC_EDIT_PRICE, m_fPrice);
	DDV_MinMaxDouble(pDX, m_fPrice, 0.0, 10000.0);
	DDX_Text(pDX, IDC_EDIT_ABSTRACT, m_strAbstract);
	DDX_Text(pDX, IDC_EDIT_PUBLISH_DATE, m_strPublishDate);
	DDV_MaxChars(pDX, m_strPublishDate, 16);
}


BEGIN_MESSAGE_MAP(CAddDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CAddDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CAddDlg::OnBnClickedButtonSearch)
END_MESSAGE_MAP()


// CAddDlg message handlers


BOOL CAddDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_comboStatus.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CAddDlg::OnBnClickedButtonSearch()
{
	do
	{
		if (!UpdateData())
		{
			break;
		}

		m_strName = m_strName.Trim();
		if (_T("") == m_strName)
		{
			MessageBox(_T("Please input book name."), _T("Book Name"), MB_OK | MB_ICONERROR);
			break;
		}

		LibraryBooks books;
		SearchBookByKey(books, m_strName);
		if (books.GetSize() <= 0)
		{
			MessageBox(_T("Don't find book.\r\nPlease input book name again.\r\nInput book information manually."),
				_T("Book Name"), MB_OK | MB_ICONERROR);
			break;
		}

		CBook book;
		CSelectBookDlg dlg;
		dlg.m_pBooks = &books;
		if (IDOK != dlg.DoModal())
		{
			break;
		}

		book = dlg.m_SelectedBook;

		SearchBookByURI(book);
		ShowBook(book);
	} while (0);
}

void CAddDlg::OnBnClickedButtonAdd()
{
	if (!UpdateData(TRUE))
	{
		return;
	}

	if (_T("") == m_strName)
	{
		MessageBox(_T("Please input book name."), _T("Book Name"), MB_OK | MB_ICONERROR);
		return;
	}

	CBook book;
	ReadBook(book);
	m_pLibrary->AddBook(book);
	m_bAdded = true;

	// clean up all items for adding next book
	m_strName = _T("");
	m_strAuthor = _T("");
	m_strPublisher = _T("");
	m_strPublishDate = _T("");
	m_fPrice = 0.0;
	m_strAbstract = _T("");

	UpdateData(FALSE);
}

void CAddDlg::ShowBook(CBook& rBook)
{
	m_strName = rBook.m_strName;
	m_strAuthor = rBook.m_strAuthor;
	m_strPublisher = rBook.m_strPublisher;
	m_fPrice = rBook.m_fPrice;
	m_strPublishDate = rBook.m_strPublishDate;
	m_strAbstract = rBook.m_strAbstract;

	UpdateData(FALSE);
}

void CAddDlg::ReadBook(CBook& rBook)
{
	int index = m_comboStatus.GetCurSel();
	m_comboStatus.GetLBText(index, m_strStatus);

	rBook.m_strName = m_strName;
	rBook.m_status = (BOOK_READ_STATUS)index;
	rBook.m_strAuthor = m_strAuthor;
	rBook.m_strPublisher = m_strPublisher;
	rBook.m_fPrice = m_fPrice;
	rBook.m_strPublishDate = m_strPublishDate;
	rBook.m_strAbstract = m_strAbstract;
}

