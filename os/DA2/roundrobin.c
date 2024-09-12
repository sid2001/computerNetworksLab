#include <stdio.h>

// Structure to represent a process
struct Process {
    int pid;         // Process ID
    int burstTime;   // Burst time
    int arrivalTime; // Arrival time
    int remainingTime; // Remaining burst time
    int waitingTime;   // Waiting time
    int turnaroundTime; // Turnaround time
};

int main() {
    int n, timeQuantum;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process processes[n];

    // Input burst time and arrival time for each process
    for(int i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        printf("Enter burst time for process %d: ", i + 1);
        scanf("%d", &processes[i].burstTime);
        printf("Enter arrival time for process %d: ", i + 1);
        scanf("%d", &processes[i].arrivalTime);
        processes[i].remainingTime = processes[i].burstTime;
        processes[i].waitingTime = 0;
    }

    printf("Enter time quantum: ");
    scanf("%d", &timeQuantum);

    int currentTime = 0, completedProcesses = 0;
    int totalWaitingTime = 0, totalTurnaroundTime = 0;

    // Round Robin scheduling
    while(completedProcesses < n) {
        int allProcessesIdle = 1;

        for(int i = 0; i < n; i++) {
            if(processes[i].remainingTime > 0 && processes[i].arrivalTime <= currentTime) {
                allProcessesIdle = 0;

                if(processes[i].remainingTime > timeQuantum) {
                    currentTime += timeQuantum;
                    processes[i].remainingTime -= timeQuantum;
                } else {
                    currentTime += processes[i].remainingTime;
                    processes[i].remainingTime = 0;
                    completedProcesses++;

                    processes[i].turnaroundTime = currentTime - processes[i].arrivalTime;
                    processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burstTime;

                    totalWaitingTime += processes[i].waitingTime;
                    totalTurnaroundTime += processes[i].turnaroundTime;
                }
            }
        }

        if(allProcessesIdle) {
            currentTime++;
        }
    }

    // Calculate average waiting time and turnaround time
    float averageWaitingTime = (float)totalWaitingTime / n;
    float averageTurnaroundTime = (float)totalTurnaroundTime / n;

    // Display results
    printf("\nProcess\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for(int i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\n",
               processes[i].pid,
               processes[i].arrivalTime,
               processes[i].burstTime,
               processes[i].waitingTime,
               processes[i].turnaroundTime);
    }

    printf("\nTotal Waiting Time: %d", totalWaitingTime);
    printf("\nAverage Waiting Time: %.2f", averageWaitingTime);
    printf("\nTotal Turnaround Time: %d", totalTurnaroundTime);
    printf("\nAverage Turnaround Time: %.2f", averageTurnaroundTime);

    return 0;
}
