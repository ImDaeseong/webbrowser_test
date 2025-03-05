#include "pch.h"
#include "framework.h"
#include "MFCApplication1WebView2.h"
#include "MFCApplication1WebView2Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMFCApplication1WebView2Dlg::CMFCApplication1WebView2Dlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_MFCAPPLICATION1WEBVIEW2_DIALOG, pParent)
{
}

void CMFCApplication1WebView2Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication1WebView2Dlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_DESTROY()
	ON_MESSAGE(WM_USER_WEBDATA1, OnMessage1)
	ON_MESSAGE(WM_USER_WEBDATA2, OnMessage2)
END_MESSAGE_MAP()

BOOL CMFCApplication1WebView2Dlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

	CoInitialize(NULL);

    initWebView();   

    return TRUE;
}

void CMFCApplication1WebView2Dlg::OnPaint()
{
    CPaintDC dc(this);
}

void CMFCApplication1WebView2Dlg::OnDestroy()
{
    CDialogEx::OnDestroy();

	CoUninitialize();
}

void CMFCApplication1WebView2Dlg::initWebView()
{
	CRect rectClient;
	GetClientRect(rectClient);

	HWND hWndParent = this->GetSafeHwnd();

	m_pWebBrowser = std::make_unique<CWebBrowser>();
	if (m_pWebBrowser != nullptr)
	{
		m_pWebBrowser->CreateAsync(
			WS_VISIBLE | WS_CHILD,
			rectClient,
			this,
			1,
			[this]() {
				CString strParam("");					
				CString content(strParam);
				CString headers(_T("Content-Type: application/x-www-form-urlencoded"));
				m_pWebBrowser->SetParentView(this);
				m_pWebBrowser->DisablePopups();
				m_pWebBrowser->NavigatePost(L"https://www.naver.com", content, headers, this->GetSafeHwnd());
				m_pWebBrowser->RegisterCallback(CWebBrowser::CallbackType::TitleChanged, [this]() {
					CString title = m_pWebBrowser->GetTitle();
					AfxGetMainWnd()->SetWindowText(title);
					});
			});
	}
}

void CMFCApplication1WebView2Dlg::NavigateWebView()
{
	if (m_pWebBrowser != nullptr)
	{
		CString strParam("");
		CString content(strParam);
		CString headers(_T("Content-Type: application/x-www-form-urlencoded"));
		m_pWebBrowser->SetParentView(this);
		m_pWebBrowser->DisablePopups();
		m_pWebBrowser->NavigatePost(L"https://www.naver.com", content, headers, this->GetSafeHwnd());
		m_pWebBrowser->RegisterCallback(CWebBrowser::CallbackType::TitleChanged, [this]() {
			CString title = m_pWebBrowser->GetTitle();
			AfxGetMainWnd()->SetWindowText(title);
			});
	}
}

LRESULT CMFCApplication1WebView2Dlg::OnMessage1(WPARAM wParam, LPARAM lParam)
{

	return 0;
}

LRESULT CMFCApplication1WebView2Dlg::OnMessage2(WPARAM wParam, LPARAM lParam)
{
	if (m_pWebBrowser != nullptr)
	{
		CRect rectClient;
		GetClientRect(rectClient);
		m_pWebBrowser->MoveWindow(10, 10, rectClient.Width() - 20, rectClient.Height() - 20);
	}

	return 0;
}