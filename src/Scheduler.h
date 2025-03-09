/**
 * @file Scheduler.h
 * @brief Declares the attributes of a scheduler, including the handler struct
 *		  and generic function declarations that are specified upon initialization
 *		  in main.
 *
 * Course: CSC3210
 * Section: 003
 * Assignment: Process Simulator
 * Name: Victor Barbulescu
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

// Header imports
#include "PCB.h"
#include "io_device.h"
#include "queue.h"
#include "priority_queue.h"

/**
* @struct SchedulerHandler
* @brief Defines a generic scheduler handler. Specifics of the functions will be
*        implemented in main and other files
*/
typedef struct SchedulerHandler {
	// Handler functions
	void* queue;  // Can be Queue_t* or PriorityQueue_t*
	PCB_t* CPU;
	void (*handle_process_start)(struct SchedulerHandler* schedulerHandler, int priority, int* PID_tracker, int* current_time);
	void (*handle_io_request)(struct SchedulerHandler* schedulerHandler, IO_Device_t* io_device, int* current_time);
	void (*handle_io_complete)(struct SchedulerHandler* schedulerHandler, IO_Device_t* io_device, int* current_time);
	PCB_t (*handle_process_end)(struct SchedulerHandler* schedulerHandler, int* current_time);
	IO_Device_t* io_devices;

	// Analytics Tracking
	int start_idle_time;
	int end_idle_time;
	int total_idle_time;
} SchedulerHandler;

// Function Prototypes for non-preemptive functions
/**
* @brief Handles the event of starting of a process using non-preemptive scheduling
* @param schedulerHandler: A reference to the current scheduler
* @param priority: a new processes priority
* @param PID_tracker: reference to the current value of PID tracker, used to assign a PID to a new process
* @param current_time: reference to the current time, used for analytics and marking events
*/
void non_preemptive_process_start(SchedulerHandler* schedulerHandler, int priority, int* PID_tracker, int* current_time);

/**
* @brief Handles the event where an active process requests an I/O device using non-preemptive scheduling
* @param schedulerHandler: A reference to the current scheduler
* @param io_device: a reference to the requested I/O device
* @param current_time: reference to the current time, used for analytics and marking events
*/
void non_preemptive_io_request(SchedulerHandler* schedulerHandler, IO_Device_t* io_device, int* current_time);

/**
* @brief Handles the event where an I/O device completes its tasks using non-preemptive scheduling
* @param schedulerHandler: A reference to the current scheduler
* @param io_device: a reference to the finished I/O device
* @param current_time: reference to the current time, used for analytics and marking events
*/
void non_preemptive_io_complete(SchedulerHandler* schedulerHandler, IO_Device_t* io_device, int* current_time);

/**
* @brief Handles the event where an active process terminates using non-preemptive scheduling
* @param schedulerHandler: A reference to the current scheduler
* @param current_time: reference to the current time, used for analytics marking events
* @return a copy of the completed process
*/
PCB_t non_preemptive_process_end(SchedulerHandler* schedulerHandler, int* current_time);



// Function Prototypes for preemptive functions
/**
* @brief Handles the event of starting of a process using preemptive scheduling
* @param schedulerHandler: A reference to the current scheduler
* @param priority: a new processes priority
* @param PID_tracker: reference to the current value of PID tracker, used to assign a PID to a new process
* @param current_time: reference to the current time, used for analytics and marking events
*/
void preemptive_process_start(SchedulerHandler* schedulerHandler, int priority, int* PID_tracker, int* current_time);

/**
* @brief Handles the event where an active process requests an I/O device using preemptive scheduling
* @param schedulerHandler: A reference to the current scheduler
* @param io_device: a reference to the requested I/O device
* @param current_time: reference to the current time, used for analytics and marking events
*/
void preemptive_io_request(SchedulerHandler* schedulerHandler, IO_Device_t* io_device, int* current_time);

/**
* @brief Handles the event where an I/O device completes its tasks using preemptive scheduling
* @param schedulerHandler: A reference to the current scheduler
* @param io_device: a reference to the finished I/O device
* @param current_time: reference to the current time, used for analytics and marking events
*/
void preemptive_io_complete(SchedulerHandler* schedulerHandler, IO_Device_t* io_device, int* current_time);

/**
* @brief Handles the event where an active process terminates using preemptive scheduling
* @param schedulerHandler: A reference to the current scheduler
* @param current_time: reference to the current time, used for analytics and marking events
* @return a copy of the completed process
*/
PCB_t preemptive_process_end(SchedulerHandler* schedulerHandler, int* current_time);

/**
* @brief Initializes a schedulerHandler struct to the right functions
* @param scheduler_handler: pointer to the scheduler handler
* @param io_devices: pointer to array of initialized I/O devices
* @param preemptive_scheduler: non-preemptive (0) or preemptive (1)
*/
static void initialize_scheduler_handler(SchedulerHandler* scheduler_handler, IO_Device_t* io_devices, int* preemptive_scheduler) {
	// Initialize scheduler based on mode
	if (*preemptive_scheduler == 0) {
		// Non-preemptive scheduler
		Queue_t* pcb_queue = (Queue_t*)malloc(sizeof(Queue_t));
		initialize_queue(pcb_queue);

		scheduler_handler->queue = pcb_queue;
		scheduler_handler->CPU = NULL;
		scheduler_handler->handle_process_start = &non_preemptive_process_start;
		scheduler_handler->handle_io_request = &non_preemptive_io_request;
		scheduler_handler->handle_io_complete = &non_preemptive_io_complete;
		scheduler_handler->handle_process_end = &non_preemptive_process_end;
		scheduler_handler->io_devices = io_devices;
	} else {
		// Preemptive scheduler
		PriorityQueue_t* pcb_queue = (PriorityQueue_t*)malloc(sizeof(PriorityQueue_t));
		initialize_priority_queue(pcb_queue);

		scheduler_handler->queue = pcb_queue;
		scheduler_handler->CPU = NULL;
		scheduler_handler->handle_process_start = &preemptive_process_start;
		scheduler_handler->handle_io_request = &preemptive_io_request;
		scheduler_handler->handle_io_complete = &preemptive_io_complete;
		scheduler_handler->handle_process_end = &preemptive_process_end;
		scheduler_handler->io_devices = io_devices;
	}

	scheduler_handler->start_idle_time = 0;
	scheduler_handler->end_idle_time = 0;
	scheduler_handler->total_idle_time = 0;
}

/**
* @brief Frees the memory of the scheduler
* @param schedulerHandler: pointer to the scheduler handler
* @param preemptive_scheduler: what type of scheduler it is
*/
static void free_scheduler_handler(SchedulerHandler* schedulerHandler, int* preemptive_scheduler) {
	if (*preemptive_scheduler == 0) {
		// If the program is non-preemptive scheduling, the backing structure is a queue.
		free_queue((Queue_t*)schedulerHandler->queue);
	} else {
		// If the program is non-preemptive scheduling, the backing structure is a priority queue.
		free_priority_queue((PriorityQueue_t*)schedulerHandler->queue);
	}

	free(schedulerHandler->queue);

	if (schedulerHandler->CPU != NULL){
		free(schedulerHandler->CPU);
	}
}


#endif //SCHEDULER_H
