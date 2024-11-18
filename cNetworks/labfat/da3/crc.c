#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define DWS 20
#define DS 10

char xor(char a, char b){
	if(a == b) return '0';
	else return '1';
}

void appendzero(char* dataword, char* divisor){
	int d_s = strlen(dataword);
	char* ptr = dataword+d_s;
	int le = strlen(divisor) - 1;
	for(int i = 0; i < le; i++) {
		*ptr = '0';
		ptr++;
	}
	return;
}
void display(char* ptr, int len,int l, int u){
	int c = len;
	c = len - abs(l-u);
	int x = 0;
	while(c--) {
		printf("%c",ptr[(l+x)%len]);
		x++;
	}
	printf("\n");
}

int check(char* rem, int l,int len){
	int ll = len;
	while(len--){
		if(rem[l]=='1') return 0;
		printf("l %d\n",l);
		l = (l + 1)%ll;
	}
	return 1;
}

int shift(char *rem,int* idx, int len) {
	int c = 0;
	while(c < len && rem[*idx] != '1') {
		c++;
		*idx = (*idx + 1)%len;
	}
	return c;
}

void encode(char* dataword, char* divisor,int flag) {
	int o_len = strlen(dataword);
	if(flag) appendzero(dataword, divisor);
	int d_s = strlen(divisor);
	int dd_s = strlen(dataword);
	
	char* rem = (char *)malloc(sizeof(char)*d_s);
	int r_l = 0;
	int r_u = 0;

	int idx = 0;

	while(dataword[idx] != '1') idx++;

	for(int i = 0; i < d_s; i++) {
		rem[i] = xor(dataword[idx], divisor[i]);
		idx++;
	}
	printf("rem 1: ");
	display(rem,d_s,r_l,r_u);
	printf("idx: %d\n",idx);
	while(1){
		int s = shift(rem, &r_l, d_s);
		printf("shifted by: %d\n",s);
		for(int i = 0; i < s && idx < dd_s; i++){
			rem[r_u++] = dataword[idx++];
			r_u = r_u%d_s;
		}
		printf("Rem: ");
		display(rem,d_s,r_l,r_u);
		printf("idx: %d\n",idx);

		if(idx == dd_s && r_l != r_u) break;
		for(int i = 0; i < d_s; i++){
			rem[(r_l+i)%d_s] = xor(rem[(r_l+i)%d_s],divisor[i]);
		}
		if(idx==dd_s) break;
		printf("Rem2: ");
		display(rem,d_s,r_l,r_u);
	}
	if(!flag) {
		if(check(rem, r_l, d_s)) printf("Correct\n");
		else printf("Invalid\n");
		return;
	}
	shift(rem,&r_l,d_s);
	printf("%d\n",abs(r_u - r_l));
	int ii = o_len + abs(r_u - r_l);
	int k = 0;
	printf("%s\n",rem);
	while(ii < dd_s){
		dataword[ii] = rem[(k+r_l)%d_s];
		k++;
		ii++;
	}
	return;
}

int main() {
	int choice;
	printf("1.Encode\n2.Decode\n");
	scanf("%d",&choice);

	char* dataword = (char *)malloc(sizeof(char)*DWS);
	char* divisor = (char *)malloc(sizeof(char)*DS);
	
	switch(choice){
		case 1:
			scanf("%s",dataword);
			scanf("%s",divisor);
			encode(dataword,divisor,1);
			printf("%s\n",dataword);
		break;
		case 2:
			scanf("%s",dataword);
			scanf("%s",divisor);
			encode(dataword,divisor,0);
		break;
	}
}
