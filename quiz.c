#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

#define MAX_CUSTOMERS 6;

int customers_count = 0;
int services_count = 0;
int customer_queue[MAX_CUSTOMERS];

sem_t hairstylist_sem;
sem_t makeup_sem;

void* hairstylist(void* args){
    sem_wait(&hairstylist_sem);
    printf("hairstylist is serving customer %d" , customer_queue[0]);
    usleep(100);
    printf("hairstylist finished serving customer %d" , customer_queue[0]);
    for(int i=0; i<customers_count; i++){

    }
    customers_count --;
    pthread_exit(NULL);
}

void* makeup(void* args){
    sem_wait(&makeup_sem);
    printf("makeup artist is serving customer %d" , customer_queue[0]);
    usleep(100);
    printf("makeup artist finished serving customer %d" , customer_queue[0]);
    for(int i=0; i<customers_count; i++){

    }
    customers_count --;
    pthread_exit(NULL);

}

int main{
    sem_init(&makeup_sem);
    sem_init(&hairstylist_sem);
    
    pthread_t hairstylist_thread;
    pthread_t makeup_thread;

    pthread_create(&hairstylist_thread, NULL, hairstylist, NULL);
    pthread_create(&makeup_thread, NULL, makeup, NULL);

    for(int i=0; i< MAX_CUSTOMERS; i++){

    }


    pthread_join(hairstylist_thread, NULL);
    pthread_join(makeup_thread, NULL)

    sem_destroy(&hairstylist_sem);
    sem_destroy(&makeup_sem);

    return 0;
}