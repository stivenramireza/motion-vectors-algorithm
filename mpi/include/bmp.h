#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>

#include <cstdio>

#define getIndex(row, col, NCOLS)((row)*(NCOLS)+(col))

class Image {
public:
  int width;
  int height;
  unsigned char* arrayOfPixels;
};

typedef struct ValueResult{
    int iFrame1,jFrame1, iFrame2, jFrame2, minimum;
} ValueResult;

void algorithm(char* frame1,int sizeFrame1,int fram1H,int frame1W,char* frame2, int frame2H, int frame2W, int taskId);

Image readBMP(char* filename);