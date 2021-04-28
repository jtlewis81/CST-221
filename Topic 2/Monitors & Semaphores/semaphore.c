/**
CST-221 Topic 2: Semaphors

Author: Jamie Lewis

In this scenario, we have two working threads that take turns adding values ("quanities") from a local array to a global array, similar to having two employees adding values from separate lists to a main inventory list. The critical section also increments the value of the variable used to point to the working index in the global array. Functionality could be added to create a mutli-dimensional array to include item names too, but for now let's keep it simple. The main program prints the final product to the terminal. Each array holds five values, increments of 10s, one being "odd" (10, 30, 50, 70, 90) and the other "even" (20, 40 , 60 , 80, 100). The final output proves that the threads take turns iterating though their arrays because the output is: 10, 20, 30, 40, etc. When each thread enters its for loop, a while loop lock checks for a turn flag, and when it is false, proceeds to set the semaphore to wait so that the critcal section can finish, and then the turn flag is changed and the semaphore is set to post so that the waiting thread can continue its for loop. This continues until each thread has finished looping through its local array, and then the main program finishes. The semaphore usage in this example is binary, like a mutex.

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX 10

// THIS IS THE SHARED RESOURCE
int qtyList[MAX];
// Int used to point at the qtyList index in both threads
int qli = 0;

// Semaphore and turn flag.
sem_t semaphore;
int turn = 0;

/**
  Thread for Source 1
*/
void* source1(void * a) {
	// local array of 5 "quantities"
	int ql1[] = { 10, 30, 50, 70, 90 };

	for(int i = 0; i < 5; i++){
		// check turn flag, lock if condition true
		while(turn != 0);
		// set semaphore to wait, ensuring other thread cannot access global array at the same time
		sem_wait(&semaphore);

		// Critical Section
		qtyList[qli] = ql1[i]; // add local array value to global array
		(qli)++; // increment global array "pointer"

		// change turn flag and set semaphore to post to allow next thread to run
		turn = 1;
		sem_post(&semaphore);
	}
}

/**
  Thread for Source 2
  works like Source 1 thread
*/
void * source2(void * a) {
	int ql2[] = { 20, 40, 60, 80, 100 };
	for(int i = 0; i < 5; i++){
		while(turn != 1);
		sem_wait(&semaphore);
		// Critical Section
		qtyList[qli] = ql2[i];
		(qli)++;
		turn = 0;
		sem_post(&semaphore);
	}
}

/**
	Main Program
*/
int main() {

	// Initialize Semaphore
	sem_init(&semaphore, 0, 1);

	// initialize thread IDs
	pthread_t tid1, tid2;

	// Create 2 threads
	pthread_create(&tid1, NULL, source1, NULL);
	pthread_create(&tid2, NULL, source2, NULL);

	// Join threads
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	// print final result of the global array to terminal
	for(int i = 0; i < MAX; i++){
		printf("%d\n", qtyList[i]);
	}

	// Thread creation cleanup
	pthread_exit(NULL);
	sem_destroy(&semaphore);
	// End main()
	return 0;
}
