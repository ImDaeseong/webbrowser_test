/* Copyright (C) 2024-2025 Stefan-Mihai MOGA
This file is part of IntelliEdit application developed by Stefan-Mihai MOGA.
IntelliEdit is an alternative Windows version to the famous Microsoft Notepad!

IntelliEdit is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Open
Source Initiative, either version 3 of the License, or any later version.

IntelliEdit is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
IntelliEdit. If not, see <http://www.opensource.org/licenses/gpl-3.0.html>*/

#include "pch.h"
#include <wrl.h>
#include "wil/com.h"
#include "MFCApplication1WebView2Dlg.h"

using namespace Microsoft::WRL;

#include "EdgeWebBrowser.h"
#include "Messages.h"

#include "WebView2.h"

#include <sstream>
#include <iomanip>

#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include "shlobj.h"

#pragma comment(lib,"Version.lib")

#include <string>
#include <list>
#include <algorithm>

#define CHECK_FAILURE_STRINGIFY(arg)         #arg
#define CHECK_FAILURE_FILE_LINE(file, line)  ([](HRESULT hr){ CheckFailure(hr, L"Failure at " CHECK_FAILURE_STRINGIFY(file) L"(" CHECK_FAILURE_STRINGIFY(line) L")"); })
#define CHECK_FAILURE                        CHECK_FAILURE_FILE_LINE(__FILE__, __LINE__)
#define CHECK_FAILURE_BOOL(value)            CHECK_FAILURE((value) ? S_OK : E_UNEXPECTED)

struct CWebBrowserImpl
{
	wil::com_ptr<ICoreWebView2Environment>    m_webViewEnvironment;
	wil::com_ptr<ICoreWebView2Environment2>   m_webViewEnvironment2;
	wil::com_ptr<ICoreWebView2>               m_webView;
	wil::com_ptr<ICoreWebView2_2>             m_webView2;
	wil::com_ptr<ICoreWebView2_16>            m_webView2_16;
	wil::com_ptr<ICoreWebView2Controller>     m_webController;
	wil::com_ptr<ICoreWebView2Settings>       m_webSettings;
};

void ShowFailure(HRESULT hr, CString const& message)
{
	CString text;
	text.Format(L"%s (0x%08X)", (LPCTSTR)message, hr);

	::MessageBox(nullptr, static_cast<LPCTSTR>(text), L"Failure", MB_OK);
}

