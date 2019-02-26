#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define f1 "../imagenes/frame1.bmp"
#define f2 "../imagenes/frame2.bmp"

using namespace std;



void loadBmp();

int getIndex(int row, int col);

int NCOLS;


/**
 * Extraído de: https://stackoverflow.com/questions/9296059/read-pixel-value-in-bmp-file
 * */
unsigned char* readBMP(char* filename){
    int i;
    FILE* f = fopen(filename, "rb");
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    // extract image height and width from header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];

    NCOLS = height;

    int size = 3*width * height; //
    unsigned char* data = new unsigned char[size]; // allocate 3 bytes per pixel
    fread(data, sizeof(unsigned char), size, f); // read the rest of the data at once
    fclose(f);

    for(i = 0; i < size; i += 3){ // 3 bytes for RGB 1 byte for R 1 for G and 1 for B
        printf("data[%i] = %i \n ",i,data[i]);
    }
    
    return data;
}


int getIndex(int row, int col) { return row*NCOLS+col; }

int main(){
    readBMP(f1);
    //loadBmp();
    return 0;
}

