#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

void calculateNeed(int need[MAX_PROCESSES][MAX_RESOURCES], int max[MAX_PROCESSES][MAX_RESOURCES], int allot[MAX_PROCESSES][MAX_RESOURCES], int p, int r) {
    for (int i = 0; i < p; i++)
        for (int j = 0; j < r; j++)
            need[i][j] = max[i][j] - allot[i][j];
}

bool isSafe(int processes[], int avail[], int max[][MAX_RESOURCES], int allot[][MAX_RESOURCES], int p, int r) {
    int need[MAX_PROCESSES][MAX_RESOURCES];
    calculateNeed(need, max, allot, p, r);

    bool finish[p];
    for (int i = 0; i < p; i++)
        finish[i] = false;

    int safeSeq[p];
    int work[r];
    for (int i = 0; i < r; i++)
        work[i] = avail[i];

    int count = 0;
    while (count < p) {
        bool found = false;
        for (int i = 0; i < p; i++) {
            if (!finish[i]) {
                int j;
                for (j = 0; j < r; j++)
                    if (need[i][j] > work[j])
                        break;

                if (j == r) {
                    for (int k = 0; k < r; k++)
                        work[k] += allot[i][k];
                    safeSeq[count++] = i;
                    finish[i] = true;
                    found = true;
                }
            }
        }

        if (!found) {
            printf("System is not in a safe state\n");
            return false;
        }
    }

    printf("System is in a safe state.\nSafe sequence is: ");
    for (int i = 0; i < p; i++)
        printf("P%d ", safeSeq[i]);
    printf("\n");

    return true;
}

bool requestResources(int processes[], int avail[], int max[][MAX_RESOURCES], int allot[][MAX_RESOURCES], int request[], int processIndex, int p, int r) {
    int need[MAX_PROCESSES][MAX_RESOURCES];
    calculateNeed(need, max, allot, p, r);

    for (int i = 0; i < r; i++) {
        if (request[i] > need[processIndex][i]) {
            printf("Error: Process has exceeded its maximum claim\n");
            return false;
        }
    }

    for (int i = 0; i < r; i++) {
        if (request[i] > avail[i]) {
            printf("Process must wait, resources are not available\n");
            return false;
        }
    }

    int tempAvail[MAX_RESOURCES];
    int tempAllot[MAX_PROCESSES][MAX_RESOURCES];
    for (int i = 0; i < r; i++) {
        tempAvail[i] = avail[i] - request[i];
        allot[processIndex][i] += request[i];
    }
    for (int i = 0; i < p; i++)
        for (int j = 0; j < r; j++)
            tempAllot[i][j] = allot[i][j];

    if (isSafe(processes, tempAvail, max, tempAllot, p, r)) {
        printf("Request can be granted.\n");
        return true;
    } else {
        for (int i = 0; i < r; i++) {
            avail[i] += request[i];
            allot[processIndex][i] -= request[i];
        }
        printf("Request cannot be granted, system would be in an unsafe state\n");
        return false;
    }
}

int main() {
    int p, r;
    printf("Enter the number of processes: ");
    scanf("%d", &p);
    printf("Enter the number of resources: ");
    scanf("%d", &r);

    int processes[p];
    int avail[r];
    int max[p][MAX_RESOURCES];
    int allot[p][MAX_RESOURCES];

    printf("Enter available resources:\n");
    for (int i = 0; i < r; i++) {
        printf("Resource %d: ", i);
        scanf("%d", &avail[i]);
    }

    printf("Enter the maximum resource matrix:\n");
    for (int i = 0; i < p; i++) {
        processes[i] = i;
        printf("For process P%d:\n", i);
        for (int j = 0; j < r; j++) {
            printf("Resource %d: ", j);
            scanf("%d", &max[i][j]);
        }
    }

    printf("Enter the allocated resource matrix:\n");
    for (int i = 0; i < p; i++) {
        printf("For process P%d:\n", i);
        for (int j = 0; j < r; j++) {
            printf("Resource %d: ", j);
            scanf("%d", &allot[i][j]);
        }
    }

    isSafe(processes, avail, max, allot, p, r);

    int request[r];
    int processIndex;
    printf("Enter process index requesting resources: ");
    scanf("%d", &processIndex);
    printf("Enter the resource request for process P%d:\n", processIndex);
    for (int i = 0; i < r; i++) {
        printf("Resource %d: ", i);
        scanf("%d", &request[i]);
    }

    requestResources(processes, avail, max, allot, request, processIndex, p, r);

    return 0;
}
