#include <stdio.h>
#include <stdbool.h>

#define NUM_RESOURCES 3
#define NUM_PROCESSES 5

int available[NUM_RESOURCES] = {10, 5, 7}; // Available instances of each resource
int maximum[NUM_PROCESSES][NUM_RESOURCES] = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
}; // Maximum demand of each process
int allocation[NUM_PROCESSES][NUM_RESOURCES] = {0}; // Resources currently allocated to each process

bool finish[NUM_PROCESSES] = {false}; // Indicates if a process has finished

bool isSafe(int process, int request[]) {
    // Check if the request can be granted
    for (int i = 0; i < NUM_RESOURCES; ++i) {
        if (request[i] > available[i] || request[i] > maximum[process][i] - allocation[process][i])
            return false;
    }

    // Try to allocate the resources temporarily and check for safety
    int work[NUM_RESOURCES];
    for (int i = 0; i < NUM_RESOURCES; ++i)
        work[i] = available[i];

    bool finishCopy[NUM_PROCESSES];
    for (int i = 0; i < NUM_PROCESSES; ++i)
        finishCopy[i] = finish[i];

    // Try to allocate the resources temporarily
    for (int i = 0; i < NUM_RESOURCES; ++i) {
        available[i] -= request[i];
        allocation[process][i] += request[i];
    }

    // Check safety
    bool safe = true;
    while (safe) {
        safe = false;
        for (int i = 0; i < NUM_PROCESSES; ++i) {
            if (!finishCopy[i]) {
                bool canFinish = true;
                for (int j = 0; j < NUM_RESOURCES; ++j) {
                    if (maximum[i][j] - allocation[i][j] > work[j]) {
                        canFinish = false;
                        break;
                    }
                }
                if (canFinish) {
                    for (int j = 0; j < NUM_RESOURCES; ++j) {
                        work[j] += allocation[i][j];
                    }
                    finishCopy[i] = true;
                    safe = true;
                }
            }
        }
    }

    // Rollback if not safe
    if (!finishCopy[process]) {
        for (int i = 0; i < NUM_RESOURCES; ++i) {
            available[i] += request[i];
            allocation[process][i] -= request[i];
        }
    }

    return finishCopy[process];
}

void requestResources(int process, int request[]) {
    printf("Process %d requesting resources: ", process);
    for (int i = 0; i < NUM_RESOURCES; ++i) {
        printf("%d ", request[i]);
    }
    printf("\n");

    if (isSafe(process, request)) {
        printf("Request granted. Process %d is safe.\n", process);
    } else {
        printf("Request denied. Process %d is unsafe.\n", process);
    }
}

void releaseResources(int process, int release[]) {
    printf("Process %d releasing resources: ", process);
    for (int i = 0; i < NUM_RESOURCES; ++i) {
        printf("%d ", release[i]);
        allocation[process][i] -= release[i];
        available[i] += release[i];
    }
    printf("\n");
}

int main() {
    int requests[NUM_PROCESSES][NUM_RESOURCES] = {
            {0, 0, 0},
            {0, 0, 0},
            {3, 3, 2},
            {2, 0, 0},
            {0, 0, 2}
    }; // Sequence of resource requests
    int releases[NUM_PROCESSES][NUM_RESOURCES] = {
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0},
            {1, 0, 0},
            {0, 0, 0}
    }; // Sequence of resource releases

    // Perform resource requests
    for (int i = 0; i < NUM_PROCESSES; ++i) {
        requestResources(i, requests[i]);
    }

    // Perform resource releases
    for (int i = 0; i < NUM_PROCESSES; ++i) {
        releaseResources(i, releases[i]);
    }

    return 0;
}
