
#include "file_system.H"
#include "assert.H"


   File::File(){
   file_id=0;
   file_size=0;
   cur_block=0;
   cur_position=0;
   block_nums=NULL;
   }

   File::File(unsigned int id){
        file_id=id;
        if (FILE_SYSTEM->LookupFile(file_id, this))//this allows LookupFile to initialize file, returns false if not found
            Console::puts("Found File\n");
        else if (FILE_SYSTEM->CreateFile(file_id)){
            cur_block=0;//initialize empty file, if write occurs we will allocate memory later
            cur_position=0;
            file_size=0;
            block_nums=NULL;
            //Console::puts("Created empty file\n");
        }
        else
            Console::puts("ERR cannot create file\n");
   }
    /* Constructor for the file handle. Set the 'current
       position' to be at the beginning of the file. */

    unsigned int File::Read(unsigned int _n, char * _buf){
        unsigned int count=_n;//initialize count
        while (count>0){
            if (EoF() && count>0)
                return 0;//error
//            Console::puts("bloc_num[cur_block] ");Console::putui(block_nums[cur_block]);Console::puts("\n");
//            Console::puts("Current Position ");Console::putui(cur_position);Console::puts("\n");
            FILE_SYSTEM->disk->read(block_nums[cur_block],disk_buff);//read block from file
            for (cur_position;cur_position<BLOCKSIZE-HEADER_SIZE;++cur_position){//cur position ranges from 0-511 in increments of 8
                if (count==1)//dont read more than were supposed to
                    break;
//                Console::puts("bloc_num[cur_block] ");Console::putui(block_nums[cur_block]);Console::puts("\n");
//                Console::puts("Current Position ");Console::putui(cur_position);Console::puts("\n");
//                Console::puts("Char Copied ");Console::putui(*(disk_buff+HEADER_SIZE+cur_position));Console::puts("\n");

                memcpy(_buf,disk_buff+HEADER_SIZE+cur_position,1);//copy from file  buffer to user buffer
                ++_buf;//increment buffer pointer
                count-=1;

            if (cur_position==(BLOCKSIZE-HEADER_SIZE)){//if statement accounts for case where we read less than a block
                //Console::puts("finsihed with old block, reading new block\n");
                cur_position=0;
                ++cur_block;
            }
        }
        return (count-_n)*-1;//returns the total amount read
    }
}
    /* Read _n characters from the file starting at the
       current location and copy them in _buf.
       Return the number of characters read. */
    void putUIntData(char* buffer, unsigned int size){
        unsigned int* ptr=(unsigned int*)buffer;
        unsigned int i=0;
        for (i=0;i<size/sizeof(unsigned int);++i)
            Console::putui(ptr[i]);
    }
    unsigned int File::Write(unsigned int _n, char * _buf){
        unsigned int count=_n;//initialize count
        while (BLOCKSIZE-HEADER_SIZE<=count){

//            putUIntData(disk_buff,10*sizeof(unsigned int));
//            Console::puts("\n DATA We want to write\n");
            if (EoF())//if we are at end of file get a new block
                GetBlock();
            //MAKE SURE TO COPY BUFFER AFTER GETBLOCK, getblock uses disk_buff
            memcpy((void*)(disk_buff+HEADER_SIZE),_buf,BLOCKSIZE-HEADER_SIZE);//copy from user buffer to file buffer
            FILE_SYSTEM->disk->write(block_nums[cur_block],disk_buff);
//            putUIntData(disk_buff,10*sizeof(unsigned int));
//            Console::puts("\n DATA WROTE\n");-
//            memset(disk_buff,0,BLOCKSIZE);//clear buffer
//            FILE_SYSTEM->disk->read(block_nums[cur_block],disk_buff);
//            putUIntData(disk_buff,10*sizeof(unsigned int));
//            Console::puts("\n DATA READ\n");
            count-=(BLOCKSIZE-HEADER_SIZE);
        }
        return (count-_n)*-1;//returns the total amount read
    }
    /* Write _n characters to the file starting at the current
       location, if we run past the end of file, we increase
       the size of the file as needed.
     */

    void File::Reset(){
            cur_position=0;
            cur_block=0;
//            Console::puts("reseting block and position ");Console::putui(cur_block);Console::putui(cur_position);
//            Console::puts("\n");
    }
    /* Set the 'current position' at the beginning of the file. */

    void File::Rewrite(){
        cur_block=0;
        while(cur_block<file_size){//release memory
            FILE_SYSTEM->DeallocateBlock(block_nums[cur_block]);
            ++cur_block;
        }
        cur_block=0;
        cur_position=0;
        block_nums=NULL;
        file_size=0;
    }
    /* Erase the content of the file. Return any freed blocks.
       Note: This function does not delete the file! It just erases its
       content. */

    BOOLEAN File::EoF(){
        if (block_nums==NULL){
            //Console::puts("EOF REACHED\n");
            return TRUE;
            }
        if (cur_block==file_size-1 && cur_position==BLOCKSIZE-HEADER_SIZE-1 ){
            //Console::puts("EOF REACHED\n");
            return TRUE;
            }
        else
            return FALSE;
    }
    /* Is the current location for the file at the end of the file? */
    BOOLEAN File::GetBlock(){
        unsigned int new_block_num=FILE_SYSTEM->AllocateBlock(0);
        //Console::puts("New block: ");Console::putui(new_block_num);Console::puts("\n");
        unsigned int* new_num_array= (unsigned int*)new unsigned int[file_size+1];
        for (unsigned int i=0;i<file_size;++i)//copy old list
            new_num_array[i]=block_nums[i];
        if (block_nums!=NULL)
            new_num_array[file_size]=new_block_num;//set new index to new block number
        else
            new_num_array[0]=new_block_num;
        ++file_size;//increment file size
        delete block_nums; //delete old array
        block_nums=new_num_array;//set pointer to new array
        return TRUE;
    }
    /* Allocates a block from global file system and updates blocknum array */

   FileSystem::FileSystem(){
        block_num=0;
        num_files=0;
        files=NULL;
        memset(disk_buff,0,BLOCKSIZE);//clear buffer

   }
   /* Just initializes local data structures. Does not connect to disk yet. */

   BOOLEAN FileSystem::Mount(SimpleDisk * _disk){
        disk=_disk;
        disk->read(0,disk_buff);
        num_files=block->size;
        for (unsigned int i=0;i<num_files;++i){
            disk->read(0,disk_buff);//refresh buffer back to root node of file system
            File* newFile= new File();//create a new file
            disk->read(block->data[i],disk_buff);//puts file inode in buffer
            newFile->file_size=block->size;
            newFile->file_id=block->id;
            unsigned int k=0;
            for ( k=0;k<newFile->file_size;++k){
               // newFile->GetBlock();//allocates a block
               // newFile->block_nums[j]=inode->blocks[j];//sets block number
            }
            push_back_file(newFile);
        }

   }
   /* Associates the file system with a disk. We limit ourselves to at most one
      file system per disk. Returns TRUE if 'Mount' operation successful (i.e. there
      is indeed a file system on the disk. */

    BOOLEAN FileSystem::Format(SimpleDisk * _disk, unsigned int _size){
    //every file uses one block as its inode, not the most efficient, but better to implement
    //can support files up to about 64 KB 127*512
    //first block of filesystem is always reserved for an array of inodes with more if needed
    //meaning our system will theoretically support up to 2^32 files but realistically we will long run out of memory before then
    //first 4 bytes of the first block is the number of files block
    disk=_disk;
    /*testing FILE_SYSTEM->disk->read(50,disk_buff);
            putUIntData(disk_buff,10*sizeof(unsigned int));
            Console::puts("\nRead initial\n");
    memset(disk_buff,'b',15);
      FILE_SYSTEM->disk->write(50,disk_buff);
            putUIntData(disk_buff+HEADER_SIZE,10*sizeof(unsigned int));
            Console::puts("\nWrote\n");
    memset(disk_buff,0x0,BLOCKSIZE);
    FILE_SYSTEM->disk->read(50,disk_buff);
            putUIntData(disk_buff+HEADER_SIZE,10*sizeof(unsigned int));
            Console::puts("\nAFTER WRITE\n");
    assert(false); */
    memset(disk_buff,0,BLOCKSIZE);//set entire disk to 0, automatically free memory
    Console::puts("Formatting Disk, may take awhile\n");
    for (int i=0;i<SYSTEM_BLOCKS;++i)
        _disk->write(i,disk_buff);
    block->availability=USED;//set block to used
    block->size=0;//write to  size block this will cause first 4 bytes to be empty which is interpereted as 0 files on disk
    _disk->write(0,disk_buff);//initializes master block
    Console::puts("Format of empty file system complete\n");
   }
   /* Wipes any file system from the given disk and installs a new, empty, file
      system that supports up to _size Byte. */

   BOOLEAN FileSystem::LookupFile(unsigned int _file_id, File * _file){

        unsigned int i=0;
        for (i=0;i<num_files+1;++i){
            if (files[i].file_id==_file_id){
                *_file=files[i];
//                Console::puts("Look UP PASSED file_id:");Console::putui(files[i].file_id);Console::puts("\n");
//                Console::puts("file count: ");Console::putui(num_files);Console::puts("\n");
                return TRUE;
            }
        }
//        Console::puts("Look UP FAILED file_id:");Console::putui(files[i].file_id);Console::puts("\n");
//        Console::puts("file count: ");Console::putui(num_files);Console::puts("\n");
//        Console::puts("requested id: ");Console::putui(_file_id);Console::puts("\n");
        return FALSE;
   }
   /* Find file with given id in file system. If found, initialize the file
      object and return TRUE. Otherwise, return FALSE. */

   BOOLEAN FileSystem::CreateFile(unsigned int _file_id){
        File* newFile=(File*) new File();

        if (LookupFile(_file_id,newFile)){
            //Console::puts("CREATION FAILED file_id:");Console::puti(_file_id);Console::puts("\n");
            return FALSE;
            }
        newFile->file_id=_file_id;
        newFile->file_size=0;
        newFile->block_nums=NULL;
        newFile->Rewrite();//simply clears all data and sets fields to 0
        //do not need to handle allocating data, write function of file will take care of that
        //but we do need to create an inode
        //Console::putui(newFile->inode_block_num);
        newFile->inode_block_num=AllocateBlock(0);//get any free block
        disk->read(newFile->inode_block_num,disk_buff);//load block in buffer
        block->availability=USED;//set block to used
        block->size=0;//size 0
        block->id=_file_id;
        disk->write(newFile->inode_block_num,disk_buff);//write file inode to disk
        push_back_file(newFile);//pushback file object
        return TRUE;
   }
   /* Create file with given id in the file system. If file exists already,
      abort and return FALSE. Otherwise, return TRUE. */

   BOOLEAN FileSystem::DeleteFile(unsigned int _file_id){
        File* oldFile;
        if (LookupFile(_file_id,oldFile)==FALSE){
            Console::puts("DELETION FAILED file_id:");Console::puti(_file_id);Console::puts("\n");
            return FALSE;
            }
        else
        return remove_file(_file_id);
   }
   /* Delete file with given id in the file system and free any disk block
      occupied by the file. */
   unsigned int FileSystem::AllocateBlock(unsigned int _block_num){
        if (_block_num!=0){//we assume the user is right
            disk->read(_block_num,disk_buff);
            block->availability=USED;//sets block header to used
            disk->write(_block_num,disk_buff);
            return _block_num;
        }
        else{//else we find a free block for them
            disk->read(block_num,disk_buff);
            int sanity_check=0;
            while (block->availability==USED){
                if (block_num>(SYSTEM_BLOCKS-1)){//look back at beginning
                    block_num=0;
                    ++sanity_check;
                    if (sanity_check>1){
                        Console::puts("ERROR NO FREE BLOCKS!!!!");
                        return 0;
                        }
                }
                ++block_num;
                disk->read(block_num,disk_buff);
            }
            disk->read(block_num,disk_buff);
            block->availability=USED;//sets block header to used
            disk->write(block_num,disk_buff);
            return block_num;
        }
   }
   /*Allocates a block from free list, for use in file*/
   void FileSystem::DeallocateBlock(unsigned int _block_num){
        disk->read(_block_num,disk_buff);
            block->availability=FREE;//sets block header to free
        disk->write(block_num,disk_buff);
   }
   /*Deallocates a block from free list, for use in file*/

   void FileSystem::push_back_file(File* newFile){
        if (files=NULL)
            files=newFile;
        else
        {
        File* new_file_array= (File*)new File[num_files+1];
        unsigned int i=0;
        for (i=0;i<num_files;++i)//copy old list
            new_file_array[i]=files[i];
        new_file_array[num_files+1]=*newFile;//set new index to new block number
        ++num_files;//increment files size
        delete files; //delete old array
        files=new_file_array;//set pointer to new array
   }
   }

   BOOLEAN FileSystem::remove_file(unsigned int _file_id){
        File* new_file_array= (File*)new File[num_files];
        BOOLEAN found=FALSE;
        for (unsigned int i=0;i<num_files;++i){//copy old list
            if (files[i].file_id==_file_id){
                found==TRUE;
                files[i].Rewrite();//erases and unaalocated memory
                DeallocateBlock(files[i].inode_block_num);//deletes inode of file
                }
            if (!found){
                new_file_array[i]=files[i];
                --num_files;
                }
            else
                new_file_array[i]=files[i+1];
        }
        delete files; //delete old array
        files=new_file_array;//set pointer to new array
        if (num_files==0)
            files=NULL;//precautionary
        return found;
   }

