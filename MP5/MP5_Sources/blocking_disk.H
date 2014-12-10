
#ifndef BLOCKING_DISK_H
#define BLOCKING_DISK_H

#include "simple_disk.H"
#include "scheduler.H"

extern Scheduler* SYSTEM_SCHEDULER;

class BlockingDisk : public SimpleDisk{
    protected:
        //OVERRIDE
        virtual void wait_until_ready();
           /*
         by overriding the wait_until ready method whenever the parents(SimpleDisk) virtual methods
         read and write are called this wait_until_ready method will be called instead of the parent method.
         This allows us to avoid reimplementing read and wait
         the alternative would be to have seperately implemented wait methods which are then called
         by the overidden read and write
        */

    public:
        BlockingDisk(DISK_ID _disk_id, unsigned int _size);
        /* Creates a SimpleDisk device with the given size connected to the MASTER
           or SLAVE slot of the primary ATA controller. */

        /*DISK OPERATIONS */
//        virtual void read(unsigned long _block_no, char * _buf);
//        /* Reads 512 Bytes from the given block of the disk and copies them
//            to the given buffer. No error check! */
//        virtual void write(unsigned long _block_no, char * _buf);
//        /* Writes 512 Bytes from the buffer to the given block on the disk. */

};

#endif