void CheckFailure(HRESULT hr, CString const& message)
{
	if (FAILED(hr))
	{
		CString text;
		text.Format(L"%s : 0x%08X", (LPCTSTR)message, hr);

		// TODO: log text

		std::exit(hr);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CWebBrowser

IMPLEMENT_DYNCREATE(CWebBrowser, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CWebBrowser properties
BEGIN_MESSAGE_MAP(CWebBrowser, CWnd)
END_MESSAGE_MAP()

CWebBrowser::CWebBrowser() :m_pImpl(new CWebBrowserImpl())
{
	m_callbacks[CallbackType::CreationCompleted] = nullptr;
	m_callbacks[CallbackType::NavigationCompleted] = nullptr;
	m_callbacks[CallbackType::AcceleratorKey] = nullptr;
	m_callbacks[CallbackType::WebMessageReceived] = nullptr;

	m_hMain = nullptr;
	m_fDPIScaleX = 0;
	m_dbScale = 0;

	m_navigationCompletedToken = { 0 };
	m_navigationStartingToken = { 0 };
	m_documentTitleChangedToken = { 0 };
	m_acceleratorKeyPressedToken = { 0 };
	m_webResourceResponseReceivedToken = { 0 };
	m_webMessageReceivedToken = { 0 };
		
	m_callbacks.clear();
}

CWebBrowser::~CWebBrowser()
{
	CloseWebView();

	// 윈도우 핸들 사용자 데이터 제거
	if (m_hWnd)
	{
		SetWindowLongPtr(m_hWnd, GWLP_USERDATA, 0);
	}	
	
	delete m_pImpl;
	m_pImpl = nullptr;
}

BOOL CWebBrowser::CreateHostWindow(
	LPCTSTR lpszClassName,
	LPCTSTR lpszWindowName,
	DWORD dwStyle,
	const RECT& rect,
	CWnd* pParentWnd,
	UINT nID)
{
	if (lpszClassName == nullptr)
		lpszClassName = GetWindowClass();

	if (!CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID))
		return FALSE;

	::SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);

	return TRUE;
}

BOOL CWebBrowser::Create(
	LPCTSTR lpszClassName,
	LPCTSTR lpszWindowName,
	DWORD dwStyle,
	const RECT& rect,
	CWnd* pParentWnd,
	UINT nID,
	CCreateContext*)
{
	if (!CreateHostWindow(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID))
		return FALSE;

	InitializeWebView();

	return TRUE;
}

BOOL CWebBrowser::CreateAsync(
	DWORD dwStyle,
	const RECT& rect,
	CWnd* pParentWnd,
	UINT nID,
	CallbackFunc onCreated)
{
	if (!CreateHostWindow(nullptr, nullptr, dwStyle, rect, pParentWnd, nID))
		return FALSE;

	m_callbacks[CallbackType::CreationCompleted] = onCreated;

	InitializeWebView();

	return TRUE;
}

void CWebBrowser::RegisterCallback(CallbackType const type, CallbackFunc callback)
{
	m_callbacks[type] = callback;
}

void CWebBrowser::CloseWebView()
{
	if (!m_pImpl) return;

	// WebView 관련 해제
	if (m_pImpl->m_webView)
	{
		m_pImpl->m_webView->remove_NavigationCompleted(m_navigationCompletedToken);
		m_pImpl->m_webView->remove_NavigationStarting(m_navigationStartingToken);
		m_pImpl->m_webView->remove_DocumentTitleChanged(m_documentTitleChangedToken);
		m_pImpl->m_webView2->remove_WebResourceResponseReceived(m_webResourceResponseReceivedToken);
		m_pImpl->m_webView->remove_WebMessageReceived(m_webMessageReceivedToken);
	}

	// AcceleratorKeyPressed 이벤트 해제
	if (m_acceleratorKeyPressedToken.value != 0 && m_pImpl->m_webController)
	{
		m_pImpl->m_webController->remove_AcceleratorKeyPressed(m_acceleratorKeyPressedToken);
	}

	// 컨트롤러 닫기
	if (m_pImpl->m_webController)
	{
		m_pImpl->m_webController->Close();
		m_pImpl->m_webController = nullptr;
	}

	//WebView 및 관련 인터페이스 해제
	m_pImpl->m_webView = nullptr;
	m_pImpl->m_webView2 = nullptr;
	m_pImpl->m_webView2_16 = nullptr;
	m_pImpl->m_webSettings = nullptr;

	// WebView 환경 해제
	m_pImpl->m_webViewEnvironment2 = nullptr;
	m_pImpl->m_webViewEnvironment = nullptr;
}

void CWebBrowser::InitializeWebView()
{
	CloseWebView();

	CString subFolder = GetInstallPath();
	CString appData = GetUserDataFolder();
	ICoreWebView2EnvironmentOptions* options = nullptr;

	HRESULT hr;
	if (_taccess(subFolder, 0) != -1)
	{
		hr = CreateCoreWebView2EnvironmentWithOptions(
			subFolder,
			appData,
			options,
			Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
				this,
				&CWebBrowser::OnCreateEnvironmentCompleted).Get());
	}
	else
	{
		hr = CreateCoreWebView2EnvironmentWithOptions(
			nullptr,
			appData,
			options,
			Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
				this,
				&CWebBrowser::OnCreateEnvironmentCompleted).Get());
	}

	if (!SUCCEEDED(hr))
	{
		CString text;
		if (hr == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
		{
			text = L"Cannot found the WebView2 component.";
		}
		else
		{
			text = L"Cannot create the webview environment.";
		}

		ShowFailure(hr, text);
	}
}

