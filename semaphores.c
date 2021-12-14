#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 1
#define DELAY   1

int buffer_1[BUFFER_SIZE];
int count_1 = 0;
int buffer_2[BUFFER_SIZE];
int count_2 = 0;
int buffer_3[BUFFER_SIZE];
int count_3 = 0;

void insertIntoBuffer(int x, int* queue, int* count) {
    queue[*count] = x;
    (*count)++;
}

int readFromBuffer(int* queue, int* count) {
    int result = queue[0];
    int i;
    for (i = 0; i < *count - 1; i ++) {
        queue[i] = queue[i+1];
    }
    (*count)--;
    return result;
}

void printBuffer(int* queue, int* count) {
    int i;
    for (i = 0; i < *count; i ++) {
        printf("%d ", queue[i]);
    }
    printf("\n");
}


sem_t sem_1_rw, sem_1_empty, sem_1_full;
sem_t sem_2_rw, sem_2_empty, sem_2_full;
sem_t sem_3_rw, sem_3_empty, sem_3_full;

void* producer_1() {
    for(;;) {
        sem_wait(&sem_1_empty);
        sem_wait(&sem_1_rw);
        
        sleep(DELAY);

        insertIntoBuffer(1, (int *)&buffer_1, &count_1);
        printf("Producer 1 wrote to buffer nr 1\n");

        sem_post(&sem_1_rw);
        sem_post(&sem_1_full);

        sem_wait(&sem_2_empty);
        sem_wait(&sem_2_rw);

        sleep(DELAY);

        insertIntoBuffer(1, (int *)&buffer_2, &count_2);
        printf("Producer 1 wrote to buffer nr 2\n");

        sem_post(&sem_2_rw);
        sem_post(&sem_2_full);

    }
}

void* producer_2() {
    for(;;) {
        sem_wait(&sem_2_empty);
        sem_wait(&sem_2_rw);

        sleep(DELAY);

        insertIntoBuffer(2, (int *)&buffer_2, &count_2);
        printf("Producer 2 wrote to buffer nr 2\n");

        sem_post(&sem_2_rw);
        sem_post(&sem_2_full);

        sem_wait(&sem_3_empty);
        sem_wait(&sem_3_rw);

        sleep(DELAY);

        insertIntoBuffer(2, (int *)&buffer_3, &count_3);
        printf("Producer 2 wrote to buffer nr 3\n");

        sem_post(&sem_3_rw);
        sem_post(&sem_3_full);

    }
}

void* consumer_1() {
    for(;;) {
        sem_wait(&sem_1_full);
        sem_wait(&sem_1_rw);

        sleep(DELAY);

        printf("Consumer 1 read %d from buffer nr 1\n", 
                readFromBuffer((int *)&buffer_1, &count_1));

        sem_post(&sem_1_rw);
        sem_post(&sem_1_empty);
    }
}

void* consumer_2() {
    for(;;) {
        sem_wait(&sem_2_full);
        sem_wait(&sem_2_rw);

        sleep(DELAY);

        printf("Consumer 2 read %d from buffer nr 2\n", 
                readFromBuffer((int *)&buffer_2, &count_2));

        sem_post(&sem_2_rw);
        sem_post(&sem_2_empty);
    }
}

void* consumer_3() {
    for(;;) {
        sem_wait(&sem_3_full);
        sem_wait(&sem_3_rw);

        sleep(DELAY);

        printf("Consumer 3 read %d from buffer nr 3\n", 
                readFromBuffer((int *)&buffer_3, &count_3));

        sem_post(&sem_3_rw);
        sem_post(&sem_3_empty);
    } 
}

int main()
{

    sem_init(&sem_1_rw, 0, BUFFER_SIZE);
    sem_init(&sem_1_empty, 0, BUFFER_SIZE);
    sem_init(&sem_1_full, 0, 0);
    sem_init(&sem_2_rw, 0, BUFFER_SIZE);
    sem_init(&sem_2_empty, 0, BUFFER_SIZE);
    sem_init(&sem_2_full, 0, 0);
    sem_init(&sem_3_rw, 0, BUFFER_SIZE);
    sem_init(&sem_3_empty, 0, BUFFER_SIZE);
    sem_init(&sem_3_full, 0, 0);

    pthread_t t1, t2, t3, t4, t5;

    pthread_create(&t1,NULL,producer_1,NULL);
    pthread_create(&t2,NULL,producer_2,NULL);
    pthread_create(&t3,NULL,consumer_1,NULL);
    pthread_create(&t4,NULL,consumer_2,NULL);
    pthread_create(&t5,NULL,consumer_3,NULL);

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    pthread_join(t3,NULL);
    pthread_join(t4,NULL);
    pthread_join(t5,NULL);


    sem_destroy(&sem_1_empty);
    sem_destroy(&sem_1_full);
    sem_destroy(&sem_1_rw);
    sem_destroy(&sem_2_empty);
    sem_destroy(&sem_2_full);
    sem_destroy(&sem_2_rw);
    sem_destroy(&sem_3_empty);
    sem_destroy(&sem_3_full);
    sem_destroy(&sem_3_rw);

    return 0;
}
