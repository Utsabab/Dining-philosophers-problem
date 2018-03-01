#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

/*create dynamic arrays for philosophers and chopsticks*/
pthread_t* threads;


/**/
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

    int i;
    for (i=0;i<num_philosophers;i++) {
    	pthread_mutex_init(&mutex[i], NULL);
	}

	i = 0;
	for (i;i<num_philosophers;i++) {
		int *copy = (int *) malloc(sizeof(*copy));
		*copy = i;
	 	pthread_create(&threads[i], NULL, runner, (void*) copy);
	}

	i = 0;
	for (i;i<num_philosophers;i++) {
     	pthread_join(threads[i], NULL);
     }
	return 0;
 }


void* runner(void* arg) {
	int param = *((int *) arg);
	srand(time(NULL)); 

	while (1) {
		printf("%d is thinking!!!\n", param);
		int t = (int) pow(10,6) + rand() % ((3 *(int) pow(10,6)) -  (int) pow(10,6));
		usleep(t);
		
		/*while both the left and right chopsticks are used for the philosopher, it waits*/
		if ((param + 1) % 2 == 0) {
			printf("%d is waiting...\n", param);
			pthread_mutex_lock(&mutex[param]);
			pthread_mutex_lock(&mutex[(param + 1) % num_philosophers]);
		}
		else {
			printf("%d is waiting...\n", param);
			pthread_mutex_lock(&mutex[(param + 1) % num_philosophers]);
			pthread_mutex_lock(&mutex[param]);
		}
						
		printf("%d is eating$$$\n", param);
		int e = (int) pow(10,6) + rand() % ((3 *(int) pow(10,6)) -  (int) pow(10,6));
		usleep(e);

		
		if ((param + 1) % 2 == 0) {
			pthread_mutex_unlock(&mutex[(param + 1) % num_philosophers]);
			pthread_mutex_unlock(&mutex[param]);
			printf("%d Releasing\n", param);
		}
		else {
			pthread_mutex_unlock(&mutex[param]);
			pthread_mutex_unlock(&mutex[(param + 1) % num_philosophers]);
			printf("%d is releasing\n", param);
		}	
		printf("%d Done eating\n", param);		
	}
	return NULL;
}