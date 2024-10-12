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

	Navigate(_T("https://naver.com"));
}

void WebDlg::Navigate(CString strURL)
{
	COleVariant vtEmpty;
	m_WebBrowser.Navigate(strURL, &vtEmpty, &vtEmpty, &vtEmpty, &vtEmpty);
}

BEGIN_EVENTSINK_MAP(WebDlg, CDialogEx)
	ON_EVENT(WebDlg, IDC_EXPLORER1, 259, WebDlg::DocumentCompleteExplorer1, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()

LRESULT WebDlg::OnDocumentCompleteScroll(WPARAM wParam, LPARAM lParam)
{
	m_WebBrowser.MoveWindow(5, 5, 500, 350);
	m_WebBrowser.HideScrollRect();
	return 0;
}

void WebDlg::DocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT*)
{
	CComPtr<IUnknown> spUnknownWB;
	CComPtr<IUnknown> spUnknownDC;

	if (FAILED(m_WebBrowser.GetControlUnknown()->QueryInterface(&spUnknownWB)))
		return;

	if (FAILED(pDisp->QueryInterface(&spUnknownDC)))
		return;

	if (spUnknownWB == spUnknownDC)
	{
		CComPtr<IWebBrowser2> spWebBrowser;
		if (SUCCEEDED(pDisp->QueryInterface(&spWebBrowser)))
		{
			spWebBrowser->put_Silent(TRUE);
		}

		SendMessage(WM_WEB_DOCUMENTCOMPLETE, 0, 0);
	}
}