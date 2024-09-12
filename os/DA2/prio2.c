#include <stdio.h>
#include <limits.h>

#define MAX_PROCESSES 100

struct Process {
    int pid;      
    int burstTime; 
    int arrivalTime;
    int priority;   
    int remainingTime;
    int waitingTime;  
    int turnaroundTime;
    int completed;  
};

struct GanttChart {
    int pid;      
    int startTime;
    int endTime;
};

int main() {
    int n;
    struct Process processes[MAX_PROCESSES];
    struct GanttChart ganttChart[MAX_PROCESSES * 10];
    int ganttChartIndex = 0;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
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
    while (completedProcesses < n) {
        int minPriority = INT_MAX;
        int processIndex = -1;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && !processes[i].completed && processes[i].priority < minPriority) {
                minPriority = processes[i].priority;
                processIndex = i;
            }
        }

        if (processIndex == -1) {
            currentTime++;
            continue;
        }

        ganttChart[ganttChartIndex].pid = processes[processIndex].pid;
        ganttChart[ganttChartIndex].startTime = currentTime;

        processes[processIndex].remainingTime--;
        currentTime++;

        ganttChart[ganttChartIndex].endTime = currentTime;
        ganttChartIndex++;

        if (processes[processIndex].remainingTime == 0) {
            processes[processIndex].completed = 1;
            completedProcesses++;

            processes[processIndex].turnaroundTime = currentTime - processes[processIndex].arrivalTime;
            processes[processIndex].waitingTime = processes[processIndex].turnaroundTime - processes[processIndex].burstTime;

            totalWaitingTime += processes[processIndex].waitingTime;
            totalTurnaroundTime += processes[processIndex].turnaroundTime;
        }
    }


    float averageWaitingTime = (float)totalWaitingTime / n;
    float averageTurnaroundTime = (float)totalTurnaroundTime / n;


    printf("\nProcess\tArrival Time\tBurst Time\tPriority\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; i++) {
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


    printf("\n\nGantt Chart:\n");
    for (int i = 0; i < ganttChartIndex; i++) {
        printf("%d P%d ", ganttChart[i].startTime, ganttChart[i].pid);
    }
    printf("%d\n", ganttChart[ganttChartIndex - 1].endTime);  

    return 0;
}
