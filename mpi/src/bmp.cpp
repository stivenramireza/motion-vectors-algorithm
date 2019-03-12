#include "../include/bmp.h"
#include <omp.h>
#include <cstdio>
#include <mpi.h>   
#include <math.h>
#define MASTER 0
#define FROM_MASTER 1
#define FROM_WORKER 2

using namespace std;

int taskId,
	numTasks,
	numWorkers,
	sourceId,
	destId,
	currentWorker = 0;

MPI_Status status;

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
        int i,j,u,l,summation,k,y;
        int nextW;
        for(i = 0; i < im1.height;i+=16){
            nextW = nextWorker(); 
            MPI_Send(&i, 1, MPI_INT, nextW, FROM_MASTER, MPI_COMM_WORLD); // dest = worker 1
            for(j = 0; j < im1.width; j+=16){
                MPI_Send(&j, 1, MPI_INT, nextW, FROM_MASTER, MPI_COMM_WORLD); // dest = worker 1
                ValueResult* dataFrame = new ValueResult();
                dataFrame->minimum = 2147483647; // Maximum value for a variable of type int.
                for(u = 0; u < im2.height-16; u++){
                    nextW = nextWorker();
                    MPI_Send(&u, 1, MPI_INT, nextW, FROM_WORKER, MPI_COMM_WORLD); // dest = worker 2
                    for(l = 0; l < im2.width-16 ; l++){
                        MPI_Send(&l, 1, MPI_INT, nextW, FROM_WORKER, MPI_COMM_WORLD); // dest = worker 2
                        summation = 0;
                        MPI_Recv(&u, 1, MPI_INT, nextW, FROM_WORKER, MPI_COMM_WORLD, &status); // source = worker 1
                        MPI_Recv(&l, 1, MPI_INT, nextW, FROM_WORKER, MPI_COMM_WORLD, &status); // source = worker 1
                        MPI_Recv(&i, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status); // source = master
                        MPI_Recv(&j, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status); // source = master
                        for(k = 0; k < 16; k++){
                            for(y = 0; y < 16; y++){
                                summation += abs(im1.arrayOfPixels[getIndex(i+k,j+y,im1.width)] - im2.arrayOfPixels[getIndex(u+k,l+y,im2.width)]);
                                MPI_Send(&summation, 1, MPI_INT, nextW, FROM_WORKER, MPI_COMM_WORLD); // dest = worker 2
                            }
                        }
                        MPI_Recv(&summation, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status); // source = worker 2
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

int nextWorker(){
	if (currentWorker >= numWorkers)
		currentWorker = 0;
	currentWorker++;
	return currentWorker;
}

int main(int argc, char *argv[]){
    const char *f1 = "../imagenes/frame1.bmp";
    const char *f2 = "../imagenes/frame2.bmp";

    Image im1 = readBMP(f1);    
    Image im2 = readBMP(f2);

    if((im1.width != im2.width) && (im1.height != im2.height)){	
        printf("Error, The images have to be with the same width and height. Try with other images");
        exit (EXIT_FAILURE);
    }

    MPI_Init(&argc, &argv);	// parametros del main()		
	MPI_Comm_rank(MPI_COMM_WORLD, &taskId); // identificador de tareas del comunicador universal
	MPI_Comm_size(MPI_COMM_WORLD, &numTasks); // numero de tareas del comunicador universal
	numWorkers = numTasks - 1; // numero de esclavos
    double start = MPI_Wtime();

	if(taskId == MASTER){
        algorithm(im1,im2);
        printf("Tiempo total en MPI: %lf\n", MPI_Wtime() - start);
	}
    MPI_Finalize();
    return 0;
}