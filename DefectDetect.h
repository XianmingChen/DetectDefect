// DefectDetect.cpp : 定义 DLL 应用程序的导出函数。
//
#ifndef __DefectDefect_H__
#define __DefectDefect_H__

#include "stdafx.h"
#include "Scan.h"

class CDefectDetectApp
{
public:
	CDefectDetectApp();
	~CDefectDetectApp();
	void Image1DTo2D(char* image,int width, int height);
	int ScanImage(int width, int height);
	void set_defect_threshold(int threshold);
	void set_WhiteCircle_threshold(int threshold);
	void set_WhitePixel_threshold(int threshold);
	void set_CircleRange_threshold(int threshold);

	void set_Total_Defect_threshold(int threshold);
private:
	pixel **input;
	int D_ImageWidth;
	int D_ImageHeight;
	int defect_area_threshold;
	int White_Circle_threshold;
	int White_pixel_threshold;
	int White_Circle_Range;

	int Total_Defect_threshold;
};

#endif


