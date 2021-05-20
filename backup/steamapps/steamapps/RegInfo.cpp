#include "pch.h"
#include "RegInfo.h"

RegInfo::RegInfo(void)
{
}

RegInfo::~RegInfo(void)
{
}

CString RegInfo::IsExistSteamInfo()
{
	TCHAR tPath[MAX_PATH];
	ULONG size{ MAX_PATH };

	CString strPath;
	CRegKey regKey;
	
	if (regKey.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Wow6432Node\\Valve\\Steam"), KEY_READ) != ERROR_SUCCESS)
	{
		return _T("");
	}

	if (regKey.QueryStringValue(_T("InstallPath"), tPath, &size) == ERROR_SUCCESS)
	{
		strPath = tPath;
	}
	regKey.Close();

	return strPath;
}