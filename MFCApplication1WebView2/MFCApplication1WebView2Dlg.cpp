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
    ON_WM_SIZE()
    ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL CMFCApplication1WebView2Dlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    CoInitialize(NULL);

    m_webViewEx = std::make_unique<WebView2Ex>();
    m_webViewEx->SetEventCallback(this);
    HRESULT hr = m_webViewEx->Create(GetSafeHwnd());
    if (FAILED(hr))
    {
        return FALSE;
    }

    return TRUE;
}

void CMFCApplication1WebView2Dlg::OnPaint()
{
    CPaintDC dc(this);
}

void CMFCApplication1WebView2Dlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);

    //시작시 호출 않됨 - m_webViewEx 가 완료되지 않아서
    ResizeWebView();
}

void CMFCApplication1WebView2Dlg::OnDestroy()
{
    CDialogEx::OnDestroy();

    CoUninitialize();
}

void CMFCApplication1WebView2Dlg::OnNavigationStarting(const std::wstring& uri)
{
    CString strMsg;
    strMsg.Format(_T("OnNavigationStarting: %s"), uri.c_str());
}

void CMFCApplication1WebView2Dlg::OnNavigationCompleted(bool bSuccess)
{
    CString strMsg = bSuccess ? _T("OnNavigationCompleted successfully.") : _T("OnNavigationCompleted failed.");

    if (bSuccess)
    {
        //ResizeWebView();
    }
}

void CMFCApplication1WebView2Dlg::OnSourceChanged(const std::wstring& source)
{
    CString strMsg;
    strMsg.Format(_T("OnSourceChanged: %s"), source.c_str());
}

void CMFCApplication1WebView2Dlg::OnDocumentTitleChanged(const std::wstring& title)
{
    SetWindowText(title.c_str());
}

//브라우저 생성 완료시
void CMFCApplication1WebView2Dlg::OnWebView2Created()
{
    if (m_webViewEx)
    {
        m_webViewEx->Navigate(L"https://www.naver.com");
        ResizeWebView();
    }
}

//새페이지 및 팝업 호출시
void CMFCApplication1WebView2Dlg::OnNewWindowRequested(const std::wstring& uri)
{
    CString strMsg;
    strMsg.Format(_T("OnNewWindowRequested: %s"), uri.c_str());
}

//단추키 호출
void CMFCApplication1WebView2Dlg::OnOnAcceleratorKey()
{
    AfxMessageBox(_T("단추키 호출"));
}

void CMFCApplication1WebView2Dlg::ResizeWebView()
{
    if (m_webViewEx)
    {
        RECT bounds;
        ::GetClientRect(GetSafeHwnd(), &bounds);

        int nleft = bounds.left + 20;
        int ntop = bounds.top + 20;
        int nwidth = bounds.right - bounds.left - 40;
        int nheight = bounds.bottom - bounds.top - 40;
        m_webViewEx->SetBounds(nleft, ntop, nwidth, nheight);
    }
}