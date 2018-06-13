#pragma once
#include "Library.h"
#include "afxwin.h"

// CRecordDlg dialog

class CRecordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRecordDlg)

public:
	CRecordDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRecordDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_RECORD };
	CLibrary* m_pLibrary;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboBooks();
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()

protected:
	void LoadBooks();

	CComboBox m_books;
	int m_iStartPage;
	int m_iEndPage;
	BOOL m_bFinished;
};
