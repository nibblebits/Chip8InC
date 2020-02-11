#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "config.h"
#include <stdbool.h>

struct chip8_keyboard
{
    char keys[CHIP8_TOTAL_KEYS];
};

void chip8_hold_key(struct chip8_keyboard* keyboard, int key);
void chip8_release_key(struct chip8_keyboard* keyboard, int key);
bool chip8_is_key_down(struct chip8_keyboard* keyboard, int key);
char chip8_get_key_from_map(const char* map, char key_value);

#endif