#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define SEED     921
#define NUM_ITER 1000000000

int main(int argc, char* argv[])
{
    int local_count = 0;
    int rank, size, i, provided;
    double x, y, z, pi, start, stop;
    
    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);
    
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int iterPerThread = NUM_ITER/size;
    MPI_Status status;
    
    srand(SEED*rank); // Important: Multiply SEED by "rank" when you introduce MPI!
    start = MPI_Wtime();
    // Calculate PI following a Monte Carlo method
    for (int iter = 0; iter < iterPerThread; iter++)
    {
        // Generate random (X,Y) points
        x = (double)random() / (double)RAND_MAX;
        y = (double)random() / (double)RAND_MAX;
        z = sqrt((x*x) + (y*y));
        
        // Check if point is in unit circle
        if (z <= 1.0)
        {
            local_count++;
        }
    }
    if (rank == 0) {
        int counts[size-1];
        int global_count = 0;
        MPI_Request requests[size-1];
        for (int source = 1; source < size; source++){            
            MPI_Irecv(&counts[source-1], 1, MPI_INT, source, MPI_ANY_TAG, MPI_COMM_WORLD, &requests[source-1]);
        }
        MPI_Waitall(size-1,requests, MPI_STATUSES_IGNORE);
        global_count += local_count;
        for (int i = 0; i < size-1; i++) {
            global_count += counts[i];
        }
        
        // Estimate Pi and display the result
        pi = ((double)global_count / (double)NUM_ITER) * 4.0;
        stop = MPI_Wtime();
        printf("The result is %f, and the calculation took %f seconds with %d threads\n", pi, stop-start, size);


    }
    else {
        MPI_Send(&local_count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); 
    }
    MPI_Finalize();
    //return 0;
}

