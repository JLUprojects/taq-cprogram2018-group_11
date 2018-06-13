
// ReadingRecordDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "Library.h"

// CReadingRecordDlg dialog
class CReadingRecordDlg : public CDialogEx
{
// Construction
public:
	CReadingRecordDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_READINGRECORD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CLibrary m_library;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonRecord();
	afx_msg void OnBnClickedButtonLibrary();
	afx_msg void OnBnClickedButtonStatistics();
	CStatic m_staticLog;
};
