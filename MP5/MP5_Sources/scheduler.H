/*
    Author: Joshua Capehart
			Based on code by:
			R. Bettati
            Department of Computer Science
            Texas A&M University

			A thread scheduler.

*/
#ifndef SCHEDULER_H
#define SCHEDULER_H

/*--------------------------------------------------------------------------*/
/* DEFINES
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* INCLUDES
/*--------------------------------------------------------------------------*/

#include "machine.H"
#include "thread.H"
#include "utils.H"//included for NULL define

/*--------------------------------------------------------------------------*/
/* SCHEDULER
/*--------------------------------------------------------------------------*/

//simple class for implementation of ready queue
class Node{
    private:
    Thread* thread;
    Node* next;
    public:
    Node(){
        thread=NULL;
        next=NULL;
    }
    Node(Thread* t){
        thread=t;
        next=NULL;
    }
    Node(Node& n){//copy constructor
        thread=n.thread;
        next=n.next;
    }

    //appends  thread to end of ready queue
    //recursive implementation allows append to be called on any node in list with the same result
    void enqueue(Thread* t){//will attempt to set next to new Node(thread), if next!= null will recursively call append on next
        if (thread==NULL)//case where list is empty
            thread=t;
        else
            if (next==NULL)// check if next exists
                next=new Node(t);//if not set next
            else
                next->enqueue(t);//else call enqueue on next
    }
    Thread* dequeue(){//returns and removes thread at the beginning of the list
        if (thread==NULL)//case where list is empty
            return NULL;
        if (next!=NULL){
            Thread* popped_t=thread;// save oridinal thread
            thread=next->thread;//set thread to the next thread
            Node* consumed=next;
            next=next->next;//set next to the next of the new head
            delete consumed;// clean  up old node
            return popped_t; //return the original thread
        }
        Thread* temp=thread;//temp pointer to return
        thread=NULL;//set thread to null so that push works correctly
        return temp;//case where there is only one item in list, return thread
    }
};

class Scheduler {

   /* The scheduler may need private members... */
    Node    ready_queue;
    int     queue_size;//book keeping for easier things like printing

public:

   Scheduler();
   /* Setup the scheduler. This sets up the ready queue, for example.
      If the scheduler implements some sort of round-robin scheme, then the
      end_of_quantum handler is installed here as well. */

   virtual void yield();
   /* Called by the currently running thread in order to give up the CPU.
      The scheduler selects the next thread from the ready queue to load onto
      the CPU, and calls the dispatcher function defined in 'threads.h' to
      do the context switch. */

   virtual void resume(Thread * _thread);
   /* Add the given thread to the ready queue of the scheduler. This is called
      for threads that were waiting for an event to happen, or that have
      to give up the CPU in response to a preemption. */

   virtual void add(Thread * _thread);
   /* Make the given thread runnable by the scheduler. This function is called
	  typically after thread creation. Depending on the
      implementation, this may not entail more than simply adding the
      thread to the ready queue (see scheduler_resume). */

   virtual void terminate(Thread * _thread);
   /* Remove the given thread from the scheduler in preparation for destruction
      of the thread. */
};



#endif
