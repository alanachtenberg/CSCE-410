/*
    File: frame_pool.C

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

#include "frame_pool.H"
#include "console.H"
/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* METHODS FOR CLASS   frame_pool */
/*--------------------------------------------------------------------------*/
/* -- GLOBAL VARIABLES -- */
    //mask to toggle bits from index 1 thru 8 of a byte
    static unsigned char MASK[] = {128, 64, 32, 16, 8, 4, 2, 1};


    int IsNthbit (unsigned char& c, int n) {
        return ((c & MASK[n]) != 0);
    }
    void SetNthbit(unsigned char& c, int n){
            c = c|MASK[n];
    }
    void ClearNthbit(unsigned char& c, int n){
            c= &(~MASK[n]);
    }

/* -- CONSTRUCTOR -- */


   FramePool::FramePool(unsigned long _base_frame_no, unsigned long _nframes, unsigned long _info_frame_no)
   {
             base_frame_no=_base_frame_no;
             info_frame_no=_info_frame_no;
             nframes=_nframes;

             if (_info_frame_no!=0){ //store information in info frame otherwise store in frame pool
                freeframe_map= (unsigned char*)(_info_frame_no*FRAME_SIZE);  //points frame
                memset(freeframe_map,0,FRAME_SIZE); // Frame size is in bytes
             }
             else{
                freeframe_map= (unsigned char*)(_base_frame_no*FRAME_SIZE);//points bitmap to first frame in pool
                memset(freeframe_map,0,FRAME_SIZE); //intializes entire info frame to 0
                memset(freeframe_map,0x80,1);//sets first bit to 1 in bit_map
                info_frame_no=base_frame_no;//store new info_frame_number
             }
    }
   /* Initializes the data structures needed for the management of this
      frame pool. This function must be called before the paging system
      is initialized.
      _base_frame_no is the frame number at the start of the physical memory
      region that this frame pool manages.
      _nframes is the number of frames in the physical memory region that this
      frame pool manages.
      e.g. If _base_frame_no is 16 and _nframes is 4, this frame pool manages
      physical frames numbered 16, 17, 18 and 19
      _info_frame_no is the frame number (within the directly mapped region) of
      the frame that should be used to store the management information of the
      frame pool. However, if _info_frame_no is 0, the frame pool is free to
      choose any frame from the pool to store management information.
      */

   unsigned long FramePool::get_frame(){
        unsigned char* map_ptr= &freeframe_map[base_frame_no/8]+base_frame_no%8; //get pointer to begining of frame pool

        for (unsigned int i=0;i<nframes/8;++i){ //access bitmap 1 byte at a time
            if(map_ptr[i]^0xFF!=0){ //do a simple check to see if entire block is full
                for (int j=0;j<8;++j){ //if block is not full, then it must have at least one empty frame
                    if(!IsNthbit(map_ptr[i],j))//linear probe block to find empty page
                    {
                        SetNthbit(map_ptr[i],j); //mark frame as used
                        return base_frame_no+i*8+j;//return frame number
                    }
                }
            }
        }
        //corner case where frame pool size is not a multiple of 8 and a free frame is not found
        if (nframes%8!=0){
            for (int j=0;j<nframes%8;++j){ //if block is not full, then it must have at least one empty frame
                if(!IsNthbit(map_ptr[nframes/8],j))//linear probe block to find empty page
                    {
                        SetNthbit(map_ptr[nframes/8],j); //mark frame as used
                        return base_frame_no+nframes*8+j;//return frame number
                    }
            }
        }

        Console::puts("Err no free frames found\n");
        return 0;
   }
   /* Allocates a frame from the frame pool. If successful, returns the frame
    * number of the frame. If fails, returns 0. */

   void FramePool::mark_inaccessible(unsigned long _base_frame_no, unsigned long _nframes){
        if(_base_frame_no<base_frame_no||_base_frame_no+_nframes>base_frame_no+nframes)
            Console::puts("Err cannot mark frames inaccessible, out of range\n");
        else
            {
            unsigned char* map_ptr= &freeframe_map[_base_frame_no/8]+_base_frame_no%8;//start of inaccesible frames
            memset(map_ptr,0,_nframes/8); //sets all but the last few bits as used
            for(unsigned int i=0;i<_nframes%8;++i){
                SetNthbit(map_ptr[_nframes/8],i);//sets remainder bits as used
                }
            }
    }
   /* Mark the area of physical memory as inaccessible. The arguments have the
    * same semantics as in the constructor.
    */

   void FramePool::release_frame(unsigned long _frame_no){
   }
   /* Releases frame back to the given frame pool.
      The frame is identified by the frame number.
      NOTE: This function is static because there may be more than one frame pool
      defined in the system, and it is unclear which one this frame belongs to.
      This function must first identify the correct frame pool and then call the frame
      pool's release_frame function. */



