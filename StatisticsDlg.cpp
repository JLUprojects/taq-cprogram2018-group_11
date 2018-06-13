// StatisticsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ReadingRecord.h"
#include "StatisticsDlg.h"
#include "afxdialogex.h"


// CStatisticsDlg dialog

IMPLEMENT_DYNAMIC(CStatisticsDlg, CDialogEx)

CStatisticsDlg::CStatisticsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStatisticsDlg::IDD, pParent)
{

}

CStatisticsDlg::~CStatisticsDlg()
{
}

void CStatisticsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BOOKS, m_comboBooks);
	DDX_Control(pDX, IDC_COMBO_PERIOD, m_comboPeriod);
	DDX_Control(pDX, IDC_STATIC_BEST_BOOK, m_staticBook);
	DDX_Control(pDX, IDC_STATIC_BEST_PAGE, m_staticPages);
	DDX_Control(pDX, IDC_STATIC_CHART, m_staticCHART);
}


BEGIN_MESSAGE_MAP(CStatisticsDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_BOOKS, &CStatisticsDlg::OnCbnSelchangeBooks)
	ON_CBN_SELCHANGE(IDC_COMBO_PERIOD, &CStatisticsDlg::OnCbnSelchangePeriod)
END_MESSAGE_MAP()


// CStatisticsDlg message handlers


BOOL CStatisticsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	LoadBooks();
	SetPeriod();
	DrawStatisticChart();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CStatisticsDlg::LoadBooks()
{
	LibraryBooks& books = m_pLibrary->GetBooks();
	INT_PTR i, cnt = books.GetSize();
	int index;

	m_comboBooks.ResetContent();

	index = m_comboBooks.AddString(_T("All Books"));
	m_comboBooks.SetItemData(index, -1);

	for (i = 0; i < cnt; i++)
	{
		CBook& book = books.GetAt(i);
		index = m_comboBooks.AddString(LPCTSTR(book.m_strName));
		m_comboBooks.SetItemData(index, book.m_id);
	}
	if (cnt > 0)
	{
		m_comboBooks.SetCurSel(0);
		OnCbnSelchangeBooks();
	}
}

static LPCTSTR g_pszPeriod[] = {
	_T("the last week"), _T("the last two weeks"),
	_T("the last month"), _T("the last two months"),
	_T("the 4 month"), _T("the last half year"),
	_T("the last year")
};
static const int g_nPeriodDays[] = { 7, 14, 31, 61, 122, 183, 365 };
static const int g_nPeriodBars[] = { 7, 14, 31, 61, 61,  61,  52 };
static const int g_nBarDays[] = { 1, 1, 1, 1, 2, 3, 7};
const int g_cnt = _countof(g_pszPeriod);

void CStatisticsDlg::SetPeriod()
{
	int i, index;

	m_comboPeriod.ResetContent();

	for (i = 0; i < g_cnt; i++)
	{
		index = m_comboPeriod.AddString(g_pszPeriod[i]);
		m_comboPeriod.SetItemData(index, i);
	}
	m_comboPeriod.SetCurSel(2);
}

void CStatisticsDlg::OnCbnSelchangeBooks()
{
	DrawStatisticChart();
}


void CStatisticsDlg::OnCbnSelchangePeriod()
{
	DrawStatisticChart();
}

void CStr2Date(const CString& strDate, COleDateTime& odtTime)
{
	LPCTSTR pszDate = LPCTSTR(strDate);
	LPTSTR pszMonth = NULL;
	LPTSTR pszDay = NULL;
	int year = _tcstol(pszDate, &pszMonth, 10);
	int month = _tcstol(pszMonth+1, &pszDay, 10);
	int day = _tcstol(pszDay+1, &pszMonth, 10);

	odtTime = COleDateTime(year, month, day,0,0,0);
}

void GetToday(COleDateTime& odtToday)
{
	COleDateTime odtNow = COleDateTime::GetCurrentTime();
	odtToday = COleDateTime(odtNow.GetYear(), odtNow.GetMonth(), odtNow.GetDay(), 0, 0, 0);
}

int GetSpanDays(const CString& strDate, const COleDateTime& odtToday)
{
	COleDateTime odtTime;
	CStr2Date(strDate, odtTime);
	COleDateTimeSpan ts = odtToday - odtTime;
	int nDays = (int)ts.GetDays();

	return nDays;
}

