#include "scheduler.H"

// You implementation here!


   Scheduler::Scheduler(){
        queue_size=0;
        //ready_queue is already defined with Node default constructor
   }
   /* Setup the scheduler. This sets up the ready queue, for example.
      If the scheduler implements some sort of round-robin scheme, then the
      end_of_quantum handler is installed here as well. */

    void Scheduler::yield(){
        if (queue_size!=0){
            --queue_size;
            Thread* next= ready_queue.dequeue(); //get next queue waiting
            Thread::dispatch_to(next);// run new thread
        }
        //if queue is empty we simply return via the call stack
   }
   /* Called by the currently running thread in order to give up the CPU.
      The scheduler selects the next thread from the ready queue to load onto
      the CPU, and calls the dispatcher function defined in 'threads.h' to
      do the context switch. */

    void Scheduler::resume(Thread * _thread){
        ready_queue.enqueue(_thread);//add thread to queue
        ++queue_size;
   }
   /* Add the given thread to the ready queue of the scheduler. This is called
      for threads that were waiting for an event to happen, or that have
      to give up the CPU in response to a preemption. */

    void Scheduler::add(Thread * _thread){
        ready_queue.enqueue(_thread);//add thread to queue
        ++queue_size;
   }
   /* Make the given thread runnable by the scheduler. This function is called
	  typically after thread creation. Depending on the
      implementation, this may not entail more than simply adding the
      thread to the ready queue (see scheduler_resume). */

    void Scheduler::terminate(Thread * _thread){
        bool found=false;
        for (int i=0;i<queue_size;++i){
            Thread* temp=ready_queue.dequeue();
            if (temp->ThreadId()==_thread->ThreadId())
                found=true;//variable used to check if we found the thread we wanted to terminate
            else //if temp matches thread do not re-enqueue it back on the list
                ready_queue.enqueue(temp);//else just push it back on
        }
        if (found)
            --queue_size;
   }
   /* Remove the given thread from the scheduler in preparation for destruction
      of the thread. */
