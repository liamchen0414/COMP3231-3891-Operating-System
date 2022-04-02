# General system calls

## kern/arch/mips/: traps and syscalls

Exceptions are the key to operating systems; they are the mechanism that enables the OS to regain control of execution and therefore do its job. You can think of exceptions as the interface between the processor and the operating system. When the OS boots, it installs an "exception handler" (carefully crafted assembly code) at a specific address in memory. When the processor raises an exception, it invokes this, which sets up a "trap frame" and calls into the operating system. Since "exception" is such an overloaded term in computer science, operating system lingo for an exception is a "trap", when the OS traps execution. Interrupts are exceptions, and more significantly for this assignment, so are system calls. Specifically, syscall.c handles traps that happen to be syscalls. Understanding at least the C code in this directory is key to being a real operating systems junkie, so we highly recommend reading through it carefully. The code is also covered in the system calls lecture.

```c
/* ISO C99 signals.  */
#define	SIGINT		2	/* Interactive attention signal.  */
#define	SIGILL		4	/* Illegal instruction.  */
#define	SIGABRT		6	/* Abnormal termination.  */
#define	SIGFPE		8	/* Erroneous arithmetic operation.  */
#define	SIGSEGV		11	/* Invalid access to storage.  */
#define	SIGTERM		15	/* Termination request.  */

#define EX_IRQ    0    /* Interrupt */
#define EX_MOD    1    /* TLB Modify (write to read-only page) */
#define EX_TLBL   2    /* TLB miss on load */
#define EX_TLBS   3    /* TLB miss on store */
#define EX_ADEL   4    /* Address error on load */
#define EX_ADES   5    /* Address error on store */
#define EX_IBE    6    /* Bus error on instruction fetch */
#define EX_DBE    7    /* Bus error on data load *or* store */
#define EX_SYS    8    /* Syscall */
#define EX_BP     9    /* Breakpoint */
#define EX_RI     10   /* Reserved (illegal) instruction */
#define EX_CPU    11   /* Coprocessor unusable */
#define EX_OVF    12   /* Arithmetic overflow */

/*
 * Function called when user-level code hits a fatal fault.
 */
static
void
kill_curthread(vaddr_t epc, unsigned code, vaddr_t vaddr)
{
	int sig = 0; // initialise signal to 0

	KASSERT(code < NTRAPCODES);
	switch (code) {
	    case EX_IRQ: // Interrupt
	    case EX_IBE: // Bus error on instruction fetch
	    case EX_DBE: // Bus error on data load or store
	    case EX_SYS: // syscall
		/* should not be seen */
		KASSERT(0);
		sig = SIGABRT; // Abnormal termination
		break;
	    case EX_MOD:
	    case EX_TLBL:
	    case EX_TLBS:
		sig = SIGSEGV;
		break;
	    case EX_ADEL:
	    case EX_ADES:
		sig = SIGBUS;
		break;
	    case EX_BP:
		sig = SIGTRAP;
		break;
	    case EX_RI:
		sig = SIGILL;
		break;
	    case EX_CPU:
		sig = SIGSEGV;
		break;
	    case EX_OVF:
		sig = SIGFPE;
		break;
	}

	/*
	 * You will probably want to change this.
	 */

	kprintf("Fatal user mode trap %u sig %d (%s, epc 0x%x, vaddr 0x%x)\n",
		code, sig, trapcodenames[code], epc, vaddr);
	panic("I don't know how to handle this\n");
}
```

```c
/* The trap frame is supposed to be 35 registers long. */
KASSERT(sizeof(struct trapframe)==(35*4));
```

## locore/trap.c

mips_trap() is the key function for returning control to the operating system. This is the C function that gets called by the assembly exception handler. kill_curthread() is the function for handling broken user programs; when the processor is in usermode and hits something it can't handle (say, a bad instruction), it raises an exception. There's no way to recover from this, so the OS needs to kill off the process. The bonus part of this assignment will include writing a useful version of this function.

## syscall/syscall.c

syscall() is the function that delegates the actual work of a system call off to the kernel function that implements it. Notice that reboot and time are the only cases currently handled.

### Questions

1. What is the numerical value of the exception code for a MIPS system call (Hint: EX_SYS)?

The value is 8

