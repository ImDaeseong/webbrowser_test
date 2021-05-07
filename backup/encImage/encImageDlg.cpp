#include "pch.h"
#include "framework.h"
#include "encImage.h"
#include "encImageDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CencImageDlg::CencImageDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ENCIMAGE_DIALOG, pParent)
	, m_bCheck(FALSE)
{
}

void CencImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_bCheck);
}

BEGIN_MESSAGE_MAP(CencImageDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &CencImageDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CencImageDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CencImageDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_CHECK1, &CencImageDlg::OnBnClickedCheck1)
END_MESSAGE_MAP()

BOOL CencImageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (m_bCheck == TRUE)
	{
		SetDlgItemText(IDC_BUTTON1, _T("폴더 선택"));
	}
	else
	{
		SetDlgItemText(IDC_BUTTON1, _T("파일 선택"));
	}

	return TRUE; 
}

void CencImageDlg::OnPaint()
{
	CPaintDC dc(this);
}

//파일 선택
void CencImageDlg::OnBnClickedButton1()
{
	if (m_bCheck == TRUE)
	{
		//폴더 선택 
		CString	strFolder;
		LPMALLOC pMalloc;

		if (::SHGetMalloc(&pMalloc) == NOERROR)
		{
			TCHAR tPath[_MAX_PATH] = { 0 };

			BROWSEINFO bi;			
			LPITEMIDLIST pidl;

			bi.hwndOwner = GetSafeHwnd();
			bi.pidlRoot = NULL;
			bi.pszDisplayName = tPath;
			bi.lpszTitle = _T("이미지 폴더를 선택 하세요.");
			bi.ulFlags = BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;
			bi.lpfn = NULL;
			bi.lParam = 0;

			if ((pidl = ::SHBrowseForFolder(&bi)) != NULL)
			{
				if (::SHGetPathFromIDList(pidl, tPath))
				{
					strFolder = tPath;
				}

				pMalloc->Free(pidl);
			}
			pMalloc->Release();
		}

		if (strFolder == "")
			return;

		SetDlgItemText(IDC_EDIT1, strFolder);
	}
	else 
	{
		//파일 선택 
		CFileDialog fIniFile(TRUE);
		fIniFile.m_ofn.lpstrTitle = _T("이미지를 선택 하세요.");

		if (fIniFile.DoModal() == IDOK)
		{
			UpdateData();
			CString strPath = fIniFile.GetPathName();
			SetDlgItemText(IDC_EDIT1, strPath);
		}
	}
}

//암호화
void CencImageDlg::OnBnClickedButton2()
{
	if (m_bCheck == TRUE)
	{
		Enc obj;

		CString strPath = _T("");
		GetDlgItemText(IDC_EDIT1, strPath);

		if (strPath == "")
		{
			((CWnd*)GetDlgItem(IDC_EDIT1))->SetFocus();
			return;
		}

		CStringArray strAry;
		obj.encAddFolder(strPath, strAry);

		int nSize = strAry.GetSize();
		for (int i = 0; i < nSize; i++)
		{
			//암호화 
			obj.encWriteFileBinaryEnc(strAry.GetAt(i));
		}
		strAry.RemoveAll();

		AfxMessageBox(_T("폴더 이미지 암호화 성공"));
	}
	else
	{
		Enc obj;

		CString strPath = _T("");
		GetDlgItemText(IDC_EDIT1, strPath);

		if (strPath == "")
		{
			((CWnd*)GetDlgItem(IDC_EDIT1))->SetFocus();
			return;
		}

		BOOL bFinder = obj.encFileExists(strPath);
		if (bFinder)
		{
			//암호화 
			obj.encWriteFileBinaryEnc(strPath);

			AfxMessageBox(_T("이미지 암호화 성공"));
		}
	}

	/*
	//암호화 복호화 테스트
	CString sPath = _T("E:\\Image\\enc.png");

	Enc obj;
	BOOL bFinder = obj.encFileExists(sPath);
	if (bFinder)
	{
		//암호화 
		obj.encWriteFileBinaryEnc(sPath);

		//복호화
		TCHAR tDecFileName[MAX_PATH];
		wsprintf(tDecFileName, _T("%s\\%s"), obj.encGetModulePath(), obj.encGetFileName(sPath));

		obj.encReadFileBinaryDec(tDecFileName);

		AfxMessageBox(_T("이미지 암복호화 성공"));
	}
	*/
}

//종료
void CencImageDlg::OnBnClickedButton3()
{
	CDialog::OnOK();
}

//체크 박스
void CencImageDlg::OnBnClickedCheck1()
{
	if (((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck())
	{
		m_bCheck = TRUE;

		SetDlgItemText(IDC_BUTTON1, _T("폴더 선택"));
	}
	else
	{
		m_bCheck = FALSE;

		SetDlgItemText(IDC_BUTTON1, _T("파일 선택"));
	}
}
