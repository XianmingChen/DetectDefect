#include "stdafx.h"
#include "ExportFunctions.h"

// 唯一的一个 CFeaSearchApp 对象
CDefectDetectApp ImageScan;

extern "C" _declspec(dllexport) int DefectDetectApp(char *img,int width, int height)
{
	int result=0;

	ImageScan.Image1DTo2D(img,width,height);
	result=ImageScan.ScanImage(width,height);

	return result;
}

extern "C" _declspec(dllexport) int set_defect_thresholdApp(int threshold)
{
	ImageScan.set_defect_threshold(threshold);
	return 0;
}

extern "C" _declspec(dllexport) int set_WhiteCircle_thresholdApp(int threshold)
{
	ImageScan.set_WhiteCircle_threshold(threshold);
	return 0;
}

extern "C" _declspec(dllexport) int set_WhitePixel_thresholdApp(int threshold)
{
	ImageScan.set_WhitePixel_threshold(threshold);
	return 0;
}

extern "C" _declspec(dllexport) int set_CircleRange_thresholdApp(int threshold)
{
	ImageScan.set_CircleRange_threshold(threshold);
	return 0;
}

extern "C" _declspec(dllexport) int set_Total_Defect_threshold(int threshold)
{
	ImageScan.set_Total_Defect_threshold(threshold);
	return 0;
}