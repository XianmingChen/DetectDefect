#ifndef __Scan_H__
#define __Scan_H__

#include "stdafx.h"
#include <math.h>

//#define White_Circle_threshold 40000
//#define White_pixel_threshold 240
//#define defect_area_threshold 375
//#define White_Circle_Range 740
#define White_Circle_Diameter_Tolerance 80

struct pixel
{
	BYTE grey;
	int region_index;
};

struct Seed
{
	 int row;
	 int col;
	 struct Seed *next;
};

struct Queue
{
	struct Seed* first;
	struct Seed* last;
};

struct range
{
	int x_left;
	int x_right;
	int y_down;
	int y_up;
};

int PopQueue(Queue *queue,int *row,int *col);
void PushQueue(Queue *queue,int row,int col);
void scan_pixel_around(pixel **input,int row,int col,int patch_index,int *patch_area,range *patch_range,Queue *queue,int width,int height,int White_pixel_threshold);
int find_white_circle(pixel **input,range *patch_range,int width,int height,int White_Circle_threshold,int White_pixel_threshold,int White_Circle_Range);

#endif

