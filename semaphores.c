#include <stdio.h>
#include <sys/sem.h>

union semun {
    int val;
    struct semid_ds *buf;
    ushort* array;
};

int allocate_semaphore(key_t key, int sem_flags) {
    return semget(key, 1, sem_flags);
}

int deallocate_semaphore(int semid) {
    union semun arg;
    return semctl(semid, 1, IPC_RMID, arg);
}

int init_semaphore(int semid, int value) {
    union semun arg;
    unsigned short values[1];
    values[0] = value;
    arg.array = values;
    return semctl(semid, 0, SETALL, arg);
}



void buf12_writer(uint* buf)
{
    printf("Wrote 1 to buffer nr 1\n");
    buf[1] = 1;
}

void buf1_reader(uint* buf)
{
    printf("Read %u from buffer nr 1\n", buf[1]);
}

int main()
{
    printf("Hello, Semaphores!\n");

    uint buf1[1], buf2[1], buf3[1];

    buf12_writer(buf1);
    buf1_reader(buf1);
    return 0;
}











