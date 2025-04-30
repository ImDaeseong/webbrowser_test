#pragma once

class WebDialog;
class CMFCApplication1WebView2Dlg : public CDialogEx
{
public:
    CMFCApplication1WebView2Dlg(CWnd* pParent = nullptr);

    WebDialog* m_pWebDlg;
    void CreateWebDlg();
    void DestroyWebDlg();

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_MFCAPPLICATION1WEBVIEW2_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg void OnDestroy();
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton2();
    DECLARE_MESSAGE_MAP()
};