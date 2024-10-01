#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <pthread.h>

#define BUFFER_SIZE 8

pthread_mutex_t lock; 


struct FrameBuffer {
	int seq_bits;
	char data[BUFFER_SIZE];
};

struct Sender {
	int window_size;
	int sf;
	int seq_bits;
	int* s_fd_send;
	int* s_fd_recv;
	struct FrameBuffer* frames;
};

struct Receiver {
	int seq_bits;
	int window_size;
	int se;
	int* r_fd_send;
	int* r_fd_recv;
};

struct Switch {
	int s_fd_recv[2];
	int s_fd_send[2];
	int r_fd_recv[2];
	int r_fd_send[2];
};


void* SwitchSenderPoint(void* arg){
	struct Switch* swt = (struct Switch*)arg;
	int seq;
	int k = 1;
	while(read(swt->s_fd_recv[0],&seq,sizeof(int))>0) {
		if(rand() % 3==0) {
			fprintf(stdout,"\nSwitchSEND::dropped frame %d\n",seq);
			fflush(stdout);
			k++;
			continue;
		}
		k++;
		fprintf(stdout,"\nSwitchSEND::Recvd Frame from sender. Forwarding...\n");
		fflush(stdout);
		if(write(swt->r_fd_send[1],&seq,sizeof(int))<1){
			printf("\nSwitchSEND::Error while forwarding to receiver.\n");
			fflush(stdout);
		}
	}
	printf("\nSwitchSenderPoint Disconnected..\n");
	close(swt->r_fd_send[1]);
	close(swt->s_fd_recv[0]);
	exit(0);
}

void* SwitchReceiverPoint(void* arg){
	struct Switch* swt = (struct Switch*)arg;
	int seq;
	int k = 1;
	while(read(swt->r_fd_recv[0],&seq,sizeof(int))>0) {
		if(rand() % 4 == 0){
			fprintf(stdout,"\nDropped ack: %d\n",seq);
			fflush(stdout);
			k++;
			continue;
		}
		k++;
		fprintf(stdout,"\nSwitchRECV::Recvd ACK %d from recvr. Forwarding...\n",seq);
		fflush(stdout);
		if(write(swt->s_fd_send[1],&seq,sizeof(int))<1){
			printf("\nSwitchRECV::Error while forwarding to sender.\n");
			fflush(stdout);
		}
	}
	printf("\nSwitchReceiverPoint disconnected\n");
	close(swt->r_fd_recv[0]);
	close(swt->s_fd_send[1]);
	exit(0);
}

void* SendFrame(void* sender) {
	struct Sender *s = (struct Sender *)sender;
	int m_seq_n = pow(2,s->seq_bits);
	while(s->sf < BUFFER_SIZE){
		int s_n;
		pthread_mutex_lock(&lock);
		for(int i = 0; i < m_seq_n - 1 && s->sf + i < BUFFER_SIZE; i++){
			s_n = (s->sf + i) % m_seq_n;
			fprintf(stdout,"\nSender::Sending frame %d sf %d",s_n,s->sf+i);
			fflush(stdout);
			if( write(s->s_fd_recv[1], &s_n, sizeof(int)) < 1 ) {
				printf("\nSender::Error while sending frame.\n");
				fflush(stdout);
				exit(1);
			}
		}
		pthread_mutex_unlock(&lock);
		sleep(1);
	}
	printf("\nSender::All frames sent successfully.\n");
	close(s->s_fd_recv[1]);
	close(s->s_fd_send[1]);
	fflush(stdout);
	return NULL;
}

void* RecvACK(void *sender) {
	//there's a loophole when the last ack gets lost then it can create
	//an infinite loop where recv might expect next 0 frame to recv but
	//sender since it hasnt received the last ack it will keep sending the 
	//last frame while the receiver will expect some other frame.
	//a solution can be stop sending frames after some time when
	//no ack arrives.
	
	//okay i was wrong a bit 
	//the receiver will send the ack of the last correct frame received
	//this will repeat for sometimes until the connection finally closed 
	//due to too much delay
	struct Sender *s = (struct Sender*)sender;
	int ack;
	int m_seq_n = pow(2,s->seq_bits);
	int ws = m_seq_n - 1;
	while(read(s->s_fd_send[0], &ack, sizeof(int)) > 0) {
		pthread_mutex_lock(&lock);
		if(ack < s->sf % m_seq_n)
			s->sf = s->sf + ((m_seq_n - s->sf % m_seq_n) + ack);
		else
			s->sf = s->sf + (ack - (s->sf) % m_seq_n);
		printf("\nSenderACK::ACK %d recvd sliding window. sf=%d\n",ack,s->sf);
		fflush(stdout);
		pthread_mutex_unlock(&lock);
	}
	printf("\nSenderRecvAck exit.\n");
	close(s->s_fd_send[0]);
	return NULL;
}

