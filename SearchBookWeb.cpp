#include "stdafx.h"
#include "SearchBookWeb.h"
#include "HttpClient.h"



void CleanHtmlText(IN OUT LPTSTR pszText)
{
	LPTSTR pbSrc = pszText;
	LPTSTR pbDest = pszText;
	LPTSTR pbEnd;
	int ccBack = 0;

	while (_T('\0') != *pbSrc)
	{
		if (_T('&') == *pbSrc)
		{
			pbSrc++;
			if (0 == _tcsnicmp(pbSrc, _T("quot;"), 5)) //&quot;
			{
				*pbDest++ = _T('\"');
				pbSrc += 5;
			}
			else if (0 == _tcsnicmp(pbSrc, _T("amp;"), 4)) //&amp;
			{
				*pbDest++ = _T('&');
				pbSrc += 4;
			}
			else if (0 == _tcsnicmp(pbSrc, _T("lt;"), 3)) //&lt;
			{
				*pbDest++ = _T('<');
				pbSrc += 3;
			}
			else if (0 == _tcsnicmp(pbSrc, _T("gt;"), 3)) //&gt;
			{
				*pbDest++ = _T('>');
				pbSrc += 3;
			}
			else if (0 == _tcsnicmp(pbSrc, _T("nbsp;"), 5)) //&nbsp;
			{
				*pbDest++ = _T(' ');
				pbSrc += 5;
			}
			else if (0 == _tcsnicmp(pbSrc, _T("copy;"), 5)) //&copy;
			{
				*pbDest++ = _T('©');
				pbSrc += 4;
			}
			else
			{
				pbEnd = _tcschr(pbSrc, _T(';'));
				if (NULL != pbEnd)
				{
					pbSrc = pbEnd + 1;
				}
				else
				{
					*pbDest++ = _T('&');
				}
			}
		}
		else if (_T('<') == *pbSrc)
		{
			pbEnd = NULL;
			ccBack = 0;

			pbSrc++;
			if (0 == _tcsnicmp(pbSrc, _T("br>"), 3)
				|| 0 == _tcsnicmp(pbSrc, _T("br "), 3)
				|| 0 == _tcsnicmp(pbSrc, _T("p>"), 2)
				|| 0 == _tcsnicmp(pbSrc, _T("/p>"), 3))
			{
				*pbDest++ = _T('\n');
				pbEnd = _tcschr(pbSrc, _T('>'));
				ccBack = 1;
			}
			
			if (NULL != pbEnd)
			{
				pbSrc = pbEnd + 1;
			}
			else
			{
				pbDest -= ccBack;
				*pbDest++ = _T('<');
			}
		}
		else
		{
			*pbDest++ = *pbSrc++;
		}
	}

	*pbDest = _T('\0');
}

void CleanHtmlText(IN OUT CString& str)
{
	TCHAR* pszText = (TCHAR*)LPCTSTR(str);
	CleanHtmlText(pszText);
}

TCHAR* GetHttpPageTagValue(OUT CString& rstrValue, IN TCHAR* pszResponse, IN LPCTSTR pszTag)
{
	TCHAR* pbEnd = pszResponse;

	do
	{
		rstrValue = _T("");

		TCHAR* pszValue = _tcsstr(pszResponse, pszTag);
		if (NULL == pszValue)
		{
			break;
		}
		pszValue += _tcslen(pszTag);

		pbEnd = _tcschr(pszValue, _T('\"'));
		if (NULL == pbEnd)
		{
			break;
		}
		*pbEnd = _T('\0');

		rstrValue = pszValue;
		*pbEnd = _T('\"');
	} while (0);

	return pbEnd;
}

TCHAR* GetHttpPageItemValue(OUT CString& rstrValue, IN TCHAR* pszResponse, IN LPCTSTR pszTag)
{
	TCHAR* pbEnd = pszResponse;

	do
	{
		rstrValue = _T("");

		TCHAR* pszValue = _tcsstr(pszResponse, pszTag);
		if (NULL == pszValue)
		{
			break;
		}
		pszValue += _tcslen(pszTag);

		pszValue = _tcschr(pszValue, _T('>'));
		if (NULL == pszValue)
		{
			break;
		}
		pszValue++;

		pbEnd = _tcschr(pszValue, _T('<'));
		if (NULL == pbEnd)
		{
			pbEnd = pszResponse;
			break;
		}
		*pbEnd = _T('\0');

		rstrValue = pszValue;
		*pbEnd = _T('<');
	} while (0);

	return pbEnd;
}

