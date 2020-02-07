#ifndef EMULATOR_H
#define EMULATOR_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#define CHIP8_DISPLAY_HEIGHT 64
#define CHIP8_DISPLAY_WIDTH 32

/* Window modifer should be equal to how much larger you want the display to be than chip8 display size.
* Currently set at 10 times larger, pixels will scale based on this value
*/
#define WINDOW_MODIFIER 10
#define WINDOW_HEIGHT CHIP8_DISPLAY_HEIGHT * WINDOW_MODIFIER;
#define WINDOW_WIDTH CHIP8_DISPLAY_WIDTH * WINDOW_MODIFIER
#define WINDOW_TITLE "Chip8 Emulator"

int emulate();
#endif