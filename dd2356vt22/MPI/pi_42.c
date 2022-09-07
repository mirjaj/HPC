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
        int tmp;
        for (int iter = 0; iter < size/2; iter++){ //source <= source/2 equivalent           
            MPI_Recv(&tmp, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            local_count+=tmp;
            tmp = 0;
            printf("Master  %s \n","receiving " );
        }
        stop = MPI_Wtime();
    // Estimate Pi and display the result
        pi = ((double)local_count / (double)NUM_ITER) * 4.0;
    
        printf("The result is %f, and the calculation took %f seconds with %d threads\n", pi, stop-start, size);


    }
    else {
        for (int b = 0; b < 3; b++){
        
            if (b == 0 && rank % 2 == 1){
                MPI_Send(&local_count, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD); 
                printf("from if statement 1\n");
            }
            if (b == 1 && rank % 4 == 2 ) {
                MPI_Send(&local_count, 1, MPI_INT, rank-2, 0, MPI_COMM_WORLD);
                printf("from if statement 2\n");
            }
            if (b == 2 && rank % 4 == 0 && rank != 0) {
                MPI_Send(&local_count, 1, MPI_INT, rank-4, 0, MPI_COMM_WORLD);
                printf("from if statement 4\n");
            }
            else {
                //receive and put into own
                int tmp;
                MPI_Recv(&tmp, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                local_count += tmp;
                printf("rank %s %d\n","receiving", rank );
            }
        }
    }

    MPI_Finalize();
    //return 0;
}

