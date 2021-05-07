#include "pch.h"
#include "TCHARutil.h"

TCHAR* TCHARutil::Left(TCHAR* _Str, size_t _Count)
{
	TCHAR* _buf = new TCHAR[_Count + 1];
	memcpy(_buf, _Str, sizeof(TCHAR) * (_Count + 1));
	_buf[_Count] = 0;
	return _buf;

	/*
	if (_Count > _tcslen(_Str))
	{
		return _Str;
	}

	TCHAR* _buf = new TCHAR[_Count+1];
	for (int i = 0; i < _Count; i++)
	{
		_buf[i] = _Str[i];
	}
	_buf[_Count] = 0;
	return _buf;
	*/	
}

TCHAR* TCHARutil::Right(TCHAR* _Str, size_t _Count)
{
	size_t len = _tcslen(_Str);
	size_t nPos = len - _Count;

	TCHAR* _buf = new TCHAR[_Count + 1];
	memcpy(_buf, _Str + nPos, sizeof(TCHAR) * (_Count + 1));
	_buf[_Count] = 0;
	return _buf;
}

TCHAR* TCHARutil::Mid(TCHAR* _Str, size_t _Off, size_t _Count)
{
	TCHAR* _buf = new TCHAR[_Count + 1];
	memcpy(_buf, _Str + _Off, sizeof(TCHAR) * (_Count + 1));
	_buf[_Count] = 0;
	return _buf;
}

TCHAR* TCHARutil::MakeLower(TCHAR* _Str)
{
	TCHAR* _buf = _tcsdup(_Str);

	size_t i = 0;
	while (_buf[i])
	{
		_buf[i] = _totlower(_buf[i]);
		++i;
	}
	return _buf;
}

TCHAR* TCHARutil::MakeUpper(TCHAR* _Str)
{
	TCHAR* _buf = _tcsdup(_Str);

	size_t i = 0;
	while (_buf[i])
	{
		_buf[i] = _totupper(_buf[i]);
		++i;
	}
	return _buf;
}

size_t TCHARutil::Find(TCHAR* _Str, TCHAR* _Strsub)
{
	int ifind = -1;
	size_t len = _tcslen(_Strsub);

	for (int i = 0; i < _tcslen(_Str); i++)
	{
		TCHAR c = _Str[i];

		for (int j = 0; j < len; ++j)
		{
			if (_Strsub[j] == c)
			{
				ifind = i;
				return ifind;
			}
		}
	}
	return ifind;
}

size_t TCHARutil::Find(TCHAR* _Str, TCHAR ch)
{	
	int ifind = -1;
	for (int i = 0; i < _tcslen(_Str); i++)
	{
		if (_Str[i] == ch)
		{
			ifind = i;
			break;
		}
	}
	return ifind;
}

size_t TCHARutil::ReserveFind(TCHAR* _Str, TCHAR ch)
{
	int ifind = -1;
	for (int i = _tcslen(_Str) - 1; i >= 0; --i)
	{
		if (_Str[i] == ch)
		{
			ifind = i;
			break;
		}
	}
	return ifind;
}

size_t TCHARutil::Split(const TCHAR* _Str, const TCHAR* _Strsub, std::vector<TCHAR*>& _Vec)
{
	_Vec.clear();

	size_t len = _tcslen(_Str);
	TCHAR* _buf = new TCHAR[len + 1];
	memcpy(_buf, _Str, sizeof(TCHAR) * (len + 1));
	_buf[len] = 0;

	TCHAR* p = _buf;
	while (true)
	{
		while (*p && _tcschr(_Strsub, *p) != NULL)
			*p++ = 0;

		if (!*p)
			break;

		_Vec.push_back(p);
		while (*p && _tcschr(_Strsub, *p) == NULL)
			p++;
	}

	return _Vec.size();
}

bool IsWhitespaceNotNull(const TCHAR ch)
{
	return ch > 0 && ch <= 0x20;
}

TCHAR* TCHARutil::Trim(TCHAR* _Str)
{
	TCHAR* _buf = _tcsdup(_Str);

	_buf = TrimLeft(_buf);
	_buf = TrimRight(_buf);
	return _buf;
}

TCHAR* TCHARutil::TrimLeft(TCHAR* _Str)
{
	TCHAR* _buf = _tcsdup(_Str);

	while (IsWhitespaceNotNull(*_buf))
		++_buf;

	return _buf;
}

TCHAR* TCHARutil::TrimRight(TCHAR* _Str)
{
	TCHAR* _buf = _tcsdup(_Str);
	size_t len = _tcslen(_Str);

	while (len > 0 && IsWhitespaceNotNull(_buf[len - 1]))
		--len;

	_buf[len] = 0;
	return _buf;
}

CString TCHARutil::Replace(TCHAR* _Str, TCHAR* _Search, TCHAR* _Replace)
{
	CString strValue(_Str);
	CString strSearch(_Search);
	CString strReplace(_Replace);
	strValue.Replace(strSearch, strReplace);
	//return T2W(strValue.GetBuffer(0));
	return strValue;
}

TCHAR* TCHARutil::Reserve(TCHAR* _Str)
{
	TCHAR* _buf = _tcsdup(_Str);
	size_t len = _tcslen(_Str);

	TCHAR ch = 0;
	for (int i = 0; i < len / 2; i++)
	{
		ch = _buf[i];
		_buf[i] = _buf[len - i - 1];
		_buf[len - i - 1] = ch;
	}
	return _buf;
}