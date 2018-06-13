
// ReadingRecordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ReadingRecord.h"
#include "ReadingRecordDlg.h"
#include "afxdialogex.h"

#include "RecordDlg.h"
#include "LibraryDlg.h"
#include "StatisticsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CReadingRecordDlg dialog



CReadingRecordDlg::CReadingRecordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CReadingRecordDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CReadingRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_LOGO2, m_staticLog);
}

BEGIN_MESSAGE_MAP(CReadingRecordDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_RECORD, &CReadingRecordDlg::OnBnClickedButtonRecord)
	ON_BN_CLICKED(IDC_BUTTON_LIBRARY, &CReadingRecordDlg::OnBnClickedButtonLibrary)
	ON_BN_CLICKED(IDC_BUTTON_STATISTICS, &CReadingRecordDlg::OnBnClickedButtonStatistics)
	ON_BN_CLICKED(IDC_BUTTON_STATISTICS, &CReadingRecordDlg::OnBnClickedButtonStatistics)
END_MESSAGE_MAP()


// CReadingRecordDlg message handlers

BOOL CReadingRecordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	HBITMAP hBm = ::LoadBitmap(::AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_LOGO));
	m_staticLog.ModifyStyle(SS_ENHMETAFILE, SS_BITMAP);
	m_staticLog.SetBitmap(hBm);

	m_library.Load();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CReadingRecordDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CReadingRecordDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CReadingRecordDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CReadingRecordDlg::OnBnClickedButtonRecord()
{
	CRecordDlg Dlg;

	Dlg.m_pLibrary = &m_library;
	if(IDOK == Dlg.DoModal())
	{
	}
	m_library.Save();
}


void CReadingRecordDlg::OnBnClickedButtonLibrary()
{
	CLibraryDlg Dlg;

	Dlg.m_pLibrary = &m_library;
	if(IDOK == Dlg.DoModal())
	{
		//
	}
}



void CReadingRecordDlg::OnBnClickedButtonStatistics()
{
	CStatisticsDlg Dlg;
	Dlg.m_pLibrary = &m_library;

	if(IDOK == Dlg.DoModal())
	{
		//
	}
}


