// DefectDetect.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "DefectDetect.h"
#include "Scan.h"

using namespace std;

CDefectDetectApp::CDefectDetectApp()
{
	input=NULL;
	D_ImageWidth=0;
	D_ImageHeight=0;
	defect_area_threshold=247;
	//White_Circle_threshold=40000;
	White_Circle_threshold=30000;
	White_pixel_threshold=240;
	//White_Circle_Range=740;
	White_Circle_Range=600;
}

CDefectDetectApp::~CDefectDetectApp()
{
	delete [] input;
}

void CDefectDetectApp::set_defect_threshold(int threshold)
{
	defect_area_threshold=threshold;
}

void CDefectDetectApp::set_WhiteCircle_threshold(int threshold)
{
	White_Circle_threshold=threshold;
}

void CDefectDetectApp::set_WhitePixel_threshold(int threshold)
{
	White_pixel_threshold=threshold;
}

void CDefectDetectApp::set_CircleRange_threshold(int threshold)
{
	White_Circle_Range=threshold;
}

void CDefectDetectApp::set_Total_Defect_threshold(int threshold)
{
	Total_Defect_threshold=threshold;
}

void CDefectDetectApp::Image1DTo2D(char* image,int width, int height)
{
	int i,j;

	if (input!=NULL)
	{
		if ((D_ImageWidth!=width)||(D_ImageHeight!=height))
		{
			delete [] input;

			D_ImageWidth=width;
			D_ImageHeight=height;
			input=new pixel *[height];
			for(i=0;i<height;i++)
			{
				input[i]=new pixel[width];
			}
		}		
	}
	else 
	{
		D_ImageWidth=width;
		D_ImageHeight=height;

		input=new pixel *[height];
		for(i=0;i<height;i++)
		{
			input[i]=new pixel[width];
		}
	}

	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			input[i][j].grey =image[i*width+j];
			input[i][j].region_index=0;
		}
	}
}

int CDefectDetectApp::ScanImage(int width, int height)
{
	int i,j;
	int patch_index=0;
	int patch_area=0;
	int total_defect_area=0;

	range white_circle_range,patch_range;
	int X_center,Y_center,X_patch,Y_patch;
	double distance,radius;

	int pop_row,pop_col;
	int popping;
	Queue *queue=NULL;

	int max_defect=0;

	if (input!=NULL)
	{
		queue=new Queue; 
		queue->first=NULL;
		queue->last=NULL;

		if (find_white_circle(input,&white_circle_range,width,height,White_Circle_threshold,White_pixel_threshold,White_Circle_Range)!=1)
		{
			return 0;
		}

		X_center=(white_circle_range.x_left+white_circle_range.x_right)/2;
		Y_center=(white_circle_range.y_down+white_circle_range.y_up)/2;
		radius=((white_circle_range.x_right-white_circle_range.x_left)/2+(white_circle_range.y_up-white_circle_range.y_down)/2)/2;

		for (i=white_circle_range.y_down;i<white_circle_range.y_up;i++)
		{
			for (j=white_circle_range.x_left;j<white_circle_range.x_right;j++)
			{
				if ((input[i][j].grey>White_pixel_threshold)&&(input[i][j].region_index==0))
				{
					patch_index++;
					patch_area=1;

					patch_range.y_down=i;
					patch_range.y_up=i;
					patch_range.x_left=j;
					patch_range.x_right=j;

					scan_pixel_around(input,i,j,patch_index,&patch_area,&patch_range,queue,width,height,White_pixel_threshold);

					popping=PopQueue(queue,&pop_row,&pop_col);
					while(popping==1)
					{
						scan_pixel_around(input,pop_row,pop_col,patch_index,&patch_area,&patch_range,queue,width,height,White_pixel_threshold);
						popping=PopQueue(queue,&pop_row,&pop_col);
					}

					X_patch=(patch_range.x_left+patch_range.x_right)/2;
					Y_patch=(patch_range.y_down+patch_range.y_up)/2;
					distance=sqrt((double)((X_patch-X_center)*(X_patch-X_center)+(Y_patch-Y_center)*(Y_patch-Y_center)));

					//if ((patch_area>defect_area_threshold)&&(distance<(radius-White_Circle_Diameter_Tolerance)))
					if(patch_area>max_defect)
						max_defect=patch_area;

					total_defect_area=total_defect_area+patch_area;
					
					if ((patch_area>defect_area_threshold)||(total_defect_area>Total_Defect_threshold))
					{
						delete []queue;
						return 2;
					}
				}
			}
		}
		delete []queue;
		return 1;
	}
	return -1;
}