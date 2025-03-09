/**
 * @file priority_queue.h
 * @brief Declares functionality of a priority queue that stores PCB's
 *
 * Course: CSC3210
 * Section: 003
 * Assignment: Process Simulator
 * Name: Victor Barbulescu
 */

#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

// Header imports
#include "PCB.h"
#include "Node.h"
// Library imports
#include <stdbool.h>


/**
* @struct PriorityQueue_t
* @brief Defines the attributes of a priority_queue
*/
typedef struct priority_queue{
	Node* head;
	Node* tail;
	int size;
}PriorityQueue_t;

/**
* @brief Initializes the contents of the priority queue struct to null
* @param queue: the queue to initialize
*/
void initialize_priority_queue(PriorityQueue_t* queue);

/**
* @brief Adds a PCB to the priority queue
* @param queue: The head of priority queue
* @param data: PCB data to add
*/
void enqueue_priority_queue(PriorityQueue_t* queue, PCB_t* data);

/**
* @brief Removes the first element in the priority queue
* @param queue: The head of priority queue
* @return the PCB of the first element in the priority queue
*/
PCB_t* dequeue_priority_queue(PriorityQueue_t* queue);

/**
* @brief View the PCB in the head of the priority queue
* @param queue: The head of priority queue
* @return the PCB of the first element in the priority queue
*/
PCB_t peek_priority_queue(const PriorityQueue_t* queue);

/**
* @brief checks if the priority queue is empty
* @param queue: The head of priority queue
* @return true if empty, false if not
*/
bool is_empty_priority_queue(const PriorityQueue_t* queue);

/**
* @brief checks the size of the priority queue
* @param queue: The head of priority queue
* @return size of the priority queue
*/
int get_size_priority_queue(const PriorityQueue_t* queue);

/**
* @brief For debugging purposes, prints the contents of the queue
* @param queue: contents to print
*/
void dump_priority_queue(PriorityQueue_t* queue);

/**
* @brief clears the contents of the priority queue from memory
* @param queue: priority queue to clear from memory
*/
void free_priority_queue(const PriorityQueue_t* queue);

#endif //PRIORITY_QUEUE_H
