/* 
    File: machine.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 12/09/04

    LOW-LEVEL MACHINE FUNCTIONS.

*/

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "machine.H"
#include "machine_low.H"

#include "assert.H"

/*--------------------------------------------------------------------------*/
/* EXPORTED FUNCTIONS */
/*--------------------------------------------------------------------------*/

int Machine::interrupts_enabled() {
  /* We check the IF flag (INTERRUPT ENABLE) in the EFLAGS status register. */
  return get_EFLAGS() & (1 << 9);
}

void Machine::enable_interrupts() {
  assert(!interrupts_enabled());
  __asm__ __volatile__ ("sti");
}

void Machine::disable_interrupts() {
  assert(interrupts_enabled());
  __asm__ __volatile__ ("cli");
}
