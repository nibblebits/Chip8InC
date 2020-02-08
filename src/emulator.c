
#include "emulator.h"
#include "simulator.h"
#include "cmemory.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

SDL_Window *window;
SDL_Renderer *renderer;

struct chip8 chip8;

// This is the default sprite set that represents sprites in the CHIP8, seek manual for more information
const char default_sprite_set[] = {0xf0, 0x90, 0x90, 0x90, 0xf0,
                                   0x20, 0x60, 0x20, 0x20, 0x70,
                                   0xf0, 0x10, 0xf0, 0x80, 0xf0,
                                   0xf0, 0x10, 0xf0, 0x10, 0xf0,
                                   0x90, 0x90, 0xf0, 0x10, 0x10,
                                   0xf0, 0x80, 0xf0, 0x10, 0xf0,
                                   0xf0, 0x80, 0xf0, 0x90, 0xf0,
                                   0xf0, 0x10, 0x20, 0x40, 0x40,
                                   0xf0, 0x90, 0xf0, 0x90, 0xf0,
                                   0xf0, 0x90, 0xf0, 0x10, 0xf0,
                                   0xf0, 0x90, 0xf0, 0x90, 0x90,
                                   0xe0, 0x90, 0xe0, 0x90, 0xe0,
                                   0xf0, 0x80, 0x80, 0x80, 0xf0,
                                   0xe0, 0x90, 0x90, 0x90, 0xe0,
                                   0xf0, 0x80, 0xf0, 0x80, 0xf0,
                                   0xf0, 0x80, 0xf0, 0x80, 0x80};

void init_emulator()
{
    //Start SDL
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow(
        WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);

    memset(&chip8, 0, sizeof(chip8));

    // Let's copy in the sprite set
    memcpy(&chip8.memory[0x00], &default_sprite_set, sizeof(default_sprite_set));

    // Chip8 should start running from 0x200

    chip8_set_PC(&chip8.registers, CHIP8_PROGRAM_LOAD_ADDRESS);
    

}

void end_emulator()
{
    SDL_Quit();
}

bool emulator_poll_events()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
        {
            return false;
        }
        break;
        }
    }

    return true;
}

void emulator_render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    SDL_Rect rpixel;
    rpixel.w = WINDOW_PIXEL_MODIFIER;
    rpixel.h = WINDOW_PIXEL_MODIFIER;

    for (int x = 0; x < CHIP8_DISPLAY_WIDTH; x++)
    {
        for (int y = 0; y < CHIP8_DISPLAY_HEIGHT; y++)
        {
            if (!chip8_is_pixel_set(&chip8.screen, x, y))
                continue;

            // Pixel is set let's draw and fill rectangle to screen
            rpixel.x = x * WINDOW_PIXEL_MODIFIER;
            rpixel.y = y * WINDOW_PIXEL_MODIFIER;
            SDL_RenderFillRect(renderer, &rpixel);
        }
    }

    SDL_RenderPresent(renderer);
}

bool emulator_loop()
{
    // Execute the instruction
    unsigned int lpc = chip8.registers.PC;
    unsigned short opcode = chip8_read_short(&chip8, chip8.registers.PC);
    // We must increment the program counter
    chip8_next_PC(&chip8.registers);

    printf("Executing %x val=%x\n", lpc, opcode);
    chip8_exec(&chip8, opcode);

    if (!emulator_poll_events())
        return false;

    emulator_render();

    return true;
}


static void emulator_load_program(char* program, size_t length)
{
    memcpy(&chip8.memory[CHIP8_PROGRAM_LOAD_ADDRESS], program, length);
}



int emulate(char* program, size_t length)
{

    init_emulator();
    emulator_load_program(program, length);
    while (1)
    {
        if (!emulator_loop())
            break;

        // We will sleep here for now but in future use the system time to do this more efficient
        // Some sleep will be neccessary however to avoid overloading CPU
        usleep(20000);
    }

out:
    //Quit SDL
    end_emulator();

    return 0;
}