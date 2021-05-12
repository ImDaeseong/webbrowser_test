#include "pch.h"
#include "framework.h"
#include "ApplicationWebView2.h"
#include "ApplicationWebView2Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Microsoft::WRL;
static wil::com_ptr<ICoreWebView2Controller> webviewController;
static wil::com_ptr<ICoreWebView2> webviewWindow;

CApplicationWebView2Dlg::CApplicationWebView2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_APPLICATIONWEBVIEW2_DIALOG, pParent)
{
}

void CApplicationWebView2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CApplicationWebView2Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL CApplicationWebView2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CoInitialize(NULL);

	HWND hWnd = GetSafeHwnd();

	CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,

		Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(

			[hWnd](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {

				env->CreateCoreWebView2Controller(hWnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
					[hWnd](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {

						if (controller != nullptr) {
							webviewController = controller;
							webviewController->get_CoreWebView2(&webviewWindow);
						}

						ICoreWebView2Settings* Settings;
						webviewWindow->get_Settings(&Settings);
						Settings->put_IsScriptEnabled(TRUE);
						Settings->put_AreDefaultScriptDialogsEnabled(TRUE);
						Settings->put_IsWebMessageEnabled(TRUE);

						RECT bounds;
						::GetClientRect(hWnd, &bounds);
						webviewController->put_Bounds(bounds);

						webviewWindow->Navigate(L"https://www.naver.com/");




						// Step 4 - Navigation events

						// register an ICoreWebView2NavigationStartingEventHandler to cancel any non-https navigation
						EventRegistrationToken token;
						webviewWindow->add_NavigationStarting(Callback<ICoreWebView2NavigationStartingEventHandler>(
							[](ICoreWebView2* webview, ICoreWebView2NavigationStartingEventArgs* args) -> HRESULT {
								PWSTR uri;
								args->get_Uri(&uri);
								std::wstring source(uri);
								if (source.substr(0, 5) != L"https") {
									args->put_Cancel(true);
								}
								CoTaskMemFree(uri);
								return S_OK;
							}).Get(), &token);




						// Step 5 - Scripting

						// Schedule an async task to add initialization script that freezes the Object object
						webviewWindow->AddScriptToExecuteOnDocumentCreated(L"Object.freeze(Object);", nullptr);
						// Schedule an async task to get the document URL
						webviewWindow->ExecuteScript(L"window.document.URL;", Callback<ICoreWebView2ExecuteScriptCompletedHandler>(
							[](HRESULT errorCode, LPCWSTR resultObjectAsJson) -> HRESULT {
								LPCWSTR URL = resultObjectAsJson;
								//doSomethingWithURL(URL);
								return S_OK;
							}).Get());




						// Step 6 - Communication between host and web content

						// Set an event handler for the host to return received message back to the web content
						webviewWindow->add_WebMessageReceived(Callback<ICoreWebView2WebMessageReceivedEventHandler>(
							[](ICoreWebView2* webview, ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT {
								PWSTR message;
								args->TryGetWebMessageAsString(&message);
								// processMessage(&message);
								webview->PostWebMessageAsString(message);
								CoTaskMemFree(message);
								return S_OK;
							}).Get(), &token);

						/*
						// Schedule an async task to add initialization script that
						// 1) Add an listener to print message from the host
						// 2) Post document URL to the host
						webviewWindow->AddScriptToExecuteOnDocumentCreated(
							L"window.chrome.webview.addEventListener(\'message\', event => alert(event.data));" \
							L"window.chrome.webview.postMessage(window.document.URL);",
							nullptr);
							*/



						return S_OK;
					}).Get());
				return S_OK;
			}).Get());

	return TRUE;  
}

void CApplicationWebView2Dlg::OnPaint()
{
	CPaintDC dc(this);
}

void CApplicationWebView2Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (webviewController != nullptr) {
		RECT bounds;
		::GetClientRect(GetSafeHwnd(), &bounds);
		webviewController->put_Bounds(bounds);
	};
}

void CApplicationWebView2Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	CoUninitialize();
}
