#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

/*create dynamic arrays for philosophers*/
pthread_t* threads;

/*create dynamic array of mutex*/
pthread_mutex_t* mutex;

int num_philosophers;

/*header for runner function*/
void* runner(void* param);

int main(int argc, char *argv[]) {
    if(argc<=1) {
        printf("You did not feed me arguments, I will die now :( ...");
        exit(1);
    }  
    num_philosophers = atoi(argv[1]);  
    mutex = (pthread_mutex_t *) malloc(num_philosophers * sizeof(pthread_mutex_t));

    /*create n number of threads for n number of philosophers*/
    threads = (pthread_t *) malloc(num_philosophers * sizeof(pthread_t));

    /*All the mutex are set to NULL*/
    int i;
    for (i=0;i<num_philosophers;i++) {
    	pthread_mutex_init(&mutex[i], NULL);
	}

	/*create thread for each philosopher*/
	i = 0;
	for (i;i<num_philosophers;i++) {
		int *copy = (int *) malloc(sizeof(*copy));
		*copy = i;
	 	pthread_create(&threads[i], NULL, runner, (void*) copy);
	}

	/*join all the threads*/
	i = 0;
	for (i;i<num_philosophers;i++) {
     	pthread_join(threads[i], NULL);
     }
	return 0;
 }


void* runner(void* arg) 
/*runner function which checks the mutex lock for the mutual exclusion*/
	{
	int param = *((int *) arg);
	srand(time(NULL)); 

	while (1) {
		printf("%d is thinking!!!\n", param);
		int t = (int) pow(10,6) + rand() % ((3 *(int) pow(10,6)) -  (int) pow(10,6));
		usleep(t);
		
		/*left & right chopsticks in use for the "param" philosopher*/

		if ((param + 1) % 2 == 0) 
		/*if the philosopher is even numbered, s/he picks right chopstick first and then left chopstick*/
		{
			printf("%d is waiting...\n", param);
			pthread_mutex_lock(&mutex[param]);
			pthread_mutex_lock(&mutex[(param + 1) % num_philosophers]);
		}
		else 
		/*if the philosopher is odd numbered, s/he picks left chopstick first and then right chopstick*/
		{
			printf("%d is waiting...\n", param);
			pthread_mutex_lock(&mutex[(param + 1) % num_philosophers]);
			pthread_mutex_lock(&mutex[param]);
		}
						
		printf("%d is eating$$$\n", param);
		int e = (int) pow(10,6) + rand() % ((3 *(int) pow(10,6)) -  (int) pow(10,6));
		usleep(e);

		
		if ((param + 1) % 2 == 0)
		/*release locks for even numbered philosopher*/
		{
			pthread_mutex_unlock(&mutex[(param + 1) % num_philosophers]);
			pthread_mutex_unlock(&mutex[param]);
			printf("%d releasing\n", param);
		}
		else 
		/*release locks for odd numbered philosophers*/
		{
			pthread_mutex_unlock(&mutex[param]);
			pthread_mutex_unlock(&mutex[(param + 1) % num_philosophers]);
			printf("%d is releasing\n", param);
		}	
		printf("%d Done eating\n", param);		
	}
	return NULL;
}