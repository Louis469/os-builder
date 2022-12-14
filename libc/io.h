#pragma once

#include "util.h"
#include "ports.h"

#define VIDEO_ADDRESS 0xb8000
#define TOTAL_ROWS 25
#define TOTAL_COLS 80
#define STD_MSG_COLOR 0x0f //black bg, white foreground
#define ERROR_MSG_COLOR 0xf4 //red bg, white background

#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

void clear();
void printf(const char *format, ...);
void del_last_char();
void putchar(char *str);
