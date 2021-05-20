#include "pch.h"
#include "framework.h"
#include "steamapps.h"
#include "steamappsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CsteamappsDlg::CsteamappsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_STEAMAPPS_DIALOG, pParent)
{	
}

void CsteamappsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CsteamappsDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &CsteamappsDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CsteamappsDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CsteamappsDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CsteamappsDlg::OnBnClickedButton4)
END_MESSAGE_MAP()

BOOL CsteamappsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetDlgItemText(IDC_EDIT_ID, _T("아이디를 입력하세요"));
	SetDlgItemText(IDC_EDIT_PASSWORD, _T("비밀번호를 입력하세요"));

	return TRUE;  
}

void CsteamappsDlg::OnPaint()
{
	CPaintDC dc(this);
}

//스팀 로그인
void CsteamappsDlg::OnBnClickedButton1()
{
	//스팀 설치 경로 확인
	RegInfo reg;
	CString strPath = reg.IsExistSteamInfo();
	if (strPath.IsEmpty())
		return;


	//스팀 실행 파일 경로
	CString strAppExe = strPath + _T("\\steam.exe");


	//아이디
	CString strID = _T("");
	GetDlgItemText(IDC_EDIT_ID, strID);


	//비밀번호
	CString strPassword = _T("");
	GetDlgItemText(IDC_EDIT_PASSWORD, strPassword);


	//로그인 정보가 없는 경우
	if (strID == "" || strPassword == "")
	{
		::ShellExecute(NULL, _T("open"), strAppExe, NULL, NULL, SW_SHOW);
		return;
	}

	//스팀 계정으로 로그인하기
	CString strParams;
	strParams.Format(_T("-login %s %s"), strID, strPassword);
	::ShellExecute(NULL, _T("open"), strAppExe, strParams, NULL, SW_SHOW);
}

//스팀 로그아웃
void CsteamappsDlg::OnBnClickedButton2()
{
	//스팀 설치 경로 확인
	RegInfo reg;
	CString strPath = reg.IsExistSteamInfo();
	if (strPath.IsEmpty())
		return;

	CString strAppExe;
	strAppExe.Format(_T("%s\\steam.exe"), strPath);
	::ShellExecute(NULL, _T("open"), strAppExe, _T("-shutdown"), NULL, SW_SHOW);
}

//스팀 게임 설치 목록
void CsteamappsDlg::OnBnClickedButton3()
{
	m_AppInfos.clear();

	//스팀 설치 경로 확인
	RegInfo reg;
	CString strPath = reg.IsExistSteamInfo();
	if (strPath.IsEmpty())
		return;

	//스팀 설치 게임 목록 확인
	CString strSteamAppsPath = strPath + _T("\\steamapps");
	CString strWildcard = strSteamAppsPath;
	strWildcard += _T("\\*.acf");

	//스팀 게임 찾기
	CFileFind finder;
	BOOL bWorking = finder.FindFile(strWildcard);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDots())
			continue;

		if (finder.IsDirectory())
			continue;

		CString strFullPath = finder.GetFilePath();
		CStdioFile stdFile(strFullPath, CFile::modeRead | CFile::typeText);
		CString strLine, strAppid, strAppname;

		while (stdFile.ReadString(strLine))
		{
			if (strLine.Find(_T("appid")) > 0)
			{
				strAppid = strLine;
				strAppid.Replace(_T("appid"), _T(""));
				strAppid.Replace(_T("\""), _T(""));
				strAppid.Replace(_T("\r"), _T(""));
				strAppid.Replace(_T("\n"), _T(""));
				strAppid.Replace(_T("\t"), _T(""));
				strAppid.Trim();
			}
			else if (strLine.Find(_T("name")) > 0)
			{
				strAppname = strLine;
				strAppname.Replace(_T("name"), _T(""));
				strAppname.Replace(_T("\""), _T(""));
				strAppname.Replace(_T("\r"), _T(""));
				strAppname.Replace(_T("\n"), _T(""));
				strAppname.Replace(_T("\t"), _T(""));
				strAppname.Trim();
			}

			if (!strAppid.IsEmpty() && !strAppname.IsEmpty())
			{
				AppInfo AppInfos;
				AppInfos.strAppid = strAppid;
				AppInfos.strAppname = strAppname;
				m_AppInfos.push_back(AppInfos);
				strAppid = _T("");
				strAppname = _T("");
				break;
			}

		}
		stdFile.Close();

	}
	finder.Close();
}

//스팀 게임 실행
void CsteamappsDlg::OnBnClickedButton4()
{
	CString strGamename = _T("FortressV2");
	CString strAppID = _T("");

	for (int i = 0; i < m_AppInfos.size(); i++)
	{
		if (m_AppInfos[i].strAppname == strGamename)
		{
			strAppID = m_AppInfos[i].strAppid;
			break;
		}
	}

	if (!strAppID.IsEmpty())
	{
		CString strParam;
		strParam.Format(_T("steam://rungameid/%s"), strAppID);
		::ShellExecute(NULL, _T("open"), strParam, NULL, NULL, SW_SHOW);
	}	
}
