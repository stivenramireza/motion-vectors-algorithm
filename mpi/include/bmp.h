#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>

#include <omp.h>
#include <cstdio>

#define getIndex(row, col, NCOLS)(row*NCOLS+col)

class Image {
public:
  int width;
  int height;
  unsigned char* arrayOfPixels;
};

typedef struct ValueResult{
    int iFrame1,jFrame1, iFrame2, jFrame2, minimum;
} ValueResult;

void algorithm(Image im1, Image im2);

Image readBMP(char* filename);