TCHAR* GetHttpPageElement(OUT CString& rstrValue, IN TCHAR* pszResponse, IN LPCTSTR pszTag)
{
	TCHAR* pbEnd = pszResponse;

	do
	{
		rstrValue = _T("");

		TCHAR* pszValue = _tcsstr(pszResponse, pszTag);
		if (NULL == pszValue)
		{
			break;
		}
		while (pszValue >= pszResponse && _T('<') != *pszValue)
		{
			pszValue--;
		}
		if (_T('<') != *pszValue)
		{
			break;
		}
		pszValue++;
		pbEnd = pszValue;
		while (_T('\0') != *pbEnd && _T(' ') != *pbEnd && _T('>') != *pbEnd)
		{
			pbEnd++;
		}
		if (_T('\0') == *pbEnd)
		{
			break;
		}
		TCHAR bEnd = *pbEnd;
		*pbEnd = _T('\0');
		CString strEndTag = _T("</");
		strEndTag += pszValue;
		strEndTag += _T(">");
		*pbEnd = bEnd;

		pszValue = _tcschr(pszValue, _T('>'));
		if (NULL == pszValue)
		{
			break;
		}
		pszValue++;

		pbEnd = _tcsstr(pszValue, LPCTSTR(strEndTag));
		if (NULL == pbEnd)
		{
			break;
		}
		*pbEnd = _T('\0');

		rstrValue = pszValue;
		*pbEnd = _T('<');
	} while (0);

	return pbEnd;
}

TCHAR* GetHttpLink(OUT CString& rstrHref, OUT CString& rstrShow, IN TCHAR* pszHtml)
{
	TCHAR* pbEnd = pszHtml;

	do
	{
		rstrHref = _T("");
		rstrShow = _T("");

		TCHAR* pszValue = _tcsstr(pszHtml, _T("<a "));
		if (NULL == pszValue)
		{
			break;
		}
		pszValue = _tcsstr(pszHtml, _T("href=\""));
		if (NULL == pszValue)
		{
			break;
		}
		pszValue += 6;

		pbEnd = _tcschr(pszValue, _T('\"'));
		if (NULL == pbEnd)
		{
			pbEnd = pszHtml;
			break;
		}
		*pbEnd = _T('\0');
		rstrHref = pszValue;
		*pbEnd = _T('\"');

		pszValue = _tcschr(pbEnd + 1, _T('>'));
		if (NULL == pszValue)
		{
			break;
		}
		pszValue++;

		pbEnd = _tcsstr(pszValue, _T("</a>"));
		if (NULL == pbEnd)
		{
			break;
		}
		*pbEnd = _T('\0');

		rstrShow = pszValue;
		*pbEnd = _T('<');
	} while (0);

	return pbEnd;
}

TCHAR Hex2Wchar(IN TCHAR* pbHex)
{
	TCHAR ch = _T('\0');
	do
	{
		if (_T('\0') == *pbHex
			||_T('\0') == *(pbHex+1)
			||_T('\0') == *(pbHex+2)
			||_T('\0') == *(pbHex+3))
		{
			break;
		}
		TCHAR chNext = *(pbHex + 4);
		*(pbHex + 4) = _T('\0');

		TCHAR* pbEnd = NULL;
		ch = (TCHAR)_tcstol(pbHex, &pbEnd, 16);

		*(pbHex + 4) = chNext;
	} while (0);

	return ch;
}
void CleanJsonHttpText(IN OUT TCHAR* pszJson)
{
	TCHAR* pbSrc = pszJson;
	TCHAR* pbDest = pszJson;
	while (_T('\0') != *pbSrc)
	{
		if (_T('\\') == *pbSrc)
		{
			pbSrc++;
			if (_T('\"') == *pbSrc)
			{
				*pbDest++ = *pbSrc++;
			}
			else if (_T('/') == *pbSrc)
			{
				*pbDest++ = *pbSrc++;
			}
			else if (_T('n') == *pbSrc)
			{
				*pbDest++ = _T('\n');
				*pbSrc++;
			}
			else if (_T('t') == *pbSrc)
			{
				//*pbDest++ = _T('\t');
				*pbSrc++;
			}
			else if (_T('u') == *pbSrc)
			{
				pbSrc++;
				if (_T('f') == *pbSrc && _T('f') == *(pbSrc + 1) && _T('0') == *(pbSrc+2) && _T('8') == *(pbSrc + 3))
				{
					int a = 9;
				}
				TCHAR ch = Hex2Wchar(pbSrc);
				if (_T('\0') == ch)
				{
					break;
				}
				*pbDest++ = ch;
				pbSrc += 4;
			}
			else
			{
				*pbDest++ = _T('\\');
			}
		}
		else
		{
			*pbDest++ = *pbSrc++;
		}
	}
	*pbDest = _T('\0');
}

