#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAX 20

int parityBits(char* dataword){
	int len = strlen(dataword);
	int p = 0;
	while(pow(2,p)<p+len+1){
		p++;
	}
	return p;
}

char xor(char a, char b){
	return a==b?'0':'1';
}

char calculateParity(char* codeword, int from,int ff){
	int ans = 0;
	int flag = ff;
	for(int i = 1; i*from - 1 < strlen(codeword);i+=2){
		int idx = from*i - 1;
		for(int j = 0; j<from && idx+j < strlen(codeword);j++){
			if(flag){
				flag = 0;
				continue;
			}
			ans += (codeword[idx+j]-'0');
		}
	}
	return ans%2==0?'0':'1';
}


void encode(char* dataword, char* codeword) {
	int pb = parityBits(dataword);
	int k = strlen(dataword) + pb;
	int p = 0;
	int idx = 0;
	for(int i = 1; i <= k; i++){
		int po = pow(2,p);
		if(po == i){
			codeword[i-1] = '#';
			p++;
		}else{
			codeword[i-1] = dataword[idx++];
		}
	}

	printf("code: %s\n",codeword);
	for(int i = 0; i < k; i++){
		if(codeword[i] == '#'){
			codeword[i] = calculateParity(codeword,i+1,1);
		}
	}
	return;
}

void check(char* codeword) {
	int k = 0;
	int idx = 0;
	char* parity = (char*)malloc(sizeof(char)*MAX);
	for(int i = 0; i < strlen(codeword); i++){
		if(pow(2,k)-1 == i){
			k++;
			parity[idx++] = calculateParity(codeword,i+1,0);
		}
	}
	printf("%s\n",parity);
	return;
}
int main() {
	printf("1.Encode\n2.Error Check\n");
	int choice;
	scanf("%d",&choice);

	char* dataword = (char *)malloc(sizeof(char)*MAX);
	char* codeword = (char *)malloc(sizeof(char)*MAX);

	switch(choice){
		case 1:
			scanf("%s",dataword);
			encode(dataword, codeword);
			printf("%s",codeword);
		break;
		case 2:
			scanf("%s",codeword);
			check(codeword);
		break;
	}
}
