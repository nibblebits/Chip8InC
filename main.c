#include "emulator.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *args[])
{
    if (argc < 2)
    {
        printf("Expected a filename to load for the emulator to run. Only standard chip8 programs are supported\n");
        return -1;
    }

    char *filename = args[1];
    printf("Will emulate %s\n", filename);

    FILE *f = fopen(filename, "r");
    if (!f)
    {
        printf("Failed to open the given filename! Try again\n");
        return -1;
    }

    // Let's read the data into the buffer
    fseek(f, 0, SEEK_END);
    size_t len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char buf[len];

    if (fread(buf, 1, len, f) != len)
    {
        printf("Failed to read the program into memory!\n");
        return -1;
    }

    int res = emulate(&buf, len);
    fclose(f);
    return res;
}