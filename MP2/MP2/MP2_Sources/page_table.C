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
        if (page_directory==NULL){// only initialize page directory and direct mapped page_table once
            page_directory = ( unsigned long*)(kernel_mem_pool->get_frame()*FRAME_SIZE); //set page directory to frame in kernel space

            unsigned long* direct_page_table= (unsigned long*)(kernel_mem_pool->get_frame()*FRAME_SIZE); // get frame for page_table
            unsigned long  direct_addr=0;
            for (unsigned int i=0;i<1024;++i){
                direct_page_table[i]=direct_addr | 3;//sets to kernel,r/w, and present 011
                direct_addr=direct_addr+FRAME_SIZE;
            }
            page_directory[0]=(unsigned long)direct_page_table|3;//forgot to add the |3.. terrible bug
        }

        for(unsigned int i=1; i<1024; ++i)// initialize all page_tables in directory to not there
            page_directory[i] = 0 | 2; // attribute set to: supervisor level, read/write, not present(010 in binary)

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

    unsigned long *page_dir = (unsigned long *)read_cr3();// read from register, because handler is static
    unsigned long *page_table;
    unsigned long page_address;

    unsigned long error = _r->err_code;
    if((error & 1) != 0)
        Console::puts("Error Protection fault\n");
    else
    {
      page_address = read_cr2();
      if(page_dir[page_address>>22] & 1 == 1)//if page_table is in mem
      {  //get page table
         page_table= (unsigned long *)((page_dir[page_address>>22])&0xFFFFF000); //& operation insures it points to a multiple of 4kb
         //load page
         page_table[(page_address>>12) & 0x3FF] =  (PageTable::process_mem_pool->get_frame()*FRAME_SIZE)|3;//setting 011 is kernel r/w and present
      }
      else //else page_table not loaded
      {
        //load page_table into directory
         page_dir[page_address>>22] = (unsigned long)((kernel_mem_pool->get_frame()*FRAME_SIZE)|3);//allocates page table from kernel mem
         page_table = (unsigned long *)((page_dir[page_address>>22])&0xFFFFF000);//get address of new page table

         for(int i=0; i<1024; i++)
         {
            page_table[i] = 0;// initialize page table to all empty
         }
        //load page into page table
         page_table[(page_address>>12) & 0x3FF] =  (PageTable::process_mem_pool->get_frame()*FRAME_SIZE)|3;

      }
    }
  }
  /* The page fault handler. */

