#pragma once


// CStaticChart
typedef struct _tagChartEntry
{
	int iX;
	double fValue;
	COLORREF crColor;
}CHART_ENTRY, *PCHART_ENTRY;

typedef CArray<CHART_ENTRY> ChartEntryArray;

class CStaticChart : public CWnd
{
	DECLARE_DYNAMIC(CStaticChart)

public:
	CStaticChart();
	virtual ~CStaticChart();

public:
	void DrawChart(IN ChartEntryArray& rChartEntryArray, IN int nTotalEntry);

protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

protected:
	int InitChart();
	int CleanChart();

	SIZE m_sizeClient;
	CBitmap m_bmChart;
	CDC m_bmDC;
};


