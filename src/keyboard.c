#include "keyboard.h"

static bool chip8_keyboard_key_in_bounds(int key)
{
    return key >= 0 && key <= CHIP8_TOTAL_KEYS;
}

void chip8_hold_key(struct chip8_keyboard *keyboard, int key)
{
    if (!chip8_keyboard_key_in_bounds(key))
        return;

    keyboard->keys[key] = true;
}

void chip8_release_key(struct chip8_keyboard *keyboard, int key)
{
    if (!chip8_keyboard_key_in_bounds(key))
        return;
    keyboard->keys[key] = false;
}

bool chip8_is_key_down(struct chip8_keyboard *keyboard, int key)
{
    if (!chip8_keyboard_key_in_bounds(key))
        return false;

    return keyboard->keys[key];
}