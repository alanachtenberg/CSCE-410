/*
    File: page_table.C

    Author: Alan Achtenebrg
            CEEN 2015
            Texas A&M University
    Date  : 09/18/2014

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "page_table.H"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* METHODS FOR CLASS   PageTable */
/*--------------------------------------------------------------------------*/

void PageTable::init_paging(FramePool * _kernel_mem_pool,
                          FramePool * _process_mem_pool,
                          const unsigned long _shared_size){

  }
  /* Set the global parameters for the paging subsystem. */

  PageTable::PageTable(){

  }
  /* Initializes a page table with a given location for the directory and the
     page table proper.
     NOTE: The PageTable object still needs to be stored somewhere! Probably it is best
           to have it on the stack, as there is no memory manager yet...
     NOTE2: It may also be simpler to create the first page table *before* paging
           has been enabled.
  */

  void PageTable::load(){

  }
  /* Makes the given page table the current table. This must be done once during
     system startup and whenever the address space is switched (e.g. during
     process switching). */

   void PageTable::enable_paging(){

  }
  /* Enable paging on the CPU. Typically, a CPU start with paging disabled, and
     memory is accessed by addressing physical memory directly. After paging is
     enabled, memory is addressed logically. */

   void PageTable::handle_fault(REGS * _r){

  }
  /* The page fault handler. */

