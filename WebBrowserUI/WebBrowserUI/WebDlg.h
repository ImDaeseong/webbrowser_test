#pragma once

#include "WebBrowser2Ex.h"

class WebDlg : public CDialogEx
{
	DECLARE_DYNAMIC(WebDlg)

public:
	WebDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~WebDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WEB_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnDocumentCompleteScroll(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	DECLARE_EVENTSINK_MAP()
	void DocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT* URL);

private:
	WebBrowser2Ex m_WebBrowser;

	void InitWebBrowser();
	void Navigate(CString strURL);	
};
