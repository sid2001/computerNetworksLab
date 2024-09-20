#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#define BUFF_SIZE 5

int gen = 1;
pthread_mutex_t mutex;
sem_t full, empty;
int buffer[BUFF_SIZE], counter, in, out;
pthread_t tid;

void *producer();
void *consumer();

void initialize_data() {
	pthread_mutex_init(&mutex, NULL);
	sem_init(&full,0,0);
	sem_init(&empty,0,BUFF_SIZE);
	counter = 0;
	in = 0;
	out = 0;
}

void *producer(){
	int item;
	while(1){
		sleep(1);
		item = rand() % 100;
		sem_wait(&empty);
		pthread_mutex_lock(&mutex);
		if(counter<BUFF_SIZE) {
			buffer[in] = item;
			counter++;
			in = (in + 1)%BUFF_SIZE;
			printf("\nProducer produced:%d\n",item);
		}
		pthread_mutex_unlock(&mutex);
		sem_post(&full);
	}
}

void *consumer() {
	int item;
	while(1) {
		sleep(1);
		sem_wait(&full);
		pthread_mutex_lock(&mutex);
		if(consumer>0){
			item = buffer[out];
			counter--;
			out = (out + 1)%BUFF_SIZE;
		}
		printf("\nConsumer consumed:%d\n",item);
		pthread_mutex_unlock(&mutex);
		sem_post(&empty);
	}
}

int main() {
	int i;
	initialize_data();
	pthread_create(&tid, NULL, producer, NULL);
	pthread_create(&tid, NULL, consumer, NULL);
	sleep(4);
	printf("\n EXIT");
	exit(0);
	return 0;
}
