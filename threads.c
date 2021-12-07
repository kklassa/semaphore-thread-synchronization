#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define DELAY 3

sem_t semaphore_1, semaphore_2, semaphore_3;

void* routine(void* args) {

    sem_wait(&semaphore_1);
    sleep(1);
    printf("Hello from thread %d\n", *(int*)args);\
    sem_post(&semaphore_1);
    free(args);


}

void* producer_1() {
    int i;
    for(i = 1;i < 10;i+=2) {
        printf("Producer 1 trying to write to buffor nr 1...\n");
        sem_wait(&semaphore_1);
        sleep(DELAY);
        printf("Producer 1 wrote %d to buffer nr 1\n", i);
        sem_post(&semaphore_1);

        printf("Producer 1 trying to write to buffor nr 2...\n");
        sem_wait(&semaphore_2);
        sleep(DELAY);
        printf("Producer 1 wrote %d to buffer nr 2\n", i);
        sem_post(&semaphore_2);
        
    }
}

void* producer_2() {
    int i;
    for(i = 0;i < 10;i+=2) {
        printf("Producer 2 trying to write to buffor nr 2...\n");
        sem_wait(&semaphore_2);
        sleep(DELAY);
        printf("Producer 2 wrote %d to buffer nr 2\n", i);
        sem_post(&semaphore_2);

        printf("Producer 2 trying to write to buffor nr 3...\n");
        sem_wait(&semaphore_3);
        sleep(DELAY);
        printf("Producer 2 wrote %d to buffer nr 3\n", i);
        sem_post(&semaphore_3);
        
    }
}

void* consumer_1() {
    int i;
    for(i=0;i<10;i++) {
        printf("Consumer 1 trying to read from buffer nr 1\n");
        sem_wait(&semaphore_1);
        sleep(DELAY);
        printf("Consumer 1 read from buffer nr 1\n");
        sem_post(&semaphore_1);
    }
}

void* consumer_2() {
    int i;
    for(i=0;i<10;i++) {
        printf("Consumer 2 trying to read from buffer nr 2\n");
        sem_wait(&semaphore_2);
        sleep(DELAY);
        printf("Consumer 2 read from buffer nr 2\n");
        sem_post(&semaphore_2);
    }
}

void* consumer_3() {
    int i;
    for(i=0;i<10;i++) {
        printf("Consumer 3 trying to read from buffer nr 3\n");
        sem_wait(&semaphore_3);
        sleep(DELAY);
        printf("Consumer 3 read from buffer nr 3\n");
        sem_post(&semaphore_3);
    }
}

int main(int argc, char *argv[]) {
    pthread_t th[5];
    sem_init(&semaphore_1, 0, 1);
    sem_init(&semaphore_2, 0, 1);
    sem_init(&semaphore_3, 0, 1);

    pthread_create(&th[0], NULL, &producer_1, NULL);
    pthread_create(&th[1], NULL, &producer_2, NULL);
    pthread_create(&th[2], NULL, &consumer_1, NULL);
    pthread_create(&th[3], NULL, &consumer_2, NULL);
    pthread_create(&th[4], NULL, &consumer_3, NULL);
    
    pthread_join(th[0], NULL);
    pthread_join(th[1], NULL);
    pthread_join(th[2], NULL);
    pthread_join(th[3], NULL);
    pthread_join(th[4], NULL);
    

    sem_destroy(&semaphore_1);
    sem_destroy(&semaphore_2);
    sem_destroy(&semaphore_3);
    
    return 0;
}