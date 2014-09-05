CPSC 410/611: MP1 -- README.TXT

This file describes the content of this directory.
A file marked with (*) may be of some interest to the 
student, and he/she may want to have a look at it.
A file marked with (**) is important for this MP, and the
student is strongly encouraged to study the contents of this file.

BOCHS Environment:
=================

FILE: 			DESCRIPTION:

BIOS-bochs-latest	BIOS file.
VGABIOS-lgpl-latest	BIOS file for the graphics system.
dev_kernel_grub.img	Image file of the boot floopy.
bochsrc.bxrc		Description file for the system. (doube-click file under windows to 
			start emulation).

COMPILATION:
===========

FILE: 			DESCRIPTION:

makefile.win (**)	Makefile for win32 environment. Type "make -f makefile.win" to generate the kernel or rename to makefile.
makefile.linux/64 (**)	Makefile for Linux environment. Type "make -f makefile.linux" to generate the kernel or rename to makefile.  
                        
linker.ld		the linker script.

OS COMPONENTS:
=============

FILE: 			DESCRIPTION:

start.asm (*)		The bootloader starts code in this file, which in turn
			jumpts to the main entry in File "kernel.C".
kernel.C (**)		Main file, where the OS components are set up, and the
                        system gets going.


console.H/C		Routines to print to the screen.
utils.H/C		Various utilities (e.g. memcpy, strlen, etc..)

UTILITIES:
==========

FILE: 			DESCRIPTION:

copykernel.bat (**)	(Windows) Simple script to copy the kernel onto the floppy image.
                        The script mounts the floppy image, copies the kernel
			image onto it, and then unmounts the floppy image again.
  			The paths in the file need to be edited to make them
			reflect the student's environment. This script is
                        supposed to be used in the win32 environment with
                        filedisk installed.


copykernel.sh (**)	(Linux) Simple script to copy the kernel onto the floppy image.
                        The script mounts the floppy image, copies the kernel
			image onto it, and then unmounts the floppy image again.
  			The paths in the file need to be edited to make them
			reflect the student's environment. This script is
                        supposed to be used in the linux environment.

