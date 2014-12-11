

#include "blocking_disk.H"
#include "console.H"
#include "assert.H"
//is_ready is implemented by SimpleDisk
        /* Creates a SimpleDisk device with the given size connected to the MASTER
           or SLAVE slot of the primary ATA controller. */
       MirroredDisk::MirroredDisk(DISK_ID _disk_id, unsigned int _size): SimpleDisk(_disk_id,_size){

       }

        //above method allows us to avoid implementing these
        /*DISK OPERATIONS */
       /* Reads 512 Bytes from the given block of the disk and copies them
        to the given buffer. No error check! */
        //OVERRIDE
        void BlockingDisk::read(unsigned long _block_no, char * _buf){
            Console::puts("BLOCKING READ\n");
            for(;;);
        }
        //OVERRIDE
        /* Writes 512 Bytes from the buffer to the given block on the disk. */
void SimpleDisk::read(unsigned long _block_no, char * _buf) {
/* Reads 512 Bytes in the given block of the given disk drive and copies them
   to the given buffer. No error check! */

  issue_operation(READ, _block_no);

  wait_until_ready();

  /* read data from port */
  int i;
  unsigned short tmpw;
  for (i = 0; i < 256; i++) {
    tmpw = inportw(0x1F0);
    _buf[i*2]   = (unsigned char)tmpw;
    _buf[i*2+1] = (unsigned char)(tmpw >> 8);
  }
}

void SimpleDisk::write(unsigned long _block_no, char * _buf) {
/* Writes 512 Bytes from the buffer to the given block on the given disk drive. */

  issue_operation(WRITE, _block_no);

  wait_until_ready();

  /* write data to port */
  int i;
  unsigned short tmpw;
  for (i = 0; i < 256; i++) {
    tmpw = _buf[2*i] | (_buf[2*i+1] << 8);
    outportw(0x1F0, tmpw);//maset
    outportw(0x1E0,tmpw)//slave
  }


