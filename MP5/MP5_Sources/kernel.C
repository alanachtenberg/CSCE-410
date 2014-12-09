/* 
    File: kernel.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 09/05/04


    This file has the main entry point to the operating system.

    MAIN FILE FOR MACHINE PROBLEM "KERNEL-LEVEL DEVICE MANAGEMENT
    AND SIMPLE FILE SYSTEM"

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- COMMENT/UNCOMMENT THE FOLLOWING LINE TO EXCLUDE/INCLUDE SCHEDULER CODE */

//#define _USES_SCHEDULER_
/* This macro is defined when we want to force the code below to use 
   a scheduler.
   Otherwise, no scheduler is used, and the threads pass control to each 
   other in a co-routine fashion.
*/

#define _USES_DISK_
/* This macro is defined when we want to exercise the disk device.
   If defined, the system defines a disk and has Thread 2 read from it.
   Leave the macro undefined if you don't want to exercise the disk code.
*/

//#define _USES_FILESYSTEM_
/* This macro is defined when we want to exercise file-system code.
   If defined, the system defines a file system, and Thread 3 issues 
   issues operations to it.
   Leave the macro undefined if you don't want to exercise file system code.
*/

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "machine.H"         /* LOW-LEVEL STUFF   */
#include "console.H"
#include "gdt.H"
#include "idt.H"             /* EXCEPTION MGMT.   */
#include "irq.H"
#include "exceptions.H"     
#include "interrupts.H"

#include "simple_timer.H"    /* TIMER MANAGEMENT  */

#include "frame_pool.H"      /* MEMORY MANAGEMENT */
#include "mem_pool.H"

#include "thread.H"         /* THREAD MANAGEMENT */

#ifdef _USES_SCHEDULER_
#include "scheduler.H"
#endif

#ifdef _USES_DISK_
#include "simple_disk.H"
#endif

#ifdef _USES_FILESYSTEM_
#include "file_system.H"
#endif

/*--------------------------------------------------------------------------*/
/* MEMORY MANAGEMENT */
/*--------------------------------------------------------------------------*/

/* -- A POOL OF FRAMES FOR THE SYSTEM TO USE */
FramePool * SYSTEM_FRAME_POOL;

/* -- A POOL OF CONTIGUOUS MEMORY FOR THE SYSTEM TO USE */
MemPool * MEMORY_POOL;

/*--------------------------------------------------------------------------*/
/* SCHEDULER */
/*--------------------------------------------------------------------------*/

#ifdef _USES_SCHEDULER_

/* -- A POINTER TO THE SYSTEM SCHEDULER */
Scheduler * SYSTEM_SCHEDULER;

#endif

/*--------------------------------------------------------------------------*/
/* DISK */
/*--------------------------------------------------------------------------*/

#ifdef _USES_DISK_

/* -- A POINTER TO THE SYSTEM DISK */
SimpleDisk * SYSTEM_DISK;

#define SYSTEM_DISK_SIZE 10485760

#endif

/*--------------------------------------------------------------------------*/
/* FILE SYSTEM */
/*--------------------------------------------------------------------------*/

#ifdef _USES_FILESYSTEM_

/* -- A POINTER TO THE SYSTEM FILE SYSTEM */
FileSystem * FILE_SYSTEM;

#endif

/*--------------------------------------------------------------------------*/
/* JUST AN AUXILIARY FUNCTION */
/*--------------------------------------------------------------------------*/

void pass_on_CPU(Thread * _to_thread) {

#ifndef _USES_SCHEDULER_

        /* We don't use a scheduler. Explicitely pass control to the next
           thread in a co-routine fashion. */
	Thread::dispatch_to(_to_thread); 

#else

        /* We use a scheduler. Instead of dispatching to the next thread,
           we pre-empt the current thread by putting it onto the ready
           queue and yielding the CPU. */

        SYSTEM_SCHEDULER->resume(Thread::CurrentThread()); 
        SYSTEM_SCHEDULER->yield();
#endif
}


/*--------------------------------------------------------------------------*/
/* CODE TO EXERCISE THE FILE SYSTEM */
/*--------------------------------------------------------------------------*/

#ifdef _USES_FILESYSTEM_

