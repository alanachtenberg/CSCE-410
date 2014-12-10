/*
    File: file_system.H

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 10/04/05

    Description: File System.


*/

#ifndef _FILE_SYSTEM_H                   // include file only once
#define _FILE_SYSTEM_H

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/
#define BLOCKSIZE 512 //bytes
#define SYSTEM_DISK_SIZE 10485760

#define SYSTEM_BLOCKS SYSTEM_DISK_SIZE/BLOCKSIZE

#define HEADER_SIZE 12

#define FREE    0x0000
#define USED    0xFFFF
#define BLOCK_DATA_SIZE 125 //bytes
/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "utils.H"
#include "simple_disk.H"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

static char disk_buff[BLOCKSIZE];//for input and output with system disk




    struct BLOCK{
        unsigned int id;
        unsigned int availability;//points to 4 bytes
        unsigned int size;//points to 4bytes
        unsigned int data[125];//points to 126*4bytes
    };

    static BLOCK* block=(BLOCK*)disk_buff;//same implementations but are used for different purposes

/*--------------------------------------------------------------------------*/
/* FORWARD DECLARATIONS */
/*--------------------------------------------------------------------------*/

class FileSystem;

extern FileSystem* FILE_SYSTEM;

/*--------------------------------------------------------------------------*/
/* F i l e */
/*--------------------------------------------------------------------------*/

class File {
    friend FileSystem;
private:
     /* -- DEFINE YOUR FILE DATA STRUCTURES HERE. */

     FileSystem   * file_system;

     unsigned int   inode_block_num;//block of file inode
     unsigned int   file_id;
     unsigned int   file_size; // in blocks
     unsigned int*  block_nums;//pointers of designated file blocks, not guaranteed to be continous
     unsigned int   cur_block; //current block position, always starts from 0
     unsigned int   cur_position; //current position in currrent block, always starts from 0


     /* -- You may want to store other information, such as
             .. position in the file
             .. cached block(s)
             .. etc.
      */

public:
    File();
    File(unsigned int id);
    /* Constructor for the file handle. Set the 'current
       position' to be at the beginning of the file. */

    unsigned int Read(unsigned int _n, char * _buf);
    /* Read _n characters from the file starting at the
       current location and copy them in _buf.
       Return the number of characters read. */

    unsigned int Write(unsigned int _n, char * _buf);
    /* Write _n characters to the file starting at the current
       location, if we run past the end of file, we increase
       the size of the file as needed.
     */

    void Reset();
    /* Set the 'current position' at the beginning of the file. */

    void Rewrite();
    /* Erase the content of the file. Return any freed blocks.
       Note: This function does not delete the file! It just erases its
       content. */

    BOOLEAN EoF();
    /* Is the current location for the file at the end of the file? */

    BOOLEAN GetBlock();//allocates a block and updates block_num array

};

/*--------------------------------------------------------------------------*/
/* F i l e S y s t e m  */
/*--------------------------------------------------------------------------*/

class FileSystem {

friend class File;

private:
     /* -- DEFINE YOUR FILE SYSTEM DATA STRUCTURES HERE. */

     SimpleDisk * disk;
     unsigned int size; //size of disk
     File*  files;
     unsigned int num_files;//number of files
     unsigned int block_num;//current block to look for free blocks
public:

   FileSystem();
   /* Just initializes local data structures. Does not connect to disk yet. */

   BOOLEAN Mount(SimpleDisk * _disk);
   /* Associates the file system with a disk. We limit ourselves to at most one
      file system per disk. Returns TRUE if 'Mount' operation successful (i.e. there
      is indeed a file system on the disk. */

   BOOLEAN Format(SimpleDisk * _disk, unsigned int _size);
   /* Wipes any file system from the given disk and installs a new, empty, file
      system that supports up to _size Byte. */

   BOOLEAN LookupFile(unsigned int _file_id, File * _file);
   /* Find file with given id in file system. If found, initialize the file
      object and return TRUE. Otherwise, return FALSE. */

   BOOLEAN CreateFile( unsigned int _file_id);
   /* Create file with given id in the file system. If file exists already,
      abort and return FALSE. Otherwise, return TRUE. */

   BOOLEAN DeleteFile(unsigned int _file_id);
   /* Delete file with given id in the file system and free any disk block
      occupied by the file. */
   void push_back_file(File* file);
   BOOLEAN remove_file( unsigned int _file_id);
   unsigned int AllocateBlock(unsigned int desired);
   /*Allocates a block from free list, for use in file*/
   void DeallocateBlock(unsigned int);
   /*Deallocates a block from free list, for use in file*/
};
#endif
