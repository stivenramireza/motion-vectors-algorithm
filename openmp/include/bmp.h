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
    int iFrame2, jFrame2, minimum;
} ValueResult;


Image readBMP(char* filename);
