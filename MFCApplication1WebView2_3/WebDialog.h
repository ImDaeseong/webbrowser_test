#pragma once
#include "WebView2Ex.h"

class CMFCApplication1WebView2Dlg;
class WebDialog : public CDialogEx, public IWebView2EventCallback
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
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

	//WebView2 콜백 구현
	virtual void OnNavigationStarting(const std::wstring& uri) override;
	virtual void OnNavigationCompleted(bool bSuccess) override;
	virtual void OnSourceChanged(const std::wstring& source) override;
	virtual void OnDocumentTitleChanged(const std::wstring& title) override;
	virtual void OnWebView2Created() override;
	virtual void OnNewWindowRequested(const std::wstring& uri) override;
	virtual void OnOnAcceleratorKey() override;

private:
	void ResizeWebView();

	std::unique_ptr<WebView2Ex> m_webViewEx{};
	CMFCApplication1WebView2Dlg* m_pParent = nullptr;
};
