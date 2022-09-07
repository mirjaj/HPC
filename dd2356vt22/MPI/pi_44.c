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
    int global_count = 0;
    int rank, size, i, provided;
    double x, y, z, pi, start, stop, n, h, sum;
    
    MPI_Init(&argc, &argv);
    
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int iterPerThread = NUM_ITER/size;    
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
   
    MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    stop = MPI_Wtime();
    if (rank == 0) {
        pi = ((double)global_count / (double)NUM_ITER) * 4.0;
        printf("The result is %f and took %f seconds with %d processes \n", pi, stop-start, size);
    }
    MPI_Finalize();
    return EXIT_SUCCESS;;
}