int rand() {
  /* Rather silly random number generator. */

  unsigned long dummy_sec;
  int           dummy_tic;

  SimpleTimer::current(dummy_sec, dummy_tic);

  return dummy_tic;
}

void exercise_file_system(FileSystem * _file_system, SimpleDisk * _simple_disk) {
  /* NOTHING FOR NOW. 
     FEEL FREE TO ADD YOUR OWN CODE. */
}

#endif

/*--------------------------------------------------------------------------*/
/* A FEW THREADS (pointer to TCB's and thread functions) */
/*--------------------------------------------------------------------------*/

Thread * thread1;
Thread * thread2;
Thread * thread3;
Thread * thread4;

void fun1() {
    Console::puts("THREAD: "); Console::puti(Thread::CurrentThread()->ThreadId()); Console::puts("\n");

    Console::puts("FUN 1 INVOKED!\n");

    for(int j = 0;; j++) {

       Console::puts("FUN 1 IN ITERATION["); Console::puti(j); Console::puts("]\n");

       for (int i = 0; i < 10; i++) {
	  Console::puts("FUN 1: TICK ["); Console::puti(i); Console::puts("]\n");
       }

       pass_on_CPU(thread2);
    }
}



void fun2() {
    Console::puts("THREAD: "); Console::puti(Thread::CurrentThread()->ThreadId()); Console::puts("\n");

    Console::puts("FUN 2 INVOKED!\n");

#ifdef _USES_DISK_
    char buf[512];
    int  read_block  = 1;
    int  write_block = 0;
#endif

    for(int j = 0;; j++) {

       Console::puts("FUN 2 IN ITERATION["); Console::puti(j); Console::puts("]\n");

#ifdef _USES_DISK_
       /* -- Read */
       Console::puts("Reading a block from disk...\n");
       /* UNCOMMENT THE FOLLOWING LINE IN FINAL VERSION. */
       SYSTEM_DISK->read(read_block, buf);

       /* -- Display */
       int i;
       for (i = 0; i < 512; i++) {
	  Console::putch(buf[i]);
       }

#ifndef _USES_FILESYSTEM_
       /* -- Write -- ONLY IF THERE IS NO FILE SYSTEM BEING EXERCISED! */
       /*             Running this piece of code on a disk with a      */
       /*             file system would corrupt the file system.       */

       Console::puts("Writing a block to disk...\n");
       /* UNCOMMENT THE FOLLOWING LINE IN FINAL VERSION. */
       SYSTEM_DISK->write(write_block, buf); 
#endif

       /* -- Move to next block */
       write_block = read_block;
       read_block  = (read_block + 1) % 10;
#else

       for (int i = 0; i < 10; i++) {
	  Console::puts("FUN 2: TICK ["); Console::puti(i); Console::puts("]\n");
       }
     
#endif

       /* -- Give up the CPU */
       pass_on_CPU(thread3);
    }
}

void fun3() {
    Console::puts("THREAD: "); Console::puti(Thread::CurrentThread()->ThreadId()); Console::puts("\n");

    Console::puts("FUN 3 INVOKED!\n");

#ifdef _USES_FILESYSTEM_

    exercise_file_system(FILE_SYSTEM);

#else

     for(int j = 0;; j++) {

       Console::puts("FUN 3 IN BURST["); Console::puti(j); Console::puts("]\n");

       for (int i = 0; i < 10; i++) {
	  Console::puts("FUN 3: TICK ["); Console::puti(i); Console::puts("]\n");
       }
    
#endif
       pass_on_CPU(thread4);
    }
}

void fun4() {
    Console::puts("THREAD: "); Console::puti(Thread::CurrentThread()->ThreadId()); Console::puts("\n");

    for(int j = 0;; j++) {

       Console::puts("FUN 4 IN BURST["); Console::puti(j); Console::puts("]\n");

       for (int i = 0; i < 10; i++) {
	  Console::puts("FUN 4: TICK ["); Console::puti(i); Console::puts("]\n");
       }

       pass_on_CPU(thread1);
    }
}

/*--------------------------------------------------------------------------*/
/* MEMORY MANAGEMENT */
/*--------------------------------------------------------------------------*/

#ifdef MSDOS
typedef unsigned long size_t;
#else
typedef unsigned int size_t;
#endif

