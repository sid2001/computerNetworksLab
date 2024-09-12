#include <stdio.h>
#include <string.h>

int main() {
    int et[20], at[10], n, i, j, temp, st[10], ft[10], wt[10], ta[10];
    int totwt = 0, totta = 0;
    float awt, ata;
    char pn[10][10], t[10];

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        printf("Enter process name, arrival time, and execution time: ");
        scanf("%s%d%d", pn[i], &at[i], &et[i]);
    }

    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (et[i] > et[j]) {
                
                temp = at[i];
                at[i] = at[j];
                at[j] = temp;
               
                temp = et[i];
                et[i] = et[j];
                et[j] = temp;
                
                strcpy(t, pn[i]);
                strcpy(pn[i], pn[j]);
                strcpy(pn[j], t);
            }
        }
    }

    
    for (i = 0; i < n; i++) {
        if (i == 0) {
            st[i] = at[i];  
        } else {
            st[i] = (ft[i - 1] > at[i]) ? ft[i - 1] : at[i];
        }

        wt[i] = st[i] - at[i];
        ft[i] = st[i] + et[i];
        ta[i] = ft[i] - at[i]; 
        totwt += wt[i]; 
        totta += ta[i];  
    }

    awt = (float)totwt / n;
    ata = (float)totta / n; 


    printf("\nPname\tArrivalTime\tExecutionTime\tWaitingTime\tTurnaroundTime");
    for (i = 0; i < n; i++) {
        printf("\n%s\t%5d\t\t%5d\t\t%5d\t\t%5d", pn[i], at[i], et[i], wt[i], ta[i]);
    }


    printf("\n\nGantt Chart:\n");
    for (i = 0; i < n; i++) {
        printf("%d %s ", st[i], pn[i]);
    }
    printf("%d\n", ft[n - 1]);


    printf("\nAverage waiting time: %.2f", awt);
    printf("\nAverage turnaround time: %.2f\n", ata);

    return 0;
}
