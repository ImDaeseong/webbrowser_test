#include "pch.h"
#include "WebView2Ex.h"

WebView2Ex::WebView2Ex() : m_hWnd(NULL) {}

WebView2Ex::~WebView2Ex()
{
    if (m_webView)
    {
        m_webView->remove_NavigationStarting(m_navigationStartingToken);
        m_webView->remove_NavigationCompleted(m_navigationCompletedToken);
        m_webView->remove_SourceChanged(m_sourceChangedToken);
        m_webView->remove_DocumentTitleChanged(m_documentTitleChangedToken);
    }
}

HRESULT WebView2Ex::Create(HWND hWnd)
{
    m_hWnd = hWnd;
    return CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [this](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {
                if (SUCCEEDED(result))
                {
                    m_webViewEnvironment = env;
                    return m_webViewEnvironment->CreateCoreWebView2Controller(m_hWnd,
                        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                            [this](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
                                if (SUCCEEDED(result))
                                {
                                    m_webViewController = controller;
                                    m_webViewController->get_CoreWebView2(&m_webView);
                                    if (m_webView)
                                    {
                                        AddEventHandlers();
                                        
                                        ICoreWebView2Settings* Settings;
                                        m_webView->get_Settings(&Settings);
                                        Settings->put_IsScriptEnabled(TRUE);
                                        Settings->put_AreDefaultScriptDialogsEnabled(TRUE);
                                        Settings->put_IsWebMessageEnabled(TRUE);

                                        RECT bounds;
                                        GetClientRect(m_hWnd, &bounds);
                                        m_webViewController->put_Bounds(bounds);

                                        //m_webView->Navigate(L"https://www.naver.com");

                                        if (m_eventCallback) m_eventCallback->OnWebView2Created();
                                    }
                                }
                                return S_OK;
                            }).Get());
                }
                return S_OK;
            }).Get());
}

void WebView2Ex::Navigate(LPCWSTR url)
{
    if (m_webView)
    {
        m_webView->Navigate(url);
    }
}

void WebView2Ex::SetBounds(int left, int top, int width, int height)
{
    if (m_webViewController)
    {
        RECT bounds = { left, top, left + width, top + height };
        m_webViewController->put_Bounds(bounds);
    }
}

void WebView2Ex::AddEventHandlers()
{
    if (m_webView)
    {
        m_webView->add_NavigationStarting(Microsoft::WRL::Callback<ICoreWebView2NavigationStartingEventHandler>(
            [this](ICoreWebView2* sender, ICoreWebView2NavigationStartingEventArgs* args) -> HRESULT {
                OnNavigationStarting(sender, args);
                return S_OK;
            }).Get(), &m_navigationStartingToken);

        m_webView->add_NavigationCompleted(Microsoft::WRL::Callback<ICoreWebView2NavigationCompletedEventHandler>(
            [this](ICoreWebView2* sender, ICoreWebView2NavigationCompletedEventArgs* args) -> HRESULT {
                OnNavigationCompleted(sender, args);
                return S_OK;
            }).Get(), &m_navigationCompletedToken);

        m_webView->add_SourceChanged(Microsoft::WRL::Callback<ICoreWebView2SourceChangedEventHandler>(
            [this](ICoreWebView2* sender, ICoreWebView2SourceChangedEventArgs* args) -> HRESULT {
                OnSourceChanged(sender, args);
                return S_OK;
            }).Get(), &m_sourceChangedToken);

        m_webView->add_DocumentTitleChanged(Microsoft::WRL::Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
            [this](ICoreWebView2* sender, IUnknown* args) -> HRESULT {
                OnDocumentTitleChanged(sender, args);
                return S_OK;
            }).Get(), &m_documentTitleChangedToken);
    }
}

void WebView2Ex::OnNavigationStarting(ICoreWebView2* sender, ICoreWebView2NavigationStartingEventArgs* args)
{
    wil::unique_cotaskmem_string uri;
    args->get_Uri(&uri);
    if (m_eventCallback) m_eventCallback->OnNavigationStarting(uri.get());
}

void WebView2Ex::OnNavigationCompleted(ICoreWebView2* sender, ICoreWebView2NavigationCompletedEventArgs* args)
{
    BOOL bSuccess;
    args->get_IsSuccess(&bSuccess);
    if (m_eventCallback) m_eventCallback->OnNavigationCompleted(bSuccess);
}

void WebView2Ex::OnSourceChanged(ICoreWebView2* sender, ICoreWebView2SourceChangedEventArgs* args)
{
    wil::unique_cotaskmem_string source;
    sender->get_Source(&source);
    if (m_eventCallback) m_eventCallback->OnSourceChanged(source.get());
}

void WebView2Ex::OnDocumentTitleChanged(ICoreWebView2* sender, IUnknown* args)
{
    wil::unique_cotaskmem_string title;
    sender->get_DocumentTitle(&title);
    if (m_hWnd && title)
    {
        ::SetWindowText(m_hWnd, title.get());
    }
    if (m_eventCallback) m_eventCallback->OnDocumentTitleChanged(title.get());
}