HRESULT CWebBrowser::OnCreateEnvironmentCompleted(
	HRESULT result,
	ICoreWebView2Environment* environment)
{
	CHECK_FAILURE(result);

	if (!environment)
		return E_FAIL;

	CHECK_FAILURE(environment->QueryInterface(IID_PPV_ARGS(&m_pImpl->m_webViewEnvironment)));
	CHECK_FAILURE(environment->QueryInterface(IID_PPV_ARGS(&m_pImpl->m_webViewEnvironment2)));

	CHECK_FAILURE(m_pImpl->m_webViewEnvironment->CreateCoreWebView2Controller(
		m_hWnd,
		Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
			this,
			&CWebBrowser::OnCreateWebViewControllerCompleted).Get()));

	return S_OK;
}

HRESULT CWebBrowser::OnCreateWebViewControllerCompleted(
	HRESULT result,
	ICoreWebView2Controller* controller)
{
	if (result == S_OK)
	{
		if (controller != nullptr)
		{
			m_pImpl->m_webController = controller;
			CHECK_FAILURE(controller->get_CoreWebView2(&m_pImpl->m_webView));

			if (!m_pImpl->m_webView)
				return E_FAIL;

			CHECK_FAILURE(m_pImpl->m_webView->QueryInterface(IID_PPV_ARGS(&m_pImpl->m_webView2)));
			CHECK_FAILURE(m_pImpl->m_webView->QueryInterface(IID_PPV_ARGS(&m_pImpl->m_webView2_16)));

			CHECK_FAILURE(m_pImpl->m_webView->get_Settings(&m_pImpl->m_webSettings));


#ifdef _DEBUG
			CHECK_FAILURE(m_pImpl->m_webSettings->put_AreDevToolsEnabled(TRUE));
#else
			CHECK_FAILURE(m_pImpl->m_webSettings->put_AreDevToolsEnabled(FALSE));
#endif

			// 상태 표시줄 비활성화 추가
			CHECK_FAILURE(m_pImpl->m_webSettings->put_IsStatusBarEnabled(FALSE));

			// 기본 컨텍스트 메뉴 비활성화 추가
			CHECK_FAILURE(m_pImpl->m_webSettings->put_AreDefaultContextMenusEnabled(FALSE));

			RegisterEventHandlers();

			ResizeToClientArea();
		}

		auto callback = m_callbacks[CallbackType::CreationCompleted];
		if (callback != nullptr)
			RunAsync(callback);
	}
	else
	{
		ShowFailure(result, L"Cannot create webview environment.");
	}

	return S_OK;
}

