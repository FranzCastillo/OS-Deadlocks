#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define LEFT (philosopher_id + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS
#define RIGHT (philosopher_id + 1) % NUM_PHILOSOPHERS

sem_t mutex;
sem_t semaphore[NUM_PHILOSOPHERS];
int state[NUM_PHILOSOPHERS];

void *philosopher(void *arg) {
    int philosopher_id = *((int *)arg);
    while (1) {
        // Thinking
        printf("Philosopher %d is thinking.\n", philosopher_id);
        sleep(1);

        // Try to acquire forks
        sem_wait(&mutex);
        state[philosopher_id] = HUNGRY;
        printf("Philosopher %d is hungry.\n", philosopher_id);
        // Check if neighbors are eating
        if (state[LEFT] != EATING && state[RIGHT] != EATING) {
            state[philosopher_id] = EATING;
            sem_post(&semaphore[philosopher_id]);
            printf("Philosopher %d is eating.\n", philosopher_id);
        }
        sem_post(&mutex);

        // If couldn't eat, wait for forks
        sem_wait(&semaphore[philosopher_id]);
        // Eating
        printf("Philosopher %d grabbed forks and is eating.\n", philosopher_id);
        sleep(2);

        // Release forks
        sem_wait(&mutex);
        state[philosopher_id] = THINKING;
        printf("Philosopher %d finished eating and is thinking.\n", philosopher_id);
        // Check if neighbors can eat now
        if (state[LEFT] == HUNGRY) {
            state[LEFT] = EATING;
            sem_post(&semaphore[LEFT]);
            printf("Philosopher %d let philosopher %d eat.\n", philosopher_id, LEFT);
        }
        if (state[RIGHT] == HUNGRY) {
            state[RIGHT] = EATING;
            sem_post(&semaphore[RIGHT]);
            printf("Philosopher %d let philosopher %d eat.\n", philosopher_id, RIGHT);
        }
        sem_post(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int philosopher_ids[NUM_PHILOSOPHERS];
    sem_init(&mutex, 0, 1);
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_init(&semaphore[i], 0, 0);
        philosopher_ids[i] = i;
    }
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_ids[i]);
    }
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }
    sem_destroy(&mutex);
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_destroy(&semaphore[i]);
    }
    return 0;
}
