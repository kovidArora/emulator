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
void Chip8::tick()
{
    // Fetch
    uint16_t opcode = fetch();

    // Decode // Execute
    execute(opcode);
}
void Chip8::execute(uint16_t opcode){
    

    switch (opcode & 0xF000){
        case 0x0000:
            //clear screen
            switch(opcode ){
                case 0x0000:
                   break;
                case 0x00E0: 
                    clear_screen();
                    break;
                case 0x00EE: 
                    returnFromSubroutine();
                    break;
            }
            break;
        case 0x1000:
            jump(opcode);
            break;
        case 0x2000:
            subroutine(opcode);
            break;
        case 0x3000:
            skipNextIfEquals(opcode);
            break;
        case 0x4000:
            break;
        case 0x5000:
            break;
        case 0x6000:
            break;
        case 0x7000:
            break;
        case 0x8000:
            break;
        case 0x9000:
            break;
        case 0xA000:
            break;
        case 0xB000:
            break;
        case 0xC000:
            break;
        case 0xD000:
            break;
        case 0xE000:
            break;
        case 0xF000:
            break;
        default :
            std::cout<<"Incorrect Opcode , something broke " ;     
             stop();
             break;
    }
}
void Chip8::stop()
{
    std::exit(EXIT_FAILURE);   // or EXIT_SUCCESS
}
uint16_t Chip8::fetch(){
    uint8_t higher_byte= ram[pc];
    uint8_t lower_byte = ram[pc+1];
    uint16_t op = (higher_byte << 8) | lower_byte;
    pc+=2;
    return op;
}
void Chip8::playSound()
{
    if(PlaySound(TEXT("../../assets/sound/Recording.wav"), NULL, SND_FILENAME | SND_ASYNC))
        {
        std::cerr << "Failed to play sound.\n";
        }
    
}
void Chip8::tick_timers(){
    if (delay_timer>0){
        delay_timer-=1;
    }
    if (sound_timer>0){
        if(sound_timer==1){
            playSound();
            std:: cout << "add a beeping sound to your code";
        }
        sound_timer-=1;
    }

}
void Chip8::reset(){
    pc = START_ADDR;
    i_reg = 0;
    sp = 0;
    delay_timer = 0;
    sound_timer = 0;

    std::fill(ram, ram + RAM_SIZE, 0);
    clear_screen();
    std::fill(v_reg, v_reg + REG_SIZE, 0);
    std::fill(stack, stack + STACK_SIZE, 0);
    std::fill(keys, keys + NUM_KEYS, false);
    std::copy(FONTSET, FONTSET + FONTSET_SIZE, ram);
}
void Chip8::push(uint16_t value){
    stack[sp]=value;
    sp++;
}
uint16_t Chip8::pop(){
    if(sp==0){
        std::cout<<"stack underflow error from the emulator";
        stop();   
    }
    sp--;
    return stack[sp];
}
void Chip8::jump(uint16_t opcode){
    pc= opcode&0x0FFF;
}
void Chip8::clear_screen(){
    std::fill(screen, screen + SCREEN_WIDTH * SCREEN_HEIGHT, false);
}
void Chip8::returnFromSubroutine(){
    --sp;
    pc = stack[sp];
}
void Chip8::subroutine(uint16_t opcode){
    stack[sp] = pc;
    ++sp;
    pc = opcode & 0x0FFF;
}
void Chip8::skipNextIfEquals(uint16_t opcode){
    //this skips by incrementing pc once here , so that in the next fetch cycle its inc again and we skip one
    uint8_t x = opcode & 0x0F00 >>8;
    uint16_t nn = opcode & 0x00FF;
    if (v_reg[x]==nn)
        pc+=2;
}; 
void Chip8::skipNextIfNotEquals(uint16_t opcode){
    //this skips by incrementing pc once here , so that in the next fetch cycle its inc again and we skip one
    uint8_t x = opcode & 0x0F00>>8;
    uint16_t nn = opcode & 0x00FF;
    if (v_reg[x]!=nn)
        pc+=2;
}; 
