#include "pch.h"
#include "framework.h"
#include "WebBrowserUI.h"
#include "WebBrowserUIDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWebBrowserUIDlg::CWebBrowserUIDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WEBBROWSERUI_DIALOG, pParent)
{
	m_pBackSkin = NULL;
	m_LoadedBrowserFlag = FALSE;
}

void CWebBrowserUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WEBBROWSER, m_WebBrowser);
}

BEGIN_MESSAGE_MAP(CWebBrowserUIDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

BOOL CWebBrowserUIDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CWebBrowserUIDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags & MK_LBUTTON)
	{
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
		SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, TRUE, 0, 0);
	}
	else
	{
		SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, FALSE, 0, 0);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

BOOL CWebBrowserUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 0);

	if (!LoadSkin())
	{
		CDialog::OnCancel();
		return FALSE;
	}

	CDC* pDC = GetDC();
	if (pDC)
	{
		DrawSkin(pDC, TRUE);
		ReleaseDC(pDC);
	}

	InitControl();

	return TRUE;  
}

void CWebBrowserUIDlg::OnPaint()
{
	CPaintDC dc(this);
	DrawSkin(&dc);
}

void CWebBrowserUIDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	if (m_pBackSkin != NULL)
		delete m_pBackSkin;
	m_pBackSkin = NULL;
}

void CWebBrowserUIDlg::MoveLocationDialog()
{
	int cx = m_pBackSkin->m_pBitmap->GetWidth();
	int cy = m_pBackSkin->m_pBitmap->GetHeight();

	RECT rcWorkArea;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkArea, 0);
	MoveWindow(((rcWorkArea.right - cx) / 2), ((rcWorkArea.bottom - cy) / 2), cx, cy);
}

void CWebBrowserUIDlg::WebBrowserMoveClick(int x, int y, int width, int height)
{
	CString strMsg;
	strMsg.Format(_T("x:%d y:%d"), x, y);
	OutputDebugString(strMsg);
}

void CWebBrowserUIDlg::InitControl()
{
	Navigate(_T("https://naver.com"));

	CRect rc;
	GetClientRect(&rc);
	m_WebBrowser.MoveWindow(rc.left + 20, rc.top + 20, rc.Width() - 40, rc.Height() - 40);
	m_WebBrowser.HideScrollRect();
	m_WebBrowser.ShowRoundRectRgn();
}

BOOL CWebBrowserUIDlg::LoadSkin()
{
	UINT nBG_ID = IDB_PNG2;
	
	m_pBackSkin = new CGdiPlusBitmapResource;
	if (!m_pBackSkin->LoadPath(_T("")))
	{
		if (!m_pBackSkin->Load(MAKEINTRESOURCE(nBG_ID), _T("PNG"), AfxGetApp()->m_hInstance))
		{
			delete m_pBackSkin;
			m_pBackSkin = NULL;
			return FALSE;
		}
	}

	MoveLocationDialog();

	return TRUE;
}

void CWebBrowserUIDlg::DrawSkin(CDC* pDC, BOOL bFirst)
{
	if (m_pBackSkin->m_pBitmap == NULL)
		return;

	Graphics gps(pDC->GetSafeHdc());
	int x = m_pBackSkin->m_pBitmap->GetWidth();
	int y = m_pBackSkin->m_pBitmap->GetHeight();

	CRect rc;
	GetClientRect(&rc);

	Color color;
	color.SetFromCOLORREF(RGB(205, 205, 205));

	CRoundRect rr;
	rr.DrawRoundRect(&gps, Rect(rc.left + 30, rc.top + 30, rc.Width() - 19, rc.Height() - 19), color, 15, 1);
	gps.DrawImage(m_pBackSkin->m_pBitmap, Rect(0, 0, x, y), 0, 0, x, y, UnitPixel);
}

void CWebBrowserUIDlg::Navigate(CString strURL)
{
	COleVariant vtEmpty;
	m_WebBrowser.Navigate(strURL, &vtEmpty, &vtEmpty, &vtEmpty, &vtEmpty);
}

BEGIN_EVENTSINK_MAP(CWebBrowserUIDlg, CDialogEx)
ON_EVENT(CWebBrowserUIDlg, IDC_WEBBROWSER, 259, CWebBrowserUIDlg::DocumentCompleteWebbrowser, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()

CString GetHtmlSource(IHTMLDocument2* pHDoc2, BOOL bHtml)
{
	if (!pHDoc2)
		return _T("");

	CString strResult;
	CComPtr<IHTMLElementCollection> pAllColl;

	if (SUCCEEDED(pHDoc2->get_all(&pAllColl)))
	{
		LONG length = 0;
		if (SUCCEEDED(pAllColl->get_length(&length)))
		{
			for (int i = 0; i < length; i++)
			{
				VARIANT vIndex = { VT_I4 };
				vIndex.lVal = i;
				CComPtr<IDispatch> pDisp;

				if (SUCCEEDED(pAllColl->item(vIndex, vIndex, &pDisp)))
				{
					CComPtr<IHTMLElement> pElement;
					if (SUCCEEDED(pDisp->QueryInterface(IID_IHTMLElement, (void**)&pElement)))
					{
						CComBSTR tagName;
						if (SUCCEEDED(pElement->get_tagName(&tagName)))
						{
							CString strTag(tagName);

							if ((bHtml && lstrcmpi(strTag, _T("HTML")) == 0) ||	(!bHtml && lstrcmpi(strTag, _T("HTML")) != 0))
							{
								CComBSTR pContent;
								if (SUCCEEDED(pElement->get_outerHTML(&pContent)))
									strResult += pContent;
							}
						}
					}
				}
			}
		}
	}
	return strResult;
}

void CWebBrowserUIDlg::DocumentCompleteWebbrowser(LPDISPATCH pDisp, VARIANT* URL)
{
	LPUNKNOWN lpUnknown = m_WebBrowser.GetControlUnknown();
	if (!lpUnknown) return;

	CComPtr<IUnknown> lpUnknownWB, lpUnknownDC;
	if (FAILED(lpUnknown->QueryInterface(IID_IUnknown, reinterpret_cast<void**>(&lpUnknownWB))) ||
		FAILED(pDisp->QueryInterface(IID_IUnknown, reinterpret_cast<void**>(&lpUnknownDC)))) {
		return;
	}

	if (lpUnknownWB == lpUnknownDC) {
		CComPtr<IWebBrowser2> thisBrowser;
		if (SUCCEEDED(pDisp->QueryInterface(IID_IWebBrowser2, reinterpret_cast<void**>(&thisBrowser)))) {
			// 자바스크립트 오류 무시
			thisBrowser->put_Silent(TRUE);

			if (m_LoadedBrowserFlag) {
				SendMessage(WM_WEB_DOCUMENTCOMPLETE, 0, 0);
				return;
			}

			m_LoadedBrowserFlag = TRUE;

			CComPtr<IHTMLDocument2> pDoc = (IHTMLDocument2*)m_WebBrowser.get_Document();
			CString szContent = GetHtmlSource(pDoc, FALSE);
			OutputDebugString(szContent);
		}
	}

}
