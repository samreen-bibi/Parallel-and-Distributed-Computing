#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define N 10000000   // increased size for visible speedup
int arr[N];
long long part[10];
int totalThreads;

void* sumPart(void* arg) {
    int id = *(int*)arg;
    int chunk = N / totalThreads;
    int start = id * chunk;
    int end = (id == totalThreads - 1) ? N : start + chunk;
    long long s = 0;
    for (int i = start; i < end; i++)
        s += arr[i];
    part[id] = s;
    return NULL;
}

double timeDiff(struct timeval a, struct timeval b) {
    return (b.tv_sec - a.tv_sec) + (b.tv_usec - a.tv_usec) / 1000000.0;
}

int main() {
    srand(time(0));
    for (int i = 0; i < N; i++)
        arr[i] = rand() % 100;

    struct timeval s1, e1;
    gettimeofday(&s1, NULL);
    long long serial = 0;
    for (int i = 0; i < N; i++)
        serial += arr[i];
    gettimeofday(&e1, NULL);
    double stime = timeDiff(s1, e1);

    printf("Serial Sum = %lld\nSerial Time = %.6f sec\n", serial, stime);

    int counts[3] = {4, 5, 10};
    for (int c = 0; c < 3; c++) {
        totalThreads = counts[c];
        pthread_t th[totalThreads];
        int id[totalThreads];

        struct timeval s2, e2;
        gettimeofday(&s2, NULL);

        for (int i = 0; i < totalThreads; i++) {
            id[i] = i;
            pthread_create(&th[i], NULL, sumPart, &id[i]);
        }

        for (int i = 0; i < totalThreads; i++)
            pthread_join(th[i], NULL);

        long long total = 0;
        for (int i = 0; i < totalThreads; i++)
            total += part[i];

        gettimeofday(&e2, NULL);
        double ptime = timeDiff(s2, e2);
        printf("\nThreads = %d\nParallel Sum = %lld\nParallel Time = %.6f sec\nSpeedup = %.2fx\n",
               totalThreads, total, ptime, stime / ptime);
    }
    return 0;
}
