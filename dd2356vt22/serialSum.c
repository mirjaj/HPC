#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>


typedef struct { double sum; char pad[128];} sums;

double serial_sum(double *x, size_t size)
{
  double sum_val = 0.0;
  for (size_t i = 0; i < size; i++) {
    sum_val += x[i];
  }
  return sum_val;
}

double omp_sum(double *x, size_t size, int threads) {
  double sum_val = 0.0;
  omp_set_num_threads(threads);
  int threadID;
 
	#pragma omp parallel for
	for (size_t i=0; i<size; i++) {
		sum_val += x[i];
  		//printf("Thread id: %d working on index %d\n",omp_get_thread_num(),i); //for DEBUG
  	}
  return sum_val;
}

double omp_critical_sum(double *x, size_t size, int threads) {
  double sum_val = 0.0;
  omp_set_num_threads(threads);
  int threadID;
 
	#pragma omp parallel for
	for (size_t i=0; i<size; i++) {
		#pragma omp critical
		sum_val += x[i];
  		//printf("Thread id: %d working on index %d\n",omp_get_thread_num(),i); //for DEBUG
  	}
  return sum_val;
}


double omp_local_sum(double *x, size_t size, int threads) {
  double sum_val = 0.0;
  double sum_local[threads];
  omp_set_num_threads(threads);
  int chunk = size / threads;
 
  #pragma omp parallel shared(sum_local)
  {
  	int threadID = omp_get_thread_num();
  	sum_local[threadID] = 0;

  	#pragma omp for 
	for (int i=0;i<size; i++) {
		sum_local[threadID] += x[i];
  		//printf("Thread id: %d working on index %d\n",omp_get_thread_num(),i); //for DEBUG
  	}
  }//end of parallel region
  
  for (int i = 0; i < threads; i++) {
  	sum_val += sum_local[i];
  }
  return sum_val;
}

double opt_local_sum(double *x, size_t size, int threads) {
  double sum_val = 0.0;
  sums sum_local[threads];
  omp_set_num_threads(threads);
  int chunk = size / threads;
 
  #pragma omp parallel shared(sum_local)
  {
  	int threadID = omp_get_thread_num();
  	sum_local[threadID].sum = 0;

  	#pragma omp for 
	for (int i=0;i<size; i++) {
		sum_local[threadID].sum += x[i];
  		//printf("Thread id: %d working on index %d\n",omp_get_thread_num(),i); //for DEBUG
  	}
  }//end of parallel region
  
  for (int i = 0; i < threads; i++) {
  	sum_val += sum_local[i].sum;
  }
  return sum_val;
}


void generate_random(double *input, size_t size)
{
  for (size_t i = 0; i < size; i++) {
    input[i] = rand() / (double)(RAND_MAX);
  }
}


int main(int argc, char const *argv[])
{
	
	size_t size = 20000000;
	double *array = malloc(sizeof(double)*size);
	generate_random(array,size);
	size_t runs = 5;
	double times[runs];
	int threads[9] = {1,2,4,8,16,20,24,28,32};
	double t1,t2,result,average,sigma;
	for (int j = 0; j < 9; j++) {
		
		for (int i = 0; i <runs; i++) {
			t1 = omp_get_wtime();
			//result = omp_critical_sum(array,size,threads[j]);
			//result = omp_sum(array,size,threads[j]);
			//result = omp_local_sum(array,size,threads[j]);
			result = opt_local_sum(array,size,threads[j]);
			t2 = omp_get_wtime();
			times[i] = (t2-t1);
			average += times[i];
	}
	
	average = average/5;
	for (int i = 0; i < 5; i++) {
		sigma += pow((times[i]-average),2);
	}
	sigma = sqrt(sigma/4);
	printf("Array size = %d elements, number of threads = %d, average time over 5 runs with padded structs = %fs, standard deviation of runs= %f s. Sum: %f \n",size, threads[j], average,sigma,result);
	}
	return 0;
}
