CPP = gpp
CPP_OPTIONS = -nostartfiles -nostdlib -fno-rtti -fno-exceptions  

all: kernel.bin

clean:
	del *.o

start.o: start.asm gdt_low.asm idt_low.asm irq_low.asm
	nasm -f aout -o start.o start.asm

utils.o: utils.C utils.H
	$(CPP) $(CPP_OPTIONS) -c -o utils.o utils.C

assert.o: assert.C assert.H
	$(CPP) $(CPP_OPTIONS) -c -o assert.o assert.C

# ==== MACHINE =====

gdt.o: gdt.C gdt.H
	$(CPP) $(CPP_OPTIONS) -c -o gdt.o gdt.C

machine.o: machine.c machine.h
	$(CPP) $(CPP_OPTIONS) -c -o machine.o machine.c

machine_low.o: machine_low.asm machine_low.H
	nasm -f aout -o machine_low.o machine_low.asm

# ==== EXCEPTIONS AND INTERRUPTS =====

idt.o: idt.C idt.H
	$(CPP) $(CPP_OPTIONS) -c -o idt.o idt.C

irq.o: irq.C irq.H
	$(CPP) $(CPP_OPTIONS) -c -o irq.o irq.C

exceptions.o: exceptions.C exceptions.H
	$(CPP) $(CPP_OPTIONS) -c -o exceptions.o exceptions.C

interrupts.o: interrupts.C interrupts.H
	$(CPP) $(CPP_OPTIONS) -c -o interrupts.o interrupts.C


# ==== DEVICES =====

console.o: console.C console.H
	$(CPP) $(CPP_OPTIONS) -c -o console.o console.C

simple_timer.o: simple_timer.C simple_timer.H
	$(CPP) $(CPP_OPTIONS) -c -o simple_timer.o simple_timer.C

# ==== MEMORY =====

frame_pool.o: frame_pool.C frame_pool.H 
	$(CPP) $(CPP_OPTIONS) -c -o frame_pool.o frame_pool.C

mem_pool.o: mem_pool.C mem_pool.H 
	$(CPP) $(CPP_OPTIONS) -c -o mem_pool.o mem_pool.C

# ==== THREADS =====

threads_low.o: threads_low.asm threads_low.h
	nasm -f aout -o threads_low.o threads_low.asm

thread.o: thread.C thread.H threads_low.h
	$(CPP) $(CPP_OPTIONS) -c -o thread.o thread.C

# ==== MAIN =====

kernel.o: kernel.C console.H gdt.H irq.H exceptions.H interrupts.H simple_timer.H frame_pool.H mem_pool.H
	$(CPP) $(CPP_OPTIONS) -c -o kernel.o kernel.C

kernel.bin: start.o utils.o kernel.o assert.o console.o gdt.o idt.o irq.o machine.o machine_low.o exceptions.o interrupts.o \
   simple_timer.o frame_pool.o mem_pool.o threads_low.o thread.o
	ld -T linker.ld -o kernel.bin start.o utils.o kernel.o assert.o console.o gdt.o idt.o \
   exceptions.o irq.o machine.o machine_low.o interrupts.o simple_timer.o frame_pool.o mem_pool.o threads_low.o thread.o
	