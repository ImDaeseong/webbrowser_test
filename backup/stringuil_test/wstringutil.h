#pragma once
class wstringutil
{
public:
	static std::wstring Left(const std::wstring& _Str, size_t _Count);
	static std::wstring Right(const std::wstring& _Str, size_t _Count);
	static std::wstring Mid(const std::wstring& _Str, size_t _Off, size_t _Count);
	static std::wstring MakeLower(const std::wstring& _Str);
	static std::wstring MakeUpper(const std::wstring& _Str);
	static size_t Find(const std::wstring& _Str, const std::wstring& _Strsub, size_t _Off);
	static size_t Find(const std::wstring& _Str, char ch, size_t _Off);
	static size_t ReserveFind(const std::wstring& _Str, const std::wstring& _Strsub);
	static size_t ReserveFind(const std::wstring& _Str, char ch);
	static size_t Split(std::wstring& _Str, std::wstring& _Strsub, std::vector<std::wstring>& _Vec);
	static std::wstring Trim(const std::wstring& _Str);
	static std::wstring TrimLeft(const std::wstring& _Str);
	static std::wstring TrimRight(const std::wstring& _Str);
	static std::wstring Reserve(const std::wstring& _Str);
	static std::wstring Replace(const std::wstring& _Str, const std::wstring& _Search, const std::wstring& _Replace);

	static std::wstring GetFilePath(const std::wstring& _Str);
	static std::wstring GetFileName(const std::wstring& _Str);
	static std::wstring RemoveExt(const std::wstring& _Str);
	static std::wstring GetFileExt(const std::wstring& _Str);
	static std::wstring GetFindStr(const std::wstring& _Str, const std::wstring& _left, const std::wstring& _right);
};

