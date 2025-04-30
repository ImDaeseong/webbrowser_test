#include "pch.h"
#include "framework.h"
#include "MFCApplication1WebView2.h"
#include "MFCApplication1WebView2Dlg.h"
#include "afxdialogex.h"
#include "WebDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMFCApplication1WebView2Dlg::CMFCApplication1WebView2Dlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_MFCAPPLICATION1WEBVIEW2_DIALOG, pParent)
{
	m_pWebDlg = nullptr;
}

void CMFCApplication1WebView2Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication1WebView2Dlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1WebView2Dlg::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication1WebView2Dlg::OnBnClickedButton2)
END_MESSAGE_MAP()

BOOL CMFCApplication1WebView2Dlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    return TRUE;
}

void CMFCApplication1WebView2Dlg::OnPaint()
{
    CPaintDC dc(this);
}

void CMFCApplication1WebView2Dlg::OnDestroy()
{
    CDialogEx::OnDestroy();

	DestroyWebDlg();
}

void CMFCApplication1WebView2Dlg::CreateWebDlg()
{
    if (m_pWebDlg == nullptr)
    {
        m_pWebDlg = new WebDialog();
        m_pWebDlg->SetParent(this);
        m_pWebDlg->Create(IDD_WEB_DIALOG, this);
        m_pWebDlg->CenterWindow();
        m_pWebDlg->ShowWindow(SW_SHOWNORMAL);
    }
}

void CMFCApplication1WebView2Dlg::DestroyWebDlg()
{
    if (m_pWebDlg)
    {
        m_pWebDlg->DestroyWindow();
        delete m_pWebDlg;
        m_pWebDlg = nullptr;
    }
}

void CMFCApplication1WebView2Dlg::OnBnClickedButton1()
{
    CreateWebDlg();
}

void CMFCApplication1WebView2Dlg::OnBnClickedButton2()
{
    DestroyWebDlg();
}
