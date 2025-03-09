/**
 * @file queue.c
 * @brief Defines functionality of a basic queue that stores PCB's
 *
 * Course: CSC3210
 * Section: 003
 * Assignment: Process Simulator
 * Name: Victor Barbulescu
 */

// Header imports
#include "queue.h"
// Library imports
#include <stdio.h>
#include <stdlib.h>

/**
* @brief Initializes the contents of the queue struct to null
* @param queue: the queue to initialize
*/
void initialize_queue(Queue_t* queue) {
	queue->head = NULL;
	queue->tail = NULL;
	queue->size = 0;
}

/**
* @brief Adds a PCB to the queue
* @param queue: The head of queue
* @param data: PCB data to add
*/
void enqueue_queue(Queue_t* queue, PCB_t* data) {
	Node* newNode = createNode(data);
	if (queue->size == 0) {
		// Initialize queue with head and tail pointers to first node
		queue->head = newNode;
		queue->tail = newNode;
	} else {
		// Update current tail node to point to new node
		queue->tail->next = newNode;
		// Update current tail node
		queue->tail = newNode;
	}

	queue->size++;
}

/**
* @brief Removes the first element in the queue
* @param queue: The head of queue
* @return the PCB of the first element in the queue
*/
PCB_t* dequeue_queue(Queue_t* queue) {
	if (is_empty_queue(queue)) {
		// Queue is empty, throw an error
		fprintf(stderr, "ERROR: Queue is already empty!\n");
		exit(1);
	}

	Node* temp = queue->head;
	queue->head = queue->head->next;
	if (queue->head == NULL) {
		// If the queue is now empty, update the tail to NULL as well
		queue->tail = NULL;
	}

	queue->size--;

	// Return the PCB data of the first element in the queue
	PCB_t* dequeuedData = temp->data;
	free(temp); // Deallocate the memory of the node
	return dequeuedData;
}

/**
* @brief View the PCB in the head of the queue
* @param queue: The head of queue
* @return the PCB of the first element in the queue
*/
PCB_t peek_queue(const Queue_t* queue) {
	if (is_empty_queue(queue)) {
		printf("ERROR: Queue is empty!\n");
		exit(1);
	}

	return *queue->head->data;
}

/**
* @brief checks if the queue is empty
* @param queue: The head of queue
* @return true if empty, false if not
*/
bool is_empty_queue(const Queue_t* queue) {
	if (queue->head == NULL) {
		return true;
	}

	return false;
}

/**
* @brief checks the size of the queue
* @param queue: The head of queue
* @return size of the queue
*/
int get_size_queue(const Queue_t* queue) {
	return queue->size;
}

/**
* @brief For debugging purposes, prints the contents of the queue
* @param queue: contents to print
*/
void dump_queue(const Queue_t* queue) {
	Node* temp = queue->head;
	int i = 0;
	while (temp != NULL) {
		printf("Node #%d: ID %d, ", i++, temp->data->PID);
		temp = temp->next;
	}
}

/**
* @brief clears the contents of the queue from memory
* @param queue: queue to clear from memory
*/
void free_queue(const Queue_t* queue) {
	Node* head = queue->head;
	while (head != NULL) {
		Node *temp = head;
		head = head->next;
		free(temp);
	}
}
