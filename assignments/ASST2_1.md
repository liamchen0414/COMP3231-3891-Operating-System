## Moving data between kernel and userland.

### vm/copyinout.c

This file contains functions for moving data between kernel and user space. Knowing when and how to cross this boundary is critical to properly implementing userlevel programs, so this is a good file to read very carefully. The framework provided is needed to safely implement application buffer handling in the read() and write() system calls. You should also examine the code in lib/uio.c.

Questions
What is the difference between UIO_USERISPACE and UIO_USERSPACE? When should one use UIO_SYSSPACE instead?
Why can the struct uio that is used to read in an ELF segment be allocated on the stack in load_segment() (i.e., where is the destination memory for the read)?
In what file are copyin() and copyout() defined? memmove()? Why can't copyin() and copyout() be implemented as simply as memmove()?

## The VFS interface.

### kern/include

The files vfs.h and vnode.h in this directory contain function declarations and comments that are directly relevant to this assignment.

Questions
How are vfs*open, vfs_close used? What other vfs*() calls are relevant?
What are VOP_READ, VOP_WRITE? How are they used?
What does VOP_ISSEEKABLE do?
Where is the struct thread defined? What does this structure contain?
Where is the struct proc defined? What does this structure contain?
Loading and running programs.
These questions mostly relate to the advanced assignment, but are worth exploring to understand how a program starts running.

kern/syscall
This directory contains some syscall implementations, and the files that are responsible for the loading and running of userlevel programs. Currently, the only files in the directory are loadelf.c, runprogram.c, and time_syscalls.c, although you may want to add more of your own during this assignment. Understanding these files is the key to getting started with the assignment, especially the advanced part, the implementation of multiprogramming. Note that to answer some of the questions, you will have to look in files outside this directory.

loadelf.c
This file contains the functions responsible for loading an ELF executable from the filesystem and into virtual memory space. Of course, at this point this virtual memory space does not provide what is normally meant by virtual memory, although there is translation between the addresses that executables "believe" they are using and physical addresses, there is no mechanism for providing more memory than exists physically.

runprogram.c
This file contains only one function, runprogram(), which is the function that is responsible for running a program from the kernel menu. Once you have designed your file system calls, a program started by runprogram() should have the standard file descriptors (stdout, stderr) available while it's running.

In the advanced assignment, runprogram() is a good base for writing the execv() system call, but only a base. When thinking about your design, you should determine what more is required for execv() that runprogram() does not need to worry about. Once you have designed your process framework, runprogram() should be altered to start processes properly within this framework.

Questions
What are the ELF magic numbers?
In runprogram(), why is it important to call vfs_close before going to usermode?
What function forces the processor to switch into usermode? Is this function machine dependent?
fork()
Answer these questions by reading the fork() man page and the sections dealing with fork() in the textbook.

Questions
What is the purpose of the fork() system call?
What process state is shared between the parent and child?
What process state is copied between the parent and child?
The userland side of system calls.
This section is mostly related to the advanced version, but is still generally insightful to understand how system calls transition into the kernel. This was covered in some detail in the system call lecture in week 4.

userland/lib/crt0
The userland C startup code. There's only one file in here, mips/crt0.S, which contains the MIPS assembly code that receives control first when a user-level program is started. It calls main(). This is the code that your execv() implementation will be interfacing to, so be sure to check what values it expects to appear in what registers and so forth.

userland/lib/libc:
There's obviously a lot of code in the OS/161 C library (and a lot more yet in a real system's C library...) We don't expect you to read it all, although it may be instructive in the long run to do so. Job interviewers have an uncanny habit of asking people to implement simple standard C functions on the whiteboard. For present purposes you need only look at the code that implements the user-level side of system calls.

errno.c
This is where the global variable errno is defined.

syscalls-mips.S
This file contains the machine-dependent code necessary for implementing the userlevel side of MIPS system calls. syscalls.S is created from this file at compile time and is the actual file assembled to put into the C library. The actual names of the system calls are placed in this file using a script called gensyscalls.sh that reads them from the kernel's header files. This avoids having to make a second list of the system calls. In a real system, typically each system call stub is placed in its own source file, to allow selectively linking them in. OS/161 puts them all together to simplify the makefiles.
Questions
What is the purpose of the SYSCALL macro?
What is the MIPS instruction that actually triggers a system call? (Answer this by reading the source in this directory, not looking somewhere else.)
