#include <cstdio>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t resource1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t resource2 = PTHREAD_MUTEX_INITIALIZER;

void *process1(void *arg) {
    while (1) {
        printf("Process 1 trying to acquire resource 1...\n");
        pthread_mutex_lock(&resource1);
        printf("Process 1 acquired resource 1.\n");

        // Introduce a small delay to make the deadlock easier to observe
        usleep(100000);

        printf("Process 1 trying to acquire resource 2...\n");
        pthread_mutex_lock(&resource2);
        printf("Process 1 acquired resource 2.\n");



        pthread_mutex_unlock(&resource2);
        printf("Process 1 released resource 2.\n");

        pthread_mutex_unlock(&resource1);
        printf("Process 1 released resource 1.\n");
    }
    return nullptr;
}

void *process2(void *arg) {
    while (1) {
        printf("Process 2 trying to acquire resource 2...\n");
        pthread_mutex_lock(&resource2);
        printf("Process 2 acquired resource 2.\n");

        // Introduce a small delay to make the deadlock easier to observe
        usleep(100000);

        printf("Process 2 trying to acquire resource 1...\n");
        pthread_mutex_lock(&resource1);
        printf("Process 2 acquired resource 1.\n");


        pthread_mutex_unlock(&resource1);
        printf("Process 2 released resource 1.\n");

        pthread_mutex_unlock(&resource2);
        printf("Process 2 released resource 2.\n");
    }
    return nullptr;
}

int main() {
    pthread_t tid1, tid2;

    pthread_create(&tid1, nullptr, process1, nullptr);
    pthread_create(&tid2, nullptr, process2, nullptr);

    pthread_join(tid1, nullptr);
    pthread_join(tid2, nullptr);

    return 0;
}
