#include "test.h"
#include "emulator.h"
#include "simulator.h"
#include "screen.h"
#include "keyboard.h"
#include "registers.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

// This is the default sprite set that represents sprites in the CHIP8, seek manual for more information
const char test_default_sprite_set[] = {
    0xff, 0xff, 0xff, 0xff, 0xff,
    0x20, 0x60, 0x20, 0x20, 0x70,
    0xf0, 0x10, 0xf0, 0x80, 0xf0,
    0xff, 0xff, 0xff, 0xff, 0xff,
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
};

struct chip8 chip8;

char fake_key_press()
{
    return 0x0b;
}

static void test_chip8_init()
{
    memset(&chip8, 0, sizeof(chip8));

    // Let's copy in the sprite set
    memcpy(&chip8.memory[0x00], &test_default_sprite_set, sizeof(test_default_sprite_set));

    // Chip8 should start running from 0x200
    chip8_set_PC(&chip8.registers, CHIP8_PROGRAM_LOAD_ADDRESS);
    chip8.wait_for_key_ptr = &fake_key_press;
}

void test_chip8_test_clear_screen()
{
    chip8_set_pixel(&chip8.screen, 0, 0, true);
    chip8_exec(&chip8, 0x00e0);
    assert(!chip8_is_pixel_set(&chip8.screen, 0, 0));
}

void test_chip8_test_return_from_subroutine()
{
    chip8_set_SP(&chip8.registers, chip8.registers.SP + 1);
    chip8.stack[chip8.registers.SP] = 0xffa0;
    chip8_exec(&chip8, 0x00ee);
    assert(chip8.registers.SP == 0);
    assert(chip8.registers.PC == 0xffa0);
}

void test_chip8_test_jump()
{
    chip8_exec(&chip8, 0x2ff4);
    assert(chip8.registers.PC == 0xff4);
}

void test_chip8_test_call()
{
    unsigned short old_pc = chip8.registers.PC;
    chip8_exec(&chip8, 0x2fa0);
    assert(chip8.registers.PC == 0xfa0);
    assert(chip8.stack[chip8.registers.SP] == old_pc);
}

void test_chip8_test_skip_next_instruction_if_equal()
{
    chip8_set_general_register(&chip8.registers, 0x05, 0x00);
    unsigned short old_pc = chip8.registers.PC;
    chip8_exec(&chip8, 0x35ff);
    assert(chip8.registers.PC == old_pc);

    chip8_set_general_register(&chip8.registers, 0x05, 0x18);
    chip8_exec(&chip8, 0x3518);
    assert(chip8.registers.PC == old_pc + 2);
}

void test_chip8_test_skip_next_instruction_if_not_equal()
{
    chip8_set_general_register(&chip8.registers, 0x00, 0x00);
    unsigned short old_pc = chip8.registers.PC;
    chip8_exec(&chip8, 0x40ff);
    assert(chip8.registers.PC == old_pc + 2);

    old_pc = chip8.registers.PC;
    assert(chip8_get_general_register(&chip8.registers, 0x00) != 0xff);
    chip8_set_general_register(&chip8.registers, 0x00, 0xff);
    chip8_exec(&chip8, 0x40ff);
    assert(chip8.registers.PC == old_pc);
}

void test_chip8_test_skip_next_instruction_if_reg_equal_reg()
{
    unsigned short old_pc = chip8.registers.PC;
    chip8_set_general_register(&chip8.registers, 0x00, 0xf4);
    chip8_set_general_register(&chip8.registers, 0x01, 0xf4);

    chip8_exec(&chip8, 0x5010);
    assert(chip8.registers.PC == old_pc + 2);
}

void test_chip8_test_load_byte_into_reg()
{
    chip8_exec(&chip8, 0x63a1);
    assert(chip8_get_general_register(&chip8.registers, 0x03) == 0xa1);
}

void test_chip8_test_add_reg_with_byte()
{
    chip8_set_general_register(&chip8.registers, 0x00, 0xf0);
    chip8_exec(&chip8, 0x7001);
    assert(chip8_get_general_register(&chip8.registers, 0x00) == 0xf1);
}

