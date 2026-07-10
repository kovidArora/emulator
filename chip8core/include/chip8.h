#pragma once

#include <cstdint>

class Chip8
{
public:
    Chip8();
    void reset();
    static constexpr int REG_SIZE=16;
    static constexpr int SCREEN_WIDTH = 64;
    static constexpr int SCREEN_HEIGHT = 32;
    static constexpr int RAM_SIZE = 4096;
    static constexpr int STACK_SIZE= 16;
    static constexpr int NUM_KEYS=16;


private:
    void push (uint16_t value );
    uint16_t pop ();
    uint16_t pc;
    uint8_t ram[RAM_SIZE];
    bool screen[SCREEN_WIDTH * SCREEN_HEIGHT];
    uint8_t v_reg[REG_SIZE];
    uint16_t i_reg;
    uint16_t stack[STACK_SIZE];
    uint8_t sp = 0;   
    bool keys[NUM_KEYS];
    uint8_t delay_timer;
    uint8_t sound_timer;
 
};