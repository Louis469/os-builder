SRC=$(wildcard libc/*.c)
SRCASM=$(wildcard asm/*.asm)

OBJS=$(patsubst libc/%.c, %.o, $(SRC))
ASMOBJS=$(patsubst asm/%.asm, asm/%.o, $(SRCASM))

install-deps:
	sudo apt install qemu-kvm nasm
	chmod +x deps.sh
	sh deps.sh

mk-files:
	mkdir iso
	touch kernel.c
	printf '#include "libc/io.h"\n#include "libc/typedefs.h"\n#include "libc/ata.h"\n#include "libc/isr.h"\n\nvoid main() {\n	//code goes here\n	printf("Hello, world!");\n	return;\n}' > kernel.c
	touch null.asm
	touch kerntry.asm
	printf 'times 10240 db 0' > null.asm
	nasm -f bin -o null.bin null.asm
	printf '[bits 32]\n[extern main]\n[global IDT_load]\n\ncall main\njmp $$\n\nIDT_load:\n    push ebp\n    mov ebp, esp\n\n    mov eax, [ebp + 8]\n    lidt [eax]\n\n    mov esp, ebp\n    pop ebp\n    ret' > kerntry.asm
	nasm -f elf -o kerntry.o kerntry.asm
	rm null.asm
	rm kerntry.asm
	echo Write your code in kernel.c!
	echo And run make compile when done!

clean:
	rm *.bin
	rm *.o
	rm kernel.c
	rm -r iso

compile: fullkern.bin
	cat "boot.bin" "fullkern.bin" > "everything.bin"
	cat "everything.bin" "null.bin" > os.bin
	
fullkern.bin: kernel.o libs libsasm
	i386-elf-ld -o "fullkern.bin" -Ttext 0x7e00 "kerntry.o" $(ASMOBJS) "kernel.o" $(OBJS) --oformat binary

kernel.o: kernel.c $(SRC)
	i386-elf-gcc -ffreestanding -m32 -g -c kernel.c -o "kernel.o"

libs: $(SRC)
	i386-elf-gcc -ffreestanding -m32 -g -c $^

libsasm: $(SRCASM)
	nasm -f elf $^

iso: os.bin
	dd status=noxfer conv=notrunc if=os.bin of=os.flp
	mv os.flp iso
	mkisofs -o os.iso -b os.flp -no-emul-boot iso/
	mv os.iso iso

run:
	qemu-system-x86_64 os.bin

bootloader:
	xxd -r -p bootsamp.txt boot.bin
