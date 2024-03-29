#include "io.h"

int CURRENT_CURSOR_POSITION = 0;

void set_cursor_position(int offset);
int get_cursor_position();
int print_char_at(char c, int col, int row, char attr);
int get_current_cursor_row();
int get_current_cursor_column();
int get_offset(int col, int row);
void itoa(uint8_t *buf, uint32_t base, uint32_t d);

void clear()
{
    char *video_memory = (char *)VIDEO_ADDRESS;
    int row = 0;
    for (row = 0; row < TOTAL_COLS * TOTAL_ROWS; row++)
    {
        video_memory[row * 2] = ' ';
        video_memory[row * 2 + 1] = STD_MSG_COLOR;
    }
    CURRENT_CURSOR_POSITION = 0x00;
    set_cursor_position(CURRENT_CURSOR_POSITION);
}

void printf(const char *format, ...)
{
    uint8_t **arg = (uint8_t **)&format;
    uint8_t c;
    uint8_t buf[20];

    arg++;

    while ((c = *format++) != 0)
    {
        if (c != '%')
            putchar(c);
        else
        {
            uint8_t *p, *p2;
            int pad0 = 0, pad = 0;

            c = *format++;
            if (c == '0')
            {
                pad0 = 1;
                c = *format++;
            }

            if (c >= '0' && c <= '9')
            {
                pad = c - '0';
                c = *format++;
            }

            switch (c)
            {
            case 'X':
            case 'd':
            case 'u':
            case 'x':
                itoa(buf, c, *((int *)arg++));
                p = buf;
                goto string;
                break;
            case 's':
                p = *arg++;
                if (!p)
                    p = (uint8_t *)"(null)";
            string:
                for (p2 = p; *p2; p2++)
                    ;
                for (; p2 < p + pad; p2++)
                    putchar(pad0 ? '0' : ' ');
                while (*p)
                    putchar(*p++);
                break;
            default:
                putchar(*((int *)arg++));
                break;
            }
        }
    }
}

void putchar(char *str)
{
    char *video_memory = (char *)VIDEO_ADDRESS;

    int cursorPosition = get_cursor_position();
    int row = get_current_cursor_row(cursorPosition);
    int column = get_current_cursor_column(cursorPosition);

    if (str == '\n')
    {
        cursorPosition = get_offset(0, row + 1);
    }
    else if (str == '\r')
    {
        cursorPosition = get_offset(0, row);
    }
    else if (str == '\t')
    {
        cursorPosition = get_offset(column + 4, row);
    }
    else
    {
        video_memory[cursorPosition] = str;
        video_memory[cursorPosition + 1] = STD_MSG_COLOR;
        cursorPosition = cursorPosition + 2;
    }

    if (cursorPosition >= TOTAL_ROWS * TOTAL_COLS * 2)
    {
        int i;

        for (i = 1; i < TOTAL_ROWS; i++)
        {
            memory_copy(get_offset(0, i) + VIDEO_ADDRESS, get_offset(0, i - 1) + VIDEO_ADDRESS, TOTAL_COLS * 2);
        }

        char *last_line = get_offset(0, TOTAL_ROWS - 1) + VIDEO_ADDRESS;
        for (i = 0; i < TOTAL_COLS * 2; i++)
        {
            last_line[i] = 0;
        }

        cursorPosition = get_offset(0, TOTAL_ROWS - 1);
    }
    set_cursor_position(cursorPosition);
}

int get_cursor_position()
{
    port_byte_out(REG_SCREEN_CTRL, 14);
    int position = port_byte_in(REG_SCREEN_DATA) << 8;

    port_byte_out(REG_SCREEN_CTRL, 15);
    position += port_byte_in(REG_SCREEN_DATA);

    return position * 2;
}

void set_cursor_position(int offset)
{
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, offset >> 8);

    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, offset & 0xff);
}

int get_current_cursor_row(int cursorPosition)
{
    return cursorPosition / (2 * TOTAL_COLS);
}

int get_current_cursor_column(int cursorPosition)
{
    return (cursorPosition - (get_current_cursor_row(cursorPosition) * 2 * TOTAL_COLS)) / 2;
}

int get_offset(int col, int row) { return 2 * (row * TOTAL_COLS + col); }

void itoa(uint8_t *buf, uint32_t base, uint32_t d)
{
    uint8_t *p = buf;
    uint8_t *p1, *p2;
    uint32_t ud = d;
    uint32_t divisor = 10;

    if (base == 'd' && d < 0)
    {
        *p++ = '-';
        buf++;
        ud = -d;
    }
    else if (base == 'x')
        divisor = 16;

    do
    {
        uint32_t remainder = ud % divisor;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    } while (ud /= divisor);

    *p = 0;
    p1 = buf;
    p2 = p - 1;
    while (p1 < p2)
    {
        uint8_t tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
}

void del_last_char()
{
    int offset = get_cursor_position() - 2;
    int row = get_current_cursor_row(offset);
    int col = get_current_cursor_column(offset);
    print_char_at(0x08, col, row, STD_MSG_COLOR);
}

int print_char_at(char c, int col, int row, char attr)
{
    uint8_t *vidmem = (uint8_t *)VIDEO_ADDRESS;
    if (!attr)
        attr = STD_MSG_COLOR;

    if (col >= TOTAL_COLS || row >= TOTAL_ROWS)
    {
        vidmem[2 * (TOTAL_COLS) * (TOTAL_ROWS)-2] = 'E';
        vidmem[2 * (TOTAL_COLS) * (TOTAL_ROWS)-1] = ERROR_MSG_COLOR;
        return get_offset(col, row);
    }

    int offset;
    if (col >= 0 && row >= 0)
        offset = get_offset(col, row);
    else
        offset = get_cursor_position();

    if (c == '\n')
    {
        row = get_current_cursor_row(offset);
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

    if (offset >= TOTAL_ROWS * TOTAL_COLS * 2)
    {
        int i;
        for (i = 1; i < TOTAL_ROWS; i++)
            memory_copy((uint8_t *)(get_offset(0, i) + VIDEO_ADDRESS),
                        (uint8_t *)(get_offset(0, i - 1) + VIDEO_ADDRESS),
                        TOTAL_COLS * 2);

        char *last_line = (char *)(get_offset(0, TOTAL_ROWS - 1) + (uint8_t *)VIDEO_ADDRESS);
        for (i = 0; i < TOTAL_COLS * 2; i++)
            last_line[i] = 0;

        offset -= 2 * TOTAL_COLS;
    }

    set_cursor_position(offset);
    return offset;
}