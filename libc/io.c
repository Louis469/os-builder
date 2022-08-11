#include "io.h"

int get_cursor_offset();
void set_cursor_offset(int offset);
int print_char(char c, int col, int row, char attr);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);


void print_top_left(char *str)
{
    unsigned char *video = ((unsigned char *)VIDEO_START);

    while (*str != '\0')
    {
        *(video++) = *str++;
        *(video++) = 0x07;
    }
}

void printrainbow(char *str)
{
    unsigned char *video = ((unsigned char *)VIDEO_START);
    unsigned int color = 0x00;

    while (*str != '\0')
    {
        *(video++) = *str++;
        if (color == swapNibbles(color))
        {
            color += 0x1;
        }
        *(video++) = color;
        if (color == 0xFF)
        {
            color = 0x00;
        }
        else
        {
            color += 0x01;
        }
    }
}

void print_at(char *message, int col, int row)
{
    
    int offset;
    if (col >= 0 && row >= 0)
        offset = get_offset(col, row);
    else
    {
        offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }

    
    int i = 0;
    while (message[i] != 0)
    {
        offset = print_char(message[i++], col, row, VGA_LIGHT_GRAY);
        
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}

void print(char *message)
{
    print_at(message, -1, -1);
}

void del_last_char()
{
    int offset = get_cursor_offset() - 2;
    int row = get_offset_row(offset);
    int col = get_offset_col(offset);
    print_char(0x08, col, row, VGA_LIGHT_GRAY);
}




int print_char(char c, int col, int row, char attr)
{
    uint8_t *vidmem = (uint8_t *)VIDEO_START;
    if (!attr)
        attr = VGA_LIGHT_GRAY;

    
    if (col >= MAX_COLS || row >= MAX_ROWS)
    {
        vidmem[2 * (MAX_COLS) * (MAX_ROWS)-2] = 'E';
        vidmem[2 * (MAX_COLS) * (MAX_ROWS)-1] = RED_ON_WHITE;
        return get_offset(col, row);
    }

    int offset;
    if (col >= 0 && row >= 0)
        offset = get_offset(col, row);
    else
        offset = get_cursor_offset();

    if (c == '\n')
    {
        row = get_offset_row(offset);
        offset = get_offset(0, row + 1);
    }
    else if (c == 0x08)
    { 
        vidmem[offset] = ' ';
        vidmem[offset + 1] = attr;
    }
    else
    {
        vidmem[offset] = c;
        vidmem[offset + 1] = attr;
        offset += 2;
    }

    
    if (offset >= MAX_ROWS * MAX_COLS * 2)
    {
        int i;
        for (i = 1; i < MAX_ROWS; i++)
            memory_copy((uint8_t *)(get_offset(0, i) + VIDEO_START),
                        (uint8_t *)(get_offset(0, i - 1) + VIDEO_START),
                        MAX_COLS * 2);

        
        char *last_line = (char *)(get_offset(0, MAX_ROWS - 1) + (uint8_t *)VIDEO_START);
        for (i = 0; i < MAX_COLS * 2; i++)
            last_line[i] = 0;

        offset -= 2 * MAX_COLS;
    }

    set_cursor_offset(offset);
    return offset;
}
int get_cursor_offset()
{

    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8;
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2;
}

void set_cursor_offset(int offset)
{

    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

void clear_screen()
{
    int screen_size = MAX_COLS * MAX_ROWS;
    int i;
    char *screen = VIDEO_START;

    for (i = 0; i < screen_size; i++)
    {
        screen[i * 2] = ' ';
        screen[i * 2 + 1] = VGA_LIGHT_GRAY;
    }
    set_cursor_offset(get_offset(0, 0));
}

int get_offset(int col, int row) { return 2 * (row * MAX_COLS + col); }
int get_offset_row(int offset) { return offset / (2 * MAX_COLS); }
int get_offset_col(int offset) { return (offset - (get_offset_row(offset) * 2 * MAX_COLS)) / 2; }