#ifndef __DIB_RELATED__H
#define __DIB_RELATED__H

#include <Windows.h>
#include <fstream>
#include <tchar.h>
#include "ErrorCode.h"

class CDIB
{

private:
	BYTE *lpImageBuf;
	BITMAPINFOHEADER bi;//–≈œ¢Õ∑
	BITMAPINFO *pbi;
	static char grayBMPfHeader[sizeof(BITMAPFILEHEADER)];
	static char grayBMPiHeader[sizeof(BITMAPINFOHEADER)];
public:
	CDIB(void);
	~CDIB(void);
	int ReadBitmap(char *filename, int &width, int &height);
	int SaveGrayBitmap(char *filename,const char *imgbuffer,int width, int height);
	int GetCols();
	int GetRows();
	int GetDataSize();
	BYTE *GetData();
};

#endif