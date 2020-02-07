#ifndef SCREEN_H
#define SCREEN_H
#include "config.h"
#include <stdbool.h>
struct chip8_screen
{
    bool pixels[CHIP8_DISPLAY_WIDTH * CHIP8_DISPLAY_HEIGHT];
};

bool chip8_set_pixel(struct chip8_screen* screen, int x, int y);
bool chip8_clear_pixel(struct chip8_screen* screen, int x, int y);
bool chip8_is_pixel_set(struct chip8_screen *screen, int x, int y);

#endif