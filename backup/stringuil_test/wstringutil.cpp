#include "pch.h"
#include "wstringutil.h"

std::wstring wstringutil::Mid(const std::wstring& _Str, size_t _Off, size_t _Count)
{
	return _Str.substr(_Off, _Count);
}

std::wstring wstringutil::Left(const std::wstring& _Str, size_t _Count)
{
	return _Str.substr(0, _Count);
}

std::wstring wstringutil::Right(const std::wstring& _Str, size_t _Count)
{
	return _Str.substr(_Str.size() - _Count, _Count);
}

std::wstring wstringutil::MakeLower(const std::wstring& _Str)
{
	std::wstring _DStr;
	std::transform(_Str.begin(), _Str.end(), std::back_inserter(_DStr), ::tolower);
	//std::transform(_Str.begin(), _Str.end(), _Str.begin(), ::tolower);
	return _DStr;
}

std::wstring wstringutil::MakeUpper(const std::wstring& _Str)
{
	std::wstring _DStr;
	std::transform(_Str.begin(), _Str.end(), std::back_inserter(_DStr), ::toupper);
	//std::transform(_Str.begin(), _Str.end(), _Str.begin(), ::toupper);
	return _DStr;
}

size_t wstringutil::Find(const std::wstring& _Str, const std::wstring& _Strsub, size_t _Off)
{
	return _Str.find(_Strsub, _Off);
}

size_t wstringutil::Find(const std::wstring& _Str, char ch, size_t _Off)
{
	return _Str.find(ch, _Off);
}

size_t wstringutil::ReserveFind(const std::wstring& _Str, const std::wstring& _Strsub)
{
	return _Str.rfind(_Strsub);
}

size_t wstringutil::ReserveFind(const std::wstring& _Str, char ch)
{
	return _Str.rfind(ch);
}

size_t wstringutil::Split(std::wstring& _Str, std::wstring& _Strsub, std::vector<std::wstring>& _Vec)
{
	_Vec.clear();

	std::wstring::size_type pos;

	pos = _Str.find(_Strsub);
	while (pos != std::string::npos) {

		_Vec.push_back(_Str.substr(0, pos));

		_Str = _Str.substr(pos + 1);
		pos = _Str.find(_Strsub);
	}

	_Vec.push_back(_Str);

	return _Vec.size();
}

std::wstring wstringutil::Trim(const std::wstring& _Str)
{
	std::wstring _DStr{ _Str };

	_DStr = TrimLeft(_DStr);
	_DStr = TrimRight(_DStr);

	return _DStr;
}

std::wstring wstringutil::TrimLeft(const std::wstring& _Str)
{
	std::wstring _DStr{ _Str };

	size_t nPos = _DStr.find_first_not_of(L" \t\r");
	if (nPos > 0)
		_DStr = _DStr.substr(nPos);

	return _DStr;
}

std::wstring wstringutil::TrimRight(const std::wstring& _Str)
{
	std::wstring _DStr{ _Str };

	size_t nPos = _DStr.find_last_not_of(L" \t\r");

	if (nPos > 0)
		_DStr = _DStr.substr(0, nPos + 1);
	return _DStr;
}

std::wstring wstringutil::Reserve(const std::wstring& _Str)
{
	std::wstring _DStr = { };

	for (int i = _Str.size() - 1; i >= 0; --i) {
		_DStr += _Str[i];
	}

	return _DStr;
}

std::wstring wstringutil::Replace(const std::wstring& _Str, const std::wstring& _Search, const std::wstring& _Replace)
{
	std::wstring _DStr = { _Str };

	size_t pos = 0;
	while ((pos = _DStr.find(_Search, pos)) != std::string::npos)
	{
		_DStr.replace(pos, _Search.length(), _Replace);

		pos += _Replace.length();
	}
	return _DStr;
}

std::wstring wstringutil::GetFilePath(const std::wstring& _Str)
{
	size_t nPos = _Str.find_last_of(L"\\");
	if (nPos > 0)
		return Left(_Str, nPos);
	return _Str;

	/*
	size_t nPos = _Str.rfind(L"\\");
	if (nPos > 0)
		return Left(_Str, nPos);

	return _Str;
	*/
}

std::wstring wstringutil::GetFileName(const std::wstring& _Str)
{
	size_t nPos = _Str.find_last_of(L"\\");
	if (nPos > 0)
		return _Str.substr(nPos + 1);

	return _Str;

	/*
	size_t nPos = _Str.rfind(L"\\");
	if (nPos > 0)
		return _Str.substr(nPos + 1);

	return _Str;
	*/
}

std::wstring wstringutil::RemoveExt(const std::wstring& _Str)
{
	return _Str.substr(0, _Str.find_last_of(L"."));
}

std::wstring wstringutil::GetFileExt(const std::wstring& _Str)
{
	size_t nPos = _Str.find_last_of(L'.');
	if (nPos > 0)
		return _Str.substr(nPos + 1);
	return _Str;

	/*
	size_t nPos = _Str.rfind(L'.');
	if (nPos > 0)
		return _Str.substr(nPos + 1);

	return _Str;
	*/
}

std::wstring wstringutil::GetFindStr(const std::wstring& _Str, const std::wstring& _left, const std::wstring& _right)
{
	size_t nleft = _Str.find(_left);
	size_t nright = _Str.find(_right);

	if (nleft > 0 && nright > 0)
		return Mid(_Str, nleft, (nright - nleft));//return Mid(_Str, nleft + 1, (nright - nleft - 1));

	return _Str;
}