void GetHttpText(OUT CString& rstrText, IN TCHAR* pszHtml)
{
	TCHAR* pszTag;
	rstrText = _T("");

	while (_T('\0') != *pszHtml)
	{
		pszTag = _tcschr(pszHtml, _T('<'));
		if (NULL == pszTag)
		{
			break;
		}
		*pszTag = _T('\0');
		if (pszHtml != pszTag)
		{
			rstrText += pszHtml;
		}
		*pszTag++ = _T('<');
		if (0 == _tcsncmp(pszTag, _T("/p>"), 3))
		{
			rstrText += _T("\n");
		}

		pszHtml = _tcschr(pszTag, _T('>'));
		if (NULL == pszHtml)
		{
			break;
		}
		pszHtml++;
	}
	if (NULL != pszHtml)
	{
		rstrText += pszHtml;
	}
}

int ParseDangdangSearchBookList(OUT LibraryBooks& rBooks, IN TCHAR* pszResponse)
{
	int idx = 1;
	CString strTag;
	TCHAR* pszNextItem = pszResponse;
	TCHAR* pszItem = NULL;

	while (_T('\0') != *pszNextItem)
	{
		strTag.Format(_T("<li ddt-pit=\"%d\" class=\""), idx++);
		pszItem = _tcsstr(pszNextItem, LPCTSTR(strTag));
		if (NULL == pszItem)
		{
			break;
		}
		pszNextItem = _tcsstr(pszItem, _T("</li>"));
		if (NULL == pszNextItem)
		{
			*pszNextItem = _T('\0');
		}
		CBook book;
		pszItem = GetHttpPageTagValue(book.m_strName, pszItem, _T("title=\""));
		book.m_strName = book.m_strName.Trim();
		CleanHtmlText(book.m_strName);

		pszItem = GetHttpPageTagValue(book.m_strURI, pszItem, _T("href=\""));

		CString strPrice;
		pszItem = GetHttpPageItemValue(strPrice, pszItem, _T("class=\"price_r\""));
		if (_T("") == strPrice)
		{
			pszItem = GetHttpPageItemValue(strPrice, pszItem, _T("class=\"search_pre_price\""));
		}
		LPCTSTR pb = LPCTSTR(strPrice);
		while (_T('\0') != *pb && (*pb < _T('0') || *pb > _T('9')))
		{
			pb++;
		}
		book.m_fPrice = _ttof(pb);
		
		GetHttpPageItemValue(book.m_strAuthor, pszItem, _T("dd_name=\'单品作者\'"));
		CleanHtmlText(book.m_strAuthor);

		GetHttpPageItemValue(book.m_strPublisher, pszItem, _T("dd_name=\'单品出版社\'"));
		CleanHtmlText(book.m_strPublisher);

		rBooks.Add(book);
		if (NULL == pszNextItem)
		{
			break;
		}
		*pszNextItem = _T('<');
		pszNextItem += 5;
	}
	return 0;
}

int ParseDangdangSearch(OUT LibraryBooks& rBooks, IN TCHAR* pszResponse)
{
	int rc = 0;
	int idx = 1;
	CString strTag;
	TCHAR* pszNextItem = pszResponse;
	TCHAR* pszItem = NULL;

	do
	{
		pszNextItem = _tcsstr(pszNextItem, _T("<body"));
		if (NULL == pszNextItem)
		{
			rc = -1;
			break;
		}

		rc = ParseDangdangSearchBookList(rBooks, pszNextItem);
	} while (0);

	return rc;
}

TCHAR* GetHttpPage(IN LPCTSTR pszHost, IN LPCTSTR pszURI)
{
	TCHAR* pszResponse = NULL;

	do
	{
		CHttpClient hc;
		int rc = 0;
		rc = hc.ConnectServer(pszHost);
		if (rc < 0)
		{
			break;
		}

		string strResponse;
		rc = hc.Get(pszURI, strResponse);
		if (rc < 0)
		{
			break;
		}

		size_t cchSize = strResponse.length() + 1;
		pszResponse = new TCHAR[cchSize];

		::MultiByteToWideChar(CP_ACP, 0, strResponse.c_str(), -1, pszResponse, cchSize);
	} while (0);

	return pszResponse;
}

