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

int n, i, bufferSize, bufferIndex, *buffer;
int *pidProd, *pidCons;

/* semaphore s=1  and delay=0 */
sem_t mutex; /* prevents multiple accesses to the buffer */
sem_t emptyCount; /* checks if buffer is empty */
sem_t fillCount; /* checks if buffer is full  */

void *producer();
void *consumer();

void main(int argc, char *argv[]){
	if (argc != 4){
		printf("arguments need to be int in the format C P B");
		perror("Wrong number of arguments");
	}
	int numProd = atoi(argv[1]);
	int numCons = atoi(argv[2]);
	bufferSize = atoi(argv[3]);

	/* Initialize semaphore s to 1  and delay to 0 */
	sem_init(&mutex, 0, 1);
	sem_init(&emptyCount, 0, bufferSize);
	sem_init(&fillCount, 0, 0);
	n=0;

	pthread_t producerThreads[numProd];
	pthread_t consumerThreads[numCons];

	pidProd[0] = pthread_create(&producerThreads[0], NULL, producer, NULL); //TODO: turn this into for loop
	pidCons[0] = pthread_create(&consumerThreads[0], NULL, consumer, NULL);
	/* producer generates requests to print r (random number) pages every 5 seconds */
	/* producer will make new thread for each request */
	/*  */
	/*  */
}


/* application that is printing */
void *producer() {
	while (1) {
		//produce item
		//request to print r number of pages where r is a random number between 1 and 10
		sem_wait(&mutex);
		//put item in buffer
		n++;
		if (n == 1){
			sem_post(&emptyCount);
		}
		sem_post(&mutex);
	}
}
/* printer */
//printer also needs to sleep when printing the pages (if printing 6 pages should sleep for 6 second)
void *consumer() {
	int m;
	while (1) {
		sem_wait(&mutex);
		//take item from buffer
		n--;
		m = n;
		sem_post(&mutex);
		//consume - print the item
		//should sleep(p) seconds where p is the number of pages printed
		if (m == 0){  /* if buffer is empty, then consumer waits until there's a request */
			sem_wait(&emptyCount);
		}
	}
}
