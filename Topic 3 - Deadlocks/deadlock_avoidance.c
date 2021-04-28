/**
	CST-221 Topic 3 - Deadlock Avoidance (2/2)

	Author: Jamie Lewis

	The program is an example of a Producer & Consumer, and consists of three counters, which track production, consumption, and available product. The main method creates two threads, one running the producer method, and the other, the consumer method. There are two mutex locks for the two "resources" that each method needs. The producer uses the produced and product counters, while the consumer uses the consumed and product counters. Before any counter is incremented/decremented, the method attempts to lock both mutexes, and then releases them after its work is completed. The consumer also checks to see that product is available before anything can be consumed. Two program files are included in the repository folder for this assignment, deadlock.c and deadlock_avoidance.c. The first one demonstrates a situation that frequently deadlocks, while the latter is the final product which implements a solution as follows.
	The deadlock_avoidance.c program implements a pthread_mutex_trylock and timer mechanism to skip one process's critical section and allow the other process to run in the event of a would-be deadlock (starvation of resources). This effectively terminates the process for one loop cycle plus the length of a sleep timer, and then allows it to restart. This proves to be very effective in allowing the program to run consistently to completion, albeit at a significant cost in time, with the program taking an average of about 10 seconds to run from start to finish. A more strictly controlled process using a turn and/or condition flag to alternate thread access to the common resource would take no more than a fraction of a second to run, for a similar number of instructions.


	See deadlock.c for the original problem.

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define MAX 5 // globally adjust the number of times each thread attempts to run

// File Output
FILE *file;

// Resource variables
int produced = 0;
int consumed = 0;

// SHARED RESOURCE
int product = 0;

// Mutex Locks = "resources"
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

/**
	The Producer Thread
	increments produced
	increments product
*/
void *producer(void *a) {
	int status;

	// stops looping when MAX production is reached
	while(produced < MAX) {

		// sleep timer allows other thread to work if this one skipped critical section on last loop. using 1 sets this thread at a higher priority.
		sleep(1);

		// lock resource 1
		pthread_mutex_lock(&lock1);
		printf("Producer locked mutex 1\n");
		fprintf(file, "Producer locked mutex 1\n");

		// attempt to lock resource 2
		status = pthread_mutex_trylock(&lock2);

		// will only execute critical section if thread was able to lock 2nd resource
		if(status == 0) {
			printf("Producer locked mutex 2\n");
			fprintf(file, "Producer locked mutex 2\n");

			// Critical Section

			// Produce product
			produced++;
			printf("Produced: %i\n", produced);
			fprintf(file, "Produced: %i\n", produced);
			product++;
			printf("Product available after production: %i\n", product);
			fprintf(file, "Product available after production: %i\n", product);

			// unlock resources normally
			pthread_mutex_unlock(&lock2);
			printf("Producer unlocked mutex 2\n");
			fprintf(file, "Producer unlocked mutex 2\n");
			pthread_mutex_unlock(&lock1);
			printf("Producer unlocked mutex 1\n\n");
			fprintf(file, "Producer unlocked mutex 1\n\n");

		} else { // if status != 0
			// unlock 1st resource after skipping critical section
			pthread_mutex_unlock(&lock1);
			printf("Could not lock 2nd resource. Producer unlocked mutex 1\n\n");
			fprintf(file, "Could not lock 2nd resource. Producer unlocked mutex 1\n\n");
		}
	}
}

/**
	The Consumer Thread
	decrements product
	increments consumed
*/
void *consumer(void *a) {
	int status;

	// stops looping when all production is consumed
	while (consumed < MAX) {

		/** timer allows other thread to work if this one skipped critical section on last loop.
			setting this sleep timer to 1 allows for faster run times, but also potentially much longer times if it runs multiple loops with no product available. tests with 1 ranged from about 5 to 13 seconds, indicating a possible slightly lower average time, but having equal timers also allows for more interesting random outputs in the log file, so it is MORE FUN!
			setting the this timer to 2 seconds lowers the thread's priority versus the producer (which is also set at 1 second), reduces the number of attempts to run with no product available, and produces runtimes consistantly @ ~10.003 seconds.
		*/
		sleep(1);

		// lock resource 1
		pthread_mutex_lock(&lock2);
		printf("Consumer locked mutex 2\n");
		fprintf(file, "Consumer locked mutex 2\n");

		// attempt to lock resource 2
		status = pthread_mutex_trylock(&lock1);

		// will only execute critical section if thread was able to lock 2nd resource
		if(status == 0) {
			printf("Consumer locked mutex 1\n");
			fprintf(file, "Consumer locked mutex 1\n");

			// Critical Section

			// Consume a product
			if(product > 0) { // do not consume if no product available
				product--;
				consumed++;
				printf("Consumed: %i\n", consumed);
				fprintf(file, "Consumed: %i\n", consumed);
				printf("Product available after consumption: %i\n", product);
				fprintf(file, "Product available after consumption: %i\n", product);

				// unlock resources normally
				pthread_mutex_unlock(&lock2);
				printf("Consumer unlocked mutex 2\n");
				fprintf(file, "Consumer unlocked mutex 2\n");
				pthread_mutex_unlock(&lock1);
				printf("Consumer unlocked mutex 1\n\n");
				fprintf(file, "Consumer unlocked mutex 1\n\n");

			} else { // no product was available
				// unlock resources with no product available
				pthread_mutex_unlock(&lock2);
				printf("No product available to consume! Consumer unlocked mutex 2\n");
				fprintf(file, "No product available to consume! Consumer unlocked mutex 2\n");
				pthread_mutex_unlock(&lock1);
				printf("No product available to consume! Consumer unlocked mutex 1\n\n");
				fprintf(file, "No product available to consume! Consumer unlocked mutex 1\n\n");
			}

		} else { // if status != 0
			// unlock 1st resource after skipping critical section
			pthread_mutex_unlock(&lock2);
			printf("Could not lock 2nd resource. Consumer unlocked mutex 2\n\n");
			fprintf(file, "Could not lock 2nd resource. Consumer unlocked mutex 2\n\n");
		}
	}
}

/**
	Main program
*/
int main(int argc, char* argv[])
{
	// open/create file to print output to
	// "w+" overwrites existing file every time program is run
	file = fopen("deadlock_avoidance_output.txt", "w+");

	// initialize thread IDs
	pthread_t t1, t2;

	// create threads
	printf("Creating Producer Thread...\n\n");
	fprintf(file, "Creating Producer Thread...\n\n");
	pthread_create(&t1, NULL, producer, (void *)t1);
	printf("Producer Thread created\n\n");
	fprintf(file, "Producer Thread created\n\n");

	printf("Creating Consumer Thread...\n\n");
	fprintf(file, "Creating Consumer Thread...\n\n");
	pthread_create(&t2, NULL, consumer, (void *)t2);
	printf("Consumer Thread created\n\n");
	fprintf(file, "Consumer Thread created\n\n");

	// wait for threads to finish
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	// thread cleanup when MAX product has been produced AND consumed
	if(produced == MAX && consumed == MAX && product == 0) {
		printf("Work complete. Product remaining (should be 0): %i\n", product);
		fprintf(file, "Work complete. Product remaining (should be 0): %i\n", product);
		printf("Cleaning up...\n\n");
		fprintf(file, "Cleaning up...\n\n");

		// close file
		fclose(file);
		// destroy mutexes
		pthread_mutex_destroy(&lock1);
		pthread_mutex_destroy(&lock2);
		// exit threads
		pthread_exit(NULL);
	}

	return 0;
}
