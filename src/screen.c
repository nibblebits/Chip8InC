#include "screen.h"
#include "emulator.h"
#include <stdio.h>
#include <memory.h>

static inline bool in_screen_bounds(int x, int y)
{
    return x >= 0 && x < CHIP8_DISPLAY_WIDTH && y >= 0 && y < CHIP8_DISPLAY_HEIGHT;
}

void chip8_clear_screen(struct chip8_screen *screen)
{
    memset(screen->pixels, 0, sizeof(screen->pixels));
}

bool chip8_set_pixel(struct chip8_screen *screen, int x, int y)
{
    if (!in_screen_bounds(x, y))
        return false;

    int index = x + (y * CHIP8_DISPLAY_WIDTH);
    screen->pixels[index] = true;
    return true;
}

bool chip8_clear_pixel(struct chip8_screen *screen, int x, int y)
{
    if (!in_screen_bounds(x, y))
        return false;

    int index = x + (y * CHIP8_DISPLAY_WIDTH);
    screen->pixels[index] = false;
    return true;
}

bool chip8_is_pixel_set(struct chip8_screen *screen, int x, int y)
{
    if (!in_screen_bounds(x, y))
        return false;

    int index = x + (y * CHIP8_DISPLAY_WIDTH);
    return screen->pixels[index];
}

bool chip8_draw_sprite(struct chip8 *chip8, int x, int y, int sloc, int n)
{
    bool pixel_hit = false;
    for (int ly = 0; ly < n; ly++)
    {
        char c = chip8->memory[sloc + ly];
        int v = c;
        for (int lx = 0; lx < 8; lx++)
        {
            int real_x_offset = x + lx;
            int real_y_offset = y + ly;
            if (!((c << lx) & 0b10000000))
                continue;


            /*
             * We must return true if a pixel collison has been detected as required by the chip8 specification
             */
            if(chip8_is_pixel_set(&chip8->screen, real_x_offset, real_y_offset))
            {
                pixel_hit = true;
            }

            
            // Pixel is set in sprite lets set the pixel
            chip8_set_pixel(&chip8->screen, real_x_offset, real_y_offset);
        }
    }

    return pixel_hit;
}