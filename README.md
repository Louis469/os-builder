# os-builder
Just a 32bit (64 bit coming soon!) os builder inspired by cfenollosa/os-tutorial/

# Features
 - Keyboard
 - Easy compiling with make
 - Disk functions
 - printf();

# Notes
 - **Warning! Do not edit any libraries unless the videos on my channel say to!**

# Getting Started:
 - Run ```make install-deps```. **This takes a very long time on slow computers! Do not cancel the process! The executables will be added to your PATH**
 - Run ```make mk-files```. This will create the files needed to compile and run your code.
 - Run ```make bootloader```. This command generates the bootsector which runs your C code.
 - Add your code in ```kernel.c```. This file is run by the bootsector on startup.
 - Run ```make compile```. This compiles the kernel and the library files and then links them together with ```ld```
 - Run ```make run```. This runs your code with qemu
 - **Optional:** Run ```make iso``` to get an iso file in ```./iso/``` directory
