#pragma once
#include "WebView2Ex.h"

class CMFCApplication1WebView2Dlg : public CDialogEx, public IWebView2EventCallback
{
public:
    CMFCApplication1WebView2Dlg(CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_MFCAPPLICATION1WEBVIEW2_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnDestroy();
    DECLARE_MESSAGE_MAP()

public:
    virtual void OnNavigationStarting(const std::wstring& uri) override;
    virtual void OnNavigationCompleted(bool bSuccess) override;
    virtual void OnSourceChanged(const std::wstring& source) override;
    virtual void OnDocumentTitleChanged(const std::wstring& title) override;
    virtual void OnWebView2Created() override;
    virtual void OnNewWindowRequested(const std::wstring& uri) override;

private:
    std::unique_ptr<WebView2Ex> m_webViewEx;

    void ResizeWebView();
};