int SearchOnDangdangKey(OUT LibraryBooks& rBooks, IN CString& rstrKey)
{
	TCHAR* pszResponse = NULL;
	
	do
	{
		CString strURI = _T("/?key=");
		string strResponse;

		strURI += rstrKey;
		strURI += "&act=input";

		pszResponse = GetHttpPage(_T("http://search.dangdang.com"), LPCTSTR(strURI));
		if (NULL == pszResponse)
		{
			break;
		}

		ParseDangdangSearch(rBooks, pszResponse);

	} while (0);
	if (NULL != pszResponse)
	{
		delete pszResponse;
	}

	return 0;
}

//var prodSpuInfo = {"productId":"24032293","mainProductId":"24032293","productName":"SRE\uff1aGoogle\u8fd0\u7ef4\u89e3\u5bc6",
//"productSubName":"\u8c37\u6b4cSRE\u6280\u672f\u6df1\u5ea6\u66dd\u5149 \u4e2d\u56fd\u8fd0\u7ef4\u9886\u519b\u4eba\u7269\u96c6
//\u4f53\u529b\u8350\u3010\u5730\u7403\u4e0a\u6709\u8fd9\u4e48\u4e00\u4e2a\u56e2\u961f\uff0c\u5c06\u8fd0\u7ef4\u63a8\u5411\u6781
//\u9650\u9ad8\u5ea6\u3011","url":"\/24032293.html","shopId":"0","isCatalog":"0","isOverSea":"0","isWireless":"0",
//"isPresent":"0","isVirtualProduct":"0","isSoldOut":"0","displayStatus":"0","categoryPath":"01.54.18.00.00.00",
//"describeMap":"","categoryId":"7417","template":"publish","productType":"0","templateId":"0","mediumId":"0",
//"supId":"2060","pathName":"\u4f01\u4e1a\u8f6f\u4ef6\u5f00\u53d1\u4e0e\u5b9e\u65bd","isGiftPackaging":"1",
//"isSupportReturnPolicy":"1","shopType":"0","energyLevel":"-1","icon":[],"ddShopId":"299",
//"isShowOriginalPrice":"1","isRecyclable":"0","isShowRobot":"1","isPod":"0","podId":"0","eBookId":""};
int GetKeyValue(IN TCHAR* pszList, IN TCHAR* pszTag, OUT CString& rValue)
{
	int rc = 0;

	do
	{
		TCHAR szTag[MAX_PATH] = _T("\"");
		_tcscat_s(szTag, _countof(szTag), pszTag);
		_tcscat_s(szTag, _countof(szTag), _T("\":"));
		TCHAR* pbTag = _tcsstr(pszList, szTag);
		if (NULL == pbTag)
		{
			rc = -1;
			break;
		}
		TCHAR* pbValue = pbTag + _tcslen(szTag);
		pbValue++; // skip double quote
		TCHAR* pbEnd = _tcschr(pbValue, _T('\"'));
		if (NULL == pbEnd)
		{
			rc = -2;
			break;
		}
		*pbEnd = _T('\0');
		rValue = pbValue;
		*pbEnd = _T('\"');
	} while (0);

	return rc;
}

//http://product.dangdang.com/index.php?r=callback%2Fdetail&productId=24032293&templateType=publish&describeMap=&shopId=0&categoryPath=01.54.18.00.00.00
//http://product.dangdang.com/index.php?r=callback%2Fdetail&productId=24032293
//&templateType=publish&describeMap=&shopId=0&categoryPath=01.54.18.00.00.00
void GetBookDetails(CBook& rBook, TCHAR* pszProdSpuInfo)
{
	TCHAR* pszResponse = NULL;

	do
	{
		CString strProdctId, strTemplateType, strDescribeMap, strShopId, strCategoryPath;
		int rc0 = GetKeyValue(pszProdSpuInfo, _T("productId"), strProdctId);
		GetKeyValue(pszProdSpuInfo, _T("template"), strTemplateType);
		GetKeyValue(pszProdSpuInfo, _T("describeMap"), strDescribeMap);
		GetKeyValue(pszProdSpuInfo, _T("shopId"), strShopId);
		int rc1 = GetKeyValue(pszProdSpuInfo, _T("categoryPath"), strCategoryPath);
		if (rc0 < 0 || rc1 < 0)
		{
			break;
		}

		CString strURI = _T("/index.php?r=callback%2Fdetail&productId=");
		strURI += strProdctId;
		strURI += _T("&templateType=") + strTemplateType;
		strURI += _T("&describeMap=") + strDescribeMap;
		strURI += _T("&shopId=") + strShopId;
		strURI += _T("&categoryPath=") + strCategoryPath;

		pszResponse = GetHttpPage(_T("http://product.dangdang.com"), LPCTSTR(strURI));
		if (NULL == pszResponse)
		{
			break;
		}
		CleanJsonHttpText(pszResponse);
		CleanHtmlText(pszResponse);
		TCHAR* pszNext = _tcsstr(pszResponse, _T("id=\"content\""));
		if (NULL == pszNext)
		{
			pszNext = _tcsstr(pszResponse, _T("id=\"abstract - all\""));
		}
		if (NULL == pszNext)
		{
			pszNext = pszResponse;
		}

		CString strElement;
		LPTSTR pszShow = _tcsstr(pszNext, _T("id=\"content-show\""));
		if (NULL != pszShow)
		{
			pszNext = pszShow;
		}
		pszNext = GetHttpPageElement(strElement, pszNext, _T("class=\"descrip\""));
		if (NULL != pszNext)
		{
			GetHttpText(rBook.m_strAbstract, (TCHAR*)LPCTSTR(strElement));
			rBook.m_strAbstract = rBook.m_strAbstract.Trim();
		}
		CleanHtmlText(rBook.m_strAbstract);

	} while (0);

	if (NULL != pszResponse)
	{
		delete pszResponse;
	}
}

