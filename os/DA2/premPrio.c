#include <stdio.h>

#define MAX_PROCESSES 10

// Structure to represent a process
struct Process {
    int pid;         // Process ID
    int burstTime;   // Burst time
    int arrivalTime; // Arrival time
    int priority;    // Priority
    int waitingTime; // Waiting time
    int turnaroundTime; // Turnaround time
    int completionTime; // Completion time
};

void calculateCompletionTimes(struct Process processes[], int n) {
    int time = 0;
    for (int i = 0; i < n; i++) {
        int highestPriorityIndex = -1;
        int highestPriority = 1000000;

        for (int j = 0; j < n; j++) {
            if (processes[j].arrivalTime <= time && processes[j].priority < highestPriority && processes[j].completionTime == 0) {
                highestPriority = processes[j].priority;
                highestPriorityIndex = j;
            }
        }

        if (highestPriorityIndex != -1) {
            time += processes[highestPriorityIndex].burstTime;
            processes[highestPriorityIndex].completionTime = time;
            processes[highestPriorityIndex].turnaroundTime = processes[highestPriorityIndex].completionTime - processes[highestPriorityIndex].arrivalTime;
            processes[highestPriorityIndex].waitingTime = processes[highestPriorityIndex].turnaroundTime - processes[highestPriorityIndex].burstTime;
        }
    }
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process processes[MAX_PROCESSES];

    // Input burst time, arrival time, and priority for each process
    for (int i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        printf("Enter burst time for process %d: ", i + 1);
        scanf("%d", &processes[i].burstTime);
        printf("Enter arrival time for process %d: ", i + 1);
        scanf("%d", &processes[i].arrivalTime);
        printf("Enter priority for process %d: ", i + 1);
        scanf("%d", &processes[i].priority);
        processes[i].completionTime = 0;
    }

    calculateCompletionTimes(processes, n);

    // Calculate average waiting time and turnaround time
    float totalWaitingTime = 0, totalTurnaroundTime = 0;

    printf("\nProcess\tArrival Time\tBurst Time\tPriority\tWaiting Time\tTurnaround Time\tCompletion Time\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               processes[i].pid,
               processes[i].arrivalTime,
               processes[i].burstTime,
               processes[i].priority,
               processes[i].waitingTime,
               processes[i].turnaroundTime,
               processes[i].completionTime);
        totalWaitingTime += processes[i].waitingTime;
        totalTurnaroundTime += processes[i].turnaroundTime;
    }

    float averageWaitingTime = totalWaitingTime / n;
    float averageTurnaroundTime = totalTurnaroundTime / n;

    printf("\nAverage Waiting Time: %.2f", averageWaitingTime);
    printf("\nAverage Turnaround Time: %.2f\n", averageTurnaroundTime);

    return 0;
}
