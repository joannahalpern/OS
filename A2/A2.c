/*
 * A2.c
 *
 *  Created on: Oct 8, 2014
 *      Author: Joanna
 */

//Instructions for A2
//buffer is an array with %
// x 10 2
//A client/producer keeps generating requests at a predefined interval TC(=5sec, say). - every 5 seconds
//the producer will put a request in the buffer (which is a random num of pages for the printer to print)
//if buffer is empty, then consumer goes into a busy/wait condition until there's a request (sleep)
//printer also needs to sleep when printing the pages (if printing 6 pages should sleep for 6 second)

//delay is a semaphore. first it get the signal (in the producer) which sets delay to 1. THen  the wait brings it back to 0
//this is to ensure onsumer doesn't everter critical section before producer
//s is to prevent multiple accesses to buffer
//delay is to check if queue is empty / buffer full (need extra condition)
//take () from queue
// m = n - this is incase n changes during
//consume is the actual printing (which takes time)

//also need to handle condition of full bufferwrite

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

int bufferSize, bufferIndex;
int buffer[];
int s;
int i, j, k, m;

/* semaphore s=1  and delay=0 */
sem_t mutex; /* prevents multiple accesses to the buffer */
sem_t emptyCount; /* checks if buffer is empty */
sem_t fillCount; /* checks if buffer is full  */

int *producer(int *arg);
int *consumer(int *arg);

int main(int argc, char *argv[]){
	if (argc != 4){
		printf("arguments need to be int in the format C P B");
		perror("Wrong number of arguments");
	}
	bufferIndex = 0;
	int numProd = atoi(argv[1]);
	int numCons = atoi(argv[2]);
	bufferSize = atoi(argv[3]);

	/* Initialize semaphore s to 1  and delay to 0 */
	sem_init(&mutex, 0, 1);
	sem_init(&emptyCount, 0, bufferSize);
	sem_init(&fillCount, 0, 0);

	printf("buffer size is %d\n", bufferSize);
	printf("everything is initialized\n\n");

	pthread_t producerThreads[numProd], consumerThreads[numCons];

	for (i=0; i<numProd; i++){
		s = pthread_create(&producerThreads[i], NULL, producer, (void *) i);
		if (s != 0) //pthread_creat() returns a 0 if it was successful else a positive int
			exit(EXIT_FAILURE);
	}
	for(j=0; j<numCons; j++){
		s = pthread_create(&consumerThreads[j], NULL, consumer, (void *) j);
		if (s != 0)
			exit(EXIT_FAILURE);
	}

	//Join all the threads to the main
	for(k=0; k<numProd; k++){
		s = pthread_join(&producerThreads[k], NULL);
		if (s != 0)
			exit(EXIT_FAILURE);
	}
	for(m=0; m<numCons; m++){
		s = pthread_join(&consumerThreads[m], NULL);
		if (s != 0)
			exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}


/* application that is printing */
int *producer(int *arg) {
	srand(time(NULL));
	int num, clientNum;
	while (1) {
		//produce item
		//request to print r number of pages where r is a random number between 1 and 10
		clientNum = (int) arg;
		num = rand();
		num = (num%10)+1;
		printf("Client %d has %d pages to print, ",clientNum, num);
		sem_wait(&emptyCount);
			sem_wait(&mutex);
				//put item in buffer
				buffer[bufferIndex] = num;
				printf("puts request in buffer %d\n",bufferIndex);
				bufferIndex = (bufferIndex+1)%bufferSize; //increment bufferIndex
			sem_post(&mutex);
		sem_post(&fillCount);
	}
}
/* printer */
//printer also needs to sleep when printing the pages (if printing 6 pages should sleep for 6 second)
int *consumer(int *arg) {
	int pages, printerNum;
	while (1) {
		printerNum = (int) arg;
		sem_wait(&fillCount);
			sem_wait(&mutex);
				//remove item from buffer
				pages = buffer[bufferIndex];
				bufferIndex = (bufferIndex+bufferSize-1)%bufferSize;
			sem_post(&mutex);
		sem_post(&emptyCount);
			printf("Printer %d starts printing %d pages from buffer %d\n", printerNum, pages, bufferIndex);
			sleep(pages);
			printf("Printer %d finishes printing %d pages from buffer %d\n", printerNum, pages, bufferIndex);
		//consume - print the item
		//should sleep(p) seconds where p is the number of pages printed
	}
}