void CWebBrowser::RegisterEventHandlers()
{
	// NavigationCompleted handler
	CHECK_FAILURE(m_pImpl->m_webView->add_NavigationCompleted(
		Callback<ICoreWebView2NavigationCompletedEventHandler>(
			[this](
				ICoreWebView2*,
				ICoreWebView2NavigationCompletedEventArgs* args) -> HRESULT
			{
				m_isNavigating = false;

				BOOL success;
				CHECK_FAILURE(args->get_IsSuccess(&success));

				if (!success)
				{
					COREWEBVIEW2_WEB_ERROR_STATUS webErrorStatus{};
					CHECK_FAILURE(args->get_WebErrorStatus(&webErrorStatus));
					if (webErrorStatus == COREWEBVIEW2_WEB_ERROR_STATUS_DISCONNECTED)
					{
						// Do something here if you want to handle a specific error case.
						// In most cases this isn't necessary, because the WebView will
						// display its own error page automatically.
					}
				}

				//스크롤 숨김 - 특정페이지만
				if (success)
				{
					wil::unique_cotaskmem_string uri;
					m_pImpl->m_webView->get_Source(&uri);
					if (wcsstr(uri.get(), L"webrtc.html") != nullptr)
					{
						m_pImpl->m_webView->ExecuteScript(
							L"document.documentElement.style.overflow='hidden';"
							L"document.body.style.overflow='hidden';",
							nullptr);
					}
				}

				/*
				//스크롤 숨김 - 전체
				if (success)
				{
					m_pImpl->m_webView->ExecuteScript(
						L"document.documentElement.style.overflow='hidden';"
						L"document.body.style.overflow='hidden';",
						nullptr);
				}
				*/

				wil::unique_cotaskmem_string uri;
				m_pImpl->m_webView->get_Source(&uri);

				if (wcscmp(uri.get(), L"about:blank") == 0)
				{
					uri = wil::make_cotaskmem_string(L"");
				}

				auto callback = m_callbacks[CallbackType::NavigationCompleted];
				if (callback != nullptr)
					RunAsync(callback);

				return S_OK;
			})
		.Get(),
		&m_navigationCompletedToken));

	// NavigationStarting handler
	CHECK_FAILURE(m_pImpl->m_webView->add_NavigationStarting(
		Callback<ICoreWebView2NavigationStartingEventHandler>(
			[this](
				ICoreWebView2*,
				ICoreWebView2NavigationStartingEventArgs* args) -> HRESULT
			{
				wil::unique_cotaskmem_string uri;
				CHECK_FAILURE(args->get_Uri(&uri));

				m_isNavigating = true;

				return S_OK;
			}).Get(), &m_navigationStartingToken));

	// DocumentTitleChanged handler
	CHECK_FAILURE(m_pImpl->m_webView->add_DocumentTitleChanged(
		Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
			[this](ICoreWebView2* sender, IUnknown* args) -> HRESULT {
				UNREFERENCED_PARAMETER(args);
				wil::unique_cotaskmem_string title;
				CHECK_FAILURE(sender->get_DocumentTitle(&title));

				m_strTitle = title.get();

				auto callback = m_callbacks[CallbackType::TitleChanged];
				if (callback != nullptr)
					RunAsync(callback);

				return S_OK;
			})
		.Get(), &m_documentTitleChangedToken));

	CHECK_FAILURE(m_pImpl->m_webView2->add_WebResourceResponseReceived(
		Callback<ICoreWebView2WebResourceResponseReceivedEventHandler>(
			[this](
				ICoreWebView2* sender,
				ICoreWebView2WebResourceResponseReceivedEventArgs* args) {
					wil::com_ptr<ICoreWebView2WebResourceRequest> request;
					wil::com_ptr<ICoreWebView2WebResourceResponseView> response;
					wil::unique_cotaskmem_string uri;
					CHECK_FAILURE(args->get_Request(&request));
					CHECK_FAILURE(request->get_Uri(&uri));
					CHECK_FAILURE(args->get_Response(&response));

					if (wcscmp(uri.get(), L"https://www.google.com") == 0 )
					{
						wil::com_ptr<ICoreWebView2HttpResponseHeaders> responseHeaders;
						CHECK_FAILURE(response->get_Headers(&responseHeaders));

						wil::unique_cotaskmem_string authHeaderValue;
						if (responseHeaders->GetHeader(_T("td"), &authHeaderValue) == S_OK)
						{
							CString* strResult = new CString;
							*strResult = authHeaderValue.get();
							strResult->Remove('"');
							if (m_hMain != NULL && *strResult != _T(""))
							{
								
								::PostMessage(m_hMain, WM_USER_WEBDATA1, NULL, LPARAM(strResult));
							}
						}
					}								
					else if (wcscmp(uri.get(), L"https://www.naver.com/") == 0 )
					{
						if (m_hMain != NULL)
						{
							::PostMessage(m_hMain, WM_USER_WEBDATA2, NULL, NULL);
						}
					}
					return S_OK;
			})
		.Get(), &m_webResourceResponseReceivedToken));

	//AcceleratorKey
	CHECK_FAILURE(m_pImpl->m_webController->add_AcceleratorKeyPressed(
		Callback<ICoreWebView2AcceleratorKeyPressedEventHandler>(
			[this](ICoreWebView2Controller* /*controller*/, ICoreWebView2AcceleratorKeyPressedEventArgs* args) -> HRESULT
			{		
				COREWEBVIEW2_KEY_EVENT_KIND kind;
				if (FAILED(args->get_KeyEventKind(&kind)) || kind != COREWEBVIEW2_KEY_EVENT_KIND_KEY_DOWN)
				{
					return S_OK;
				}

				UINT32 key = 0;
				if (FAILED(args->get_VirtualKey(&key)))
				{
					return S_OK;
				}

				const bool ctrl = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
				const bool shift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
				const bool alt = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;

				if (key == VK_F1)
				{
					args->put_Handled(TRUE);
					return S_OK;
				}

				if (key == 'Y' && ctrl && shift && alt)
				{
					args->put_Handled(TRUE);
					auto callback = m_callbacks[CallbackType::AcceleratorKey];
					if (callback)
						RunAsync(callback);
					return S_OK;
				}

				// Ctrl + Shift + R (Refresh)
				if (ctrl && shift && key == 'R')
				{
					args->put_Handled(TRUE);
					return S_OK;
				}

				if (key == VK_CANCEL && ctrl)
				{
					args->put_Handled(TRUE);
					return S_OK;
				}
				
				return S_OK;
			})
		.Get(), &m_acceleratorKeyPressedToken));

	//WebMessageReceived
	CHECK_FAILURE(m_pImpl->m_webView->add_WebMessageReceived(
		Callback<ICoreWebView2WebMessageReceivedEventHandler>(
			[this](ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT {
				UNREFERENCED_PARAMETER(args);
				wil::unique_cotaskmem_string message;
				CHECK_FAILURE(args->TryGetWebMessageAsString(&message));

				m_strReceive = message.get();

				auto callback = m_callbacks[CallbackType::WebMessageReceived];
				if (callback != nullptr)
					RunAsync(callback);

				return S_OK;
			})
		.Get(), &m_webMessageReceivedToken));
}

