#pragma once

#include <afxwin.h>
#include <wrl.h>
#include <wil/com.h>
#include <WebView2.h>
#include <sstream>

class IWebView2EventCallback {
public:
    virtual void OnNavigationStarting(const std::wstring& uri) = 0;
    virtual void OnNavigationCompleted(bool bSuccess) = 0;
    virtual void OnSourceChanged(const std::wstring& source) = 0;
    virtual void OnDocumentTitleChanged(const std::wstring& title) = 0;
    virtual void OnWebView2Created() = 0;
    virtual void OnNewWindowRequested(const std::wstring& uri) = 0;
};

class WebView2Ex {
public:
    WebView2Ex();
    ~WebView2Ex();
    HRESULT Create(HWND hWnd);
    void Navigate(LPCWSTR url);
    void NavigatePost(LPCWSTR url, const std::wstring& postData, LPCWSTR contentType);
    void SetBounds(int left, int top, int width, int height);
    void SetEventCallback(IWebView2EventCallback* callback) { m_eventCallback = callback; }

private:
    void AddEventHandlers();
    void OnNavigationStarting(ICoreWebView2* sender, ICoreWebView2NavigationStartingEventArgs* args);
    void OnNavigationCompleted(ICoreWebView2* sender, ICoreWebView2NavigationCompletedEventArgs* args);
    void OnSourceChanged(ICoreWebView2* sender, ICoreWebView2SourceChangedEventArgs* args);
    void OnDocumentTitleChanged(ICoreWebView2* sender, IUnknown* args);
    void OnNewWindowRequested(ICoreWebView2* sender, ICoreWebView2NewWindowRequestedEventArgs* args);

    Microsoft::WRL::ComPtr<ICoreWebView2_3> m_webView;
    Microsoft::WRL::ComPtr<ICoreWebView2Environment3> m_webViewEnvironment;
    Microsoft::WRL::ComPtr<ICoreWebView2Controller> m_webViewController;

    HWND m_hWnd;
    
    EventRegistrationToken m_navigationStartingToken;
    EventRegistrationToken m_navigationCompletedToken;
    EventRegistrationToken m_sourceChangedToken;
    EventRegistrationToken m_documentTitleChangedToken;
    EventRegistrationToken m_newWindowRequestedToken;
    
    IWebView2EventCallback* m_eventCallback;
};