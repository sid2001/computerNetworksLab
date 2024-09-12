#include <stdio.h>

int main() {
    int p[10], at[10], bt[10], ct[10], tat[10], wt[10], startTime[10], endTime[10], i, j, temp = 0, n;
    float awt = 0, atat = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter process IDs:\n");
    for (i = 0; i < n; i++) {
        scanf("%d", &p[i]);
    }

    printf("Enter %d arrival times:\n", n);
    for (i = 0; i < n; i++) {
        scanf("%d", &at[i]);
    }

    printf("Enter %d burst times:\n", n);
    for (i = 0; i < n; i++) {
        scanf("%d", &bt[i]);
    }

    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (at[j] > at[j + 1]) {
                temp = p[j + 1];
                p[j + 1] = p[j];
                p[j] = temp;
                temp = at[j + 1];
                at[j + 1] = at[j];
                at[j] = temp;
                temp = bt[j + 1];
                bt[j + 1] = bt[j];
                bt[j] = temp;
            }
        }
    }

    startTime[0] = at[0];
    ct[0] = at[0] + bt[0];
    endTime[0] = ct[0]; 

    for (i = 1; i < n; i++) {
        if (ct[i - 1] < at[i]) {
            startTime[i] = at[i]; 
        } else {
            startTime[i] = ct[i - 1]; 
        }
        ct[i] = startTime[i] + bt[i];
        endTime[i] = ct[i]; 
    }

    printf("\nProcess\tArrival Time\tBurst Time\tCompletion Time\tTAT\tWT");
    for (i = 0; i < n; i++) {
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
        atat += tat[i];
        awt += wt[i];
    }
    atat /= n;
    awt /= n;

    for (i = 0; i < n; i++) {
        printf("\nP%d\t%d\t\t%d\t\t%d\t\t%d\t%d", p[i], at[i], bt[i], ct[i], tat[i], wt[i]);
    }

    printf("\n\nAverage Turnaround Time: %.2f", atat);
    printf("\nAverage Waiting Time: %.2f", awt);

    printf("\n\nGantt Chart:\n");
    for (i = 0; i < n; i++) {
        printf("%d P%d ", startTime[i], p[i]);
    }
    printf("%d\n", endTime[n - 1]);

    return 0;
}
