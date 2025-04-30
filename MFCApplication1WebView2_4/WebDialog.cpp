#include "pch.h"
#include "MFCApplication1WebView2.h"
#include "WebDialog.h"
#include "afxdialogex.h"
#include "MFCApplication1WebView2Dlg.h"

IMPLEMENT_DYNAMIC(WebDialog, CDialogEx)

WebDialog::WebDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WEB_DIALOG, pParent)
{
	m_pWebBrowser = nullptr;
}

WebDialog::~WebDialog()
{
}

void WebDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(WebDialog, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_USER_WEBDATA1, OnMessage1)
	ON_MESSAGE(WM_USER_WEBDATA2, OnMessage2)
END_MESSAGE_MAP()


BOOL WebDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CoInitialize(NULL);

	initWebView();

	return TRUE;
}

void WebDialog::OnPaint()
{
	CPaintDC dc(this);
}

void WebDialog::OnDestroy()
{
	CDialogEx::OnDestroy();

	if (m_pWebBrowser)
	{
		//명시적 자원 해제(CWebBrowser::~CWebBrowser 호출됨)
		m_pWebBrowser.reset(); 
	}

	CoUninitialize();
}

void WebDialog::initWebView()
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

				m_pWebBrowser->RegisterCallback(CWebBrowser::CallbackType::AcceleratorKey, [this]() {
					AfxMessageBox(_T("단추키 호출"));
					});
			});
	}
}

void WebDialog::NavigateWebView()
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
		m_pWebBrowser->RegisterCallback(CWebBrowser::CallbackType::AcceleratorKey, [this]() {
			AfxMessageBox(_T("단추키 호출"));
			});
	}
}

LRESULT WebDialog::OnMessage1(WPARAM wParam, LPARAM lParam)
{

	return 0;
}

LRESULT WebDialog::OnMessage2(WPARAM wParam, LPARAM lParam)
{
	if (m_pWebBrowser != nullptr)
	{
		CRect rectClient;
		GetClientRect(rectClient);
		m_pWebBrowser->MoveWindow(10, 10, rectClient.Width() - 20, rectClient.Height() - 20);
	}

	return 0;
}

void WebDialog::SetParent(CMFCApplication1WebView2Dlg* pParent)
{
	m_pParent = pParent;
}
