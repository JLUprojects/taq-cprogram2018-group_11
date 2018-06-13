// StaticChart.cpp : implementation file
//

#include "stdafx.h"
#include "ReadingRecord.h"
#include "StaticChart.h"


// CStaticChart

IMPLEMENT_DYNAMIC(CStaticChart, CWnd)

CStaticChart::CStaticChart()
{

}

CStaticChart::~CStaticChart()
{
}


BEGIN_MESSAGE_MAP(CStaticChart, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CStaticChart message handlers
BOOL CStaticChart::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;// CWnd::OnEraseBkgnd(pDC);
}


void CStaticChart::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if (!m_bmChart.GetSafeHandle())
	{
		InitChart();
	}
	if (m_bmDC.GetSafeHdc())
	{
		dc.BitBlt(0, 0, m_sizeClient.cx, m_sizeClient.cy, &m_bmDC, 0, 0, SRCCOPY);
	}
}

int CStaticChart::InitChart()
{
	int rc = 0;

	do
	{
		if (m_bmChart.GetSafeHandle())
		{
			break;
		}

		CClientDC dc(this);
		RECT rcClient;
		GetClientRect(&rcClient);
		m_sizeClient.cx = rcClient.right - rcClient.left;
		m_sizeClient.cy = rcClient.bottom - rcClient.top;

		m_bmChart.CreateCompatibleBitmap(&dc, m_sizeClient.cx, m_sizeClient.cy);
		m_bmDC.CreateCompatibleDC(&dc);
		m_bmDC.SelectObject(&m_bmChart);

		CleanChart();
	} while (0);

	return rc;
}

int CStaticChart::CleanChart()
{
	int rc = 0;

	do
	{
		if (!m_bmChart.GetSafeHandle())
		{
			break;
		}
		CBrush br;
		br.CreateSolidBrush(RGB(0xff, 0xff, 0xff));
		CBrush* pOldBr = (CBrush*)m_bmDC.SelectObject(&br);
		m_bmDC.Rectangle(0, 0, m_sizeClient.cx, m_sizeClient.cy);
		m_bmDC.SelectObject(pOldBr);
	} while (0);

	return rc;
}

void CStaticChart::DrawChart(IN ChartEntryArray& rChartEntryArray, IN int nTotalEntry)
{
	do
	{
		CleanChart();
		INT_PTR nEntryCount = rChartEntryArray.GetSize();
		if (nEntryCount <=0
			|| nTotalEntry < nEntryCount)
		{
			break;
		}

		InitChart();
		double nItemWidth = (double)m_sizeClient.cx / (double)(nTotalEntry * 3 + 4);

		// find min, max
		CHART_ENTRY entry = rChartEntryArray.GetAt(0);
		int i;
		double fMin = entry.fValue;
		double fMax = entry.fValue;
		for (i = 1; i < nEntryCount; i++)
		{
			entry = rChartEntryArray.GetAt(i);
			if (entry.fValue < fMin) fMin = entry.fValue;
			if (entry.fValue > fMax) fMax = entry.fValue;
		}
		if (fMin > 0) fMin = 0.0;
		double fScaleY = 1.0;
		if (nEntryCount <= 1)
		{
			fScaleY = m_sizeClient.cy / fMax*0.9;
		}
		else
		{
			fScaleY = m_sizeClient.cy / (fMax - fMin)*0.9;
		}

		CBrush br,*pOldBr;
		CPen pen, *pOldPen;
		int x, y;
		for (i = 0; i < nEntryCount; i++)
		{
			entry = rChartEntryArray.GetAt(i);
			br.CreateSolidBrush(entry.crColor);
			pen.CreatePen(PS_SOLID, 1, entry.crColor);
			pOldBr = m_bmDC.SelectObject(&br);
			pOldPen = m_bmDC.SelectObject(&pen);
			x = (int)(entry.iX*nItemWidth * 3 + nItemWidth);
			y = m_sizeClient.cy - (int)(entry.fValue*fScaleY);
			m_bmDC.Rectangle(x, y, (int)(x + nItemWidth * 2), m_sizeClient.cy - 1);

			m_bmDC.SelectObject(pOldPen);
			pen.DeleteObject();
			m_bmDC.SelectObject(pOldBr);
			br.DeleteObject();

			TRACE(_T("Bar: (x,y)-(x,y): (%d,%d)-(%d,%d)\n"), x, y, x + nItemWidth * 2, m_sizeClient.cy - 1);
		}
	} while (0);

	Invalidate();
}