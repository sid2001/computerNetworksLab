#include <stdio.h>
#include <limits.h>

#define MAX_PROCESSES 100

// Structure to represent a process
struct Process {
    int pid;        // Process ID
    int burstTime;  // Burst time
    int arrivalTime;// Arrival time
    int priority;   // Priority
    int remainingTime; // Remaining burst time
    int waitingTime;   // Waiting time
    int turnaroundTime; // Turnaround time
    int completed;  // Completion status
};

int main() {
    int n;
    struct Process processes[MAX_PROCESSES];

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    // Input burst time, arrival time, and priority for each process
    for(int i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        printf("Enter burst time for process %d: ", i + 1);
        scanf("%d", &processes[i].burstTime);
        printf("Enter arrival time for process %d: ", i + 1);
        scanf("%d", &processes[i].arrivalTime);
        printf("Enter priority for process %d: ", i + 1);
        scanf("%d", &processes[i].priority);
        processes[i].remainingTime = processes[i].burstTime;
        processes[i].completed = 0;
    }

    int currentTime = 0, completedProcesses = 0;
    int totalWaitingTime = 0, totalTurnaroundTime = 0;

    // Scheduling loop
    while(completedProcesses < n) {
        int minPriority = INT_MAX;
        int processIndex = -1;

        // Find the process with the highest priority (smallest number) that has arrived
        for(int i = 0; i < n; i++) {
            if(processes[i].arrivalTime <= currentTime && !processes[i].completed && processes[i].priority < minPriority) {
                minPriority = processes[i].priority;
                processIndex = i;
            }
        }

        if(processIndex == -1) {
            // No process is ready to execute; increment time
            currentTime++;
            continue;
        }

        // Execute the process with the highest priority
        processes[processIndex].remainingTime--;
        currentTime++;

        // If the process is completed
        if(processes[processIndex].remainingTime == 0) {
            processes[processIndex].completed = 1;
            completedProcesses++;

            processes[processIndex].turnaroundTime = currentTime - processes[processIndex].arrivalTime;
            processes[processIndex].waitingTime = processes[processIndex].turnaroundTime - processes[processIndex].burstTime;

            totalWaitingTime += processes[processIndex].waitingTime;
            totalTurnaroundTime += processes[processIndex].turnaroundTime;
        }
    }

    // Calculate average waiting time and turnaround time
    float averageWaitingTime = (float)totalWaitingTime / n;
    float averageTurnaroundTime = (float)totalTurnaroundTime / n;

    // Display results
    printf("\nProcess\tArrival Time\tBurst Time\tPriority\tWaiting Time\tTurnaround Time\n");
    for(int i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               processes[i].pid,
               processes[i].arrivalTime,
               processes[i].burstTime,
               processes[i].priority,
               processes[i].waitingTime,
               processes[i].turnaroundTime);
    }

    printf("\nTotal Waiting Time: %d", totalWaitingTime);
    printf("\nAverage Waiting Time: %.2f", averageWaitingTime);
    printf("\nTotal Turnaround Time: %d", totalTurnaroundTime);
    printf("\nAverage Turnaround Time: %.2f", averageTurnaroundTime);

    return 0;
}
