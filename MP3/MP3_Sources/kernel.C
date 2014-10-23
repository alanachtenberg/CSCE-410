/*
    File: kernel.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 12/09/27


    This file has the main entry point to the operating system.

*/


/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

#define GB * (0x1 << 30)
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


/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "machine.H"        /* LOW-LEVEL STUFF */
#include "console.H"
#include "gdt.H"
#include "idt.H"            /* LOW-LEVEL EXCEPTION MGMT. */
#include "irq.H"
#include "exceptions.H"
#include "interrupts.H"

#include "simple_timer.H"   /* SIMPLE TIMER MANAGEMENT */

#include "page_table.H"
#include "paging_low.H"

#include "vm_pool.H"

/*--------------------------------------------------------------------------*/
/* MEMORY ALLOCATION */
/*--------------------------------------------------------------------------*/

void TestPassed();
void TestFailed();
void GenerateMemoryReferences(VMPool *pool, int size1, int size2);

/*--------------------------------------------------------------------------*/
/* MEMORY ALLOCATION */
/*--------------------------------------------------------------------------*/

VMPool *current_pool;

#ifdef MSDOS
typedef unsigned long size_t;
#else
typedef unsigned int size_t;
#endif

//replace the operator "new"
void * operator new (size_t size) {
  unsigned long a = current_pool->allocate((unsigned long)size);
  return (void *)a;
}

//replace the operator "new[]"
void * operator new[] (size_t size) {
  unsigned long a = current_pool->allocate((unsigned long)size);
  return (void *)a;
}

//replace the operator "delete"
void operator delete (void * p) {
  current_pool->release((unsigned long)p);
}

//replace the operator "delete[]"
void operator delete[] (void * p) {
  current_pool->release((unsigned long)p);
}

/*--------------------------------------------------------------------------*/
/* EXCEPTION HANDLERS */
/*--------------------------------------------------------------------------*/

/* -- EXAMPLE OF THE DIVISION-BY-ZERO HANDLER */

void dbz_handler(REGS * r) {
  Console::puts("DIVISION BY ZERO\n");
  for(;;);
}


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

    /* ---- INSTALL PAGE FAULT HANDLER -- */

    class PageFault_Handler : public ExceptionHandler {
      public:
      virtual void handle_exception(REGS * _regs) {
        PageTable::handle_fault(_regs);
      }
    } pagefault_handler;

    ExceptionHandler::register_handler(14, &pagefault_handler);

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

    PageTable::init_paging(&kernel_mem_pool,
                           &process_mem_pool,
                           4 MB);

    PageTable pt1;

    pt1.load();
    PageTable::enable_paging();

    /* -- INITIALIZE THE TWO VIRTUAL MEMORY PAGE POOLS -- */
    VMPool code_pool(512 MB, 256 MB, &process_mem_pool, &pt1);
    VMPool heap_pool(1 GB, 256 MB, &process_mem_pool, &pt1);



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

    Console::puts("I am starting with an extensive test of the memory allocator.\n");
    Console::puts("Please be patient...\n");
    Console::puts("Testing the memory allocation on code_pool...\n");
    GenerateMemoryReferences(&code_pool, 50, 100);
    Console::puts("Testing the memory allocation on heap_pool...\n");
    GenerateMemoryReferences(&heap_pool, 50, 100);

   TestPassed();
}

void GenerateMemoryReferences(VMPool *pool, int size1, int size2)
{
   current_pool = pool;
   for(int i=1; i<size1; i++) {
      int *arr = new int[size2 * i];
      if(pool->is_legitimate((unsigned long)arr) == FALSE) {
        Console::puts("is_legitimate test failed\n");
         TestFailed();
      }
      for(int j=0; j<size2*i; j++) {
         arr[j] = j;
      }
      for(int j=size2*i - 1; j>=0; j--) {
         if(arr[j] != j) {
            TestFailed();
         }
      }
      delete arr;
   }
}

void TestFailed()
{
   Console::puts("Test Failed\n");
   for(;;);
}

void TestPassed()
{
   Console::puts("Test Passed! Congratulations!\n");
   for(;;);
}
