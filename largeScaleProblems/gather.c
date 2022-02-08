#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  int sendarray[2];
  int i, size, rank, *rbuf;

  MPI_Init(&argc, &argv);
  
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  sendarray[0] = rank;
  sendarray[1] = rank;

  if (rank == 0) {
    rbuf = (int *) malloc(size * 2 * sizeof(int));
    printf("Pre gather\n");
    for (i = 0; i < 2 * size; i++)
      printf("%d ", rbuf[i]);
    printf("\n");
  }

  
  MPI_Gather( sendarray, 2, MPI_INT, rbuf,
	      2, MPI_INT, 0, MPI_COMM_WORLD );

  if (rank == 0) {
    printf("Post gather\n");
    for (i = 0; i < 2 * size; i++)
      printf("%d ", rbuf[i]);
    printf("\n");
  }

  MPI_Finalize();
  
}
