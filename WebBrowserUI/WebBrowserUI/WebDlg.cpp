#include "pch.h"
#include "WebBrowserUI.h"
#include "WebDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(WebDlg, CDialogEx)

WebDlg::WebDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WEB_DIALOG, pParent)
{
}

WebDlg::~WebDlg()
{
}

void WebDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, m_WebBrowser);
}

BEGIN_MESSAGE_MAP(WebDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_WEB_DOCUMENTCOMPLETE, OnDocumentCompleteScroll)
END_MESSAGE_MAP()

BOOL WebDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitWebBrowser();

	return TRUE;  
}

void WebDlg::OnPaint()
{
	CPaintDC dc(this);

}

void WebDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

}

void WebDlg::InitWebBrowser()
{
	m_WebBrowser.MoveWindow(5, 5, 500, 350);

	Navigate(_T("https://github.com"));
}

void WebDlg::Navigate(CString strURL)
{
	COleVariant vtEmpty;
	m_WebBrowser.Navigate(strURL, &vtEmpty, &vtEmpty, &vtEmpty, &vtEmpty);
}

BEGIN_EVENTSINK_MAP(WebDlg, CDialogEx)
	ON_EVENT(WebDlg, IDC_EXPLORER1, 259, WebDlg::OnDocumentcompleteExplorer1, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()

void WebDlg::OnDocumentcompleteExplorer1(LPDISPATCH pDisp, VARIANT* URL)
{
	HRESULT   hr;
	LPUNKNOWN lpUnknown;
	LPUNKNOWN lpUnknownWB = NULL;
	LPUNKNOWN lpUnknownDC = NULL;
	lpUnknown = m_WebBrowser.GetControlUnknown();

	if (lpUnknown)
	{
		hr = lpUnknown->QueryInterface(IID_IUnknown, (LPVOID*)&lpUnknownWB);
		ASSERT(SUCCEEDED(hr)); if (FAILED(hr))return;

		hr = pDisp->QueryInterface(IID_IUnknown, (LPVOID*)&lpUnknownDC);
		ASSERT(SUCCEEDED(hr));

		if (SUCCEEDED(hr) && lpUnknownWB == lpUnknownDC)
		{
			IWebBrowser2* thisBrowser = NULL;
			HRESULT hr;

			hr = pDisp->QueryInterface(IID_IWebBrowser2, reinterpret_cast<void**>(&thisBrowser));

			//자바스크립트 오류 무시
			if (SUCCEEDED(hr)) {				
				thisBrowser->put_Silent(TRUE); 		
			}

			if (thisBrowser)thisBrowser->Release();
			if (thisBrowser)thisBrowser = NULL;

			if (lpUnknownWB)lpUnknownWB->Release();
			if (lpUnknownDC)lpUnknownDC->Release();

			SendMessage(WM_WEB_DOCUMENTCOMPLETE, 0, 0);
		}
	}
}

LRESULT WebDlg::OnDocumentCompleteScroll(WPARAM wParam, LPARAM lParam)
{
	m_WebBrowser.MoveWindow(5, 5, 500, 350);
	m_WebBrowser.HideScrollRect();
	return 0;
}