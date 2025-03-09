/**
 * @file io_device.h
 * @brief Declares the attributes and functions of a standard I/O device.
 * This includes the current PCB using the I/O device and a queue for the next PCB's
 *
 * Course: CSC3210
 * Section: 003
 * Assignment: Process Simulator
 * Name: Victor Barbulescu
 */

#ifndef IO_DEVICE_H
#define IO_DEVICE_H

// Header imports
#include "queue.h"
#include "PCB.h"

/**
* @struct IO_Device_t
* @brief Defines the attributes of an IO device
*/
typedef struct IO_DEVICE {
	int id;
	PCB_t* active_pcb;
	Queue_t queue;
} IO_Device_t;

/**
* @brief Initializes the contents of the io device struct
* @param io_device: the IO device to initialize
* @param id: the ID of the new I/O device
*/
static void initialize_io_device(IO_Device_t* io_device, int id){
	io_device->id = id;
	io_device->active_pcb = NULL;
	initialize_queue(&io_device->queue);
}

/**
* @brief Adds a PCB to the IO devices queue (or automatically into the active IO device
* category if none are queued
* @param io_device: an IO device
* @param pcb: the PCB to add to the I/O queue
*/
void add_io_request(IO_Device_t* io_device, PCB_t* pcb);

/**
* @brief Finishes the current PCB in the io device and activates next one
* @param io_device: an IO device
* @return a list of completed PCB's
*/
PCB_t* complete_current_io_operation(IO_Device_t* io_device);

/**
* @brief For debugging purposes, prints the contents of the IO devices queue
* @param io_device: contents to print
*/
void dump_io_device(IO_Device_t* io_device);

#endif //IO_DEVICE_H
