
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


void algorithm(unsigned char frame1[],int sizeFrame1,int frame1H,int frame1W,unsigned char* frame2, int frame2H, int frame2W, int taskId){
    int getout = 0;
    #pragma omp parallel for schedule(dynamic)
    for(int i = 0; i < frame1H;i+=16){
        for(int j = 0; j < frame1W; j+=16){

            ValueResult* dataFrame = new ValueResult();
            dataFrame->minimum = 2147483647; // Maximum value for a variable of type int.
            getout++;
            for(int u = 0; u < frame2H; u++){
                for(int l = 0; l < frame2W ; l++){
                    
                    int summation = 0;
                    for(int k = 0; k < 16; k++){
                        for(int y = 0; y < 16; y++){
                            summation += abs(frame1[getIndex(i+k,j+y,frame1W)] - frame2[getIndex(u+k,l+y,frame2W)]);
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
            if(getout >= sizeFrame1) goto endExecution;
        }
    }
    endExecution:
    printf("end execution %i \n",taskId);  
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
    int N = im1.height*im1.width;
    clock_t begin = clock();
    MPI_Init(&argc, &argv);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    
    if (taskid == 0) {//master space
        total = im1.height*im1.width;
        totalMacroBlock = (im1.height/16)*(im1.width/16);

        macroPerN = floor( totalMacroBlock/numtasks);
        
        extraMacroBlock = totalMacroBlock - macroPerN*numtasks ; //extra iterations
        macroblockSize = 16*16;
        int iterationsPerN = macroPerN*macroblockSize;
        printf(" primera posicion: %i \n",im1.arrayOfPixels[0]);
        for(int dest = 1; dest < numtasks; dest++){
            MPI_Send(&macroPerN, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            MPI_Send(&im1.height, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            MPI_Send(&im1.width, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            MPI_Send(&im2.height, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            MPI_Send(&im2.width, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            printf("dest %i \n",dest);
            MPI_Send(&im1.arrayOfPixels[macroPerN*macroblockSize*dest], macroPerN*macroblockSize, MPI_CHAR, dest, 1, MPI_COMM_WORLD);

            MPI_Send(im2.arrayOfPixels, N, MPI_CHAR, dest, 1, MPI_COMM_WORLD);
        }
        printf("task 0: %i \n",im1.arrayOfPixels[0]);
        algorithm(im1.arrayOfPixels,macroPerN,im1.height,im1.width,im2.arrayOfPixels,im2.height,im2.width,0);
        if(extraMacroBlock > 0){
            //do something
        }
    }

    if(taskid > 0){ // slaves
        int source = 0;
        int heightIm1,widthIm1,heightIm2,widthIm2;
        MPI_Status status;
        macroblockSize = 16*16;
        
        unsigned char im2Array[N];
        MPI_Recv(&macroPerN, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&heightIm1, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&widthIm1, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&heightIm2, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&widthIm2, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        
        
        unsigned char im1Array[macroPerN*macroblockSize];
        MPI_Recv(im1Array, macroPerN*macroblockSize, MPI_CHAR, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(im2Array, N, MPI_CHAR, source, 1, MPI_COMM_WORLD, &status);
        printf("primer valor: %i por hilo: %i\n",im1Array[0],taskid);
        printf("tamaño de array de macrobloques: %i tamaño de frame2: %i\n",(sizeof(im1Array)/sizeof(*im1Array)),(sizeof(im2Array)/sizeof(*im2Array)));
        printf("task 0: %i \n",im1Array[0]);
        algorithm(im1Array,macroPerN,heightIm1,widthIm1,im2Array,heightIm2,widthIm2,taskid);
    }   

    MPI_Finalize();
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    
    printf("The time is %.6f minutes\n", elapsed_secs/60);
    return 0;
}