```c
/*
 * MIPS exception codes.
 */
#define EX_IRQ    0    /* Interrupt */
#define EX_MOD    1    /* TLB Modify (write to read-only page) */
#define EX_TLBL   2    /* TLB miss on load */
#define EX_TLBS   3    /* TLB miss on store */
#define EX_ADEL   4    /* Address error on load */
#define EX_ADES   5    /* Address error on store */
#define EX_IBE    6    /* Bus error on instruction fetch */
#define EX_DBE    7    /* Bus error on data load *or* store */
#define EX_SYS    8    /* Syscall */
#define EX_BP     9    /* Breakpoint */
#define EX_RI     10   /* Reserved (illegal) instruction */
#define EX_CPU    11   /* Coprocessor unusable */
#define EX_OVF    12   /* Arithmetic overflow */
```

2. How many bytes is an instruction in MIPS? (Answer this by reading syscall() carefully, not by looking somewhere else.)

See kern/arch/mips/syscall/syscall.c. Also observe the switch statement. This is the code that determines the system call invoked and the calls the appropriate routine in the kernel. Also observe the convention or prefixing the in-kernel implementation of a system call with sys\_, which you should follow as well.

```c
  /*
    * Now, advance the program counter, to avoid restarting
    * the syscall over and over again.
    */

  tf->tf_epc += 4;
```

3. What is the contents of the struct trapframe? Where is the struct trapframe that is passed into syscall() stored?

```c
/*
 * Structure describing what is saved on the stack during entry to
 * the exception handler.
 *
 * This must agree with the code in exception-*.S.
 */

struct trapframe {
	uint32_t tf_vaddr;	/* coprocessor 0 vaddr register */
	uint32_t tf_status;	/* coprocessor 0 status register */
	uint32_t tf_cause;	/* coprocessor 0 cause register */
	uint32_t tf_lo;
	uint32_t tf_hi;
	uint32_t tf_ra;		/* Saved register 31 */
	uint32_t tf_at;		/* Saved register 1 (AT) */
	uint32_t tf_v0;		/* Saved register 2 (v0) */
	uint32_t tf_v1;		/* etc. */
	uint32_t tf_a0;
	uint32_t tf_a1;
	uint32_t tf_a2;
	uint32_t tf_a3;
	uint32_t tf_t0;
	uint32_t tf_t1;
	uint32_t tf_t2;
	uint32_t tf_t3;
	uint32_t tf_t4;
	uint32_t tf_t5;
	uint32_t tf_t6;
	uint32_t tf_t7;
	uint32_t tf_s0;
	uint32_t tf_s1;
	uint32_t tf_s2;
	uint32_t tf_s3;
	uint32_t tf_s4;
	uint32_t tf_s5;
	uint32_t tf_s6;
	uint32_t tf_s7;
	uint32_t tf_t8;
	uint32_t tf_t9;
	uint32_t tf_gp;
	uint32_t tf_sp;
	uint32_t tf_s8;
	uint32_t tf_epc;	/* coprocessor 0 epc register */
};
```

4. What would be required to implement a system call that took more than 4 arguments?

There are basically two options here:

4.1 Create a specific convention between the stubs in the user-level C-library that uses some more registers in the register set as input arguments (as we have done for the system call number).

4.2 Alternatively, we could use the C-compiler calling convention, which places extra arguments on the userlevel stack. The operating system would then have to obtain the arguments from the applications stack. This has to be done very carefully as there is no guarantee the application has a valid user-level stack pointer when the system call is invoked.

The calling conventions in OS161 for syscalls are as follows: Like ordinary function calls, the first 4 32-bit arguments are passed in the 4 argument registers a0-a3. 64-bit arguments are passed in _aligned_ pairs of registers, that is, either a0/a1 or a2/a3. This means that if the first argument is 32-bit and the second is 64-bit, a1 is unused.

5. What is the purpose of userptr_t?

Basically, userptr_t is the type of pointers to memory that are supplied by applications (which can't be trusted to be valid). We use this type to avoid accidentally dereferencing one of these unsafe pointers in our kernel code.

```c
/*
 * Define userptr_t as a pointer to a one-byte struct, so it won't mix
 * with other pointers.
 */

struct __userptr { char _dummy; };
typedef struct __userptr *userptr_t;
typedef const struct __userptr *const_userptr_t;
```
