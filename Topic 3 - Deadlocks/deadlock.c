/**
	CST-221 Topic 3 - Deadlock Avoidance (1/2)

	Author: Jamie Lewis

	The progam is an example of a Producer & Consumer, and consists of three counters, which track production, consumption, and available product. The main method creates two threads, one running the producer method, and the other, the consumer method. There are two mutex locks for "locking" the two "resources" that each method needs. The producer uses the produced and product counters, while the consumer uses the consumed and product counters. Before any counter is incremented/decremented, the method attempts to lock both mutexes, and then releases them after its work is completed.

	However, this program demonstrates an unsafe state in which a deadlock will likely occur, although not necessarily. Sometimes this program will run to completion, but most of the time it will not. The two methods attempt to lock the mutexes in reverse order, allowing the possibility for each thread to lock one of the needed "resources", preventing the other from locking its second resource, thus creating a deadlock. A turn flag has been left out of this program to exaggerate the occurance of a deadlock.

	See deadlock_avoidance.c for an implemented solution to this problem.

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

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

	while(product < MAX) {

		// lock "resources"
		pthread_mutex_lock(&lock1);
		printf("Producer locked mutex 1\n");
		fprintf(file, "Producer locked mutex 1\n");
		pthread_mutex_lock(&lock2);
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

		// unlock "resources"
		pthread_mutex_unlock(&lock2);
		printf("Producer unlocked mutex 2\n");
		fprintf(file, "Producer unlocked mutex 2\n");
		pthread_mutex_unlock(&lock1);
		printf("Producer unlocked mutex 1\n");
		fprintf(file, "Producer unlocked mutex 1\n\n");
	}
}

/**
	The Consumer Thread
	decrements product
	increments consumed
*/
void *consumer(void *a) {

	while (consumed < MAX) {

		// lock "resources"
		pthread_mutex_lock(&lock2);
		printf("Consumer locked mutex 2\n");
		fprintf(file, "Consumer locked mutex 2\n");
		pthread_mutex_lock(&lock1);
		printf("Consumer locked mutex 1\n");
		fprintf(file, "Consumer locked mutex 1\n");

		// Critical Section

		// Consume a product
		if(product > 0) {
			product--;
			consumed++;
			printf("Consumed: %i\n", consumed);
			fprintf(file, "Consumed: %i\n", consumed);
			printf("Product available after consumption: %i\n", product);
			fprintf(file, "Product available after consumption: %i\n", product);
		} else {
			printf("No product to consume yet!\n");
			fprintf(file, "No product to consume yet!\n");
		}


		// unlock "resources"
		pthread_mutex_unlock(&lock2);
		printf("Consumer unlocked mutex 2\n");
		fprintf(file, "Consumer unlocked mutex 2\n");
		pthread_mutex_unlock(&lock1);
		printf("Consumer unlocked mutex 1\n");
		fprintf(file, "Consumer unlocked mutex 1\n\n");
	}
}

/**
	Main program
*/
int main(int argc, char* argv[])
{
	// open/create file to print output to
	// "w+" overwrites existing file every time program is run
	file = fopen("deadlock_output.txt", "w+");

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

	fclose(file);

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
