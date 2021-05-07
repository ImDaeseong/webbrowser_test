#pragma once
class TCHARutil
{
public:
	static TCHAR* Left(TCHAR* _Str, size_t _Count);
	static TCHAR* Right(TCHAR* _Str, size_t _Count);
	static TCHAR* Mid(TCHAR* _Str, size_t _Off, size_t _Count);
	static TCHAR* MakeLower(TCHAR* _Str);
	static TCHAR* MakeUpper(TCHAR* _Str);
	static size_t Find(TCHAR* _Str, TCHAR* _Strsub);
	static size_t Find(TCHAR* _Str, TCHAR ch);
	static size_t ReserveFind(TCHAR* _Str, TCHAR ch);
	static size_t Split(const TCHAR* _Str, const TCHAR* _Strsub, std::vector<TCHAR*>& _Vec);
	static TCHAR* Trim(TCHAR* _Str);
	static TCHAR* TrimLeft(TCHAR* _Str);
	static TCHAR* TrimRight(TCHAR* _Str);
	static CString Replace(TCHAR* _Str, TCHAR* _Search, TCHAR* _Replace);
	static TCHAR* Reserve(TCHAR* _Str);
};