int ParseDangdangSearchId(CBook& rBook, TCHAR* pszResponse)
{
	do
	{
		TCHAR* pszProdSpuInfo = _tcsstr(pszResponse, _T("prodSpuInfo"));
		if (NULL != pszProdSpuInfo)
		{
			pszProdSpuInfo =  _tcschr(pszProdSpuInfo, _T('{'));
			TCHAR* pbEnd = _tcschr(pszProdSpuInfo, _T('}'));
			if (NULL != pszProdSpuInfo && NULL != pbEnd)
			{
				pszProdSpuInfo++;
				*pbEnd = _T('\0');
				GetBookDetails(rBook, pszProdSpuInfo);
				*pbEnd = _T('}');
			}
		}

		TCHAR* pszNext = _tcsstr(pszResponse, _T("class=\"messbox_info\""));
		if (NULL == pszNext)
		{
			break;
		}
		CString strElement, strHref;
		pszNext = GetHttpPageElement(strElement, pszNext, _T("id=\"author\""));
		GetHttpLink(strHref, rBook.m_strAuthor, (TCHAR*)LPCTSTR(strElement));
		CleanHtmlText(rBook.m_strAuthor);

		pszNext = GetHttpPageElement(strElement, pszNext, _T("dd_name=\"出版社\""));
		GetHttpLink(strHref, rBook.m_strPublisher, (TCHAR*)LPCTSTR(strElement));
		CleanHtmlText(rBook.m_strPublisher);

		pszNext = GetHttpPageElement(rBook.m_strPublishDate, pszNext, _T("class=\"t1\""));
		if (0 == _tcsncmp(LPCTSTR(rBook.m_strPublishDate), _T("出版时间:"), 5))
		{
			rBook.m_strPublishDate = rBook.m_strPublishDate.Mid(5);
		}
		CleanHtmlText(rBook.m_strPublishDate);
	} while (0);

	return 0;
}
//pszURL= http://product.dangdang.com/24228101.html
int SearchOnDangdagnUrl(IN OUT CBook& rBook)
{
	TCHAR* pszResponse = NULL;

	do
	{
		CString uri = rBook.m_strURI;
		LPCTSTR pb = _tcschr(LPCTSTR(uri)+7, _T('/'));
		int idx = pb - LPCTSTR(uri);
		CString host = uri.Left(idx);
		uri = uri.Mid(idx);

		pszResponse = GetHttpPage(LPCTSTR(host), LPCTSTR(uri));
		if (NULL == pszResponse)
		{
			break;
		}

		ParseDangdangSearchId(rBook, pszResponse);

	} while (0);

	if (NULL != pszResponse)
	{
		delete pszResponse;
	}

	return 0;
}

//-----------------------------------------------------------------------------
int SearchBookByKey(OUT LibraryBooks& rBooks, IN CString& rstrKey)
{
	return SearchOnDangdangKey(rBooks, rstrKey);
}

int SearchBookByURI(IN OUT CBook& rBook)
{
	return SearchOnDangdagnUrl(rBook);
}

//http://search.dangdang.com/?key=9787508307312&act=input&noresult=1
//https://www.amazon.cn/s/ref=nb_sb_noss?__mk_zh_CN=%E4%BA%9A%E9%A9%AC%E9%80%8A%E7%BD%91%E7%AB%99&url=search-alias%3Daps&field-keywords=9787508307312
