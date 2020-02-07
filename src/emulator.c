
#include "emulator.h"
#include <stdio.h>
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
    memcpy(&chip8.memory[0x00], default_sprite_set, sizeof(default_sprite_set));
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
    if (!emulator_poll_events())
        return false;

    emulator_render();
    return true;
}

int emulate()
{

    init_emulator();

    while (1)
    {
        if (!emulator_loop())
            break;
    }

out:
    //Quit SDL
    end_emulator();

    return 0;
}