#include "pch.h"
#include "WebBrowser2Ex.h"
#include "WebBrowserUIDlg.h"

BEGIN_MESSAGE_MAP(WebBrowser2Ex, CWEBBROWSER)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

BOOL WebBrowser2Ex::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_LBUTTONUP)
	{
		POINT pos;
		GetCursorPos(&pos);
		ScreenToClient(&pos);

		CWebBrowserUIDlg* pMain = (CWebBrowserUIDlg*)AfxGetMainWnd();
		pMain->WebBrowserMoveClick(pos.x, pos.y);
	}

	return CWEBBROWSER::PreTranslateMessage(pMsg);
}
