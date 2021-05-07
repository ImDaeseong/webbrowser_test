#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

class CApplicationWebView2App : public CWinApp
{
public:
	CApplicationWebView2App();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()	
};

extern CApplicationWebView2App theApp;
