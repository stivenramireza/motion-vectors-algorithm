#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#define getIndex(row, col, NCOLS)(row*NCOLS+col)

class Image {
public:
  int width;
  int height;
  unsigned char* arrayOfPixels;
};

typedef struct ValueResult{
    int x,y,minimo;
} ValueResult;


Image readBMP(char* filename);
