#include <stdlib.h>
#include <stdio.h>


struct Thread{
    int thread_id;
    long long dummy_array[8096];
};

//simple node class for implementation of linked list and ready queue
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
    //appends  thread to end of ready queue
    //recursive implementation allows append to be called on any node in list with the same result
    void enqueue(Thread* t){//will attempt to set next to new Node(thread), if next!= null will recursively call append on next
        if (thread==NULL)//case where list is empty
            thread=t;
        else
            if (next==NULL)// check if next exists
                next=new Node(t);//if not set next
            else
                next->enqueue(t);//else call push on next
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

int main(){
    Node list;
    while(1){

    for (int i=0;i<10;++i){
        Thread* thread= new Thread;
        thread->thread_id=i;
        list.push(thread);
    }

     for (int i=0;i<5;++i){
        Thread* t=list.pop();
        delete t;
      //  printf("%d\n",t->thread_id);
    }
    for (int i=0;i<5;++i){
        Thread* thread= new Thread;
        thread->thread_id=i;
        list.push(thread);
    }
     for (int i=0;i<10;++i){
        Thread* t=list.pop();
        printf("%d\n",t->thread_id);
        delete t;
    }

    }
    return 0;
}
