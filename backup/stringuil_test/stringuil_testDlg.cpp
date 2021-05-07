#include "pch.h"
#include "framework.h"
#include "stringuil_test.h"
#include "stringuil_testDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CstringuiltestDlg::CstringuiltestDlg(CWnd* pParent /*=nullptr*/) : CDialogEx(IDD_STRINGUIL_TEST_DIALOG, pParent)
{
}

void CstringuiltestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CstringuiltestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &CstringuiltestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CstringuiltestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CstringuiltestDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CstringuiltestDlg::OnBnClickedButton4)
END_MESSAGE_MAP()

BOOL CstringuiltestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  
}

void CstringuiltestDlg::OnPaint()
{
	CPaintDC dc(this);
}

//std::string
void CstringuiltestDlg::OnBnClickedButton1()
{
	int nCount = 4;
	std::string _str = "aBcdeFghiJklmn1234oPqrst";
	
	std::string a1 = stringutil::Left(_str, nCount);
	std::string a2 = stringutil::Right(_str, nCount);
	std::string a3 = stringutil::Mid(_str, 0, nCount);
	std::string a4 = stringutil::MakeLower(_str);
	std::string a5 = stringutil::MakeUpper(_str);
	size_t a6 = stringutil::Find(_str, "1234", 0);
	size_t a7 = stringutil::Find(_str, '3', 0);

	CString stMsg;
	stMsg.Format(_T("a6:%d a7:%d\n"), a6,a7);
	OutputDebugString(stMsg);

	size_t a8 = stringutil::ReserveFind(_str, "1234");
	size_t a9 = stringutil::ReserveFind(_str, '3');
			
	stMsg.Format(_T("a8:%d a9:%d\n"), a8, a9);
	OutputDebugString(stMsg);
		
	std::string a10 = stringutil::GetFilePath("C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe");
	std::string a11 = stringutil::GetFileName("C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe");
	std::string a12 = stringutil::RemoveExt(a11);
	std::string a13 = stringutil::GetFileExt("C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe");
	std::string a14 = stringutil::GetFindStr("C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe", "chrome.exe", ".");

	stMsg = CString(a14.c_str());
	OutputDebugString(stMsg);

	std::vector<std::string> _Vec;
	std::string _strPath = "C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe";
	std::string _delimit = "\\";
	if (stringutil::Split(_strPath, _delimit, _Vec) > 0)
	{
		for (int i = 0; i < _Vec.size(); i++)
		{
			std::string str = std::to_string(i) + " 데이터 " + _Vec[i];			
			//stMsg.Format(_T("_Vec:%s\n"), CString(_Vec[i].c_str()));
			//OutputDebugString(stMsg);
		}
	}
	
	std::string a15 = stringutil::TrimLeft("  C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe");
	std::string a16 = stringutil::TrimRight("C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe   ");
	std::string a17 = stringutil::Trim("  C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe   ");

	std::string a18 = stringutil::Reserve(a17);

	std::string a19 = stringutil::Replace(a17, "chrome.exe","msedge.exe");
}

//std::wstring
void CstringuiltestDlg::OnBnClickedButton2()
{
	int nCount = 4;
	std::wstring _str = L"aBcdeFghiJklmn1234oPqrst";

	std::wstring a1 = wstringutil::Left(_str, nCount);
	std::wstring a2 = wstringutil::Right(_str, nCount);
	std::wstring a3 = wstringutil::Mid(_str, 0, nCount);
	std::wstring a4 = wstringutil::MakeLower(_str);
	std::wstring a5 = wstringutil::MakeUpper(_str);
	size_t a6 = wstringutil::Find(_str, L"1234", 0);
	size_t a7 = wstringutil::Find(_str, L'3', 0);

	CString stMsg;
	stMsg.Format(_T("a6:%d a7:%d\n"), a6, a7);
	OutputDebugString(stMsg);

	size_t a8 = wstringutil::ReserveFind(_str, L"1234");
	size_t a9 = wstringutil::ReserveFind(_str, L'3');

	stMsg.Format(_T("a8:%d a9:%d\n"), a8, a9);
	OutputDebugString(stMsg);

	std::wstring a10 = wstringutil::GetFilePath(L"C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe");
	std::wstring a11 = wstringutil::GetFileName(L"C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe");
	std::wstring a12 = wstringutil::RemoveExt(a11);
	std::wstring a13 = wstringutil::GetFileExt(L"C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe");
	std::wstring a14 = wstringutil::GetFindStr(L"C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe", L"chrome.exe", L".");

	stMsg = CString(a14.c_str());
	OutputDebugString(stMsg);

	std::vector<std::wstring> _Vec;
	std::wstring _strPath = L"C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe";
	std::wstring _delimit = L"\\";
	if (wstringutil::Split(_strPath, _delimit, _Vec) > 0)
	{
		for (int i = 0; i < _Vec.size(); i++)
		{
			std::wstring str = std::to_wstring(i) + L" 데이터 " + _Vec[i];
			//stMsg.Format(_T("_Vec:%s\n"), CString(_Vec[i].c_str()));
			//OutputDebugString(stMsg);
		}
	}

	std::wstring a15 = wstringutil::TrimLeft(L"  C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe");
	std::wstring a16 = wstringutil::TrimRight(L"C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe   ");
	std::wstring a17 = wstringutil::Trim(L"  C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe   ");

	std::wstring a18 = wstringutil::Reserve(a17);

	std::wstring a19 = wstringutil::Replace(a17, L"chrome.exe", L"msedge.exe");
}

