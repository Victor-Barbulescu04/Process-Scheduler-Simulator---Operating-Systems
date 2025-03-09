# Process Scheduler Simulator - Operating Systems

This project was for my Operating Systems class at MSOE. Its goal is to simulate both non-preemptive and preemptive scheduling within an operating systems scheduler. This program does not actually schedule any real processes, but it does simulate what that process might look like using various data structures and variables as placeholders for real-world mechanisms. This project is designed to run in the Linux terminal, so if you're interested in running it yourself that'd be the place to do it.

This program operates under the assumption that the input file describing what sort of events are occurring at some time is created in such a way that the events can actually occur. For example, a process cannot be scheduled CPU time if it was never created in the first place. The input file simply tells the scheduler what event should happen at what time, and the scheduler handles actually making that happen. This repo comes with a number of sample and test files, as well as a summary of their results. Below is a description of what the different operation types are and how they're expected to be read in by the program.


| Operation Type | Operation     | Description                                           | Additional Values        |
|----------------|---------------|-------------------------------------------------------|--------------------------|
| 1              | Process Start | Start a process                                       | The process priority     |
| 2              | I/O Request   | The current running process requests an I/O operation | Identifier of the device |
| 3              | I/O End       | The request for an I/O operation completed            | Identifier of the device |
| 4              | Process End   | The current running process ends                      | None                     |

A number of data structures were required to build this program from scratch. To get this program operating effectively, I created implementations for a linked list, queue, and priority queue, as well as a number of structs to properly organize the data and keep track of pointers. Behind every datatype I created, there's a suite of supporting functions that allow it to operate as intended. Most of these can be found in the header files and respective .c implementation files.

Overall, I'm incredibly happy with and proud of my implementation. The code here reflects my best effort as
a programmer and accentuates the values that I maintain when I program. There is thorough documentation across
every aspect of the project, and different moving pieces of the project are abstracted out into their own structs,
functions, and files. My commitment to this approach ultimately resulted in my main program being only about 100 lines
of code, which makes it very easy to follow. I was also very strict with my dynamic memory management rules and
only allocated and deallocated memory in certain places. I created a number of functions to help me achieve this
goal, and it ultimately resulted in a program with no leaks (thank God for valgrind by the way, it saved me so much headache).

The one part of the project I'm not totally happy about is how messy the non_preempting_handler.c and preempting_handler.c files ended up being. I did my best to comment everything I was doing to show what individual chunks of code do,
but the functions in those files handle everything from memory allocation to scheduling logic to analytics tracking.
If I had the time, I might abstract out the analytics part into its own analytics handler struct with its
own functions. That would help with the clutter problem in those two .c files.