void CWebBrowser::ResizeToClientArea()
{
	if (m_pImpl->m_webController)
	{
		RECT bounds;
		GetClientRect(&bounds);
		m_pImpl->m_webController->put_Bounds(bounds);

		CDC* pDC = GetDC();
		m_fDPIScaleX = GetDeviceCaps(pDC->GetSafeHdc(), LOGPIXELSX) / 96.0f;

		if (m_fDPIScaleX != 1.0f)
		{
			m_dbScale = 1 / m_fDPIScaleX;
			m_pImpl->m_webController->put_ZoomFactor(m_dbScale);
		}
	}
}

RECT CWebBrowser::GetBounds()
{
	RECT rc{ 0,0,0,0 };
	if (m_pImpl->m_webController)
	{
		m_pImpl->m_webController->get_Bounds(&rc);
	}

	return rc;
}

void CWebBrowser::Resize(LONG const width, LONG const height)
{
	SetWindowPos(nullptr, 0, 0, width, height, SWP_NOMOVE | SWP_NOREPOSITION);
}

CString CWebBrowser::GetLocationURL()
{
	CString url;
	if (m_pImpl->m_webView)
	{
		wil::unique_cotaskmem_string uri;
		m_pImpl->m_webView->get_Source(&uri);

		if (wcscmp(uri.get(), L"about:blank") == 0)
		{
			uri = wil::make_cotaskmem_string(L"");
		}

		url = uri.get();
	}

	return url;
}

CString CWebBrowser::NormalizeUrl(CString url)
{
	if (url.Find(_T("://")) < 0)
	{
		if (url.GetLength() > 1 && url[1] == ':')
			url = _T("file://") + url;
		else
			url = _T("http://") + url;
	}

	return url;
}

void CWebBrowser::NavigateTo(CString url)
{
	m_pImpl->m_webView->Navigate(NormalizeUrl(url));
}

void CWebBrowser::Navigate(CString const& url, CallbackFunc onComplete)
{
	if (m_pImpl->m_webView)
	{
		m_callbacks[CallbackType::NavigationCompleted] = onComplete;
		NavigateTo(url);
	}
}

