﻿#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.

class CMFCApplication1WebView2App : public CWinApp
{
public:
	CMFCApplication1WebView2App();

public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};

extern CMFCApplication1WebView2App theApp;
