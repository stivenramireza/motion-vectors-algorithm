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

class 


Image readBMP(char* filename);
