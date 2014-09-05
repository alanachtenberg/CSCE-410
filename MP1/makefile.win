GPP_OPTIONS = -nostartfiles -nostdlib -fno-rtti -fno-exceptions

all: kernel.bin

clean:
	del *.o

# ==== KERNEL ENTRY POINT ====

start.o: start.asm
	nasm -f aout -o start.o start.asm

# ==== UTILITIES ====

utils.o: utils.H utils.C
	gpp $(GPP_OPTIONS) -c -o utils.o utils.C

# ==== DEVICES ====

console.o: console.H console.C
	gpp $(GPP_OPTIONS) -c -o console.o console.C

# ==== KERNEL MAIN FILE ====

kernel.o: kernel.C
	gpp $(GPP_OPTIONS) -c -o kernel.o kernel.C
	

kernel.bin: start.o kernel.o console.o utils.o linker.ld
	ld -T linker.ld -o kernel.bin start.o kernel.o console.o utils.o
	