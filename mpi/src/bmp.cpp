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

void initMPI(int argc, char **argv)
{
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &taskId);
	MPI_Comm_size(MPI_COMM_WORLD, &numTasks);
	numWorkers = numTasks - 1;
}

int nextWorker()
{
	if (currentWorker >= numWorkers)
		currentWorker = 0;
	currentWorker++;
	return currentWorker;
}