void UpdateReadRecord(CBook* pBook, const CString& rstrDate, int nPages)
{
	INT_PTR i, records;
	BookRecords& brs = pBook->GetRecords();
	records = brs.GetSize();
	for (i = 0; i < records; i++)
	{
		CReadRecord& rr = brs.GetAt(i);
		if (rr.m_strDate == rstrDate)
		{
			rr.m_iEndPage += nPages;
			break;
		}
	}
	if (i >= records)
	{
		CReadRecord rr;
		rr.m_iStartPage = 0;
		rr.m_iEndPage = nPages;
		rr.m_strDate = rstrDate;
		pBook->AddRecord(rr);
	}
}

void GenerateFakeBookRecords(CBook* pBook, CLibrary* pLibrary, int nPeriodDays, const COleDateTime& odtToday)
{
	INT_PTR i, j, books, records;
	int nSpanDays, nMaxPage = 0, nPage = 0;
	LibraryBooks& bookArray = pLibrary->GetBooks();
	books = bookArray.GetSize();

	for (j = 0; j < books; j++)
	{
		CBook& book = bookArray.GetAt(j);
		BookRecords& brs = book.GetRecords();
		records = brs.GetSize();
		for (i = 0; i < records; i++)
		{
			const CReadRecord& rr = brs.GetAt(i);
			nSpanDays = GetSpanDays(rr.m_strDate, odtToday);
			if (nSpanDays > nPeriodDays)
			{
				continue;
			}
			nPage = rr.m_iEndPage - rr.m_iStartPage + 1;
			UpdateReadRecord(pBook, rr.m_strDate, nPage);

			if (nPage > nMaxPage)
			{
				pBook->m_strName = book.m_strName;
				nMaxPage = nPage;
			}
		}
	}
}

void UpdateChartEntryArray(ChartEntryArray& rcea, int iX, double& rfValue)
{
	INT_PTR i, entryCount = rcea.GetSize();
	for (i = 0; i < entryCount; i++)
	{
		CHART_ENTRY& entry = rcea.GetAt(i);
		if (entry.iX == iX)
		{
			entry.fValue += rfValue;
			rfValue = entry.fValue;
			break;
		}
	}
	if (i >= entryCount)
	{
		CHART_ENTRY entry;
		entry.iX = iX;
		entry.crColor = RGB(0, 0xff, 0);
		entry.fValue = rfValue;
		rcea.Add(entry);
	}
}

void CStatisticsDlg::DrawStatisticChart()
{
	int iBook = m_comboBooks.GetCurSel();
	int iBookId = (int)m_comboBooks.GetItemData(iBook);
	int iPeriod = m_comboPeriod.GetCurSel();
	int iPeriodIdx = m_comboPeriod.GetItemData(iPeriod);
	CBook fakeBook;
	CBook* pBook = NULL;

	COleDateTime odtToday;
	GetToday(odtToday);
	int nPeriodDays = g_nPeriodDays[iPeriodIdx];
	if (-1 == iBookId)
	{// draw all data, create a fake book
		pBook = &fakeBook;
		GenerateFakeBookRecords(pBook, m_pLibrary, nPeriodDays, odtToday);
	}
	else
	{
		pBook = m_pLibrary->FindBook(iBookId);
	}

	BookRecords& brs = pBook->GetRecords();

	ChartEntryArray cea;
	INT_PTR i, recordCount = brs.GetSize();
	int nBars = g_nPeriodBars[iPeriodIdx];
	int nBarDays = g_nBarDays[iPeriodIdx];
	int nSpanDays;
	int iX = -1;
	double fValue;
	for (i = 0; i < recordCount; i++)
	{
		const CReadRecord& rr = brs.GetAt(i);
		nSpanDays = GetSpanDays(rr.m_strDate, odtToday);
		if (nSpanDays > nPeriodDays)
		{
			continue;
		}
		iX = nBars - nSpanDays / nBarDays;
		fValue = rr.m_iEndPage - rr.m_iStartPage;
		UpdateChartEntryArray(cea, iX, fValue);

	}

	double fMax = 0.0;
	int iMax = 0;
	if (cea.GetSize() > 0)
	{
		CHART_ENTRY& entry = cea.GetAt(0);
		fMax = entry.fValue;
	}

	if (cea.GetSize() > 1)
	{
		for (i = 1; i < cea.GetSize(); i++)
		{
			CHART_ENTRY& entry = cea.GetAt(i);
			if (entry.fValue > fMax)
			{
				fMax = entry.fValue;
				iMax = i;
			}
		}

		CHART_ENTRY& entryMax = cea.GetAt(iMax);
		entryMax.crColor = RGB(0xff, 0, 0);
	}

	m_staticCHART.DrawChart(cea, nBars);

	CString strPages;
	strPages.Format(_T("%.0f"), fMax);
	m_staticBook.SetWindowText(LPCTSTR(pBook->m_strName));
	m_staticPages.SetWindowText(LPCTSTR(strPages));
}

