/**
 * @file PCB.h
 * @brief Defines attributes of the simulated process control block
 *
 * Course: CSC3210
 * Section: 003
 * Assignment: Process Simulator
 * Name: Victor Barbulescu
 */

#ifndef PCB_H
#define PCB_H

/**
* Defines the 5 states that a process can exist in:
* New, Ready, Running, Waiting, Terminated
*/
typedef enum state {
	READY,
	RUNNING,
	BLOCKED,
} State;

/**
* Represents a process control block and defines attributes of a process.
* Also logs runtime statistics of a particular process.
*/
typedef struct pcb_t{
	// Process Identifiers
	int PID;
	int priority;
	int target_io_device;
	State state;

	// Process runtime statistic trackers
	int start_ready_time;
	int end_ready_time;
	int total_ready_time;

	int start_io_blocked_time;
	int end_io_blocked_time;
	int total_io_blocked_time;
}PCB_t;

/**
* @brief Initializes the contents of a PCB to 0
* @param pcb: the PCB to initialize
*/
static void initialize_PCB(PCB_t* pcb) {
	pcb->PID = 0;
	pcb->priority = 0;
	pcb->target_io_device = 0;
	pcb->state = READY;
	pcb->start_ready_time = 0;
	pcb->end_ready_time = 0;
	pcb->total_ready_time = 0;
	pcb->start_io_blocked_time = 0;
	pcb->end_io_blocked_time = 0;
	pcb->total_io_blocked_time = 0;
}

#endif //PCB_H
