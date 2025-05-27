#pragma once
#include "EdgeWebBrowser.h"

class CMFCApplication1WebView2Dlg;
class WebDialog : public CDialogEx
{
	DECLARE_DYNAMIC(WebDialog)

public:
	WebDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~WebDialog();

	void SetParent(CMFCApplication1WebView2Dlg* pParent);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WEB_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnMessage1(WPARAM, LPARAM);
	afx_msg LRESULT OnMessage2(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

private:
	void initWebView();
	void NavigateWebView();
	void SendMesaage(CString strMesssage);

	std::unique_ptr<CWebBrowser> m_pWebBrowser{};
	CMFCApplication1WebView2Dlg* m_pParent = nullptr;
};
