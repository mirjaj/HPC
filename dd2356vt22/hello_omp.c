#include <stdio.h>
#include <omp.h>
int main(void){
    int id;	
    #pragma omp parallel 
    {
        id = omp_get_thread_num();
	printf("Hello from thread %d! \n", id);
    }

    return 0;
}
