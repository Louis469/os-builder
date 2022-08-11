#pragma once
#include "math.h"
#include "ports.h"
#include "util.h"

#define VIDEO_START (char*)0xb8000
#define VGA_LIGHT_GRAY 7
#define MAX_ROWS 25
#define MAX_COLS 80
#define RED_ON_WHITE 0xf4
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

void printrainbow(char *str);
void clear_screen();
void print_at(char *message, int col, int row);
void print(char *message);
void del_last_char();
