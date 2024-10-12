#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

class CWebBrowserUIApp : public CWinApp
{
public:
	CWebBrowserUIApp();

public:
	virtual BOOL InitInstance();


	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();

private:
	ULONG_PTR m_gdiplusToken;
	void SetClassName();
};

extern CWebBrowserUIApp theApp;
