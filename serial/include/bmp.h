#include <stdio.h>
#include <stdlib.h>
#include <iostream>


class Image {
public:
  int width;
  int height;
  unsigned char* arrayOfPixels;
  int getIndex(int row, int col);
};

typedef struct ValueResult{
    int x,y,minimo;
} ValueResult;


Image readBMP(char* filename);
