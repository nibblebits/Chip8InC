#ifndef SCREEN_H
#define SCREEN_H
#include "config.h"
#include <stdbool.h>

struct chip8;
struct chip8_screen
{
    bool pixels[CHIP8_DISPLAY_HEIGHT][CHIP8_DISPLAY_WIDTH];
};

void chip8_clear_screen(struct chip8_screen* screen);
bool chip8_set_pixel(struct chip8_screen* screen, int x, int y, bool on);
bool chip8_clear_pixel(struct chip8_screen* screen, int x, int y);
bool chip8_is_pixel_set(struct chip8_screen *screen, int x, int y);

/**
 * Reads "n" number of bytes from the location specified in the "sloc" variable from chip8 memory.
 * Memory read is then drawn to the screen at location "x" and "y". If sprite exceeds screen bounds then it will wrap around 
 * and draw from the other end of the screen. 
 * 
 * \return Returns true if the drawing of these sprites caused a collision with an already set pixel otherwise false. This is not for error handling pixels will still be drawn
 * \param chip8 The chip8 instance
 * \param x The x coordinate to start drawing the pixels
 * \param y The y coordinate to start drawing the pixels
 * \param sloc The location of the sprite in memory
 * \param n The amount of bytes to read
 */

bool chip8_draw_sprite(struct chip8* chip8, int x, int y, int sloc, int n);
#endif