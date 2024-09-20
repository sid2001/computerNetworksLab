#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int shared_var = 0;
sem_t semaphore;

void* increment(void* arg) {
	for(int i = 0;i<5;i++) {
		sem_wait(&semaphore);
		shared_var++;
		printf("Incremented: shared_var = %d\n", shared_var);
		sem_post(&semaphore);
		sleep(1);
	}
	return NULL;
}

void* decrement(void* arg) {
	for(int i = 0;i<5;++i) {
		sem_wait(&semaphore);
		shared_var--;
		printf("Decremented: shared_var = %d\n", shared_var);
		sem_post(&semaphore);
		sleep(1);
	}
	return NULL;
}

int main() {
	pthread_t thread1, thread2;
	sem_init(&semaphore, 0, 1);

	pthread_create(&thread1, NULL, increment, NULL);
	pthread_create(&thread2, NULL, decrement, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	sem_destroy(&semaphore);

	printf("Final value of shared_var = %d\n", shared_var);
	return 0;
}
