#pragma once
#include "afxwin.h"
#include "Library.h"
#include "afxdtctl.h"
#include "ATLComTime.h"

// CAddDlg dialog

class CAddDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddDlg)

public:
	CAddDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddDlg();

// Dialog Data
	CLibrary* m_pLibrary;
	bool m_bAdded;

	CString m_strStatus;
	enum { IDD = IDD_DIALOG_ADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnBnClickedButtonAdd();

	DECLARE_MESSAGE_MAP()

	void ShowBook(CBook& rBook);
	void ReadBook(CBook& rBook);
public:
	CComboBox m_comboStatus;
	CString m_strName;
	CString m_strAuthor;
	CString m_strPublisher;
	CString m_strPublishDate;
	double m_fPrice;
	CString m_strAbstract;
};
