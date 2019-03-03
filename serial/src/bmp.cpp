
#include "../include/bmp.h"

#define f1 "../imagenes/frame1.bmp"
#define f2 "../imagenes/frame2.bmp"

using namespace std;










/**
 * robado de: https://stackoverflow.com/questions/9296059/read-pixel-value-in-bmp-file
 * */
Image readBMP(char* filename){
    
    int i;
    FILE* f = fopen(filename, "rb");
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header
    char b = info[0];
    char m = info[1];
    
    printf("b: %c m: %c \n  ",b,m);
    // extract image height and width from header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];
    Image im;

    im.width = width;
    im.height = height;

    unsigned short int  bits = *(short *) &info[28];
    printf("bits: %i\n",bits);
    

    int size = width * height; //
    unsigned char* data = new unsigned char[size]; // allocate 1 byte per pixel
    fread(data, sizeof(unsigned char), size, f); // read the rest of the data at once
    fclose(f);
    printf("height: %i, width: %i\n",height,width);
    //for(i = 0; i < size; i++){ // remember the format in bmp is  bgr 3 bytes for RGB 1 byte for R 1 for G and 1 for B
    //    printf("data[%i] = %i \n ",i,data[i]);
    //}
    
    im.arrayOfPixels = data;
    return im;
}


int Image::getIndex(int row, int col) { return row*width+col; }



void algorithm(Image im1, Image im2){
    int result[im1.height/15];

    for(int i = 0; i < im1.height;i+=15){
        for(int j = 0; j < im1.width; j+=15){
            int minimo = NULL;
            printf("inicia frame2\n");
            for(int u = 0; u < im2.height-15; u++){
                for(int l = 0; l < im2.width-15 ; l++){
                    int sumatoria = 0;
                    for(int k = 0; k < 15*15; k++){
                        sumatoria += abs(im1.arrayOfPixels[im1.getIndex(i,j)+k] - im2.arrayOfPixels[im2.getIndex(u,l)+k]);
                        
                    }
                   if(minimo != NULL){
                    if(sumatoria<minimo){
                        minimo = sumatoria;
                    }
                   }else{
                       minimo = sumatoria;
                   }
                }
            }
            printf("minimo : %i \n",minimo);
            result[i] = minimo;
        }
    }
}


int main(){
    Image im1 = readBMP(f1);
    Image im2 = readBMP(f2);
    //loadBmp();
    
    algorithm(im1,im2);
    
    return 0;
}