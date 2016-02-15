// Snapshot.cpp : 定义 DLL 应用程序的导出函数。
//
#include "stdafx.h"
#include "DIBRelated.h"

char CDIB::grayBMPfHeader[sizeof(BITMAPFILEHEADER)]={0x42,0x4D,0x36,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x36,0x04,0x00,0x00};
char CDIB::grayBMPiHeader[sizeof(BITMAPINFOHEADER)]={0x28,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x01,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

CDIB::CDIB(void)
{
	lpImageBuf=NULL;
	pbi=NULL;
}

CDIB::~CDIB(void)
{
	if(lpImageBuf!=NULL)
	{
		HeapFree(GetProcessHeap(), 0, lpImageBuf);
		lpImageBuf=NULL;
	}
	if(pbi!=NULL)
	{
		HeapFree(GetProcessHeap(), 0, pbi);
		pbi=NULL;
	}
}

int CDIB::SaveGrayBitmap(char *filename,const char *imgbuffer,int width, int height)
{
	int result=0,i;
	int numQuad=1<<8;
	std::ofstream fpout;
	char *tmpChar1,*tmpChar2,*tmpChar3,*tmpChar4;
	unsigned int tmpNumBytes=0,tmpImgBytes=0,tmpNumQuad=0;
	int tmpWidth,tmpHeight;
	RGBQUAD *quad;
	HANDLE tmpHandle;

	tmpWidth=width;
	tmpHeight=height;

	fpout.open(filename,_IOSbinary);
	if(fpout.is_open())
	{
		tmpImgBytes=width*height;
		tmpNumQuad=numQuad*sizeof(RGBQUAD);
		tmpNumBytes=tmpImgBytes+sizeof(BITMAPFILEHEADER)+tmpNumQuad+sizeof(BITMAPINFOHEADER);

		tmpChar1=(char*)&tmpNumBytes;
		tmpChar2=(char*)&tmpWidth;
		tmpChar3=(char*)&tmpHeight;
		tmpChar4=(char*)&tmpImgBytes;
		for(i=0;i<4;i++)
		{
			grayBMPfHeader[2+i]=tmpChar1[i];
			grayBMPiHeader[4+i]=tmpChar2[i];
			grayBMPiHeader[8+i]=tmpChar3[i];
			grayBMPiHeader[20+i]=tmpChar4[i];
		}

		tmpHandle=GetProcessHeap();
		quad=(RGBQUAD*)HeapAlloc(tmpHandle,0,tmpNumQuad);
		 for(i=0;i<numQuad;i++)        
		{
			quad[i].rgbRed=i;        
			quad[i].rgbGreen=i;        
			quad[i].rgbBlue=i;        
			quad[i].rgbReserved=i;        
		}
		fpout.write(grayBMPfHeader,sizeof(BITMAPFILEHEADER));
		fpout.write(grayBMPiHeader,sizeof(BITMAPINFOHEADER));
		fpout.write((char*)quad,tmpNumQuad);
		if(imgbuffer!=NULL)
		{
			fpout.write(imgbuffer,tmpImgBytes);
		}
		else
		{
			result=-2;
		}
		HeapFree(tmpHandle,0,quad);
	}
	else
	{
		result=-1;
	}
	fpout.close();
	return result;
}

int CDIB::ReadBitmap(char *filename, int &width, int &height)
{
	int result=_FILE_NOT_OPEN;
	std::ifstream fp;
	//std::ofstream fpbf,fpbi,fpquad;

	BITMAPFILEHEADER bf;//文件头
	RGBQUAD *quad;//调色板
	int numQuad;
	SIZE_T tmpNumBytes=0;

	//char testFilename[512];

	fp.open(filename,_IOSbinary);

	//write file for test
	/*strcpy_s(testFilename,"bf_");
	strcat_s(testFilename,filename);
	fpbf.open(testFilename,_IOSbinary);
	strcpy_s(testFilename,"bi_");
	strcat_s(testFilename,filename);
	fpbi.open(testFilename,_IOSbinary);
	strcpy_s(testFilename,"quad_");
	strcat_s(testFilename,filename);
	fpquad.open(testFilename,_IOSbinary);*/

	if(fp.is_open())
	{

		//读取文件头
		fp.read((char*)&bf,sizeof(bf));
		
		//write file for test
		/*if(fpbf.is_open())
		{
			fpbf.write((char*)&bf,sizeof(bf));
		}
		else
		{
			result=-5;
		}*/

		if(bf.bfType==0x4d42) //"BM"
		{
			//读取文件信息头
			fp.read((char*)&bi,sizeof(bi));

			//fpbi.write((char*)&bi,sizeof(bi));//for test

			//计算调色板数目
			numQuad=0;
			if (bi.biBitCount<24)
			{
				numQuad=1<<bi.biBitCount;
			}

			//为图像信息pbi申请空间
			if(pbi!=NULL)
			{
				HeapFree(GetProcessHeap(), 0, pbi);
				pbi=NULL;
			}

			tmpNumBytes=sizeof(BITMAPINFOHEADER)+numQuad*sizeof(RGBQUAD);
			pbi=(BITMAPINFO*)HeapAlloc(GetProcessHeap(),0,tmpNumBytes);
			memcpy(pbi,&bi,sizeof(bi)); 
			quad=(RGBQUAD*)(pbi+sizeof(BITMAPINFOHEADER));
			//读取调色板
			if (numQuad!=0)
			{
				tmpNumBytes=sizeof(RGBQUAD)*numQuad;
				fp.read((char*)quad,tmpNumBytes);
				
				//fpquad.write((char*)quad,tmpNumBytes);//for test

				bi.biSizeImage=bf.bfSize-bf.bfOffBits;
				if(lpImageBuf!=NULL)
				{
					HeapFree(GetProcessHeap(), 0, lpImageBuf);
					lpImageBuf=NULL;
				}
				lpImageBuf=(BYTE*)HeapAlloc(GetProcessHeap(),0,bi.biSizeImage);
				//读取图像数据
				fp.read((char*)lpImageBuf,bi.biSizeImage);
				width=bi.biWidth;
				height=bi.biHeight;
				//fpout.write((char*)lpImageBuf,bi.biSizeImage);//for test

				//图像读取完毕，关闭文件
				result=_SUCCESS;
			}
		}
		else
		{
			result=_FILE_NOT_BITMAP;//文件不是BMP文件
		}
	}
	else
	{
		result=_FILE_NOT_OPEN;//文件不存在
	}
	fp.close();
	//fpbf.close();
	//fpbi.close();
	//fpquad.close();
	return result;
}

int CDIB::GetCols()
{
	int result=0;
	if(lpImageBuf!=NULL)
	{
		result=bi.biWidth;
	}
	return result;
}

int CDIB::GetRows()
{
	int result=0;
	if(lpImageBuf!=NULL)
	{
		result=bi.biHeight;
	}
	return result;
}

int CDIB::GetDataSize()
{
	int result=0;
	if(lpImageBuf!=NULL)
	{
		result=bi.biSizeImage;
	}
	return result;
}

BYTE *CDIB::GetData()
{
	return lpImageBuf;
}