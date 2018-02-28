#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_t* threads;
int* chopsticks;

pthread_mutex_t mutex;
pthread_cond_t cond_var;

int num_philosophers;

void* runner(void* param);

int main(int argc, char *argv[]) {
    if(argc<=1) {
        printf("You did not feed me arguments, I will die now :( ...");
        exit(1);
     }  
     num_philosophers = atoi(argv[1]);  

     chopsticks = (int *) malloc(num_philosophers * sizeof(int));

     int i = 0;
     for (i;i<num_philosophers;i++) {
     	chopsticks[i] = 1;
     }

     threads = (pthread_t *) malloc(num_philosophers * sizeof(pthread_t));


     pthread_mutex_init(&mutex, NULL);
	 pthread_cond_init(&cond_var, NULL);

	 i = 0;
	 for (i;i<num_philosophers;i++) {
	 	int *copy = (int *) malloc(sizeof(*copy));

		*copy = i;
	 	pthread_create(&threads[i], NULL, runner, (void*) copy);
	}
 }


void* runner(void* arg) {
	int param = *((int *) arg);
	while (true) {
		printf("%d is thinking!!!\n", param);
		pthread_mutex_lock(&mutex);
		while (chopsticks[param] == 0 || chopsticks[(param + 1) % num_philosophers] == 0) {
			printf("%d is waiting...\n", param);
			pthread_cond_wait(&mutex, &cond_var);
		}
		chopsticks[param] = 0;
		chopsticks[(param + 1) % num_philosophers] = 0;

		printf("%d is eating$$$\n", param);
		pthread_cond_signal(&cond_var);
		pthread_mutex_unlock(&mutex);
	}
	chopsticks[param] = 1;
	chopsticks[(param + 1) % num_philosophers] = 1;
}