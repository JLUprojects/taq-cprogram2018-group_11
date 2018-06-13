#pragma once
#include "afxwin.h"
#include "Library.h"

// CSelectBookDlg dialog

class CSelectBookDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectBookDlg)

public:
	CSelectBookDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectBookDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SELECT_BOOK };
	LibraryBooks* m_pBooks;
	CBook m_SelectedBook;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_listBoxBooks;
};
