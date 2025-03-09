/**
 * @file main.c
 * @brief Program entry point.  Runs the process scheduler simulation
 *
 * Course: CSC3210
 * Section: 003
 * Assignment: Process Simulator
 * Name: Victor Barbulescu
 */

// Header imports
#include "io_device.h"
#include "Scheduler.h"
#include "linkedList.h"
// Library imports
#include <stdio.h>

/**
 * @brief Program entry procedure for the process scheduler simulation
 */
int main(int argc, char* argv[]) {

    if(argc != 2) {
        printf("Usage: procsim param_file\n");
        return 1;
    }

    // Initialize I/O Devices
    IO_Device_t io_devices[10];
    for (int i = 0; i < 10; i++) {
        initialize_io_device(&io_devices[i], i);
    }

    FILE* input = fopen(argv[1], "r");

    // Read in the simulation parameters
    int preemptive_scheduler;
    fscanf(input, "%d", &preemptive_scheduler);
    printf("Simulation Starting. Preemption: %s\n\n", preemptive_scheduler == 1 ? "true" : "false");

    // Read each event
    int event_time;
    int operation_code;
    int priority;
    int io_device;

    // PID tracker instantiates process at 1 and iterates up as processes are added
    int PID_tracker = 1;

    // Instantiate and initialize scheduler handler. Specifics of its functions are defined in
    // non_preempting_handler.c and preempting_handler.c
    SchedulerHandler scheduler_handler;
    initialize_scheduler_handler(&scheduler_handler, io_devices, &preemptive_scheduler);

    // Linked list to track process analytics
    LL_Node* process_analytics_data = NULL;

    // Process events (shared logic)
    while (fscanf(input, "%d %d", &event_time, &operation_code) == 2) {
        switch (operation_code) {
            case 1: {
                // Handle process start
                fscanf(input, "%d", &priority);
                scheduler_handler.handle_process_start(&scheduler_handler, priority, &PID_tracker, &event_time);
                break;
            }
            case 2: {
                // Handle I/O request
                fscanf(input, "%d", &io_device);
                scheduler_handler.handle_io_request(&scheduler_handler, &scheduler_handler.io_devices[io_device], &event_time);
                break;
            }
            case 3: {
                // Handle I/O request complete
                fscanf(input, "%d", &io_device);
                scheduler_handler.handle_io_complete(&scheduler_handler, &scheduler_handler.io_devices[io_device], &event_time);
                break;
            }

            case 4: {
                // Handle process end
                PCB_t ended_process = scheduler_handler.handle_process_end(&scheduler_handler, &event_time);
                append_to_linked_list(&process_analytics_data, ended_process);
                break;
            }

            default: break;
        }
    }

    fclose(input);

    // Analytics outputs
    printf("\nSimulation ended at time: %d\n", event_time);
    printf("System idle time: %d\n\n", scheduler_handler.total_idle_time);
    print_list(process_analytics_data);

    // Memory management
    free_scheduler_handler(&scheduler_handler, &preemptive_scheduler);
    free_list(process_analytics_data);

    return 0;
}