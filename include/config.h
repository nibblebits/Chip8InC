#ifndef CONFIG_H
#define CONFIG_H


#define CHIP8_DISPLAY_WIDTH 64
#define CHIP8_DISPLAY_HEIGHT 32
#define CHIP8_MEMORY_SIZE 4096
#define CHIP8_TOTAL_REGISTERS 16
#define CHIP8_STACK_SIZE 16
#define CHIP8_CLOCK_RATE_MS 16
/* Window modifer should be equal to how much larger you want the display to be than chip8 display size.
* Currently set at 10 times larger, pixels will scale based on this value
*/
#define WINDOW_PIXEL_MODIFIER 10
#define WINDOW_WIDTH CHIP8_DISPLAY_WIDTH * WINDOW_PIXEL_MODIFIER
#define WINDOW_HEIGHT CHIP8_DISPLAY_HEIGHT * WINDOW_PIXEL_MODIFIER
#define WINDOW_TITLE "Chip8 Emulator"

#endif