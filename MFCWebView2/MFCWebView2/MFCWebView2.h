#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

class CMFCWebView2App : public CWinApp
{
public:
	CMFCWebView2App();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CMFCWebView2App theApp;
