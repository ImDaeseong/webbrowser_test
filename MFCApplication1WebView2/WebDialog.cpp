#include "pch.h"
#include "MFCApplication1WebView2.h"
#include "WebDialog.h"
#include "afxdialogex.h"
#include "MFCApplication1WebView2Dlg.h"
#include "JsonManager.h"

IMPLEMENT_DYNAMIC(WebDialog, CDialogEx)

//인식해야할 영역 좌표
static RECT rc = { 214, 1, 395, 45 };

WebDialog::WebDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WEB_DIALOG, pParent)
{
	m_webViewEx = nullptr;
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
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL WebDialog::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;
    return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL WebDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

    if (FAILED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED)))
    {
        return FALSE;
    }

	m_webViewEx = std::make_unique<WebView2Ex>();
	m_webViewEx->SetEventCallback(this);

    if (FAILED(m_webViewEx->Create(GetSafeHwnd())))
    {
        return FALSE;
    }
	
	return TRUE;
}

void WebDialog::OnPaint()
{
	CPaintDC dc(this);

    DrawRedRectangle(&dc);;
}

void WebDialog::DrawRedRectangle(CDC* pDC)
{
    CBrush brush(RGB(255, 0, 0));
    pDC->FillRect(&rc, &brush);
}

void WebDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	//시작시 호출 않됨 - m_webViewEx 가 완료되지 않아서
	ResizeWebView();
}

void WebDialog::OnDestroy()
{
	CDialogEx::OnDestroy();
    m_webViewEx.reset();
    CoUninitialize();
}

void WebDialog::OnNavigationStarting(const std::wstring& uri)
{
    CString strMsg;
    strMsg.Format(_T("OnNavigationStarting: %s"), uri.c_str());
}

void WebDialog::OnNavigationCompleted(bool bSuccess)
{
    CString strMsg = bSuccess ? _T("OnNavigationCompleted successfully.") : _T("OnNavigationCompleted failed.");

    if (bSuccess)
    {
        //ResizeWebView();
    }
}

void WebDialog::OnSourceChanged(const std::wstring& source)
{
    CString strMsg;
    strMsg.Format(_T("OnSourceChanged: %s"), source.c_str());
}

void WebDialog::OnDocumentTitleChanged(const std::wstring& title)
{
    SetWindowText(title.c_str());
}

//브라우저 생성 완료시
void WebDialog::OnWebView2Created()
{
    if (m_webViewEx)
    {
        m_webViewEx->Navigate(L"https://www.naver.com");
        //m_webViewEx->Navigate(L"file:///E:/node_test/SignalingServer/webrtc.html");

        ResizeWebView();
    }
}

//새페이지 및 팝업 호출시
void WebDialog::OnNewWindowRequested(const std::wstring& uri)
{
    CString strMsg;
    strMsg.Format(_T("OnNewWindowRequested: %s"), uri.c_str());
}

void WebDialog::OnWebMessageReceived(const std::wstring& message)
{
    //CString strMsg = _T("");
    //strMsg.Format(_T("OnWebMessageReceived: %s \r\n"), message.c_str());
    //OutputDebugString(strMsg);

    CJsonManager man;
    if (!man.LoadFromString(message.c_str()))
        return;

    CString sType = man.GetValueAsCString(_T("type"));
    if (sType.CompareNoCase(_T("click")) == 0)
    {
        Json::Value data = man.GetJsonObject(_T("data"));
        if (!data.isNull() && data.isObject())
        {
            int x = data["x"].asInt();
            int y = data["y"].asInt();
            int pageX = data["pageX"].asInt();
            int pageY = data["pageY"].asInt();
            int screenX = data["screenX"].asInt();
            int screenY = data["screenY"].asInt();
            CString target = CJsonManager::ConvertUtf8ToCString(data["target"].asString());
            CString targetId = CJsonManager::ConvertUtf8ToCString(data["targetId"].asString());
            CString targetClass = CJsonManager::ConvertUtf8ToCString(data["targetClass"].asString());

            POINT pt = { x, y };

            if (PtInRect(&rc, pt) == TRUE)
            {
                CString strMsg;
                strMsg.Format(_T("x:%d y:%d"), pt.x, pt.y);
                OutputDebugString(strMsg);
            }
        }
    }
}

//단추키 호출
void WebDialog::OnOnAcceleratorKey()
{
    SendMessage(_T("스크립트로 메시지 보내기"));
    //AfxMessageBox(_T("단추키 호출"));
}

void WebDialog::ResizeWebView()
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

void WebDialog::SetParent(CMFCApplication1WebView2Dlg* pParent)
{
	m_pParent = pParent;
}

void WebDialog::SendMessage(CString strMessage)
{
    //자바스크립트 호출로 메시지 전달
    CString strSend;
    strSend.Format(_T("receiveFromForm('%s');"), strMessage);

    if (m_webViewEx)
    {
        m_webViewEx->ExecuteScript(strSend);
    }
}