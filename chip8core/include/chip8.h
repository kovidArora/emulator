#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <cstdint>
#include <random>



#pragma once

class Chip8
{
public:
    Chip8();
    void tick();
    void reset();
    void tick_timers();
    

    static constexpr int REG_SIZE=16;
    static constexpr int SCREEN_WIDTH = 64;
    static constexpr int SCREEN_HEIGHT = 32;
    static constexpr int RAM_SIZE = 4096;
    static constexpr int STACK_SIZE= 16;
    static constexpr int NUM_KEYS=16;



private:
    std::mt19937 rng{std::random_device{}()};

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

    void waitForKeyPress(uint16_t opcode);
    void updateSoundTimer(uint16_t opcode);
    void updateDelayTimer(uint16_t opcode);
    void storeDelayTimer(uint16_t opcode);
    void drawSprite(uint16_t opcode);
    void skipIfKeyPressed(uint16_t opcode);
    void skipIfKeyNotPressed(uint16_t opcode);
    void skipNextIfEquals(uint16_t opcode); 
    void skipNextIfNotEquals(uint16_t opcode); 
    void skipNextIfEqualsRegister(uint16_t opcode);
    void setRegister(uint16_t opcode);
    void incrementRegister(uint16_t opcode);
    void setRegisterToRegister(uint16_t opcode);
    void returnFromSubroutine();
    void jump(uint16_t opcode);
    void subroutine(uint16_t opcode);
    void clear_screen();
    void stop();
    void execute(uint16_t opcode);
    uint16_t fetch();
    void push (uint16_t value );
    uint16_t pop ();
    void playSound();
    void setRegisterBitwise(uint16_t opcode);
    void sumWithCarry(uint16_t opcode);
    void subtractWithBorrow(uint16_t opcode);
    void rightShift(uint16_t opcode);
    void reverseSubtractWithBorrow(uint16_t opcode);
    void leftShift(uint16_t opcode);
    void skipNextIfNotEqualsRegister(uint16_t opcode);
    void updateIRegister(uint16_t opcode);
    void jumpToSum(uint16_t opcode );
    void random(uint16_t opcode);
};