//replace the operator "new"
void * operator new (size_t size) {
    unsigned long a = MEMORY_POOL->allocate((unsigned long)size);
    return (void *)a;
}

//replace the operator "new[]"
void * operator new[] (size_t size) {
    unsigned long a = MEMORY_POOL->allocate((unsigned long)size);
    return (void *)a;
}

//replace the operator "delete"
void operator delete (void * p) {
    MEMORY_POOL->release((unsigned long)p);
}

//replace the operator "delete[]"
void operator delete[] (void * p) {
    MEMORY_POOL->release((unsigned long)p);
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

    /* -- INITIALIZE MEMORY -- */
    /*    NOTE: We don't have paging enabled in this MP. */
    /*    NOTE2: This is not an exercise in memory management. The implementation
                of the memory management is accordingly *very* primitive! */

    /* ---- Initialize a frame pool; details are in its implementation */
    FramePool system_frame_pool;
    SYSTEM_FRAME_POOL = &system_frame_pool;
   
    /* ---- Create a memory pool of 256 frames. */
    MemPool memory_pool(SYSTEM_FRAME_POOL, 256);
    MEMORY_POOL = &memory_pool;

    /* -- INITIALIZE THE TIMER (we use a very simple timer).-- */

    /* Question: Why do we want a timer? We have it to make sure that 
                 we enable interrupts correctly. If we forget to do it,
                 the timer "dies". */

    SimpleTimer timer(100); /* timer ticks every 10ms. */
    InterruptHandler::register_handler(0, &timer);
    /* The Timer is implemented as an interrupt handler. */

#ifdef _USES_SCHEDULER_

    /* -- SCHEDULER -- IF YOU HAVE ONE -- */
  
    Scheduler system_scheduler = Scheduler();
    SYSTEM_SCHEDULER = &system_scheduler;

#endif
   
#ifdef _USES_DISK_

    /* -- DISK DEVICE -- IF YOU HAVE ONE -- */

    SimpleDisk system_disk = SimpleDisk(MASTER, SYSTEM_DISK_SIZE);
    SYSTEM_DISK = &system_disk;

#endif

#ifdef _USES_FILESYSTEM_

     /* -- FILE SYSTEM  -- IF YOU HAVE ONE -- */

     FileSystem file_system = FileSystem();
     FILE_SYSTEM = &file_system;

#endif


    /* NOTE: The timer chip starts periodically firing as 
             soon as we enable interrupts.
             It is important to install a timer handler, as we 
             would get a lot of uncaptured interrupts otherwise. */  

    /* -- ENABLE INTERRUPTS -- */

    machine_enable_interrupts();

    /* -- MOST OF WHAT WE NEED IS SETUP. THE KERNEL CAN START. */

    Console::puts("Hello World!\n");

    /* -- LET'S CREATE SOME THREADS... */

    Console::puts("CREATING THREAD 1...\n");
    char * stack1 = new char[1024];
    thread1 = new Thread(fun1, stack1, 1024);
    Console::puts("DONE\n");

    Console::puts("CREATING THREAD 2...");
    char * stack2 = new char[1024];
    thread2 = new Thread(fun2, stack2, 1024);
    Console::puts("DONE\n");

    Console::puts("CREATING THREAD 3...");
    char * stack3 = new char[1024];
    thread3 = new Thread(fun3, stack3, 1024);
    Console::puts("DONE\n");

    Console::puts("CREATING THREAD 4...");
    char * stack4 = new char[1024];
    thread4 = new Thread(fun4, stack4, 1024);
    Console::puts("DONE\n");

#ifdef _USES_SCHEDULER_

    /* WE ADD thread2 - thread4 TO THE READY QUEUE OF THE SCHEDULER. */

    SYSTEM_SCHEDULER->add(thread2);
    SYSTEM_SCHEDULER->add(thread3);
    SYSTEM_SCHEDULER->add(thread4);

#endif

    /* -- KICK-OFF THREAD1 ... */

    Console::puts("STARTING THREAD 1 ...\n");
    Thread::dispatch_to(thread1);

    /* -- AND ALL THE REST SHOULD FOLLOW ... */
 
    assert(FALSE); /* WE SHOULD NEVER REACH THIS POINT. */

    /* -- WE DO THE FOLLOWING TO KEEP THE COMPILER HAPPY. */
    return 1;
}
