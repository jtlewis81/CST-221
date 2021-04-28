/**
CST-221 Topic 2: Producer and Consumer

Author: Jamie Lewis

My approach to resolving the Producer/Consumer problem was to implement a turn variable, a pthread_mutex, and pthread_cond, which each thread uses along with a while loop to check if it is its turn to run. When a thread’s while loop condition becomes false, the next function is executed. For the Producer thread, that involves incrementing a variable called theProduct, announcing its value in the terminal, setting the buffer variable to theProduct’s new value via a method called put(), and then changing the turn variable, unlocking the pthread_mutex, and signaling the pthread_cond to allow the Consumer thread to continue beyond its while loop. The Consumer thread then gets theProduct value via the get() method, reads the value to the terminal, and finally changes the turn value, unlocks the pthread_mutex, and signals the pthread_cond to allow the Producer to run again when the for loop in the main() method cycles. The for loop is set to run 10 times for this program.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Variable used to indicate updated data in the program as it runs
int theProduct = 0;

// THIS IS THE SHARED RESOURCE
int buffer = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t flag = PTHREAD_COND_INITIALIZER;
int turn = 0;

int produce(){
	// increment the product so we indicate change throughout the running program
	theProduct = ++theProduct;
	// Statement printed to the terminal
	printf("Produced: %i\n", theProduct);
	// return the new value
	return theProduct;

}

void put(int i) {
	// set the buffer value to the newest product
	buffer = theProduct;
}

void consume(int i){
	// "use" the "product"
	printf("Consumed: %i\n", i);

}

int get(){
	return buffer;
}

/**
* The Producer Thread
*/
void * producer(void * a) {
	int i;
	// check to see if this thread can run
	pthread_mutex_lock(&lock);
	while(turn == 1){
		pthread_cond_wait(&flag, &lock);
	}

	// Critical Section

	// Make a new "product"
	i = produce();
	// Update buffer value
	put(i);

	// set conditions to allow other thread to run
	turn = 1;
	pthread_mutex_unlock(&lock);
	pthread_cond_signal(&flag);
}

/**
* The Consumer Thread
*/
void * consumer(void * a) {
	int i;
	// check to see if this thread can run
	pthread_mutex_lock(&lock);
	while(turn == 0){
		pthread_cond_wait(&flag, &lock);
	}


	// Critical Section

	// read the buffer value
	i = get();
	// use the buffer value by reading it to the terminal
	consume(i);

	// set conditions to allow other thread to run
	turn = 0;
	pthread_mutex_unlock(&lock);
	pthread_cond_signal(&flag);

}


int main() {

	// initialize thread IDs
	pthread_t tid1, tid2;

	// run each thread 10 times
	for (int i = 0; i < 10; i++) {
		// Create 2 threads
		pthread_create(&tid1, NULL, producer, (void *)&tid1);
		pthread_create(&tid2, NULL, consumer, (void *)&tid2);
	}

	// Thread creation cleanup
	pthread_exit(NULL);
	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&flag);
	// End main()
	return 0;

}
