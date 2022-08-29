#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

double serial_sum(double *x, size_t size)
{
  double sum_val = 0.0;
  for (size_t i = 0; i < size; i++) {
    sum_val += x[i];
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
	double times[5];
	double t1,t2,result,average,sigma;
	for (int i = 0; i <5; i++) {
		t1 = omp_get_wtime();
		result = serial_sum(array,size);
		t2 = omp_get_wtime();
		times[i] = (t2-t1);
		average += times[i];
	}
	
	average = average/5;
	for (int i = 0; i < 5; i++) {
		sigma += pow((times[i]-average),2);
	}
	sigma = sqrt(sigma/4);
	printf("Array size = 20M elements, Sum = %f , average time over 5 runs= %fs, standard deviation of runs= %f s.\n", result, average,sigma);

	return 0;
}
