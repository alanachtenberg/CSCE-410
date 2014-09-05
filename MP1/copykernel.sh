#!/bin/sh
# This code mounts the disk file, copies the code, and 
# unmounts the disk file afterwards.

sudo mount -o loop dev_kernel_grub.img /mnt/floppy
sudo cp kernel.bin /mnt/floppy/
sudo umount /mnt/floppy/
