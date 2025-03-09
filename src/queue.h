/**
 * @file queue.h
 * @brief Declares functionality of a basic queue that stores PCB's
 *
 * Course: CSC3210
 * Section: 003
 * Assignment: Process Simulator
 * Name: Victor Barbulescu
 */

#ifndef QUEUE_H
#define QUEUE_H

// Header imports
#include "PCB.h"
#include "Node.h"
// Library imports
#include <stdbool.h>


/**
* @struct Queue_t
* @brief Defines the attributes of a queue
*/
typedef struct queue{
	Node* head;
	Node* tail;
	int size;
}Queue_t;

/**
* @brief Initializes the contents of the queue struct to null
* @param queue: the queue to initialize
*/
void initialize_queue(Queue_t* queue);


/**
* @brief Adds a PCB to the queue
* @param queue: The head of queue
* @param data: PCB data to add
*/
void enqueue_queue(Queue_t* queue, PCB_t* data);

/**
* @brief Removes the first element in the queue
* @param queue: The head of queue
* @return the PCB of the first element in the queue
*/
PCB_t* dequeue_queue(Queue_t* queue);

/**
* @brief View the PCB in the head of the queue
* @param queue: The head of queue
* @return the PCB of the first element in the queue
*/
PCB_t peek_queue(const Queue_t* queue);

/**
* @brief checks if the queue is empty
* @param queue: The head of queue
* @return true if empty, false if not
*/
bool is_empty_queue(const Queue_t* queue);

/**
* @brief checks the size of the queue
* @param queue: The head of queue
* @return size of the queue
*/
int get_size_queue(const Queue_t* queue);

/**
* @brief For debugging purposes, prints the contents of the queue
* @param queue: contents to print
*/
void dump_queue(const Queue_t* queue);

/**
* @brief clears the contents of the queue from memory
* @param queue: queue to clear from memory
*/
void free_queue(const Queue_t* queue);

#endif //QUEUE_H
