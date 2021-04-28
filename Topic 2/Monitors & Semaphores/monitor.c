/**
CST-221 Topic 2: Monitors

Author: Jamie Lewis

In this scenario, we have two working threads that take turns adding values ("quanities") from a local array to a global array, similar to having two employees adding values from separate lists to a main inventory list. The critical section also increments the value of the variable used to point to the working index in the global array. Functionality could be added to create a mutli-dimensional array to include item names too, but for now let's keep it simple. The main program prints the final product to the terminal. Each array holds five values, increments of 10s, one being "odd" (10, 30, 50, 70, 90) and the other "even" (20, 40 , 60 , 80, 100). The final output proves that the threads take turns iterating though their arrays because the output is: 10, 20, 30, 40, etc. Since the C programming language does not have monitors we have to implement our own locks and conditions, and control the locking of the threads ourselves. We are using pthread_lock and pthread_cond along with a turn flag variable. When each thread enters its for loop, the pthread is locked, and a while loop checks the condition until the check against the turn flag returns false. Then the critical section is executed, and then the turn flag is changed, the condition signal is sent, and the lock is released. This continues until each thread has finished looping through its local array, and then the main program finishes.

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

// Use pthread_mutex and pthread_condition since we do not have actual monitors in C
pthread_mutex_t monitor_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t monitor_cond = PTHREAD_COND_INITIALIZER;
int turn = 0;

/**
  Thread for Source 1
*/
void* source1(void * a) {
	// local array of 5 "quantities"
	int ql1[] = { 10, 30, 50, 70, 90 };

	for(int i = 0; i < 5; i++){

		// lock the monitor mutex
		pthread_mutex_lock(&monitor_lock);
		// wait for condition signal while it is not this thread's turn
		while(turn == 1){
			pthread_cond_wait(&monitor_cond, &monitor_lock);
		}

		// Critical Section
		qtyList[qli] = ql1[i]; // add local array value to global array
		(qli)++; // increment global array "pointer"

		// change turn flag, unlock monitor mutex, and signal condition to allow next thread to run
		turn = 1;
		pthread_mutex_unlock(&monitor_lock);
		pthread_cond_signal(&monitor_cond);
	}
}

/**
  Thread for Source 2
  works like Source 1 thread
*/
void * source2(void * a) {
	int ql2[] = { 20, 40, 60, 80, 100 };
	for(int i = 0; i < 5; i++){

		pthread_mutex_lock(&monitor_lock);
		while(turn == 0){
			pthread_cond_wait(&monitor_cond, &monitor_lock);
		}
		// Critical Section
		qtyList[qli] = ql2[i];
		(qli)++;
		turn = 0;
		pthread_mutex_unlock(&monitor_lock);
		pthread_cond_signal(&monitor_cond);
	}
}

/**
	Main Program
*/
int main() {

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
	pthread_mutex_destroy(&monitor_lock);
	pthread_cond_destroy(&monitor_cond);
	// End main()
	return 0;
}
