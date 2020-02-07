
#include "emulator.h"
SDL_Window *window;
SDL_Renderer *renderer;


void init_emulator()
{
    //Start SDL
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow(
        WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640,
        320,
        SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);
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