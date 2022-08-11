#include "libc/io.h"
#include "libc/typedefs.h"
#include "libc/isr.h"
#include "libc/timer.h"
#include "libc/kbd.h"

void main() {
	//code goes here
	// print("Hello, world!\n");
	install_isr();

	irq_install();
	return;
}