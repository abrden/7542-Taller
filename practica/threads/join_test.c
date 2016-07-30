#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* run(void* arg) {
	char* msg = (char*) arg;
	printf("The msg is: %s\n", msg);
	return NULL;
}

void main(void) {
	char* msg = "Hi from the thread";
	pthread_t t;
	pthread_create(&t, NULL, run, msg);
	sleep(2); // Give priority to thread
	pthread_join(t, NULL);
}
