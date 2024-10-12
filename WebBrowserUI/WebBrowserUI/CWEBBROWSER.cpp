#include "pch.h"
#include "CWEBBROWSER.h"

IMPLEMENT_DYNCREATE(CWEBBROWSER, CWnd)

void CWEBBROWSER::ShowRoundRectRgn()
{
	RECT rc;
	int nX, nY;
	GetClientRect(&rc);

	nX = GetSystemMetrics(SM_CXVSCROLL);
	nY = GetSystemMetrics(SM_CYVSCROLL);
	SetWindowPos(NULL, rc.left, rc.top, (rc.right + nX), (rc.bottom + nY), SWP_NOMOVE | SWP_NOZORDER);

	rc.top = 0;
	rc.left = 0;
	rc.bottom -= rc.top;
	rc.right -= rc.left;

	HRGN hRgn = NULL;
	hRgn = CreateRoundRectRgn(rc.left + 12, rc.top + 12, rc.right - 12, rc.bottom - 12, 22, 22);
	SetWindowRgn(hRgn, TRUE);
}

void CWEBBROWSER::HideScrollRect()
{
	RECT rc;
	int nX, nY;
	GetClientRect(&rc);

	nX = GetSystemMetrics(SM_CXVSCROLL);
	nY = GetSystemMetrics(SM_CYVSCROLL);
	SetWindowPos(NULL, rc.left, rc.top, (rc.right + nX), (rc.bottom + nY), SWP_NOMOVE | SWP_NOZORDER);
}