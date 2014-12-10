
#include "blocking_disk.H"
#include "console.H"
#include "assert.H"
//is_ready is implemented by SimpleDisk
        /* Creates a SimpleDisk device with the given size connected to the MASTER
           or SLAVE slot of the primary ATA controller. */
       BlockingDisk::BlockingDisk(DISK_ID _disk_id, unsigned int _size): SimpleDisk(_disk_id,_size){

       }
        //OVERRIDE

        void BlockingDisk::wait_until_ready() {

            while (!is_ready()) {
            SYSTEM_SCHEDULER->resume(Thread::CurrentThread());//place current thread on ready queue
            SYSTEM_SCHEDULER->yield();//yield processor while we wait for io to be ready
            }
        }

        //above method allows us to avoid implementing these
        /*DISK OPERATIONS */
       /* Reads 512 Bytes from the given block of the disk and copies them
        to the given buffer. No error check! */
//        //OVERRIDE
//        void BlockingDisk::read(unsigned long _block_no, char * _buf){
//            Console::puts("BLOCKING READ\n");
//            for(;;);
//        }
//        //OVERRIDE
//        /* Writes 512 Bytes from the buffer to the given block on the disk. */
//        void BlockingDisk::write(unsigned long _block_no, char * _buf){
//            Console::puts("BLOCKING WRITE\n");
//            for(;;);
//        }

