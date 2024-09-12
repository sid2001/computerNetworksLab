#include <stdio.h>

struct Process {
    int pid;  
    int burstTime;
    int arrivalTime;
    int remainingTime;
    int waitingTime;
    int turnaroundTime;
};

struct GanttChart {
    int pid;
    int startTime;
    int endTime;
};

int main() {
    int n, timeQuantum;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process processes[n];
    struct GanttChart ganttChart[100];
    int ganttIndex = 0; 

    for (int i = 0; i < n; i++) {
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

    while (completedProcesses < n) {
        int allProcessesIdle = 1;

        for (int i = 0; i < n; i++) {
            if (processes[i].remainingTime > 0 && processes[i].arrivalTime <= currentTime) {
                allProcessesIdle = 0;

                ganttChart[ganttIndex].pid = processes[i].pid;
                ganttChart[ganttIndex].startTime = currentTime;

                if (processes[i].remainingTime > timeQuantum) {
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

                ganttChart[ganttIndex].endTime = currentTime;
                ganttIndex++;
            }
        }

        if (allProcessesIdle) {
            currentTime++;
        }
    }

    float averageWaitingTime = (float)totalWaitingTime / n;
    float averageTurnaroundTime = (float)totalTurnaroundTime / n;

    printf("\nProcess\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; i++) {
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

    printf("\n\nGantt Chart:\n");
    for (int i = 0; i < ganttIndex; i++) {
        printf("%d P%d ", ganttChart[i].startTime, ganttChart[i].pid);  
    }
    printf("%d\n", ganttChart[ganttIndex - 1].endTime);  

    return 0;
}
