#pragma once

#include <cstdint>

class Chip8
{
public:
    static constexpr int SCREEN_WIDTH = 64;
    static constexpr int SCREEN_HEIGHT = 32;
    static constexpr int RAM_SIZE = 4096;

private:
    uint16_t pc;
    uint8_t ram[RAM_SIZE];
    bool screen[SCREEN_WIDTH * SCREEN_HEIGHT];
};