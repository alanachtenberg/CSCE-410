
#include "page_table.H"
#include "vm_pool.H"
#include "console.H"
VMPool::VMPool(unsigned long _base_address,unsigned long _size,FramePool *_frame_pool,PageTable *_page_table){

        base_address=_base_address;
        size=_size;
        frame_pool=_frame_pool;
        page_table=_page_table;
        region_count=0;
        max_regions=PageTable::PAGE_SIZE/sizeof(allocated_region_info);//gives max that can fit into a single frame
      /*  int pages=size/(PageTable::PAGE_SIZE);
        if (size%(PageTable::PAGE_SIZE)!=0)//determines the smallest multiple of pages that is equal to or greater than size in bytes
            ++pages;*/

        allocated_list=  (allocated_region_info*)(PageTable::PAGE_SIZE * (frame_pool->get_frame()));
        page_table->register_vmpool(this);//register the pool
        Console::puts("VMPool constructed\n");

}
   /* Initializes the data structures needed for the management of this
    * virtual-memory pool.
    * _base_address is the logical start address of the pool.
    * _size is the size of the pool in bytes.
    * _frame_pool pointsallocate(unsigned long _size){

   }*/

   unsigned long VMPool::allocate(unsigned long _size){
        //unsigned int list_index=region_count%LISTCAPACITY;
        if(_size==0)
            return 0;//failed allocation because size was 0

        unsigned long start = 0; //where the starting address of the new region will be


         /*   if(region_count==max_regions){//lets allocate a new frame to handle more regions
                allocated_region_info* temp = allocated_list;
                start = allocated_list[region_count-1].base_address + allocated_list[region_count-1].size;
                allocated_list=  (allocated_region_info*)(PageTable::PAGE_SIZE * (frame_pool->get_frame()));
                memset(allocated_list,0,PageTable::PAGE_SIZE); //init mem to 0
                allocated_list[0].base_address=start;
                allocated_list[0].size=_size;
                allocated_list[0].prev=temp;
                ++region_count;
                max_regions=max_regions+LISTCAPACITY;
                return start;
            }*/
            if(region_count==0)// if allocated list is empty
                start = base_address;
            else{
                start = allocated_list[region_count-1].base_address + allocated_list[region_count-1].size;
            }
            allocated_list[region_count].base_address = start;
            allocated_list[region_count].size = _size;
            //if (region_count<PageTable::PAGE_SIZE) //if there is only one list
            //    allocated_list[region_count].prev = NULL;
            //else
            //    allocated_list[list_index].prev=allocated_list[0].prev; // if there are multiple lists
            ++region_count; /*NOTE prev is NOT the prev region_info, but it is a pointer to the beginning
                       of the previously allocated list*/
            if(region_count>max_regions){
                Console::puts("out of space");
                for(;;);
                }
            return start;
    }
   /* Allocates a region of _size bytes of memory from the virtual
    * memory pool. If successful, returns the virtual address of the
    * start of the allocated region of memory. If fails, returns 0. */


   void VMPool::release(unsigned long _start_address){
                //find the region to be released from the start addr
        unsigned int index=0;
        for(unsigned int i; i < region_count; i++)
            if(allocated_list[i].base_address == _start_address){
                index=i;
                break;
            }

        for (unsigned int j = 0; j < allocated_list[index].size/PageTable::PAGE_SIZE; j++)
        {
            page_table->free_page(_start_address);
            _start_address = _start_address + PageTable::PAGE_SIZE;
        }
        //restructure array

        allocated_region_info* old= allocated_list;
        allocated_list=  (allocated_region_info*)(PageTable::PAGE_SIZE * (frame_pool->get_frame()));
        unsigned int new_count=0;
        for (unsigned int k=0;k<region_count;++k){
            if (k!=index)
                allocated_list[new_count]=old[k];
            ++new_count;
        }
        frame_pool->release_frame((unsigned long)old/PageTable::PAGE_SIZE);
        // flush the TLB
        page_table->load();
   }
   /* Releases a region of previously allocated memory. The region
    * is identified by its start address, which was returned when the
    * region was allocated. */

   BOOLEAN VMPool::is_legitimate(unsigned long _address){
     /*unsigned long temp_count=region_count;
     if (region_count==0)
        return true;
     allocated_region_info* temp_list=allocated_list;
        while(temp_count>0)
        {
            for(unsigned int i=0;i<LISTCAPACITY;++i){
                if(temp_list[i].size=0)
                    return false;//case where nothing is left in list
                if((temp_list[i].base_address <= _address) && (_address < (temp_list[i].base_address + temp_list[i].size)))
                    return true;
                --temp_count;
			}
			if (temp_list[0].prev==NULL)
                return false;
            else
                temp_list=temp_list[0].prev;//sets next list to check to previously allocated list
        }
        Console::puts("Not sure why we got here! ,but it might have something to do with region_count...\n");
        return false;//hopefully we should never get here */
        //for(unsigned int i = 0; i < ; i++)

        if(!allocated_list)
            if((allocated_list[0].base_address <= _address) && (_address < (allocated_list[0].base_address + allocated_list[0].size)))
                Console::puts("");
                return true;
        return false;
   }
   /* Returns FALSE if the address is not valid. An address is not valid
    * if it is not part of a region that is currently allocated. */
