#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

/*create dynamic arrays for philosophers and chopsticks*/
pthread_t* threads;
int* chopsticks;

/**/
pthread_mutex_t* mutex;
pthread_cond_t cond_var;

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

    chopsticks = (int *) malloc(num_philosophers * sizeof(int));

    /*initialize all the chopsticks to 1 as available initially*/
    int i = 0;
    for (i;i<num_philosophers;i++) {
    	chopsticks[i] = 1;
    }

    /*create n number of threads for n number of philosophers*/
    threads = (pthread_t *) malloc(num_philosophers * sizeof(pthread_t));

    for (i=0;i<num_philosophers;i++) {
    	pthread_mutex_init(&mutex[i], NULL);
	}

	pthread_cond_init(&cond_var, NULL);

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
		pthread_mutex_lock(&mutex);

		/*while both the left and right chopsticks are used for the philosopher, it waits*/
		while (chopsticks[param] == 0 || chopsticks[(param + 1) % num_philosophers] == 0) {
			printf("chola chola\n");
			printf("%d is waiting...\n", param);
			pthread_cond_wait(&mutex, &cond_var);
			
		}
		/*chopsticks used are set to 0 as they are being used in a criticial section*/
		chopsticks[param] = 0;
		chopsticks[(param + 1) % num_philosophers] = 0;

		printf("%d is eating$$$\n", param);
		int e = (int) pow(10,6) + rand() % ((3 *(int) pow(10,6)) -  (int) pow(10,6));
		usleep(e);

		/*chopsticks are set to 1 as they are out of critical section and available*/
		chopsticks[param] = 1;
		chopsticks[(param + 1) % num_philosophers] = 1;
		pthread_cond_signal(&cond_var);
		pthread_mutex_unlock(&mutex);
	}
	
}