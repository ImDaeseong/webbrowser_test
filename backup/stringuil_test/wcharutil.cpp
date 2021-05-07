#include "pch.h"
#include "wcharutil.h"

size_t wcharutil::GetLength(const wchar_t* _Str)
{
	if (!_Str) return 0;
	return wcslen(_Str);
}

bool wcharutil::IsSpace(wchar_t ch)
{
	if (ch == ' ' || ch == '\r' || ch == '\t' || ch == '\n')
	{
		return true;
	}

	return false;
}

wchar_t* wcharutil::Left(const wchar_t* _Str, size_t _Count)
{
	wchar_t* _buf = new wchar_t[_Count + 1];
	memcpy(_buf, _Str, sizeof(wchar_t) * (_Count + 1));
	_buf[_Count] = L'\0';
	return _buf;

	/*
	size_t len = _tcslen(_Str);

	if (_Count == 0 || _Count > len)
		return _Str;

	wchar_t* buf = new wchar_t[_Count + 1];
	wmemset(buf, 0, _Count + 1);

	for (int i = 0; i < _Count; ++i, ++_Str)
	{
		buf[i] = *_Str;
	}
	return buf;
	*/
}

wchar_t* wcharutil::Right(const wchar_t* _Str, size_t _Count)
{
	size_t len = ::wcslen(_Str);
	size_t nPos = len - _Count;

	wchar_t* _buf = new wchar_t[_Count + 1];
	memcpy(_buf, _Str + nPos, sizeof(wchar_t) * (_Count + 1));
	_buf[_Count] = L'\0';

	return _buf;
}

wchar_t* wcharutil::Mid(const wchar_t* _Str, size_t _Off, size_t _Count)
{
	wchar_t* _buf = new wchar_t[_Count + 1];
	memcpy(_buf, _Str + _Off, sizeof(wchar_t) * (_Count + 1));
	_buf[_Count] = L'\0';

	return _buf;
}

wchar_t* wcharutil::MakeLower(const wchar_t* _Str)
{
	size_t len = ::wcslen(_Str);
	wchar_t* _buf = new wchar_t[len + 1];
	memcpy(_buf, _Str, sizeof(wchar_t) * (len + 1));
	_buf[len] = L'\0';

	::_wcslwr_s(_buf, len + 1);
	return _buf;
}

wchar_t* wcharutil::MakeUpper(const wchar_t* _Str)
{
	size_t len = ::wcslen(_Str);
	wchar_t* _buf = new wchar_t[len + 1];
	memcpy(_buf, _Str, sizeof(wchar_t) * (len + 1));
	_buf[len] = L'\0';

	::_wcsupr_s(_buf, len + 1);
	return _buf;
}

size_t wcharutil::Find(wchar_t* _Str, const wchar_t* _Strsub)
{
	int ifind = -1;
	size_t len = ::wcslen(_Strsub);

	for (int i = 0; i < wcslen(_Str); i++)
	{
		wchar_t c = _Str[i];

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

	/*
	size_t len = ::wcslen(_Strsub);
	while (*_Str)
	{
		wchar_t c = *_Str;
		for (int i = 0; i < len; ++i)
		{
			if (_Strsub[i] == c)
				return _Str;
		}
		++_Str;
	}
	return 0;
	*/
}

size_t wcharutil::Find(wchar_t* _Str, char ch)
{
	int ifind = -1;
	for (int i = 0; i < wcslen(_Str); i++)
	{
		if (_Str[i] == ch)
		{
			ifind = i;
			break;
		}
	}
	return ifind;

	/*
	while (*_Str)
	{
		if (*_Str == ch)
			return _Str;
		++_Str;
	}
	return 0;
	*/
}

size_t wcharutil::ReserveFind(const wchar_t* _Str, char ch)
{
	CString strValue(_Str);
	int ifind = strValue.ReverseFind(ch);
	return ifind;

	/*
	int ifind = -1;
	for (int i = wcslen(_Str) - 1; i >= 0; --i)
	{
		if (_Str[i] == ch)
		{
			ifind = i;
			break;
		}
	}
	return ifind;
	*/
}

size_t wcharutil::Split(const wchar_t* _Str, const wchar_t* _Strsub, std::vector<wchar_t*>& _Vec)
{
	_Vec.clear();

	size_t len = ::wcslen(_Str);
	wchar_t* _buf = new wchar_t[len + 1];
	memcpy(_buf, _Str, sizeof(wchar_t) * (len + 1));
	_buf[len] = L'\0';

	wchar_t* p = _buf;
	while (true)
	{
		while (*p && wcschr(_Strsub, *p) != NULL)
			*p++ = 0;

		if (!*p)
			break;

		_Vec.push_back(p);
		while (*p && wcschr(_Strsub, *p) == NULL)
			p++;
	}

	return _Vec.size();
}

CString wcharutil::Trim(wchar_t* _Str)
{
	CString strValue(_Str);
	strValue.TrimLeft();
	strValue.TrimRight();
	//return T2W(strValue.GetBuffer(0));
	return strValue;
}

CString wcharutil::TrimLeft(wchar_t* _Str)
{
	CString strValue(_Str);
	strValue.TrimLeft();
	//return T2W(strValue.GetBuffer(0));
	return strValue;

	/*
	while (0x20 == _Str[0] || '\t' == _Str[0])
	{
		_Str++;
		if (('\0' == _Str[0]) || ('\n' == _Str[0]))
		{
			return NULL;
		}
	}
	return _Str;
	*/
}

CString wcharutil::TrimRight(wchar_t* _Str)
{
	CString strValue(_Str);
	strValue.TrimRight();
	//return T2W(strValue.GetBuffer(0));
	return strValue;
}

CString wcharutil::Replace(const wchar_t* _Str, const wchar_t* _Search, const wchar_t* _Replace)
{
	CString strValue(_Str);
	CString strSearch(_Search);
	CString strReplace(_Replace);
	strValue.Replace(strSearch, strReplace);
	//return T2W(strValue.GetBuffer(0));
	return strValue;
}

wchar_t* wcharutil::Reserve(wchar_t* _Str)
{
	size_t len = ::wcslen(_Str);
	wchar_t* _buf = new wchar_t[len + 1];
	memcpy(_buf, _Str, sizeof(wchar_t) * (len + 1));
	_buf[len] = L'\0';

	::_wcsrev(_buf);
	return _buf;
}
