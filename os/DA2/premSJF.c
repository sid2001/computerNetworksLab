#include <stdio.h>
#include <string.h>
#include <limits.h>

#define MAX_PROCESSES 10

struct Process {
    char pname[10]; // Process name
    int at;         // Arrival time
    int bt;         // Burst time
    int rt;         // Remaining time
    int st;         // Start time
    int ft;         // Finish time
    int wt;         // Waiting time
    int ta;         // Turnaround time
};

int main() {
    int n, i, j, currentTime = 0, completed = 0, minRemainingTime, shortest = -1;
    float totalWT = 0, totalTA = 0;
    struct Process processes[MAX_PROCESSES];
    int ganttStart[MAX_PROCESSES * 2]; 
    int ganttEnd[MAX_PROCESSES * 2];   
    char ganttChart[MAX_PROCESSES * 2][10]; 
    int ganttCount = 0;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        printf("Enter process name, arrival time & burst time: ");
        scanf("%s%d%d", processes[i].pname, &processes[i].at, &processes[i].bt);
        processes[i].rt = processes[i].bt; 
    }

    // SJF Preemptive Scheduling
    while (completed < n) {
        minRemainingTime = INT_MAX;
        shortest = -1;

        for (i = 0; i < n; i++) {
            if (processes[i].at <= currentTime && processes[i].rt > 0 && processes[i].rt < minRemainingTime) {
                minRemainingTime = processes[i].rt;
                shortest = i;
            }
        }

        if (shortest == -1) {
            currentTime++;
            continue;
        }

        // Check if the process changes in the Gantt chart
        if (ganttCount == 0 || strcmp(ganttChart[ganttCount - 1], processes[shortest].pname) != 0) {
            if (ganttCount > 0) {
                ganttEnd[ganttCount - 1] = currentTime;
            }
            ganttStart[ganttCount] = currentTime;
            strcpy(ganttChart[ganttCount++], processes[shortest].pname);
        }

        currentTime++;
        processes[shortest].rt--;

        if (processes[shortest].rt == 0) {
            completed++;
            processes[shortest].ft = currentTime;
            processes[shortest].ta = processes[shortest].ft - processes[shortest].at;
            processes[shortest].wt = processes[shortest].ta - processes[shortest].bt;
            totalWT += processes[shortest].wt;
            totalTA += processes[shortest].ta;
        }
    }

    ganttEnd[ganttCount - 1] = currentTime; 

    printf("\nGantt Chart:\n");
    for (i = 0; i < ganttCount; i++) {
        printf("%d %s ", ganttStart[i], ganttChart[i]);
    }
    printf("%d\n", ganttEnd[ganttCount - 1]);

    printf("\nProcess\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (i = 0; i < n; i++) {
        printf("%s\t%d\t\t%d\t\t%d\t\t%d\n", processes[i].pname, processes[i].at, processes[i].bt, processes[i].wt, processes[i].ta);
    }

    printf("\nAverage Waiting Time: %.2f\n", totalWT / n);
    printf("Average Turnaround Time: %.2f\n", totalTA / n);

    return 0;
}
