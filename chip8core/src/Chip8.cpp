#include "chip8.h"
#include <algorithm>
#include <iostream>
constexpr int FONTSET_SIZE=80;
constexpr int START_ADDR= 0x200;
const uint8_t FONTSET[FONTSET_SIZE]={
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};
            
Chip8::Chip8()
{
    pc = START_ADDR;
    i_reg = 0;
    sp = 0;
    delay_timer = 0;
    sound_timer = 0;

    std::fill(ram, ram + RAM_SIZE, 0);
    std::fill(screen, screen + SCREEN_WIDTH * SCREEN_HEIGHT, false);
    std::fill(v_reg, v_reg + REG_SIZE, 0);
    std::fill(stack, stack + STACK_SIZE, 0);
    std::fill(keys, keys + NUM_KEYS, false);
    std::copy(FONTSET, FONTSET + FONTSET_SIZE, ram);
}
void Chip8::reset(){
    pc = START_ADDR;
    i_reg = 0;
    sp = 0;
    delay_timer = 0;
    sound_timer = 0;

    std::fill(ram, ram + RAM_SIZE, 0);
    std::fill(screen, screen + SCREEN_WIDTH * SCREEN_HEIGHT, false);
    std::fill(v_reg, v_reg + REG_SIZE, 0);
    std::fill(stack, stack + STACK_SIZE, 0);
    std::fill(keys, keys + NUM_KEYS, false);
    std::copy(FONTSET, FONTSET + FONTSET_SIZE, ram);
}
void Chip8::push(uint16_t value){
    stack[sp]==value;
    sp++;
}
uint16_t Chip8::pop(){
    if(sp==0){
        std::cout<<"stack underflow error from the emulator";
        //TODO: add code to stop emulation later
    }
    sp--;

    return stack[sp];
}