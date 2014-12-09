CSCE 410/611: MP5 -- README.TXT

This file describes the content of this directory.
A file marked with (*) may be of some interest to the 
student, and he/she may want to have a look at it.
A file marked with (**) is important for this MP, and the
student is encouraged to study the contents of this file.
Files marked with (**) may need to be modified in this MP.

BOCHS Environment:
==================

FILE: 			DESCRIPTION:

BIOS-bochs-latest	BIOS file.
VGABIOS-lgpl-latest	BIOS file for the graphics system.
dev_kernel_grub.img	Image file of the boot floopy.
bochsrc.bxrc		Description file for the system. (doube-click file to 
			start emulation).

c.img                   File representations for disk "C" and disk "D"
d.img                   "C" is connected to the MASTER port on ATA-0,
                        "D" is connected to the SLAVE port.

COMPILATION:
===========

FILE: 			DESCRIPTION:

makefile (**)		type "make" to generate the kernel.
makefile.linux (**)	Makefile for Linux environment. overwrite makefile with this file
                        if you are in a Linux environemnt.
makefile.linux64(**)	Makefile for Linux 64 bit environment.

linker.ld		the linker script.

OS COMPONENTS:
=============

FILE: 			DESCRIPTION:

start.asm (*)		The bootloader starts code in this file, which in turn
			jumpts to the main entry in File "kernel.C".
kernel.C (**)		Main file, where the OS components are set up, and the
                        system gets going.

assert.h/c		Implements the "assert()" utility.
utils.h/c		Various utilities (e.g. memcpy, strlen, etc..)

gdt.H/C			Global Descriptor Table.
gdt_low.asm		Low-level GDT code, included in "start.asm".
idt.H/C			Interrupt Descriptor Table.
idt_low.asm		Low-level IDT code, included in "start.asm".
irq.H/C                 Re-mapping of IRQ's into the IDT.
irq_low.asm		Low-level IRQ stuff. (Primarily the interrupt service
			routines and the routine stub that branches out to the
			interrupt dispatcher in "interrupts.c". Included in
  			"start.asm".
exceptions.H/C (*)	The exception dispatcher.
interrupts.H/C		The interrupt dispatcher.

console.H/C		Routines to print to the screen.

machine.H/C		Definitions of some system constants and low-level
			machine operations. (Primarily enable/disable interrupts)

simple_timer.H/C (*)	Routines to control the periodic interval timer.


frame_pool.H/C (*)	Definition and implementation of a vanilla free frame pool
                        manager. 

mem_pool.H/C (*)        Definition and implementation of a vanilla memory allocator.

thread.H/C (*)	        Tread Control Block (TCB), creation and low-level dispatching
                        of threads.

threads_low.asm (*)	Low-level tread context switch code.

scheduler.H (**)        Definition of a simple scheduler. Add your own implementation.
simple_disk.H/C (**)    Simple LBA28 Disk driver. Uses busy waiting from operation 
                        issue until disk is ready for data transfer. Use this class
                        as base class for BlockingDisk.

file_system.H (**)      Partial definition of class File and class FileSystem. You are
                        to implement the missing .C file.

UTILITIES:
==========

FILE: 			DESCRIPTION:

copykernel.bat (**)	Simple script to copy the kernel onto the floppy image.
                        The script mounts the floppy image, copies the kernel
			image onto it, and then unmounts the floppy image again.
  			The paths in the file need to be edited to make them
			reflect the environment. 


copykernel.sh (**)	Same for a Linux environment


