#pragma once
class wcharutil
{
public:
	static size_t GetLength(const wchar_t* _Str);
	static bool IsSpace(wchar_t ch);
	static wchar_t* Left(const wchar_t* _Str, size_t _Count);
	static wchar_t* Right(const wchar_t* _Str, size_t _Count);
	static wchar_t* Mid(const wchar_t* _Str, size_t _Off, size_t _Count);
	static wchar_t* MakeLower(const wchar_t* _Str);
	static wchar_t* MakeUpper(const wchar_t* _Str);
	static size_t Find(wchar_t* _Str, const wchar_t* _Strsub);
	static size_t Find(wchar_t* _Str, char ch);
	static size_t ReserveFind(const wchar_t* _Str, char ch);
	static size_t Split(const wchar_t* _Str, const wchar_t* _Strsub, std::vector<wchar_t*>& _Vec);
	static CString Trim(wchar_t* _Str);
	static CString TrimLeft(wchar_t* _Str);
	static CString TrimRight(wchar_t* _Str);
	static CString Replace(const wchar_t* _Str, const wchar_t* _Search, const wchar_t* _Replace);
	static wchar_t* Reserve(wchar_t* _Str);
};

