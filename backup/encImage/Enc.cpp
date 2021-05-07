#include "pch.h"
#include "Enc.h"

bool Enc::encFileExists(CString strFilename)
{
	bool bFind = false;
	
	CFile file;
	CFileException excOutputfile;

	try
	{
		if (file.Open(strFilename, CFile::modeRead, &excOutputfile))
		{
			bFind = true;
			file.Close();
		}
	}
	catch (...) {
	}

	return bFind;
}

void Enc::encDeleteFile(CString strFilename)
{
	try
	{
		CFile file;
		file.Remove(strFilename);
	}
	catch (...) {
	}
}

CString Enc::encGetFilePath(CString strFilename)
{
	int nPos = strFilename.ReverseFind('\\');
	if (nPos > 0)
		return strFilename.Left(nPos);

	return strFilename;
}

CString Enc::encGetFileName(CString strFilename)
{
	CString strFullName = strFilename;
	int nFindExt = 0;
	int i = 0;
	for (i = 0; i < strFullName.GetLength(); )
	{
		nFindExt = strFullName.Find('\\', nFindExt);
		if (nFindExt == -1) break;
		nFindExt++;
		i = nFindExt;
	}

	if (i > 0)
		return strFullName.Mid(i, strFullName.GetLength());
	else
		return strFullName;
}

CString Enc::encGetModulePath()
{
	TCHAR tPath[MAX_PATH];
	::GetModuleFileName(NULL, tPath, MAX_PATH);
	
	CString strPath = tPath;
    strPath = strPath.Left(strPath.ReverseFind('\\') + 1);
    return strPath;
}

void Enc::encWriteFileBinaryEnc(CString strFilename)
{
	ifstream readFile;
	ofstream writeFile;
	char chByte;
	char chWriteByte;

	TCHAR tFilename[MAX_PATH];
	wsprintf(tFilename, _T("%s\\%s"), encGetModulePath(), encGetFileName(strFilename));
	
	readFile.open(strFilename, ios::in | ios::binary);
	writeFile.open(tFilename, ios::out | ios::binary);

	while (!readFile.eof())
	{
		chByte = readFile.get();

		if (readFile.fail())
		{
			break;
		}

		chWriteByte = (int)chByte + 23;
		writeFile.put(chWriteByte);
	}
	readFile.close();
	writeFile.close();
}

void Enc::encReadFileBinaryDec(CString strFilename)
{
	ifstream readFile;
	ofstream writeFile;
	char chByte;
	char chWriteByte;
		
	TCHAR tFilename[MAX_PATH];
	wsprintf(tFilename, _T("%s\\복호화.png"), encGetModulePath());

	readFile.open(strFilename, ios::in | ios::binary);
	writeFile.open(tFilename, ios::out | ios::binary);

	while (!readFile.eof())
	{
		chByte = readFile.get();

		if (readFile.fail())
		{
			break;
		}

		chWriteByte = (int)chByte - 23;
		writeFile.put(chWriteByte);
	}
	readFile.close();
	writeFile.close();
}

bool Enc::encAddFolder(CString strFilename, CStringArray& strAry)
{
	CFileFind finder;

	CString strWildcard = strFilename;
	strWildcard += _T("\\*.png");

	CString strFullPath;

	BOOL bWorking = finder.FindFile(strWildcard);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDots())
			continue;

		if (finder.IsDirectory())
		{
			//디렉토리일때는 재귀호출 
			strFullPath = finder.GetFilePath();
			encAddFolder(strFullPath, strAry);
		}
		else
		{
			strFullPath = finder.GetFilePath();
			strAry.Add(strFullPath);
		}
	}

	finder.Close();
	return true;
}
