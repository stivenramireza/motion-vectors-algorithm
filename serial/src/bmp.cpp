
#include "../include/bmp.h"

#define f1 "../imagenes/frame1.bmp"
#define f2 "../imagenes/frame2.bmp"

using namespace std;

/**
 * Extraído de: https://stackoverflow.com/questions/9296059/read-pixel-value-in-bmp-file
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
    printf("header: %i",info[10]);
    im.width = width;
    im.height = height;

    unsigned short int  bits = *(short *) &info[28];
    printf("bits: %i\n",bits);
    

    int row_padded = (width + 3) & (~3);
    int size = width * height;
    unsigned char* data = new unsigned char[size];
    unsigned int matrix_addr = *(int *) &info[10];
    fseek(f, matrix_addr-54, SEEK_CUR);
    fread(data, sizeof(unsigned char), size, f); 
    
    fclose(f);
    
    printf("height: %i, width: %i\n",height,width);
    printf("first value: %i\n",data[0]);
    //for(i = 0; i < size; i++){ // remember the format in bmp is  bgr 3 bytes for RGB 1 byte for R 1 for G and 1 for B
    //    printf("data[%i] = %i \n ",i,data[i]);
    //}
    im.arrayOfPixels = data;
    im.height = height;
    im.width = width;
    
    return im;
}



void algorithm(Image im1, Image im2){
    ValueResult* result[im1.height/15];

    for(int i = 0; i < im1.height;i+=15){
        for(int j = 0; j < im1.width; j+=15){
            ValueResult* minimo = new ValueResult();
            minimo->minimo =99999999999;
            printf("inicia frame2\n");
            for(int u = 0; u < im2.height-15; u++){
                for(int l = 0; l < im2.width-15 ; l++){
                    int sumatoria = 0;
                    for(int k = 0; k < 15; k++){
                        for(int y = 0; y < 15; y++){
                            sumatoria += abs(im1.arrayOfPixels[getIndex(i+k,j+y,im1.width)] - im2.arrayOfPixels[getIndex(u+k,l+y,im2.width)]);
                        }
                    }
                    
                    if(sumatoria < minimo->minimo){
                        minimo->minimo = sumatoria;
                        minimo->x = i;
                        minimo->y = j;
                    }
                    
                    if(sumatoria == 0) goto endFrame2;
                   
                }
            }
            endFrame2:
            printf("minimo : %i \n",minimo->minimo);
            result[i] = minimo;
        }
    }
    for(int i = 0;i < im1.height/15 ; i++){
        printf("x: %i  y: %i minimo: %i ",result[i]->x,result[i]->y,result[i]->minimo);
    }
}


int main(){
    Image im1 = readBMP(f1);
    Image im2 = readBMP(f2);
    
    
    algorithm(im1,im2);
    return 0;
}