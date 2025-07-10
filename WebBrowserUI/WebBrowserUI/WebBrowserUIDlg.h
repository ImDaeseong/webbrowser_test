#pragma once

#include "WebBrowser2Ex.h"

class CWebBrowserUIDlg : public CDialogEx
{
public:
	CWebBrowserUIDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WEBBROWSERUI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

public:
	DECLARE_EVENTSINK_MAP()
	void DocumentCompleteWebbrowser(LPDISPATCH pDisp, VARIANT* URL);

private:
	WebBrowser2Ex m_WebBrowser;

	void DrawSkin(CDC* pDC, BOOL bFirst = FALSE);
	BOOL LoadSkin();
	void MoveLocationDialog();
	CGdiPlusBitmapResource* m_pBackSkin;

	void InitControl();
	void Navigate(CString strURL);
	BOOL m_LoadedBrowserFlag;

	void DrawRedRectangle(CDC* pDC);

public:
	void WebBrowserMoveClick(int x, int y, int width = 0, int height = 0);
};
