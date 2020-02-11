
#include "emulator.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

SDL_Window *window;
SDL_Renderer *renderer;

struct chip8 chip8;

const char keymap[CHIP8_TOTAL_KEYS] = {
    SDLK_1, SDLK_2, SDLK_3, SDLK_c,
    SDLK_4, SDLK_5, SDLK_6, SDLK_d,
    SDLK_7, SDLK_8, SDLK_9, SDLK_e,
    SDLK_a, SDLK_0, SDLK_b, SDLK_f};

char handle_key_press_event(SDL_Event *event)
{
    if (event->type != SDL_KEYDOWN)
        return -1;

    char symbol = event->key.keysym.sym;
    char chip8_keyboard_code = chip8_get_key_from_map(keymap, symbol);
    if (chip8_keyboard_code != -1)
    {
        chip8_hold_key(&chip8.keyboard, chip8_keyboard_code);
    }
    return chip8_keyboard_code;
}

char handle_key_event(SDL_Event *event)
{
    switch (event->type)
    {

    case SDL_KEYDOWN:
    {
        return handle_key_press_event(event);
    }
    break;

    case SDL_KEYUP:
    {
        char symbol = event->key.keysym.sym;
        char chip8_keyboard_code = chip8_get_key_from_map(keymap, symbol);
        if (chip8_keyboard_code != -1)
        {
            chip8_release_key(&chip8.keyboard, chip8_keyboard_code);
        }
    }
    break;

    default:
        return -1;
    }
}

char wait_for_key_press()
{

    SDL_Event event;
    char key = -1;
    while (SDL_WaitEvent(&event))
    {
        key = handle_key_press_event(&event);
        if (key != -1)
        {
            return key;
        }
    }
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

        handle_key_event(&event);
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

    chip8_init(&chip8, wait_for_key_press, emulator_render);
}



int emulate(char *program, size_t length, int clock_rate)
{
    init_emulator();
    chip8_load(&chip8, program, length);

    while (1)
    {
        if (!emulator_poll_events())
            break;

        if (!chip8_loop(&chip8))
            break;

        /**
         * We sleep here with this function, a better implementation would be to store current time and check if new time is above
         * 16 miliseconds, however sleep is sufficient for this implementation
         */
        usleep(clock_rate);
    }

out:
    //Quit SDL
    end_emulator();

    return 0;
}