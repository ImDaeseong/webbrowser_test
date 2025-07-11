#include "pch.h"
#include "WebBrowser2Ex.h"
#include "WebBrowserUIDlg.h"

BEGIN_MESSAGE_MAP(WebBrowser2Ex, CWEBBROWSER)
END_MESSAGE_MAP()

BOOL WebBrowser2Ex::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_LBUTTONUP)
	{
		POINT pos;
		GetCursorPos(&pos);
		
		CWebBrowserUIDlg* pMain = (CWebBrowserUIDlg*)AfxGetMainWnd();
		if (pMain && ::IsWindow(pMain->GetSafeHwnd()))
		{
			//부모 다이얼로그 기준으로
			::ScreenToClient(pMain->GetSafeHwnd(), &pos);

			pMain->WebBrowserMoveClick(pos.x, pos.y);
		}		
	}

	return CWEBBROWSER::PreTranslateMessage(pMsg);
}
