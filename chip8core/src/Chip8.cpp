#include "chip8.h"
#include <algorithm>

Chip8::Chip8()
{
    pc = 0x200;
    i_reg = 0;
    sp = 0;
    delay_timer = 0;
    sound_timer = 0;

    std::fill(ram, ram + RAM_SIZE, 0);
    std::fill(screen, screen + SCREEN_WIDTH * SCREEN_HEIGHT, false);
    std::fill(v_reg, v_reg + REG_SIZE, 0);
    std::fill(stack, stack + STACK_SIZE, 0);
    std::fill(keys, keys + NUM_KEYS, false);
}