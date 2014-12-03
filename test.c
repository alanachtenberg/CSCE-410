#include <stdlib.h>



struct Thread{
    int thread_id;
};


class Node{//simple node class for implementation of linked list and ready queue

    private:
    Thread* thread;
    Node* next;
    public:
    Node(Thread* t){
        thread=t;
        next=NULL;
    }
    //appends  thread to end of ready queue
    //recursive implementation allows append to be called on any node in list with the same result
    void push(Thread& t){//will attempt to set next to new Node(thread), if next!= null will recursively call append on next
        if (next==null)
            next=new Node(t);
        else
            next->push(t);
    }
    Thread* pop(){//returns and removes thread at the beginning of the list
        if (next!=null){
            Thread* popped_t=thread;// save oridinal thread
            thread=next->thread;//set thread to the next thread
            Node* consumed=next;
            next=next->next;//set next to the next of the new head
            delete consumed;// clean  up old node
            return popped_t; //return the original thread
        }
        //!!!!popping empty list will be handled elsewhere!!!!, likely by maintaining a size integer
        return thread;//case where there is only one item in list, return thread
    }

};

int main(){
    Node list;
    for (int i=0;i<10;++i){
        Thread thread;
        thread.thread_id=i;
        list.push(thread);
    }

     for (int i=0;i<10;++i){
        Thread t=list.pop();
        printf("%d\n",t.thread_id);
    }

    return 0;
}
