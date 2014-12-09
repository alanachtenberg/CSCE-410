/* 
    File: machine.c

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 09/04/13

    LOW-LEVEL MACHINE FUNCTIONS.

*/

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "machine.H"

#include "assert.H"

#include "threads_low.H"

/*--------------------------------------------------------------------------*/
/* EXPORTED FUNCTIONS */
/*--------------------------------------------------------------------------*/

int machine_interrupts_enabled() {
  /* We check the IF flag (INTERRUPT ENABLE) in the EFLAGS status register. */
  return get_EFLAGS() & (1 << 9);
}

void machine_enable_interrupts() {
  assert(!machine_interrupts_enabled());
  __asm__ __volatile__ ("sti");
}

void machine_disable_interrupts() {
  assert(machine_interrupts_enabled());
  __asm__ __volatile__ ("cli");
}
