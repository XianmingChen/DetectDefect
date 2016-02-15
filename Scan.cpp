#include "stdafx.h"
#include <iostream>
#include "Scan.h"
using namespace std;

int PopQueue(Queue *queue,int *row,int *col)
{
	 Seed *pointer=NULL;
	 if(queue->first==NULL)
	 {
		return 0;
	 }

	 pointer=queue->first;
	 *row=pointer->row;
	 *col=pointer->col;

	 if(queue->first->next==NULL)
	 {
		queue->first=NULL;
		queue->last=NULL;
	 }
	 else
	 {
		queue->first=pointer->next;
	 }
	 free(pointer);

	 return 1;
}

void PushQueue(Queue *queue,int row,int col)
{
	 Seed *pointer=NULL;
	 pointer=(Seed*)malloc(sizeof(Seed));

	 pointer->row=row;
	 pointer->col=col;
	 if(queue->first==NULL)
	 {
		  queue->first=pointer;
		  queue->last=pointer;
		  pointer->next=NULL;
	 }
	 else
	 {
		  pointer->next=NULL;
		  queue->last->next=pointer;
		  queue->last=pointer;
	 }
}

void scan_pixel_around(pixel **input,int row,int col,int patch_index,int *patch_area,range *patch_range,Queue *queue,
					   int width,int height,int White_pixel_threshold)
{
	int i,j;
	int new_row,new_col;

	for (i=-1;i<2;i++)
	{
		for (j=-1;j<2;j++)
		{
			if ((i!=0)||(j!=0))
			{
				new_row=row+i;
				if(new_row>=height)
					break;
				new_col=col+j;

				if ((input[new_row][new_col].grey>White_pixel_threshold)&&(input[new_row][new_col].region_index==0))
				{
					input[new_row][new_col].region_index=patch_index;
					*patch_area=*patch_area+1;

					if (new_row<patch_range->y_down)
					{
						patch_range->y_down=new_row;
					}
					else if (new_row>patch_range->y_up)
					{
						patch_range->y_up=new_row;
					}
						
					if (new_col<patch_range->x_left)
					{
						patch_range->x_left=new_col;
					}
					else if (new_col>patch_range->x_right)
					{
						patch_range->x_right=new_col;
					}

					if((new_row>0)&&(new_row<height)&&(new_col>0)&&(new_col<width))
					{
						PushQueue(queue,new_row,new_col);
					}
				}
			}
		}
	}
}


int find_white_circle(pixel **input,range *patch_range,int width,int height,
					  int White_Circle_threshold,int White_pixel_threshold,int White_Circle_Range)
{
	int i,j;
	int patch_index=0;
	int patch_area=0;

	int pop_row,pop_col;
	int popping;
	Queue *queue=NULL;

	int x_range=0, y_range=0;
	int x_diff=0,y_diff=0;

	queue=new Queue; 
	queue->first=NULL;
	queue->last=NULL;

	for (i=1;i<height-1;i++)
	{
		for (j=1;j<width-1;j++)
		{
			if ((input[i][j].grey>White_pixel_threshold)&&(input[i][j].region_index==0))
			{
				patch_index++;
				patch_area=1;
				patch_range->y_down=i;
				patch_range->y_up=i;
				patch_range->x_left=j;
				patch_range->x_right=j;

				scan_pixel_around(input,i,j,patch_index,&patch_area,patch_range,queue,width,height,White_pixel_threshold);

				popping=PopQueue(queue,&pop_row,&pop_col);
				while(popping==1)
				{
					scan_pixel_around(input,pop_row,pop_col,patch_index,&patch_area,patch_range,queue,width,height,White_pixel_threshold);
					popping=PopQueue(queue,&pop_row,&pop_col);
				}

				x_range=patch_range->x_right-patch_range->x_left;
				y_range=patch_range->y_up-patch_range->y_down;

				if (x_range>White_Circle_Range)
					x_diff=x_range-White_Circle_Range;
				else
					x_diff=White_Circle_Range-x_range;

				if (y_range>White_Circle_Range)
					y_diff=y_range-White_Circle_Range;
				else
					y_diff=White_Circle_Range-y_range;

				if ((patch_area>White_Circle_threshold)&&(x_diff<White_Circle_Diameter_Tolerance)&&(y_diff<White_Circle_Diameter_Tolerance))
				{
					delete []queue;
					return 1;
				}
			}
		}
	}
	delete []queue;
	return 0;
}