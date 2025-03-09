/**
 * @file preempting_handler.c
 * @brief Defines the preemptive scheduler functions declared in Scheduler.h
 *
 * Course: CSC3210
 * Section: 003
 * Assignment: Process Simulator
 * Name: Victor Barbulescu
 */

// Header imports
#include "Scheduler.h"
#include "priority_queue.h"
#include "io_device.h"
#include "PCB.h"
// Library imports
#include <stdlib.h>

// Function templates
static int compare_priority(const void* a, const void* b);

/**
* @brief Handles the event of starting of a process using preemptive scheduling
* @param schedulerHandler: A reference to the current scheduler
* @param priority: a new processes priority
* @param PID_tracker: reference to the current value of PID tracker, used to assign a PID to a new process
* @param current_time: reference to the current time, used for analytics marking events
*/
void preemptive_process_start(SchedulerHandler* schedulerHandler, int priority, int* PID_tracker, int* current_time) {
	// Create a new PCB for the new process
	PCB_t* new_process = (PCB_t*)malloc(sizeof(PCB_t));  // Dynamically allocate memory
	initialize_PCB(new_process);
	new_process->priority = priority;
	new_process->PID = *PID_tracker;

	// Iterate the PID tracker so new processes get new ID's
	(*PID_tracker)++;

	printf("%d: Starting process with PID: %d PRIORITY: %d\n", *current_time, new_process->PID, new_process->priority);
	// Add the new process to the queue. If no process is currently active/current process is
	// of lower priority, assign straight to CPU
	if (schedulerHandler->CPU == NULL){
		// No process is currently running in the CPU, assign new process to CPU
		new_process->state = RUNNING;
		schedulerHandler->CPU = new_process;
		printf("%d: Process scheduled to run with PID: %d PRIORITY: %d\n", *current_time, new_process->PID, new_process->priority);

		// System analytics
		schedulerHandler->end_idle_time = *current_time;
		schedulerHandler->total_idle_time += schedulerHandler->end_idle_time - schedulerHandler->start_idle_time;
	} else if (schedulerHandler->CPU->priority < new_process->priority) {
		// New process is of higher priority than current process, assign new process to CPU
		schedulerHandler->CPU->state = READY;
		schedulerHandler->CPU->start_ready_time = *current_time;
		enqueue_priority_queue(schedulerHandler->queue, schedulerHandler->CPU);

		new_process->state = RUNNING;
		schedulerHandler->CPU = new_process;
		printf("%d: Process scheduled to run with PID: %d PRIORITY: %d\n", *current_time, new_process->PID, new_process->priority);
	} else {
		// New process is of lower or equal priority than current process, add to queue
		new_process->state = READY;
		enqueue_priority_queue(schedulerHandler->queue, new_process);

		// Process analytics
		new_process->start_ready_time = *current_time;
	}
}

/**
* @brief Handles the event where an active process requests an I/O device using preemptive scheduling
* @param schedulerHandler: A reference to the current scheduler
* @param io_device: a reference to the requested I/O device
* @param current_time: reference to the current time, used for analytics marking events
*/
void preemptive_io_request(SchedulerHandler* schedulerHandler, IO_Device_t* io_device, int* current_time) {
	// Send the current PCB to the desired I/O device
	PCB_t* io_requested_process = schedulerHandler->CPU;
	io_requested_process->state = BLOCKED;
	printf("%d: Process with PID: %d waiting for I/O device %d\n", *current_time, io_requested_process->PID, io_device->id);
	add_io_request(io_device, io_requested_process);

	// Process analytics
	io_requested_process->start_io_blocked_time = *current_time;

	// CPU next process logic
	if (get_size_priority_queue(schedulerHandler->queue) > 0){
		// If there are processes in the queue, send the first in line to the CPU
		PCB_t* new_process = dequeue_priority_queue((PriorityQueue_t*)schedulerHandler->queue);
		new_process->state = RUNNING;

		schedulerHandler->CPU = new_process;  // Assign the new process to the CPU
		printf("%d: Process scheduled to run with PID: %d PRIORITY: %d\n", *current_time, new_process->PID, new_process->priority);

		// Process analytics
		new_process->end_ready_time = *current_time;
		new_process->total_ready_time += new_process->end_ready_time - new_process->start_ready_time;
	} else {
		// If there are no processes in the queue, the CPU goes idle
		schedulerHandler->CPU = NULL;

		// System analytics
		schedulerHandler->start_idle_time = *current_time;
	}
}

