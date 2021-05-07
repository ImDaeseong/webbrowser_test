#pragma once
class stringutil
{
public:
	static std::string Left(const std::string& _Str, size_t _Count);
	static std::string Right(const std::string& _Str, size_t _Count);
	static std::string Mid(const std::string& _Str, size_t _Off, size_t _Count);
	static std::string MakeLower(const std::string& _Str);
	static std::string MakeUpper(const std::string& _Str);
	static size_t Find(const std::string& _Str, const std::string& _Strsub, size_t _Off);
	static size_t Find(const std::string& _Str, char ch, size_t _Off);
	static size_t ReserveFind(const std::string& _Str, const std::string& _Strsub);
	static size_t ReserveFind(const std::string& _Str, char ch);
	static size_t Split(std::string& _Str, std::string& _Strsub, std::vector<std::string>& _Vec);
	static std::string Trim(const std::string& _Str);
	static std::string TrimLeft(const std::string& _Str);
	static std::string TrimRight(const std::string& _Str);
	static std::string Reserve(const std::string& _Str);
	static std::string Replace(const std::string& _Str, const std::string& _Search, const std::string& _Replace);

	static std::string GetFilePath(const std::string& _Str);
	static std::string GetFileName(const std::string& _Str);
	static std::string RemoveExt(const std::string& _Str);
	static std::string GetFileExt(const std::string& _Str);
	static std::string GetFindStr(const std::string& _Str, const std::string& _left, const std::string& _right);
};

