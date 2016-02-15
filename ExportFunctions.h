#ifndef __EXPORTS_FUNCTIONS_H__
#define __EXPORTS_FUNCTIONS_H__

#include "DefectDetect.h"
extern "C" _declspec(dllexport) int DefectDetectApp(char *img,int width, int height);
extern "C" _declspec(dllexport) int set_defect_thresholdApp(int threshold);
extern "C" _declspec(dllexport) int set_WhiteCircle_thresholdApp(int threshold);
extern "C" _declspec(dllexport) int set_WhitePixel_thresholdApp(int threshold);
extern "C" _declspec(dllexport) int set_CircleRange_thresholdApp(int threshold);
extern "C" _declspec(dllexport) int set_Total_Defect_threshold(int threshold);
#endif