/**
* @brief Handles the event where an I/O device completes its tasks using preemptive scheduling
* @param schedulerHandler: A reference to the current scheduler
* @param io_device: a reference to the finished I/O device
* @param current_time: reference to the current time, used for analytics marking events
*/
void preemptive_io_complete(SchedulerHandler* schedulerHandler, IO_Device_t* io_device, int* current_time) {
	/// Remove the processes from the I/O device
	int io_device_queue_size = io_device->queue.size + 1;
	PCB_t* io_processes[io_device_queue_size];
	// Dump all processes into the temp array
	for (int i = 0; i < io_device_queue_size; i++) {
		io_processes[i] = complete_current_io_operation(io_device);
	}

	// Process analytics
	for (int i = 0; i < io_device_queue_size; i++) {
		io_processes[i]->end_io_blocked_time = *current_time;
		io_processes[i]->total_io_blocked_time += io_processes[i]->end_io_blocked_time - io_processes[i]->start_io_blocked_time;
	}

	// If necessary, sort the io processes array based on priority of the processes
	// This ensure that the first element exiting the I/O device is the highest priority one
	if (io_device_queue_size > 1) {
		qsort(io_processes, io_device_queue_size, sizeof(PCB_t*), compare_priority);
	}

	// Print IO complete for this device
	printf("%d: I/O completed for I/O device %d\n", *current_time, io_device->id);

	// CPU next process logic
	if (schedulerHandler->CPU == NULL){
		// If no process is currently running in the CPU, send the first process to the CPU
		// and the rest to the queue
		io_processes[0]->state = RUNNING;
		schedulerHandler->CPU = io_processes[0];
		printf("%d: Process scheduled to run with PID: %d PRIORITY: %d\n", *current_time, io_processes[0]->PID, io_processes[0]->priority);

		// Queue remaining processes
		for (int i = 1; i < io_device_queue_size; i++) {
			io_processes[i]->state = READY;
			io_processes[i]->start_ready_time = *current_time;
			enqueue_priority_queue(schedulerHandler->queue, io_processes[i]);
		}

		// System analytics
		schedulerHandler->end_idle_time = *current_time;
		schedulerHandler->total_idle_time += (schedulerHandler->end_idle_time - schedulerHandler->start_idle_time);
	} else if (schedulerHandler->CPU->priority < io_processes[0]->priority) {
		// First process is of higher priority than current process, assign new process to CPU
		// and the rest to the queue
		schedulerHandler->CPU->state = READY;
		schedulerHandler->CPU->start_ready_time = *current_time;
		enqueue_priority_queue(schedulerHandler->queue, schedulerHandler->CPU);
		io_processes[0]->state = RUNNING;
		schedulerHandler->CPU = io_processes[0];
		printf("%d: Process scheduled to run with PID: %d PRIORITY: %d\n", *current_time, io_processes[0]->PID, io_processes[0]->priority);

		// Queue remaining processes
		for (int i = 1; i < io_device_queue_size; i++) {
			io_processes[i]->state = READY;
			io_processes[i]->start_ready_time = *current_time;
			enqueue_priority_queue(schedulerHandler->queue, io_processes[i]);
		}
	} else {
		// First process is of lower or equal priority than current process, add all processes to queue
		for (int i = 0; i < io_device_queue_size; i++) {
			io_processes[i]->state = READY;
			io_processes[i]->start_ready_time = *current_time;
			enqueue_priority_queue(schedulerHandler->queue, io_processes[i]);
		}
	}

}

/**
* @brief Handles the event where an active process terminates using preemptive scheduling
* @param schedulerHandler: A reference to the current scheduler
* @param current_time: reference to the current time, used for analytics marking events
* @return a copy of the completed process
*/
PCB_t preemptive_process_end(SchedulerHandler* schedulerHandler, int* current_time) {
	// Retrieve and free memory of current process
	PCB_t* ended_process = schedulerHandler->CPU;
	const PCB_t temp = *ended_process;
	printf("%d: Ending process with PID: %d\n", *current_time, ended_process->PID);
	free(ended_process);

	// CPU next process logic
	if (get_size_priority_queue(schedulerHandler->queue) > 0){
		// If there are processes in the queue, send the first in line to the CPU
		PCB_t* new_process = dequeue_priority_queue((PriorityQueue_t*)schedulerHandler->queue);
		new_process->state = RUNNING;

		schedulerHandler->CPU = new_process;  // Assign the new process to the CPU
		printf("%d: Process scheduled to run with PID: %d PRIORITY: %d\n", *current_time, new_process->PID, new_process->priority);

		// Process analytics
		new_process->end_ready_time = *current_time;
		new_process->total_ready_time += new_process->end_ready_time - new_process->start_ready_time;
	} else {
		// If there are no processes in the queue, the CPU goes idle
		schedulerHandler->CPU = NULL;

		// System analytics
		schedulerHandler->start_idle_time = *current_time;
	}

	return temp;
}

// Comparison function for sorting in descending order
int compare_priority(const void* a, const void* b) {
	PCB_t* pcb1 = *(PCB_t**)a;  // Dereferencing the pointer-to-pointer
	PCB_t* pcb2 = *(PCB_t**)b;

	return pcb2->priority - pcb1->priority;
}
