#pragma once
#include "afxwin.h"
#include "Library.h"
#include "StaticChart.h"

// CStatisticsDlg dialog

class CStatisticsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStatisticsDlg)

public:
	CStatisticsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStatisticsDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_STATISTICS };
	CLibrary* m_pLibrary;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeBooks();
	afx_msg void OnCbnSelchangePeriod();

	DECLARE_MESSAGE_MAP()

	void LoadBooks();
	void SetPeriod();
	void DrawStatisticChart();
public:
	CComboBox m_comboBooks;
	CComboBox m_comboPeriod;
	CStatic m_staticBook;
	CStatic m_staticPages;
	CStaticChart m_staticCHART;
};