void StartSender(struct Switch *swt, struct FrameBuffer *frame) {
	close(swt->r_fd_recv[0]);
	close(swt->r_fd_recv[1]);
	close(swt->r_fd_send[0]);
	close(swt->r_fd_send[1]);
	close(swt->s_fd_send[1]);
	close(swt->s_fd_recv[0]);
	int send_bit,recv_bit;
	int flag;
	struct Sender s;
	s.sf = 0;
	s.seq_bits = frame->seq_bits; 
	s.s_fd_send = swt->s_fd_send;
	s.s_fd_recv = swt->s_fd_recv;
	s.frames = frame;
	//s.window_size = frame->window_size;
	pthread_t sen, rec;
	pthread_create(&sen, NULL,SendFrame,(void *)(&s));
	pthread_create(&rec, NULL, RecvACK,(void *)(&s));
	
	if(pthread_join(sen,NULL)) {
		printf("\nerror joining thread 1\n");
		fflush(stdout);
		exit(1);
	}
	if(pthread_join(rec,NULL)) {
		printf("\nerror joining thread 2\n");
		fflush(stdout);
		exit(1);
	}
	exit(0);
}

void StartReceiver(struct Switch *swt, struct FrameBuffer* frame){
	struct Receiver r;
	close(swt->r_fd_send[1]);
	close(swt->r_fd_recv[0]);
	close(swt->s_fd_send[0]);
	close(swt->s_fd_send[1]);
	close(swt->s_fd_recv[0]);
	close(swt->s_fd_recv[1]);
	r.seq_bits = frame->seq_bits;
	r.se = 0;
	r.r_fd_send = swt->r_fd_send;
	r.r_fd_recv = swt->r_fd_recv;
	int m_seq_n = pow(2,r.seq_bits);
	int seq = 0;
	FILE *fp;
	fp = fopen("output.txt","w");
	
	while(read(r.r_fd_send[0],&seq,sizeof(int))>0) {
		if(r.se == seq) {
			int ack = (seq+1) % m_seq_n;
			fprintf(fp,"%d",seq);
			fflush(fp);
			printf("\nReceiver::Recvd frame: %d\nSending ACK for frame %d.\n",seq,seq);
			if(write(r.r_fd_recv[1],&ack,sizeof(int))<1){
				printf("\nReceiver::Error in sending ACK.\n");
				fflush(stdout);
				exit(1);
			}
			r.se = ack;
		}else {
			printf("\nReceiver::Frame recvd %d need %d. Out of order.\n",seq,r.se);
			fflush(stdout);
			if(write(r.r_fd_recv[1],&(r.se),sizeof(int))<1){
				printf("\nReceiver::Error in sending ACK.\n");
				fflush(stdout);
				exit(1);
			}
			printf("\nReceiver::Resent last correct frame ACK %d\n",r.se);
			fflush(stdout);
		}
	}
	fclose(fp);
	close(r.r_fd_recv[1]);
	exit(0);
} 
void StartSwitch(struct Switch *swt) {
	//close fds not used
	pthread_t sen,rec;
	close(swt->r_fd_send[0]);
	close(swt->r_fd_recv[1]);
	close(swt->s_fd_send[0]);
	close(swt->s_fd_recv[1]);
	pthread_create(&sen,NULL,SwitchSenderPoint,(void *)swt);
	pthread_create(&rec,NULL,SwitchReceiverPoint,(void *)swt);

	if(pthread_join(sen,NULL)) {
		printf("\nerror joining thread 1\n");
		exit(1);
	}
	if(pthread_join(rec,NULL)) {
		printf("\nerror joining thread 2\n");
		exit(1);
	}
	exit(0);
}
int main() {
	struct FrameBuffer frame;
	frame.seq_bits = 0;
	
	printf("Enter seq bits: ");
	scanf("%d", &frame.seq_bits);
	
	struct Switch swt;
	pipe(swt.s_fd_recv);
	pipe(swt.s_fd_send);
	pipe(swt.r_fd_recv);
	pipe(swt.r_fd_send);

	int spid, rpid, sepid,status;
	if((spid = fork())==0) {
		printf("\nSwitch started.\n");
		StartSwitch(&swt);
	}else if((rpid = fork())==0) {
		printf("\nReceiver started.\n");
		StartReceiver(&swt,&frame);
	}else if((sepid = fork())==0){
		printf("\nSender started.\n");
		StartSender(&swt,&frame);
	}
	waitpid(sepid, &status, 0);
	printf("Sender process finished.\n");
	waitpid(rpid, &status, 0);
	printf("Receiver process finished.\n");
	waitpid(spid, &status, 0);
	printf("Switch process finished.\n");
	
	exit(0);
}

