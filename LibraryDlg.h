#pragma once
#include "afxcmn.h"
#include "Library.h"

// CLibraryDlg dialog

class CLibraryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLibraryDlg)

public:
	CLibraryDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLibraryDlg();

// Dialog Data
	CLibrary* m_pLibrary;
	CListCtrl m_listCtrlBook;
	enum { IDD = IDD_DIALOG_LIBRARY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAddbook();

	DECLARE_MESSAGE_MAP()

	void ShowBooks();
};
