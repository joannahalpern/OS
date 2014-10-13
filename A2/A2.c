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

int bufferSize, bufferIndex;
int buffer[], pidProd[], pidCons[];
int s, pidP, pidC;

/* semaphore s=1  and delay=0 */
sem_t mutex; /* prevents multiple accesses to the buffer */
sem_t emptyCount; /* checks if buffer is empty */
sem_t fillCount; /* checks if buffer is full  */

int *producer(int *arg);
int *consumer(int printerNum);

//int *producer(int *arg) {
////	int clientNum = (int) &arg;
////	printf("Client %d ",pidProd[clientNum]);
//	printf("I made it into the producer thread\n");
//	printf("blah blah blahhhhh\n");
//	printf("blah blah blahhhhh\n");
//	printf("blah blah blahhhhh\n");
//	printf("blah blah blahhhhh\n");
//	return 0;
//}

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

	pthread_t p, c;
	pidProd[0] = pthread_create(&p, NULL, producer, 0);
	if (pidProd[0] != 0)
		exit(EXIT_FAILURE);
	pidC = pthread_create(&c, NULL, consumer, 5);
//	if (pidC != 0)
//		exit(EXIT_FAILURE);

	s = pthread_join(&p, NULL);
	s = pthread_join(&c, NULL);

//	pthread_t producerThreads[numProd];
//	pthread_t consumerThreads[numCons];

//	pidProd[0] = pthread_create(&producerThreads[0], NULL, producer, 0); //TODO: turn this into for loop
//	s = pthread_create(&producerThreads[0], NULL, producer, 0); //TODO: turn this into for loop
//
//	s = pthread_join(&producerThreads[0], NULL);
//	printf("ahhhhh");
//	printf("ahhhhh\n");
//	printf("ahhhhh");
//	printf("ahhhhh\n");
//	for (s=0; s<100; s++){
//		printf("ahhhhh");
//	}
//	pidCons[0] = pthread_create(&consumerThreads[0], NULL, consumer, 0);
	/* producer generates requests to print r (random number) pages every 5 seconds */
	/* producer will make new thread for each request */
	/*  */
	/*  */
	//TODO: join all the threads at the end (ie 2 for loops)

	exit(EXIT_SUCCESS);
}


/* application that is printing */
int *producer(int *arg) {
	int num;
	while (1) {
		//produce item
		//request to print r number of pages where r is a random number between 1 and 10
		num = 2;
		printf("Client %d has %d pages to print, ",9, num);
		sem_wait(&emptyCount);
			sem_wait(&mutex);
				//put item in buffer
				buffer[bufferIndex] = num;
				printf("puts request in buffer %d\n",bufferIndex);
//				bufferIndex = (bufferIndex+1)%bufferSize; //increment bufferIndex
				bufferIndex++;
			sem_post(&mutex);
		sem_post(&fillCount);
	}
}
/* printer */
//printer also needs to sleep when printing the pages (if printing 6 pages should sleep for 6 second)
int *consumer(int printerNum) {
	int pages;
	while (1) {
		sem_wait(&fillCount);
			sem_wait(&mutex);
				//remove item from buffer
				pages = buffer[bufferIndex];
				bufferIndex = (bufferIndex+bufferSize-1)%bufferSize;
			sem_post(&mutex);
		sem_post(&emptyCount);
			printf("Printer %d starts printing %d pages from buffer %d", 9, pages, 9);
			sleep(pages);
		//consume - print the item
		//should sleep(p) seconds where p is the number of pages printed
	}
}
