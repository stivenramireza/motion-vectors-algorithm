
#include "../include/bmp.h"
#include "mpi.h"
#include "math.h"

using namespace std;

/**
 * Extraído de: https://stackoverflow.com/questions/9296059/read-pixel-value-in-bmp-file
 * */
Image readBMP(const char* filename){    
    
    FILE* f = fopen(filename, "rb");
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header
    
    // extract image height and width from header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];
    Image im;
    
    im.width = width;
    im.height = height;

    unsigned short int  bits = *(short *) &info[28];
    if(bits != 8){
        printf("Error, The image has to be of 8 Bits");
        exit (EXIT_FAILURE);
    }
    
    int row_padded = (width + 3) & (~3);
    int size = width * height;
    unsigned char* data = new unsigned char[size];
    unsigned int matrix_addr = *(int *) &info[10];
    fseek(f, matrix_addr-54, SEEK_CUR);
    fread(data, sizeof(unsigned char), size, f); 
    
    fclose(f);
    
    im.arrayOfPixels = data;
    im.height = height;
    im.width = width;
    
    return im;
}


void algorithm(Image im1, Image im2){
    
    ValueResult* matrixResults[im1.height/16][im1.width/16];
    
    for(int i = 0; i < im1.height;i+=16){
        for(int j = 0; j < im1.width; j+=16){

            ValueResult* dataFrame = new ValueResult();
            dataFrame->minimum = 2147483647; // Maximum value for a variable of type int.
            
            for(int u = 0; u < im2.height; u++){
                for(int l = 0; l < im2.width ; l++){

                    int summation = 0;
                    for(int k = 0; k < 16; k++){
                        for(int y = 0; y < 16; y++){
                            summation += abs(im1.arrayOfPixels[getIndex(i+k,j+y,im1.width)] - im2.arrayOfPixels[getIndex(u+k,l+y,im2.width)]);
                        }
                    }
                    
                    if(summation < dataFrame->minimum){
                        dataFrame->minimum = summation;
                        dataFrame->iFrame2 = u;
                        dataFrame->jFrame2 = l;           

                        if(summation == 0) goto endFrame2;
                    }
                }
            }
            endFrame2:  
            matrixResults[i/16][j/16] = dataFrame;
        }
    }

    printf("Matrix Results\n");
    for(int i = 0; i < im1.height/16;i++){
        printf("[");
        for(int j = 0; j < im1.width/16; j++){
            printf(" %i",matrixResults[i][j]->minimum);
        }
        printf("]\n");
    }
        
}



int main(int argc, char *argv[]){
    const char *f1 = "../imagenes/frame1.bmp";
    const char *f2 = "../imagenes/frame1.bmp";

    Image im1 = readBMP(f1);    
    Image im2 = readBMP(f2);

    if((im1.width != im2.width) && (im1.height != im2.height)){
        printf("Error, The images have to be with the same width and height, Try with other images");
        exit (EXIT_FAILURE);
    }

    int taskid,numtasks,total,totalMacroBlock,macroblockSize,macroPerN,extraMacroBlock;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    macroblockSize = 16*16;
    if (taskid == 0) {//master space
        total = im1.height*im1.width;
        totalMacroBlock = (im1.height/16)*(im1.width/16);
        macroPerN = ceil( totalMacroBlock/numtasks);
        extraMacroBlock = totalMacroBlock - macroPerN*numtasks ; //extra iterations
        for(int dest = 1; dest < numtasks; dest++){
            MPI_Send(&macroPerN, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            MPI_Send(&im1.height, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            MPI_Send(&im1.width, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            MPI_Send(&im2.height, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            MPI_Send(&im2.width, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            MPI_Send(&im1.arrayOfPixels[macroPerN*dest*macroblockSize],macroPerN*macroblockSize ,MPI_INT, dest,1,  MPI_COMM_WORLD);
            MPI_Send(&im2.arrayOfPixels, im2.height*im2.width, MPI_INT, dest, 1, MPI_COMM_WORLD);
        }

    }

    if(taskid != 0){ // slaves
        int source = 0;
        int heightim1,widthim1,heightim2,widthim2;
        int *im1ArrayP;
        int *im2ArrayP;
        MPI_Status status;

        MPI_Recv(&macroPerN, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&heightim1, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&widthim1, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&heightim2, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&widthim2, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&im1ArrayP, macroPerN*macroblockSize, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&im2ArrayP, heightim2*widthim2, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        printf("recibido macroperN: en el hilo: %i\n",taskid);
    }   

    

    return 0;
}