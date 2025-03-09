/**
 * @file io_device.c
 * @brief defines the attributes and functions of a standard I/O device.
 * This includes the current PCB using the I/O device and a queue for the next PCB's
 *
 * Course: CSC3210
 * Section: 003
 * Assignment: Process Simulator
 * Name: Victor Barbulescu
 */

// Header imports
#include "io_device.h"

/**
* @brief Adds a PCB to the IO devices queue (or automatically into the active IO device
* category if none are queued
* @param io_device: an IO device
* @param pcb: the PCB to add to the I/O queue
*/
void add_io_request(IO_Device_t* io_device, PCB_t* pcb) {
	if (io_device->active_pcb == NULL) {
		// If there is no active IO operation, set the active operation to the inputted PCB
		io_device->active_pcb = pcb;
	} else {
		// If there is an active IO operation, add the new request to the queue
		enqueue_queue(&io_device->queue, pcb);
	}
}

/**
* @brief Finishes the current PCB in the io device and activates next one
* @param io_device: an IO device
* @return the completed PCB
*/
PCB_t* complete_current_io_operation(IO_Device_t* io_device) {
	// Retrieve the current active PCB
	PCB_t* current_pcb = io_device->active_pcb;

	if (io_device->queue.size > 0) {
		// If there is another request in the queue, set the next PCB in queue to active
		io_device->active_pcb = dequeue_queue(&io_device->queue);
	} else {
		// If there are no remaining IO requests in the queue, set active PCB to NULL
		io_device->active_pcb = NULL;
	}
	current_pcb->state = READY;
	return current_pcb;
}

/**
* @brief For debugging purposes, prints the contents of the IO devices queue
* @param io_device: contents to print
*/
void dump_io_device(IO_Device_t* io_device) {
	dump_queue(&io_device->queue);
}