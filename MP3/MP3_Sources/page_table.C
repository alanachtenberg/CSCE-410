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
//define static data
    PageTable* PageTable::current_page_table; /* pointer to currently loaded page table object */
    unsigned int    PageTable::paging_enabled;     /* is paging turned on (i.e. are addresses logical)? */
    FramePool*      PageTable::kernel_mem_pool;    /* Frame pool for the kernel memory */
    FramePool*      PageTable::process_mem_pool;   /* Frame pool for the process memory */
    unsigned long   PageTable::shared_size;        /* size of shared address space */

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

void PageTable::init_paging(FramePool * _kernel_mem_pool, FramePool * _process_mem_pool, const unsigned long _shared_size){
    PageTable::kernel_mem_pool = _kernel_mem_pool; //pointers
    PageTable::process_mem_pool = _process_mem_pool;
    PageTable::shared_size = _shared_size;//size of shared kernel memory
  }
  /* Set the global parameters for the paging subsystem. */

  PageTable::PageTable(){
        if (page_directory==NULL){// only initialize page directory once
            page_directory = ( unsigned long*)(process_mem_pool->get_frame()*FRAME_SIZE); //set page directory to frame in process pool
            memset(page_directory,0,FRAME_SIZE);
            /*direct mapping of first 4 MB*/
            unsigned long* direct_page_table= (unsigned long*)(process_mem_pool->get_frame()*FRAME_SIZE); // get frame for page_table
            unsigned long  direct_addr=0;
            for (unsigned int i=0;i<1024;++i){
                direct_page_table[i]=direct_addr | 3;//sets to kernel,r/w, and present 011
                direct_addr=direct_addr+FRAME_SIZE;
            }
            page_directory[0]=(unsigned long)direct_page_table|3;//forgot to add the |3.. terrible bug
        }

        for(unsigned int i=1; i<1023; ++i)// initialize all page_tables in directory to not there
            page_directory[i] = 0 | 2; // attribute set to: supervisor level, read/write, not present(010 in binary)
        page_directory[1023]=(unsigned long)page_directory|3;//set last entry in page directory to point to itself
        //used to modify the page directory using logical addresses by recursion
        for (unsigned int i=0;i<VM_ARRAY_SIZE;++i)
            vmpool_array[i]=NULL;
  }
  /* Initializes a page table with a given location for the directory and the
     page table proper.
     NOTE: The PageTable object still needs to be stored somewhere! Probably it is best
           to have it on the stack, as there is no memory manager yet...
     NOTE2: It may also be simpler to create the first page table *before* paging
           has been enabled.
  */

    void PageTable::load(){
        current_page_table = this;
        write_cr3((unsigned long)page_directory);//cast pointer to uint to match arguments, realistically theyre the same
  }
  /* Makes the given page table the current table. This must be done once during
     system startup and whenever the address space is switched (e.g. during
     process switching). */

    void PageTable::enable_paging(){
        paging_enabled = 1;
        write_cr0(read_cr0() | 0x80000000);
  }
  /* Enable paging on the CPU. Typically, a CPU start with paging disabled, and
     memory is accessed by addressing physical memory directly. After paging is
     enabled, memory is addressed logically. */

   void PageTable::handle_fault(REGS * _r){
   Console::puts("Page fault handler started\n");

    unsigned long * page_dir = (unsigned long *)0xFFFFF000;//when accessed this address recursively points to the page directory, by referencing itself twice
    /*Console::puts("page directory ADDRESS!\n");
    Console::putui(*page_dir);
    for (;;);*/
    unsigned long page_address;
    unsigned long* page_table;

    unsigned long error = _r->err_code;
    if((error & 1) != 0)
        Console::puts("Error Protection fault\n");
    else
    {
      page_address = read_cr2();

      VMPool** vm_array=current_page_table->vmpool_array;
      int vm_index=-1;
      for(unsigned int i=0;i<VM_ARRAY_SIZE;++i)
            if(vm_array[i]!=NULL){
                if (vm_array[i]->is_legitimate(page_address)){
                    vm_index=i;
                    break;
                    }
            }
      if (vm_index<0)
            Console::puts("INVALID ADDRESS\n");

      unsigned long directory_index; //first 10 bits
      unsigned long page_table_index; //next 10 bits;
      directory_index=page_address>>22;
      page_table_index=page_address>>12 &0x3FF;
      if(page_dir[directory_index] & 1 == 1)//if page_table is in mem
      {
         //get page table
         page_table= (unsigned long *)(0xFFC00000|(directory_index<<12)); //& operation insures it points to a multiple of 4kb
         //load page
         page_table[page_table_index] =  (PageTable::process_mem_pool->get_frame()*FRAME_SIZE)|3;//setting 011 is kernel r/w and present
      }
      else //else page_table not loaded
      {
        //load page_table into directory
         page_dir[directory_index] = (unsigned long)((process_mem_pool->get_frame()*FRAME_SIZE)|3);//allocates page table from kernel mem
         page_table = (unsigned long *)(0xFFC00000|(directory_index<<12));//get address of new page table

         for(int i=0; i<1024; i++)
         {
            page_table[i] = 0|2;// initialize page table to all empty
         }
        //load page into page table
         page_table[page_table_index] =  (PageTable::process_mem_pool->get_frame()*FRAME_SIZE)|3;
        Console::puts("HERE I AM\n");
      }
    }
  }
  /* The page fault handler. */

  void PageTable::free_page(unsigned long _page_no){
	unsigned long directory_idex = _page_no >> 22;
	unsigned long page_index = (_page_no >> 12) & 0x3FF;
	unsigned long* page_table = (unsigned long*)(0xFFC00000 | (directory_idex << 12));
    unsigned long frame_number= page_table[page_index];
	process_mem_pool->release_frame(frame_number);

  }
  /* Release the frame associated with the page _page_no */

  void PageTable::register_vmpool(VMPool *_pool){
        int index=-1;
        for (unsigned int i=0;i<VM_ARRAY_SIZE;++i) //find empty index for vmpool
            if (vmpool_array[i]==NULL)
                index=i;
        if (index>=0){
            vmpool_array[index]=_pool;//register pool
            Console::puts("register vmpool is successful\n");//report error if you cant register

        }
        else
            Console::puts("ERR register VMPool failed, array is full\n");//report error if you cant register
  }
  /* The page table needs to know about where it gets its pages from.
     For this, we have VMPools register with the page table. */