void CWebBrowser::NavigatePost(CString const& url, CString const& content, CString const& headers, std::function<void()> onComplete)
{
	if (!m_pImpl->m_webView) return;

	CString normalizedUrl{ NormalizeUrl(url) };

	m_callbacks[CallbackType::NavigationCompleted] = onComplete;

	wil::com_ptr<ICoreWebView2WebResourceRequest> webResourceRequest;
	wil::com_ptr<IStream> postDataStream = SHCreateMemStream(
		reinterpret_cast<const BYTE*>(static_cast<LPCTSTR>(content)),
		content.GetLength() + 1);

	CHECK_FAILURE(m_pImpl->m_webViewEnvironment2->CreateWebResourceRequest(
		CT2W(normalizedUrl),
		L"POST",
		postDataStream.get(),
		CT2W(headers),
		&webResourceRequest));

	CHECK_FAILURE(m_pImpl->m_webView2->NavigateWithWebResourceRequest(webResourceRequest.get()));
}

void CWebBrowser::NavigatePost(CString const& url, CString const& content, CString const& headers, HWND hMain, std::function<void()> onComplete)
{
	if (!m_pImpl->m_webView) return;

	m_hMain = hMain;

	CString normalizedUrl{ NormalizeUrl(url) };

	m_callbacks[CallbackType::NavigationCompleted] = onComplete;

	wil::com_ptr<ICoreWebView2WebResourceRequest> webResourceRequest;
	wil::com_ptr<IStream> postDataStream = SHCreateMemStream(
		reinterpret_cast<const BYTE*>(static_cast<LPCTSTR>(content)),
		content.GetLength() + 1);

	CHECK_FAILURE(m_pImpl->m_webViewEnvironment2->CreateWebResourceRequest(
		CT2W(normalizedUrl),
		L"POST",
		postDataStream.get(),
		CT2W(headers),
		&webResourceRequest));

	CHECK_FAILURE(m_pImpl->m_webView2->NavigateWithWebResourceRequest(webResourceRequest.get()));
}

void CWebBrowser::PrintDocument()
{
	if (m_pImpl->m_webView)
	{
		m_pImpl->m_webView->ExecuteScript(L"window.print();", nullptr);
	}
}

void CWebBrowser::Stop()
{
	if (m_pImpl->m_webView)
	{
		m_pImpl->m_webView->Stop();
	}
}

void CWebBrowser::Reload()
{
	if (m_pImpl->m_webView)
	{
		m_pImpl->m_webView->Reload();
	}
}

void CWebBrowser::GoBack()
{
	if (m_pImpl->m_webView)
	{
		BOOL possible = FALSE;
		m_pImpl->m_webView->get_CanGoBack(&possible);
		if (possible)
			m_pImpl->m_webView->GoBack();
	}
}

void CWebBrowser::GoForward()
{
	if (m_pImpl->m_webView)
	{
		BOOL possible = FALSE;
		m_pImpl->m_webView->get_CanGoForward(&possible);
		if (possible)
			m_pImpl->m_webView->GoForward();
	}
}

void CWebBrowser::DisablePopups()
{
	if (m_pImpl->m_webSettings)
	{
		m_pImpl->m_webSettings->put_AreDefaultScriptDialogsEnabled(FALSE);
	}
}

PCTSTR CWebBrowser::GetWindowClass()
{
	static PCTSTR windowClass = []
		{
			static TCHAR const* className = L"EdgeBrowserHost";

			WNDCLASSEX wcex;
			wcex.cbSize = sizeof(WNDCLASSEX);

			wcex.style = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = WndProcStatic;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = 0;
			wcex.hInstance = AfxGetInstanceHandle();
			wcex.hIcon = nullptr;
			wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
			wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			wcex.lpszMenuName = nullptr;
			wcex.lpszClassName = className;
			wcex.hIconSm = nullptr;

			ATOM result = RegisterClassEx(&wcex);
			if (result == 0)
			{
				[[maybe_unused]] DWORD lastError = ::GetLastError();
			}

			return className;
		}();

	return windowClass;
}

LRESULT CALLBACK CWebBrowser::WndProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (auto app = (CWebBrowser*)::GetWindowLongPtr(hWnd, GWLP_USERDATA))
	{
		LRESULT result = 0;
		if (app->HandleWindowMessage(hWnd, message, wParam, lParam, &result))
		{
			return result;
		}
	}

	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

