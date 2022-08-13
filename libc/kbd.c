#include "kbd.h"

#define BACKSPACE 0x0E
#define ENTER 0x1C
#define LSHIFT 0x2A
#define RSHIFT 0x36

static char key_buffer[256];

char *shellprefix = "Shell$> ";

#define SC_MAX 57
const char *sc_name[] = {"ERROR", "Esc", "1", "2", "3", "4", "5", "6",
                         "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E",
                         "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl",
                         "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`",
                         "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".",
                         "/", "RShift", "Keypad *", "LAlt", "Spacebar"};
const char sc_ascii[] = {'?', '?', '1', '2', '3', '4', '5', '6',
                         '7', '8', '9', '0', '-', '=', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y',
                         'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G',
                         'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V',
                         'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '};

static void keyboard_callback(registers_t regs)
{

    uint8_t scancode = port_byte_in(0x60);

    if (scancode > SC_MAX)
        return;
    if (scancode == BACKSPACE)
    {
        backspace(key_buffer);
        del_last_char();
    }
    else if (scancode == ENTER)
    {
        printf("\n");
        user_input(key_buffer, shellprefix);
        key_buffer[0] = '\0';
    }
    else
    {
        char letter = sc_ascii[(int)scancode];

        char str[2] = {letter, '\0'};
        append(key_buffer, letter);
        printf(str);
    }
    UNUSED(regs);
}

void init_keyboard()
{
    register_interrupt_handler(IRQ1, keyboard_callback);
    printf(shellprefix);
}