/*
    File: kernel.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 12/09/03


    This file has the main entry point to the operating system.

*/


/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

#define MB * (0x1 << 20)
#define KB * (0x1 << 10)
#define KERNEL_POOL_START_FRAME ((2 MB) / (4 KB))
#define KERNEL_POOL_SIZE ((2 MB) / (4 KB))
#define PROCESS_POOL_START_FRAME ((4 MB) / (4 KB))
#define PROCESS_POOL_SIZE ((28 MB) / (4 KB))
/* definition of the kernel and process memory pools */

#define MEM_HOLE_START_FRAME ((15 MB) / (4 KB))
#define MEM_HOLE_SIZE ((1 MB) / (4 KB))
/* we have a 1 MB hole in physical memory starting at address 15 MB */

#define FAULT_ADDR (4 MB)
/* used in the code later as address referenced to cause page faults. */
#define NACCESS ((1 MB) / 4)
/* NACCESS integer access (i.e. 4 bytes in each access) are made starting at address FAULT_ADDR */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "machine.H"     /* LOW-LEVEL STUFF   */
#include "console.H"
#include "gdt.H"
#include "idt.H"          /* LOW-LEVEL EXCEPTION MGMT. */
#include "irq.H"
#include "exceptions.H"
#include "interrupts.H"

#include "simple_timer.H" /* TIMER MANAGEMENT */

#include "page_table.H"
#include "paging_low.H"

/*--------------------------------------------------------------------------*/
/* MAIN ENTRY INTO THE OS */
/*--------------------------------------------------------------------------*/

int main() {

    GDT::init();
    Console::init();
    IDT::init();
    ExceptionHandler::init_dispatcher();
    IRQ::init();
    InterruptHandler::init_dispatcher();


    /* -- EXAMPLE OF AN EXCEPTION HANDLER -- */

    class DBZ_Handler : public ExceptionHandler {
      public:
      virtual void handle_exception(REGS * _regs) {
        Console::puts("DIVISION BY ZERO!\n");
        for(;;);
      }
    } dbz_handler;

    ExceptionHandler::register_handler(0, &dbz_handler);


    /* -- INITIALIZE FRAME POOLS -- */

    FramePool kernel_mem_pool(KERNEL_POOL_START_FRAME,
                              KERNEL_POOL_SIZE,
                              0);
    unsigned long process_mem_pool_info_frame = kernel_mem_pool.get_frame();
    FramePool process_mem_pool(PROCESS_POOL_START_FRAME,
                               PROCESS_POOL_SIZE,
                               process_mem_pool_info_frame);
    process_mem_pool.mark_inaccessible(MEM_HOLE_START_FRAME, MEM_HOLE_SIZE);

    /* -- INITIALIZE MEMORY (PAGING) -- */

    /* ---- INSTALL PAGE FAULT HANDLER -- */

    class PageFault_Handler : public ExceptionHandler {
      public:
      virtual void handle_exception(REGS * _regs) {
        PageTable::handle_fault(_regs);
      }
    } pagefault_handler;

    ExceptionHandler::register_handler(14, &pagefault_handler);
    
    /* ---- INITIALIZE THE PAGE TABLE -- */

    PageTable::init_paging(&kernel_mem_pool,
                           &process_mem_pool,
                           4 MB);

    PageTable pt;

    pt.load();

    PageTable::enable_paging();

    /* -- INITIALIZE THE TIMER (we use a very simple timer).-- */
    
    SimpleTimer timer(100); /* timer ticks every 10ms. */
    InterruptHandler::register_handler(0, &timer);

    /* NOTE: The timer chip starts periodically firing as
             soon as we enable interrupts.
             It is important to install a timer handler, as we
             would get a lot of uncaptured interrupts otherwise. */

    /* -- ENABLE INTERRUPTS -- */

    Machine::enable_interrupts();

    /* -- MOST OF WHAT WE NEED IS SETUP. THE KERNEL CAN START. */

    Console::puts("Hello World!\n");

    /* -- GENERATE MEMORY REFERENCES */

    int *foo = (int *) FAULT_ADDR;
    int i;

    for (i=0; i<NACCESS; i++) {
       foo[i] = i;
    }

    for (i=0; i<NACCESS; i++) {
       if(foo[i] != i) {
          Console::puts("TEST FAILED for access number:");
          Console::putui(i);
          Console::puts("\n");
          break;
       }
    }
    if(i == NACCESS) {
       Console::puts("TEST PASSED\n");
    }

    /* -- NOW LOOP FOREVER */
    for(;;);

    /* -- WE DO THE FOLLOWING TO KEEP THE COMPILER HAPPY. */
    return 1;
}
