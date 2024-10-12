#pragma once
#include "CWEBBROWSER.h"

class WebBrowser2Ex :  public CWEBBROWSER
{
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
};