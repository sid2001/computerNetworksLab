#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/wait.h>
#include <math.h>

#define BUFFER_SIZE 8

pthread_mutex_t mutex;

struct Switch{
	int seq_bits;
	int s_fd_send[2];
	int s_fd_recv[2];
	int r_fd_send[2];
	int r_fd_recv[2];
};

struct FrameBuffer{
	int seq_bits;
	char data[BUFFER_SIZE];
};

struct Sender{
	int sf;
	int seq_bits;
	int* s_fd_send;
	int* s_fd_recv;
};

struct Receiver{
	int se;
	int seq_bits;
	int* r_fd_send;
	int* r_fd_recv;
};


void* senderPoint(void* arg){
	struct Switch* swt = (struct Switch*)arg;
	int data;
	while(read(swt->s_fd_recv[0],&data,sizeof(data))>0){
		printf("SwitchSen: Received data %d\n",data);
		fflush(stdout);
		if(rand()%3==0){
			printf("SS: Forwarding frame %d\n",data);
			fflush(stdout);
			write(swt->r_fd_send[1],&data,sizeof(data));
		}else{
			printf("SS: Dropping fame %d\n",data);
			fflush(stdout);
		}
	}
}

void* receiverPoint(void* arg){
	struct Switch* swt = (struct Switch*)arg;
	int data;
	while(read(swt->r_fd_recv[0],&data,sizeof(data))>0){
		printf("SwitchSen: Received ack %d\n",data);
		fflush(stdout);
		if(rand()%2==0){
			printf("SS: Forwarding ack %d\n",data);
			fflush(stdout);
			write(swt->s_fd_send[1],&data,sizeof(data));
		}else{
			printf("SS: Dropping ack %d\n",data);
			fflush(stdout);
		}
	}
}

void* senSender(void* arg){
	struct Sender* sen = (struct Sender*)arg;
	int win = (int)pow(2,sen->seq_bits);

	while(sen->sf < BUFFER_SIZE){
		pthread_mutex_lock(&mutex);
		for(int i = 0; i < win - 1 && sen->sf + i < BUFFER_SIZE; i++){
			int f = (sen->sf + i)%win;
			printf("Sender: sending frame %d %d\n",f,sen->sf + i);
			fflush(stdout);
			if(write(sen->s_fd_recv[1],&f,sizeof(f)) < 1) {
				printf("Sender: Error sending frame %d %d\n",f,sen->sf + i);
				fflush(stdout);
				exit(1);
			}
		}
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
}

void* senACK(void* arg){
	struct Sender* sen = (struct Sender*)arg;
	int win = (int)pow(2,sen->seq_bits);
	int ack;
	while(read(sen->s_fd_send[0],&ack,sizeof(ack)) > 0){
		printf("SenderACK: Received ACK %d\n",ack);
		fflush(stdout);
		pthread_mutex_lock(&mutex);
		int s_sf = sen->sf % win;
		if(ack < s_sf) sen->sf = sen->sf + (win - s_sf + ack);
		else sen->sf = sen->sf + ack - s_sf;
		
		printf("SenderACK: Sliding window to %d %d\n",sen->sf%win,sen->sf);
		fflush(stdout);
		pthread_mutex_unlock(&mutex);
	}

}

void startSender(struct Switch* swt, struct FrameBuffer* fr){
	struct Sender sen;
	sen.seq_bits = fr->seq_bits;
	sen.s_fd_send = swt->s_fd_send;
	sen.s_fd_recv = swt->s_fd_recv;
	sen.sf = 0;

	pthread_t p1,p2;

	pthread_create(&p1,NULL,senSender,(void *)&sen);
	pthread_create(&p2, NULL, senACK,(void *)&sen);

	if(pthread_join(p1,NULL)){
		printf("Flag1\n");
		exit(1);
	}
	if(pthread_join(p2,NULL)){
		printf("Flag2\n");
		exit(1);
	}

	return;
}

void startSwitch(struct Switch* swt) {
	pthread_t p1, p2;
	pthread_create(&p1,NULL,senderPoint,(void *)swt);
	pthread_create(&p2, NULL, receiverPoint,(void *)swt);

	pthread_join(p1,NULL);
	pthread_join(p2,NULL);

	return;
}

void startReceiver(struct Switch* swt){
	struct Receiver rec;
	
	rec.r_fd_send = swt->r_fd_send;
	rec.r_fd_recv = swt->r_fd_recv;
	rec.seq_bits = swt->seq_bits;
	rec.se = 0;
	int data;
	int win = (int)pow(2, rec.seq_bits);
	FILE* fptr;
	fptr = fopen("output.txt","w");
	if (fptr == NULL) {
		printf("Error opening file!\n");
	}

	while(read(rec.r_fd_send[0],&data,sizeof(data)) > 0){
		if(data != rec.se){
			printf("Receiver: Out of order frame %d\n",data);
			fflush(stdout);
			if(write(rec.r_fd_recv[1],&rec.se,sizeof(rec.se)) > 0){
				printf("Receiver: Sent ack %d\n",rec.se);
				fflush(stdout);
			}else{
				printf("Receiver: Error sending ack\n");
				exit(1);
			}
		}else{
			int ack = (rec.se + 1)%win;
			rec.se = ack;
			fprintf(fptr,"%d",data);
			fflush(fptr);
			printf("Receiver: Recvd frame %d\n",data);
			fflush(stdout);
			if(write(rec.r_fd_recv[1],&ack,sizeof(ack)) > 0){
				printf("Receiver: Sent ack %d\n",ack);
				fflush(stdout);
			}else{
				printf("Receiver: Error sending ack\n");
				fflush(stdout);
				exit(1);
			}
		}
	}
	return;
}

int main() {
	struct FrameBuffer frames;
	int st,sen,rec;
	
	printf("Enter number of address bits: ");
	scanf("%d",&frames.seq_bits);
	struct Switch swt;
	swt.seq_bits = frames.seq_bits;
	pipe(swt.s_fd_send);
	pipe(swt.s_fd_recv);
	pipe(swt.r_fd_send);
	pipe(swt.r_fd_recv);

	if((st = fork()) == 0){
		startSwitch(&swt);
	}
	else if((rec = fork()) == 0){
		startReceiver(&swt);
	}
	else if((sen = fork()) == 0){
		startSender(&swt,&frames);
	}
	waitpid(st,NULL,0);
	waitpid(rec,NULL,0);
	waitpid(sen,NULL,0);
}