void test_chip8_test_set_reg_to_reg()
{
    chip8_set_general_register(&chip8.registers, 0x00, 0xf0);
    chip8_set_general_register(&chip8.registers, 0x01, 0xaa);

    chip8_exec(&chip8, 0x8010);
    assert(chip8_get_general_register(&chip8.registers, 0x00) == 0xaa);
}

void test_chip8_test_or_reg_with_reg()
{
    chip8_set_general_register(&chip8.registers, 0x00, 0xf0);
    chip8_set_general_register(&chip8.registers, 0x01, 0xaa);

    chip8_exec(&chip8, 0x8011);
    assert(chip8_get_general_register(&chip8.registers, 0x00) == (0xaa | 0xf0));
}

void test_chip8_test_and_reg_with_reg()
{
    chip8_set_general_register(&chip8.registers, 0x00, 0xf0);
    chip8_set_general_register(&chip8.registers, 0x01, 0xaa);

    chip8_exec(&chip8, 0x8012);
    assert(chip8_get_general_register(&chip8.registers, 0x00) == (0xaa & 0xf0));
}

void test_chip8_test_xor_reg_with_reg()
{
    chip8_set_general_register(&chip8.registers, 0x00, 0xf0);
    chip8_set_general_register(&chip8.registers, 0x01, 0xaa);

    chip8_exec(&chip8, 0x8013);
    assert(chip8_get_general_register(&chip8.registers, 0x00) == (0xaa ^ 0xf0));
}

void test_chip8_test_add_reg_with_reg()
{
    chip8_set_general_register(&chip8.registers, 0x00, 0xf0);
    chip8_set_general_register(&chip8.registers, 0x01, 0x01);

    chip8_exec(&chip8, 0x8014);
    assert(chip8_get_general_register(&chip8.registers, 0x00) == 0xf1);
    assert(!chip8_get_general_register(&chip8.registers, 0x0f));
}

void test_chip8_test_add_reg_with_reg_test_carry()
{
    chip8_set_general_register(&chip8.registers, 0x00, 254);
    chip8_set_general_register(&chip8.registers, 0x01, 20);

    chip8_exec(&chip8, 0x8014);
    assert(chip8_get_general_register(&chip8.registers, 0x00) == 18);
    assert(chip8_get_general_register(&chip8.registers, 0x0f));
}

void test_chip8_test_sub_reg_with_reg()
{
    chip8_set_general_register(&chip8.registers, 0x00, 0xf0);
    chip8_set_general_register(&chip8.registers, 0x01, 0x01);

    chip8_exec(&chip8, 0x8015);
    assert(chip8_get_general_register(&chip8.registers, 0x00) == 0xef);
    assert(chip8_get_general_register(&chip8.registers, 0x0f));
}

void test_chip8_test_shr()
{
    chip8_set_general_register(&chip8.registers, 0x00, 0xff);

    chip8_exec(&chip8, 0x8016);
    assert(chip8_get_general_register(&chip8.registers, 0x00) & 0b00000001);
}

void test_chip8_test_subn()
{
    chip8_set_general_register(&chip8.registers, 0x00, 0xf0);
    chip8_set_general_register(&chip8.registers, 0x01, 0xff);

    chip8_exec(&chip8, 0x8017);
    assert(chip8_get_general_register(&chip8.registers, 0x00) == 0x0f);
    assert(chip8_get_general_register(&chip8.registers, 0x0f));
}

void test_chip8_test_shl()
{
    chip8_set_general_register(&chip8.registers, 0x00, 0xf0);

    chip8_exec(&chip8, 0x801E);
    assert(chip8_get_general_register(&chip8.registers, 0x00) & 0b10000000);
}

void test_chip8_test_skip_instruction_if_vx_equal_vy()
{
    unsigned short old_pc = chip8.registers.PC;
    chip8_set_general_register(&chip8.registers, 0x00, 0xa1);
    chip8_set_general_register(&chip8.registers, 0x01, 0xa0);
    chip8_exec(&chip8, 0x9010);
    assert(chip8.registers.PC == old_pc + 2);
}