bool CWebBrowser::HandleWindowMessage(
	HWND, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* result)
{
	*result = 0;

	switch (message)
	{
		case WM_SIZE:
		{
			if (lParam != 0)
			{
				ResizeToClientArea();
				return true;
			}
			break;
		}
		case MSG_RUN_ASYNC_CALLBACK:
		{
			auto* task = reinterpret_cast<CallbackFunc*>(wParam);
			(*task)();
			delete task;
			return true;
		}
		break;
	}

	return false;
}

void CWebBrowser::RunAsync(CallbackFunc callback)
{
	auto* task = new CallbackFunc(callback);
	PostMessage(MSG_RUN_ASYNC_CALLBACK, reinterpret_cast<WPARAM>(task), 0);
}

bool CWebBrowser::IsWebViewCreated() const
{
	return m_pImpl->m_webView != nullptr;
}

CString CWebBrowser::GetInstallPath()
{
	CString path = GetInstallPathFromDisk();
	if (path.IsEmpty())
	{
		path = GetInstallPathFromRegistry();
	}
	return path;
}

CString CWebBrowser::GetInstallPathFromRegistry()
{
	CString path;
	HKEY handle = nullptr;
	LSTATUS result = ERROR_FILE_NOT_FOUND;

	result = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Microsoft EdgeWebView)",
		0,
		KEY_READ,
		&handle);

	if (result != ERROR_SUCCESS)
		result = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
			LR"(SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Uninstall\Microsoft EdgeWebView)",
			0,
			KEY_READ,
			&handle);

	if (result == ERROR_SUCCESS)
	{
		TCHAR buffer[MAX_PATH + 1] = { 0, };
		DWORD type = REG_SZ;
		DWORD size = MAX_PATH;
		result = RegQueryValueEx(handle, L"InstallLocation", 0, &type, reinterpret_cast<LPBYTE>(buffer), &size);
		if (result == ERROR_SUCCESS)
			path += CString{ buffer };

		TCHAR version[100 + 1] = { 0, };
		size = 100;
		result = RegQueryValueEx(handle, L"Version", 0, &type, reinterpret_cast<LPBYTE>(version), &size);
		if (result == ERROR_SUCCESS)
		{
			if (path.GetAt(path.GetLength() - 1) != L'\\')
				path += L"\\";
			path += CString{ version };
		}
		else
			path.Empty();

		RegCloseKey(handle);
	}

	return path;
}

CString CWebBrowser::GetInstallPathFromDisk()
{
	CString path = GetProgramFilesFolder() + L"\\Microsoft\\EdgeWebView\\Application\\";
	if (_taccess(path, 0) == -1)
	{
		path.Replace(_T("Program Files"), _T("Program Files (x86)"));
		if (_taccess(path, 0) == -1) return {};
	}

	WIN32_FIND_DATA ffd{ 0 };
	HANDLE hFind = FindFirstFile(path + _T("*"), &ffd);
	if (hFind == INVALID_HANDLE_VALUE) return {};

	std::list<std::wstring> version;

	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			int major, minor, build, revision;
			if (_stscanf_s(ffd.cFileName, _T("%d.%d.%d.%d"), &major, &minor, &build, &revision) == 4)
			{
				version.push_back(ffd.cFileName);
			}
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	FindClose(hFind);

	if (version.size() <= 0) return {};

	version.sort([](const std::wstring& _lhs, const std::wstring& _rhs) { return _lhs > _rhs; });

	return path + version.begin()->c_str();
}

CString CWebBrowser::GetUserDataFolder()
{
	TCHAR szPath[MAX_PATH]{ 0 };
	::SHGetFolderPath(nullptr, CSIDL_LOCAL_APPDATA, nullptr, 0, szPath);
	::PathAppend(szPath, LR"(\Myfolder\)");
	::PathAppend(szPath, L"Myfolder");
	return CString{ szPath };
}