//wchar_t
void CstringuiltestDlg::OnBnClickedButton3()
{
	int nCount = 4;
	wchar_t* _str = L"aBcdeFghiJklmn1234oPqrst";

	size_t len = wcharutil::GetLength(_str);
	wchar_t* a1 = wcharutil::Left(_str, nCount);
	wchar_t* a2 = wcharutil::Right(_str, nCount);
	wchar_t* a3 = wcharutil::Mid(_str, 0, nCount);
	wchar_t* a4 = wcharutil::MakeLower(_str);
	wchar_t* a5 = wcharutil::MakeUpper(_str);
	size_t a6 = wcharutil::Find(_str, L"1234");
	size_t a7 = wcharutil::Find(_str, L'3');

	CString stMsg;
	stMsg.Format(_T("a6:%d a7:%d\n"), a6, a7);
	OutputDebugString(stMsg);

	size_t a8 = wcharutil::ReserveFind(_str, L'3');

	stMsg.Format(_T("a8:%d\n"), a8);
	OutputDebugString(stMsg);

	std::vector<wchar_t*> _Vec;
	wchar_t* _strPath = L"C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe";
	wchar_t* _delimit = L"\\";
	if (wcharutil::Split(_strPath, _delimit, _Vec) > 0)
	{
		for (int i = 0; i < _Vec.size(); i++)
		{
			std::wstring str = std::to_wstring(i) + L" 데이터 " + _Vec[i];
			stMsg.Format(_T("_Vec:%s\n"), _Vec[i]);
			OutputDebugString(stMsg);
		}
	}

	CString strTemp(L"  C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe");
	strTemp.TrimLeft();
	wchar_t* wstr = T2W(strTemp.GetBuffer(0));

	CString str15 = wcharutil::TrimLeft(L"  C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe");
	wchar_t* a15 = T2W(str15.GetBuffer(0));
	
	CString str16 = wcharutil::TrimRight(L"C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe   ");
	wchar_t* a16 = T2W(str16.GetBuffer(0));

	CString str17 = wcharutil::Trim(L"  C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe   ");
	wchar_t* a17 = T2W(str17.GetBuffer(0));

	wchar_t* a18 = wcharutil::Reserve(a17);

	CString str19 = wcharutil::Replace(a17, L"chrome.exe", L"msedge.exe");
	wchar_t* a19 = T2W(str19.GetBuffer(0));	
}

//TCHAR*
void CstringuiltestDlg::OnBnClickedButton4()
{
	int nCount = 4;
	TCHAR* _str = _T("aBcdeFghiJklmn1234oPqrst");

	TCHAR* a1 = TCHARutil::Left(_str, nCount);
	TCHAR* a2 = TCHARutil::Right(_str, nCount);
	TCHAR* a3 = TCHARutil::Mid(_str, 0, nCount);
	TCHAR* a4 = TCHARutil::MakeLower(_str);
	TCHAR* a5 = TCHARutil::MakeUpper(_str);
	size_t a6 = TCHARutil::Find(_str, _T("1234"));
	size_t a7 = TCHARutil::Find(_str, _T('3'));

	CString stMsg;
	stMsg.Format(_T("a6:%d a7:%d\n"), a6, a7);
	OutputDebugString(stMsg);

	size_t a8 = TCHARutil::ReserveFind(_str, L'3');

	stMsg.Format(_T("a8:%d\n"), a8);
	OutputDebugString(stMsg);

	std::vector<TCHAR*> _Vec;
	TCHAR* _strPath = _T("C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe");
	TCHAR* _delimit = _T("\\");
	if (TCHARutil::Split(_strPath, _delimit, _Vec) > 0)
	{
		for (int i = 0; i < _Vec.size(); i++)
		{
			stMsg.Format(_T("_Vec:%s\n"), _Vec[i]);
			OutputDebugString(stMsg);
		}
	}

	TCHAR* a15 = TCHARutil::TrimLeft(_T("  C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe"));
	TCHAR* a16 = TCHARutil::TrimRight(_T("C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe   "));
	TCHAR* a17 = TCHARutil::Trim(_T("  C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe   "));

	TCHAR* a18 = TCHARutil::Reserve(a17);

	CString str19 = TCHARutil::Replace(a17,_T("chrome.exe"), _T("msedge.exe"));

	TCHAR* a19 = str19.GetBuffer(str19.GetLength());
	str19.ReleaseBuffer();
}
