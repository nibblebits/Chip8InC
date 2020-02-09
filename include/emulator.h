#ifndef EMULATOR_H
#define EMULATOR_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "screen.h"
#include "registers.h"
#include "keyboard.h"
#include "config.h"

typedef char(*WAIT_FOR_KEY_FUNC)();
struct chip8
{
    struct chip8_screen screen;
    struct chip8_registers registers;
    struct chip8_keyboard keyboard;
    char memory[CHIP8_MEMORY_SIZE];

    // Holds return addresses for subroutine calls
    unsigned short stack[CHIP8_STACK_SIZE];

    // This function pointer is invoked when chip8 should wait for a key
    WAIT_FOR_KEY_FUNC wait_for_key_ptr;

};

int emulate();
#endif