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

bool chip8_set_pixel(struct chip8_screen *screen, int x, int y, bool on)
{
    if (!in_screen_bounds(x, y))
        return false;

    screen->pixels[y][x] = on;
    return true;
}

bool chip8_is_pixel_set(struct chip8_screen *screen, int x, int y)
{
    if (!in_screen_bounds(x, y))
        return false;

    return screen->pixels[y][x];
}

bool chip8_draw_sprite(struct chip8 *chip8, int x, int y, int sloc, int n)
{

#warning SOMETHING IS WRONG WITH THE DRAWING ROUTINE, REGARDING KEEPING TRACK OF HIT COUNTERS FOR PONG

    bool pixel_hit = false;

    for (int ly = 0; ly < n; ly++)
    {
        char c = chip8->memory[sloc + ly];
        for (int lx = 0; lx < 8; lx++)
        {
            if ((c & (0x80 >> lx)) != 0)
            {

                if (chip8->screen.pixels[(y + ly) % CHIP8_DISPLAY_HEIGHT][(x + lx) % CHIP8_DISPLAY_WIDTH])
                {
                    pixel_hit = true;
                }

                chip8->screen.pixels[(y + ly) % CHIP8_DISPLAY_HEIGHT][(x + lx) % CHIP8_DISPLAY_WIDTH] ^= true;
            }
        }
    }

    return pixel_hit;
}