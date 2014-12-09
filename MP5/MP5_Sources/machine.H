/* 
    File: machine.h

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 09/03/05

    Description: low-level definitions. 

*/

#ifndef _machine_H_                   // include file only once
#define _machine_H_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* Data and code segment for kernel */

#define KERNEL_DS 0x10
#define KERNEL_CS 0x08


/* Memory Management */

#define PAGE_SIZE 4096
#define PT_ENTRIES_PER_PAGE 1024

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

/* REGISTER CONTEXT ON THE STACK WHEN EXCEPTION/INTERRUPT REACHES EXCEPTION
   DISPATCHER OR INTERRUPT DISPATCHER. 
   THIS IS ALSO USED IN THREAD SCHEDULING (for context switching) */
typedef struct regs {

    /* segment descriptors */
    unsigned int gs;
    unsigned int fs;
    unsigned int es;
    unsigned int ds;

    /* General purpose registers, managed by pusha/popa in x86. */
    unsigned int edi;
    unsigned int esi;
    unsigned int ebp;
    unsigned int esp;
    unsigned int ebx;
    unsigned int edx;
    unsigned int ecx;
    unsigned int eax;

    /* In the low-level handlers (i.e. before we call the 
       exception or interrupt dispatcher) we push number
       of the interrupt and error code onto the stack. */
    unsigned int int_no;
    unsigned int err_code;

    /* These registers are pushed on the stack when 
       the exception occurs. */
    unsigned int eip;
    unsigned int cs;
    unsigned int eflags;

    /* These registers are pushed on the stack only when
       the exception occured in user mode.
       As long as you are in kernel mode, the register context
       is 68 bytes long, and not 76! */
    unsigned int useresp;
    unsigned int ss;    
} REGS;
 

/*--------------------------------------------------------------------------*/
/* EXPORTED FUNCTIONS */ 
/*--------------------------------------------------------------------------*/

/*
 Manage interrupts. This is done by checking the Interrupt Enabled flag in the 
 EFLAG status register and by issuing STI/CLI instructions.
 (NOTE: If we have multiple threads, each has its own status register.)
*/

extern int machine_interrupts_enabled();
/* Returns FALSE if interrupts are disabled, and (1<<9) if they are enabled. */

extern void machine_enable_interrupts();
extern void machine_disable_interrupts();
/* Issue CLI/STI instructions. */

#endif