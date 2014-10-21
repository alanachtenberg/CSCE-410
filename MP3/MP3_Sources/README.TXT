CSCE 410/611/613: MP2 -- README.TXT

This file describes the content of this directory.
A file marked with (*) may be of some interest to the 
student, and he/she may want to have a look at it.
A file marked with (**) is important for this MP, and the
student is strongly encouraged to study the contents of this file.

BOCH Environment:
=================

FILE: 			DESCRIPTION:

BIOS-bochs-latest	BIOS file.
VGABIOS-lgpl-latest	BIOS file for the graphics system.
dev_kernel_grub.img	Image file of the boot floopy.
bochsrc.bxrc		Description file for the system. (doube-click file to 
			start emulation).

COMPILATION:
===========

FILE: 			DESCRIPTION:

makefile (**)		Makefile for win32 environment. Type "make" to generate the kernel.
makefile.linux (**)	Makefile for Linux environment. overwrite makefile with this file
                        if you are in a Linux environment.
makefile.linux64 (**)	Makefile for Linux 64-bit  environment. overwrite makefile with this file
                        if you are in a Linux 64-bit environment.
linker.ld		the linker script.

OS COMPONENTS:
=============

FILE: 			DESCRIPTION:

start.asm (*)		The bootloader starts code in this file, which in turn
			jumps to the main entry in File "kernel.C".
kernel.C (**)		Main file, where the OS components are set up, and the
                        system gets going.

assert.H/C		Implements the "assert()" utility.
utils.H/C		Various utilities (e.g. memcpy, strlen, etc..)

gdt.H/C			Global Descriptor Table.
gdt_low.asm		Low-level GDT code, included in "start.asm".
idt.H/C			Interrupt Descriptor Table.
idt_low.asm		Low-level IDT code, included in "start.asm".
irq.H/C                 mapping of IRQ's into the IDT.
irq_low.asm		Low-level IRQ stuff. (Primarily the interrupt service
			routines and the routine stub that branches out to the
		        interrupt dispatcher in "interrupts.C". Included in
  			"start.asm".
exceptions.H/C (*)	The exception dispatcher.
interrupts.H/C		The interrupt dispatcher.

console.H/C		Routines to print to the screen.

simple_timer.H/C (*)	Routines to control the periodic interval timer.

machine.H/C (*)		Definitions of some system constants and low-level
			machine operations. (Primarily memory sizes, register set, and
                        enable/disable interrupts)

machine_low.H/asm       Various low-level x86 specific stuff.

paging_low.H/asm (**)	Low-level code to control the registers needed for 
			memory paging.

page_table.H (**)	Definition of the page table interface.
frame_pool.H (**)	Definition of the free frame pool interface.
vm_pool.H (**)          Definition of the virtual memory pool interface.

UTILITIES:
==========

FILE: 			DESCRIPTION:

copykernel.bat (**)	(windows) Simple script to copy the kernel onto the floppy image.
                        The script mounts the floppy image, copies the kernel
			image onto it, and then unmounts the floppy image again.
  			The paths in the file need to be edited to make them
			reflect the student's environment. This script is
                        supposed to be used in the win32 environment with
                        filedisk installed.


copykernel.sh (**)	(linux) Simple script to copy the kernel onto the floppy image.
                        The script mounts the floppy image, copies the kernel
			image onto it, and then unmounts the floppy image again.
  			The paths in the file need to be edited to make them
			reflect the student's environment. This script is
                        supposed to be used in the linux environment.