void test_chip8_test_set_I()
{
    chip8_set_I(&chip8.registers, 0x00);
    chip8_exec(&chip8, 0xafff);
    assert(chip8.registers.I == 0xfff);
}

void test_chip8_test_jump_nnn_plus_v_zero()
{
    chip8_set_general_register(&chip8.registers, 0, 0xff);
    chip8_exec(&chip8, 0xba00);
    assert(chip8.registers.PC == 0xa00 + 0xff);
}

void test_chip8_test_random_byte_and_kk()
{
    chip8_set_general_register(&chip8.registers, 0, 0x00);
    chip8_exec(&chip8, 0xc0f0);
    // Bare in mind sometimes zero will be returned from random, this will result in assert failure
    assert(chip8_get_general_register(&chip8.registers, 0) != 0x00);
}

void test_chip8_draw_sprite()
{
    chip8_clear_screen(&chip8.screen);
    chip8_set_general_register(&chip8.registers, 0, 0x00);
    chip8_set_general_register(&chip8.registers, 1, 0x00);
    chip8_set_I(&chip8.registers, 0);

    chip8_exec(&chip8, 0xd005);

    // Character we set has all pixels set, just for this test. Not part of real chip8 character set. Seek test_default_sprite_set
    for (int y = 0; y < 5; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            assert(chip8_is_pixel_set(&chip8.screen, x, y));
        }
    }

    // Running this again should cause an XOR causing all bits to become zero, lets test for that now
    chip8_exec(&chip8, 0xd005);
    for (int y = 0; y < 5; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            assert(!chip8_is_pixel_set(&chip8.screen, x, y));
        }
    }
}

void test_chip8_test_skip_instruction_if_key_pressed()
{
    unsigned short old_pc = chip8.registers.PC;
    chip8_hold_key(&chip8.keyboard, 0x03);
    chip8_set_general_register(&chip8.registers, 5, 0x03);
    chip8_exec(&chip8, 0xE59E);
    assert(chip8.registers.PC == old_pc + 2);
    chip8_release_key(&chip8.keyboard, 0x03);

    old_pc = chip8.registers.PC;
    chip8_set_general_register(&chip8.registers, 5, 0x03);
    chip8_exec(&chip8, 0xE59E);
    assert(chip8.registers.PC == old_pc);
}

void test_chip8_test_skip_instruction_if_key_not_pressed()
{
    unsigned short old_pc = chip8.registers.PC;
    chip8_set_general_register(&chip8.registers, 5, 0x03);
    chip8_exec(&chip8, 0xE5A1);
    assert(chip8.registers.PC == old_pc + 2);

    chip8_hold_key(&chip8.keyboard, 0x03);
    old_pc = chip8.registers.PC;
    chip8_set_general_register(&chip8.registers, 5, 0x03);
    chip8_exec(&chip8, 0xE5A1);
    assert(chip8.registers.PC == old_pc);
    chip8_release_key(&chip8.keyboard, 0x03);
}

void test_chip8_test_set_reg_to_delay_timer()
{
    chip8_set_general_register(&chip8.registers, 0x03, 0x00);
    chip8_set_delay_timer(&chip8.registers, 0xf0);
    chip8_exec(&chip8, 0xf307);
    unsigned char reg = chip8_get_general_register(&chip8.registers, 0x03);
    assert(reg == chip8.registers.delay_timer);
}

void test_chip8_test_wait_for_key_press()
{
    chip8_set_general_register(&chip8.registers, 0x03, 0x00);
    chip8_exec(&chip8, 0xf30a);
    assert(chip8_get_general_register(&chip8.registers, 0x03) == 0x0b);
}

void test_chip8_set_delay_timer()
{
    chip8_set_general_register(&chip8.registers, 0x03, 0xbb);
    chip8_exec(&chip8, 0xf315);
    assert(chip8.registers.delay_timer == 0xbb);
}