CString CWebBrowser::GetProgramFilesFolder()
{
	TCHAR szPath[MAX_PATH]{ 0 };
	::SHGetSpecialFolderPath(nullptr, szPath, CSIDL_PROGRAM_FILES, FALSE);
	return CString{ szPath };
}

void CWebBrowser::GetSelectedText(TextSelectionFunc callback)
{
	if (m_pImpl->m_webView != nullptr)
	{
		m_pImpl->m_webView->ExecuteScript(L"window.getSelection().toString()",
			Callback<ICoreWebView2ExecuteScriptCompletedHandler>(
				[callback](HRESULT error, PCWSTR result) -> HRESULT
				{
					if (error != S_OK) {
						ShowFailure(error, L"ExecuteScript failed");
					}

					CW2T text(result);
					callback(CString(text.m_psz));

					return S_OK;
				}).Get());
	}
}

void CWebBrowser::ExecuteScript(CString const& code)
{
	if (m_pImpl->m_webView != nullptr)
	{
		m_pImpl->m_webView->ExecuteScript(CT2W(code).m_psz,
			Callback<ICoreWebView2ExecuteScriptCompletedHandler>(
				[](HRESULT error, PCWSTR result) -> HRESULT
				{
					UNREFERENCED_PARAMETER(result);
					if (error != S_OK) {
						ShowFailure(error, L"ExecuteScript failed");
					}

					return S_OK;
				}).Get());
	}
}

void CWebBrowser::ShowPrintUI(bool const systemDialog)
{
	if (m_pImpl->m_webView != nullptr)
	{
		CHECK_FAILURE(m_pImpl->m_webView2_16->ShowPrintUI(systemDialog ? COREWEBVIEW2_PRINT_DIALOG_KIND_SYSTEM : COREWEBVIEW2_PRINT_DIALOG_KIND_BROWSER));
	}
}

void CWebBrowser::PrintToPDF(bool const landscape, std::function<void(bool, CString)> callback)
{
	WCHAR defaultName[MAX_PATH] = L"default.pdf";

	OPENFILENAME openFileName = {};
	openFileName.lStructSize = sizeof(openFileName);
	openFileName.hwndOwner = nullptr;
	openFileName.hInstance = nullptr;
	openFileName.lpstrFile = defaultName;
	openFileName.lpstrFilter = L"PDF File\0*.pdf\0";
	openFileName.nMaxFile = MAX_PATH;
	openFileName.Flags = OFN_OVERWRITEPROMPT;

	if (::GetSaveFileName(&openFileName))
	{
		wil::com_ptr<ICoreWebView2PrintSettings> printSettings = nullptr;
		wil::com_ptr<ICoreWebView2Environment6> webviewEnvironment6;
		CHECK_FAILURE(m_pImpl->m_webViewEnvironment->QueryInterface(IID_PPV_ARGS(&webviewEnvironment6)));

		if (webviewEnvironment6)
		{
			CHECK_FAILURE(webviewEnvironment6->CreatePrintSettings(&printSettings));
			CHECK_FAILURE(printSettings->put_Orientation(landscape ? COREWEBVIEW2_PRINT_ORIENTATION_LANDSCAPE :
				COREWEBVIEW2_PRINT_ORIENTATION_PORTRAIT));
		}

		wil::com_ptr<ICoreWebView2_7> webview2_7;
		CHECK_FAILURE(m_pImpl->m_webView->QueryInterface(IID_PPV_ARGS(&webview2_7)));
		if (webview2_7)
		{
			m_printToPdfInProgress = true;

			CHECK_FAILURE(webview2_7->PrintToPdf(
				openFileName.lpstrFile,
				printSettings.get(),
				Callback<ICoreWebView2PrintToPdfCompletedHandler>(
					[this, callback, &openFileName](HRESULT errorCode, BOOL isSuccessful) -> HRESULT {
						CHECK_FAILURE(errorCode);

						m_printToPdfInProgress = false;

						callback(isSuccessful, openFileName.lpstrFile);

						return S_OK;
					})
				.Get()));
		}
	}
}
