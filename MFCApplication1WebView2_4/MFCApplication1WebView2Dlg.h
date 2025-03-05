#pragma once
#include "EdgeWebBrowser.h"

class CMFCApplication1WebView2Dlg : public CDialogEx
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
    afx_msg void OnDestroy();
    afx_msg LRESULT OnMessage1(WPARAM, LPARAM);
    afx_msg LRESULT OnMessage2(WPARAM, LPARAM);
    DECLARE_MESSAGE_MAP()

private:
    std::unique_ptr<CWebBrowser> m_pWebBrowser{};

    void initWebView();
    void NavigateWebView();
};