void test_chip8_set_sound_timer()
{
    chip8_set_general_register(&chip8.registers, 0x03, 0xab);
    chip8_exec(&chip8, 0xf318);
    assert(chip8.registers.sound_timer == 0xab);
}

void test_chip8_add_i_with_vx()
{
    chip8_set_I(&chip8.registers, 0x0f);
    chip8_set_general_register(&chip8.registers, 0x04, 0x05);
    chip8_exec(&chip8, 0xf41E);
    assert(chip8.registers.I == 0x0f + 0x05);
}

void test_chip8_set_i_to_sprite_at_vx()
{
    chip8_set_general_register(&chip8.registers, 0x00, 0x03);
    chip8_exec(&chip8, 0xf029);
    assert(chip8.registers.I == 0x03 * 5);

    // Ensure I points to correct character in set
    for (int y = 0; y < 5; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            assert(!chip8_is_pixel_set(&chip8.screen, x, y));
        }
    }
}


void test_chip8_test_BCD()
{
    chip8_set_I(&chip8.registers, 0x210);
    chip8_set_general_register(&chip8.registers, 0x00, 187);
    chip8_exec(&chip8, 0xf033);
    assert(chip8.memory[chip8.registers.I] == 1);
    assert(chip8.memory[chip8.registers.I+1] == 8);
    assert(chip8.memory[chip8.registers.I+2] == 7);
}



void test_chip8_test_store_registers()
{
    chip8_set_I(&chip8.registers, 0x210);
    for (int i = 0; i < 0x0f; i++)
    {
        chip8_set_general_register(&chip8.registers, i, i);
    }

    chip8_exec(&chip8, 0xff55);
    for (int i = 0; i < 0x0f; i++)
    {
        assert(chip8.memory[chip8.registers.I+i] == i);
    }

}

void test_chip8_test_read_registers()
{
    chip8_set_I(&chip8.registers, 0x210);
    memset(&chip8.memory[chip8.registers.I], 0xff, 0x0f);


    chip8_exec(&chip8, 0xff65);
    for (int i = 0; i < 0x0f; i++)
    {
        assert(chip8_get_general_register(&chip8.registers, i) == 0xff);
    }

}





void test_chip8_test()
{
    test_chip8_test_clear_screen();
    test_chip8_test_return_from_subroutine();
    test_chip8_test_jump();
    test_chip8_test_call();
    test_chip8_test_skip_next_instruction_if_equal();
    test_chip8_test_skip_next_instruction_if_not_equal();
    test_chip8_test_skip_next_instruction_if_reg_equal_reg();
    test_chip8_test_load_byte_into_reg();
    test_chip8_test_add_reg_with_byte();
    test_chip8_test_set_reg_to_reg();
    test_chip8_test_or_reg_with_reg();
    test_chip8_test_and_reg_with_reg();
    test_chip8_test_xor_reg_with_reg();
    test_chip8_test_add_reg_with_reg();
    test_chip8_test_add_reg_with_reg_test_carry();
    test_chip8_test_sub_reg_with_reg();
    test_chip8_test_shr();
    test_chip8_test_subn();
    test_chip8_test_shl();
    test_chip8_test_skip_instruction_if_vx_equal_vy();
    test_chip8_test_set_I();
    test_chip8_test_jump_nnn_plus_v_zero();
    test_chip8_test_random_byte_and_kk();
    test_chip8_draw_sprite();
    test_chip8_test_skip_instruction_if_key_pressed();
    test_chip8_test_skip_instruction_if_key_not_pressed();
    test_chip8_test_set_reg_to_delay_timer();
    test_chip8_test_wait_for_key_press();
    test_chip8_set_delay_timer();
    test_chip8_set_sound_timer();
    test_chip8_add_i_with_vx();
    test_chip8_set_i_to_sprite_at_vx();
    test_chip8_test_BCD();
    test_chip8_test_store_registers();
    test_chip8_test_read_registers();
}

void test_chip8()
{
    test_chip8_init();
    test_chip8_test();
}