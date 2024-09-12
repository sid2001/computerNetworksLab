#include <stdio.h>
struct proc
{
	int a;
	int b;
	int no;
	int wt;
	int tat;
};

struct proc read(int i) {
	struct proc p;
	printf("\n\nThe process no.:%d.\n",i);
	p.no = i;
	printf("Enter the arrival time:");
	scanf("%d",&p.a);
	printf("Enter the burst time:");
	scanf("%d",&p.b);
	return p;
}

int main() {
	struct proc p[10], temp;
	int n, g[10];
	float att = 0, awt = 0;
	printf("Enter the number of process");
	scanf("%d",&n);

	int i, j;
	for(i = 0;i<n;i++) {
		p[i] = read(i);
	}
	for(i = 0; i<n;i++) {
		for(j = i;j<n;j++){
			if(p[i].a > p[j].a){
				temp = p[i];
				p[i] = p[j];
				p[j] = temp;
			}
		}
	}
	g[0] = 0;
	for(i = 0; i<n; i++) 
		g[i+1] = g[i] - p[i].b;
	for(i = 0; i<n; i++){

	}
}
