#pragma once

class CMFCWebView2Dlg : public CDialogEx
{
public:
	CMFCWebView2Dlg(CWnd* pParent = nullptr);	// standard constructor

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCWEBVIEW2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()	
};
