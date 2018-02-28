#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_t* threads;
int* chopsticks;

pthread_mutex_t mutex;
pthread_cond_t cond_var;

int num_philosophers;

int main(int argc, char *argv[]) {
    if(argc<=1) {
        printf("You did not feed me arguments, I will die now :( ...");
        exit(1);
     }  
     int arg1 = atoi(argv[1]);  

    pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond_var, NULL);
 }

