#pragma once
class Enc
{
public:
	bool encFileExists(CString strFilename);
	void encDeleteFile(CString strFilename);
	CString encGetFilePath(CString strFilename);
	CString encGetFileName(CString strFilename);
	CString encGetModulePath();
	void encWriteFileBinaryEnc(CString strFilename);
	void encReadFileBinaryDec(CString strFilename);
	bool encAddFolder(CString strFilename, CStringArray& strAry);
};

