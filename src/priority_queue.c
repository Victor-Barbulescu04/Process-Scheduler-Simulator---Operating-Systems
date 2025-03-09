/**
 * @file priority_queue.c
 * @brief Defines functionality of a priority queue that stores PCB's
 *
 * Course: CSC3210
 * Section: 003
 * Assignment: Process Simulator
 * Name: Victor Barbulescu
 */


// Header imports
#include "priority_queue.h"
// Library imports
#include <stdio.h>
#include <stdlib.h>

/**
* @brief Initializes the contents of the priority queue struct to null
* @param queue: the queue to initialize
*/
void initialize_priority_queue(PriorityQueue_t* queue) {
	queue->head = NULL;
	queue->tail = NULL;
	queue->size = 0;
}

/**
* @brief Adds a PCB to the priority queue
* @param queue: The head of priority queue
* @param data: PCB data to add
*/
void enqueue_priority_queue(PriorityQueue_t* queue, PCB_t* data) {
	Node* newNode = createNode(data);
	if (is_empty_priority_queue(queue)) {
		// Initialize queue with head and tail pointers to first node
		queue->head = newNode;
		queue->tail = newNode;
	} else if (data->priority > queue->head->data->priority) {
		// If the new PCB has higher priority than the head, insert at the head
		newNode->next = queue->head;
		queue->head = newNode;
	} else if (newNode->data->priority <= queue->tail->data->priority) {
		// If the new PCB has a lower priority than the tail, insert at the tail
		queue->tail->next = newNode;
		queue->tail = newNode;
	} else {
		// Create pointer to traverse priority queue and search for
		// insertion point
		Node* temp = queue->head;

		// If it's not higher priority than head, traverse the queue till
		// an insertion point is found
		while (temp->next != NULL && newNode->data->priority < temp->next->data->priority) {
			temp = temp->next;
		}

		// Insert new node in between current two nodes
		Node* oldNext = temp->next;
		temp->next = newNode;
		newNode->next = oldNext;
	}

	queue->size++;
}

/**
* @brief Removes the first element in the priority queue
* @param queue: The head of priority queue
* @return the PCB of the first element in the priority queue
*/
PCB_t* dequeue_priority_queue(PriorityQueue_t* queue) {
	if (is_empty_priority_queue(queue)) {
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
* @brief View the PCB in the head of the priority queue
* @param queue: The head of priority queue
* @return the PCB of the first element in the priority queue
*/
PCB_t peek_priority_queue(const PriorityQueue_t* queue) {
	if (is_empty_priority_queue(queue)) {
		printf("ERROR: Queue is empty!\n");
		exit(1);
	}

	return *queue->head->data;
}

/**
* @brief checks if the priority queue is empty
* @param queue: The head of priority queue
* @return true if empty, false if not
*/
bool is_empty_priority_queue(const PriorityQueue_t* queue) {
	if (queue->head == NULL) {
		return true;
	}

	return false;
}

/**
* @brief checks the size of the priority queue
* @param queue: The head of priority queue
* @return size of the priority queue
*/
int get_size_priority_queue(const PriorityQueue_t* queue) {
	return queue->size;
}

/**
* @brief For debugging purposes, prints the contents of the queue
* @param queue: contents to print
*/
void dump_priority_queue(PriorityQueue_t* queue) {
	Node* temp = queue->head;
	int i = 0;
	while (temp != NULL) {
		printf("Node #%d: ID: %d, Priority %d | ", i++, temp->data->PID, temp->data->priority);
		temp = temp->next;
	}
}

/**
* @brief clears the contents of the priority queue from memory
* @param queue: priority queue to clear from memory
*/
void free_priority_queue(const PriorityQueue_t* queue) {
	Node* head = queue->head;
	while (head != NULL) {
		Node *temp = head;
		head = head->next;
		free(temp);
	}
}
