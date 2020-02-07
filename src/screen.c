#include "screen.h"

static inline bool in_screen_bounds(int x, int y)
{
    return x >= 0 && x < CHIP8_DISPLAY_WIDTH && y >= 0 && y < CHIP8_DISPLAY_HEIGHT;
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
