#include "pch.h"
#include "stringutil.h"

std::string stringutil::Mid(const std::string& _Str, size_t _Off, size_t _Count)
{
	return _Str.substr(_Off, _Count);
}

std::string stringutil::Left(const std::string& _Str, size_t _Count)
{
	return _Str.substr(0, _Count);
}

std::string stringutil::Right(const std::string& _Str, size_t _Count)
{
	return _Str.substr(_Str.size() - _Count, _Count);
}

std::string stringutil::MakeLower(const std::string& _Str)
{
	std::string _DStr;
	std::transform(_Str.begin(), _Str.end(), std::back_inserter(_DStr), ::tolower);
	//std::transform(_Str.begin(), _Str.end(), _Str.begin(), ::tolower);
	return _DStr;
}

std::string stringutil::MakeUpper(const std::string& _Str)
{
	std::string _DStr;
	std::transform(_Str.begin(), _Str.end(), std::back_inserter(_DStr), ::toupper);
	//std::transform(_Str.begin(), _Str.end(), _Str.begin(), ::toupper);
	return _DStr;
}

size_t stringutil::Find(const std::string& _Str, const std::string& _Strsub, size_t _Off)
{
	return _Str.find(_Strsub, _Off);
}

size_t stringutil::Find(const std::string& _Str, char ch, size_t _Off)
{
	return _Str.find(ch, _Off);
}

size_t stringutil::ReserveFind(const std::string& _Str, const std::string& _Strsub)
{
	return _Str.rfind(_Strsub);
}

size_t stringutil::ReserveFind(const std::string& _Str, char ch)
{
	return _Str.rfind(ch);
}

size_t stringutil::Split(std::string& _Str, std::string& _Strsub, std::vector<std::string>& _Vec)
{
	_Vec.clear();

	std::string::size_type pos;

	pos = _Str.find(_Strsub);
	while (pos != std::string::npos) {

		_Vec.push_back(_Str.substr(0, pos));

		_Str = _Str.substr(pos + 1);
		pos = _Str.find(_Strsub);
	}

	_Vec.push_back(_Str);

	return _Vec.size();
}

std::string stringutil::Trim(const std::string& _Str)
{
	std::string _DStr{ _Str };

	_DStr = TrimLeft(_DStr);
	_DStr = TrimRight(_DStr);

	return _DStr;
}

std::string stringutil::TrimLeft(const std::string& _Str)
{
	std::string _DStr{ _Str };

	size_t nPos = _DStr.find_first_not_of(" \t\r");
	if (nPos > 0)
		_DStr = _DStr.substr(nPos);

	return _DStr;
}

std::string stringutil::TrimRight(const std::string& _Str)
{
	std::string _DStr{ _Str };

	size_t nPos = _DStr.find_last_not_of(" \t\r");

	if (nPos > 0)
		_DStr = _DStr.substr(0, nPos + 1);
	return _DStr;
}

std::string stringutil::Reserve(const std::string& _Str)
{
	std::string _DStr = { };

	for (int i = _Str.size() - 1; i >= 0; --i) {
		_DStr += _Str[i];
	}

	return _DStr;
}

std::string stringutil::Replace(const std::string& _Str, const std::string& _Search, const std::string& _Replace)
{
	std::string _DStr = { _Str };

	size_t pos = 0;
	while ((pos = _DStr.find(_Search, pos)) != std::string::npos)
	{
		_DStr.replace(pos, _Search.length(), _Replace);

		pos += _Replace.length();
	}
	return _DStr;
}

std::string stringutil::GetFilePath(const std::string& _Str)
{
	size_t nPos = _Str.rfind("\\");
	if (nPos > 0)
		return Left(_Str, nPos);

	return _Str;
}

std::string stringutil::GetFileName(const std::string& _Str)
{
	size_t nPos = _Str.rfind("\\");
	if (nPos > 0)
		return _Str.substr(nPos + 1);

	return _Str;
}

std::string stringutil::RemoveExt(const std::string& _Str)
{
	return _Str.substr(0, _Str.find_last_of("."));
}

std::string stringutil::GetFileExt(const std::string& _Str)
{
	size_t nPos = _Str.rfind('.');
	if (nPos > 0)
		return _Str.substr(nPos + 1);

	return _Str;
}

std::string stringutil::GetFindStr(const std::string& _Str, const std::string& _left, const std::string& _right)
{
	size_t nleft = _Str.find(_left);
	size_t nright = _Str.find(_right);

	if (nleft > 0 && nright > 0)
		return Mid(_Str, nleft, (nright - nleft));//return Mid(_Str, nleft + 1, (nright - nleft - 1));

	return _Str;
}