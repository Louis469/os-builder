#include "ports.h"

unsigned char port_byte_in(unsigned short port)
{
    unsigned char result;

    __asm__("in %%dx, %%al"
            : "=a"(result)
            : "d"(port));
    return result;
}

void port_byte_out(unsigned short port, unsigned char data)
{

    __asm__("out %%al, %%dx"
            :
            : "a"(data), "d"(port));
}

unsigned short port_word_in(unsigned short port)
{
    unsigned short result;
    __asm__("in %%dx, %%ax"
            : "=a"(result)
            : "d"(port));
    return result;
}

void port_word_out(unsigned short port, unsigned short data)
{
    __asm__("out %%ax, %%dx"
            :
            : "a"(data), "d"(port));
}

void port_long_out(uint32_t port, uint32_t value) {
	   __asm__ __volatile__("outl %%eax,%%dx"::"d" (port), "a" (value));
};

uint32_t port_long_in(uint32_t port) {
    uint32_t result;
    __asm__ __volatile__("inl %%dx,%%eax":"=a" (result):"d"(port));
    return result;
}