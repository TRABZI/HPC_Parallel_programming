#include <stdio.h>
#include <pthread.h>

#ifndef SIZE
#define SIZE 256
#endif

#ifndef NUM_THREADS  
#define NUM_THREADS 4
#endif


#define CHUNK SIZE/NUM_THREADS

int id[NUM_THREADS];
double sum,a[SIZE], b[SIZE];

pthread_t tid[NUM_THREADS];
pthread_mutex_t mutex_sum;

void* dot_product(void* id){
	//Define local variables of the thread
	size_t i;
	int begin_index = *(int*)id *CHUNK;
	int end_index = (*(int*)id+1) *CHUNK;
	double local_sum =0;
	//Calculation part
	for (i=begin_index; i<end_index;i++) {
		local_sum=local_sum + a[i]*b[i];
	}

	pthread_mutex_lock(&mutex_sum);
	sum=sum+local_sum;
	pthread_mutex_unlock(&mutex_sum);
	return NULL;
}

int main(int argc, char const *argv[])
{
	size_t i;
	sum=0;

	// Initialization
	for (i=0;i<SIZE;i++){
		a[i]=i*0.1;
		b[i]=i*1;
	}

	pthread_mutex_init(&mutex_sum, NULL);

	for(i=0; i<NUM_THREADS; i++){
		id[i]=i;
		pthread_create(&tid[i], NULL, dot_product, (void*)&id[i]);
		printf("thread[%ld] is running\n", i);
	}


	for (i=0; i<NUM_THREADS;i++){
		pthread_join(tid[i], NULL);
	}
	printf("threads are done \n");

	pthread_mutex_destroy(&mutex_sum);

	printf("Result of dot production calculated by %d threads\n", NUM_THREADS );
	printf("sum=%g\n", sum);


	return 0;
}



