# Lecture 1 
## What's an operating system?
1. Provides an abstract machine: it extends the basic hardware with added functionality. It provided high-level abstractions and hides the details of the hardware
2. Resource shareing manager: responsible for allocating resources to users and processes to ensure there is no:
    1. No starvation
    2. progress
    3. allocation is according to some desired policy (for example some programs are ran in high priority)
    4. efficiencly used

## Modes:
1. user mode or application mode: only has access to a subset of the instruction set of the architecture
2. privileged mode: has access to all the instructions of the architecture
* other divisions: only subset of the memory is accessible to users

## Kernel
1. The portion of the operating system that is running in privileged mode
2. Usually stays in main memory
3. Contains fundamental functionality
4. Contains most-frequently used functions

## Structure
<p align"center">
    <img src="images/StructureOS.png"  />
<p>

The application invokes the system call library which has a wrapper of the lower level functionality in the kernel
1. functions such as strcmp() and memcpy() are pure libary functions: manipulating memory within the application
2. open(), close(), read(), write(), dup2(), fork()......they cross the user-kernel boundary

* The operating system relinquishes control of the processeor to execute other programs by using two options
1. using syscalls to regain control
2. timer interrupts(dead while loop for example)

* Monolithic OS Structure: Has many interdependencies:

# Lecture 2

## Processes & Threads Processes
1. Also called a task or job
2. Execution of an individual program
3. Encompasses one or more threads

## Process/Thread model
1. Running
2. Blocked
3. Ready

<p align"center">
    <img src="images/processmodel.png"  />
<p>

### Threads
1. Unit of execution
2. Can be traced
3. Belongs to a process

### The thread model is a model that seperates execution from the environment. Items stored per thread include:
1. Program counter
2. Registers
3. Stack
4. State
5. Local variables

<p align"center">
    <img src="images/threadmodel.png"  />
<p>

the hardware only has one program counter and it gives the illusion that each program has its own program counter, the image below shows conceptual model of 4 independent, sequential processess with a single thread each. There is only one program active at any instant

<p align"center">
    <img src="images/conconrrencyillusion.png"  />
<p>

### PCB
Process creation -> process termination: A processes’ information is stored in a process control block (PCB). The PCBs form a process table
1. we need process management: registers, program counter, status word, stack pointer, PCB(process id, registers, process state, priority, pointers to memory)
2. memory management(pointer to text segment, pointer to data segment, pointer to stack segment)
3. file management(root directory, working directory, file descriptors, open file tables, user id, group id)

Context Switch
A context switch refers to either a switch between threads or a switch between processes.

A context switch can occur anytime the operating system is invoked

On a system call
On an exception
On an interrupt
Concurrency & Synchronisation Concurrency Example
The following example demonstrates a race condition. Since the global variable count is shared between threads, but the function increment() isn’t run mutually exclusively, depending on the execution order, count could result in a different value after different runs of the program.

Thread 1

void increment() {
  int i;
  i = count;
  i = i + 1;
  count = i;
}
Thread 2

void increment() {
  int i;
  i = count;
  i = i - 1;
  count = i;
}
Critical Region
A critical region is a region of code where shared resources are accessed. In the example above, the critical region is from the line i = count to the line count = i.

We can control access to the shared resource by controlling access to the code that accesses the resource. As shown above, uncoordinated entry to the critical region results in a race condition.

To coordinate access to critical regions we can use a number of solutions:

Locks
Acquire lock before critical region, release after critical region
Disabling Interrupts
Before entering a critical region, disable interrupts
After leaving the critical region, enable interrupts
Hardware Support
Test and set instruction
If lock == 0, set and acquire the lock
If lock == 1, another process/thread has the lock
Semaphores
P(): test/wait/down
V(): increment/signal/up
Monitors
A set of procedures, variables, data types are grouped in a special kind of module, a monitor
Only one process/thread can be in the monitor at any one time
Semaphores
Semaphores wait for a resource if it’s not available. During waiting, processes are put into a queue to avoid busy waiting.

When a resource is available, a process will signal, and any process waiting on that resource will be resumed.

Condition Variables
A condition variable is basically a container of threads that are waiting for a certain condition. They allow a process to wait within a monitor until some condition is met, before regaining exclusive access and resuming their task.