all: main

main: priority_queue.o queue.o non_preempting_handler.o preempting_handler.o io_device.o main.o linkedList.o
	gcc linkedList.o priority_queue.o queue.o io_device.o non_preempting_handler.o preempting_handler.o main.o -o main.out
	rm -f *.o

main.o: main.c io_device.h Scheduler.h linkedList.h
	gcc -c main.c

priority_queue.o: PCB.h Node.h
	gcc -c priority_queue.c

queue.o: PCB.h Node.h
	gcc -c queue.c

io_device.o: queue.h PCB.h
	gcc -c io_device.c

non_preempting_handler.o: Scheduler.h queue.h io_device.h PCB.h
	gcc -c non_preempting_handler.c

preempting_handler.o: Scheduler.h priority_queue.h io_device.h PCB.h
	gcc -c preempting_handler.c

linkedList.o: linkedList.h
	gcc -c linkedList.c

run:
	./main.out

clean:
	rm -f *.out

