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
                default :
                    std::cout<<"Incorrect Opcode in opcode group 0x0XXX, something broke " ;     
                    stop();
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
            skipNextIfNotEquals(opcode);
            break;
        case 0x5000:
            if((opcode & 0x000F)==0)
            skipNextIfEqualsRegister(opcode);
            break;
        case 0x6000:
            setRegister(opcode);
            break;
        case 0x7000:
            incrementRegister(opcode);
            break;
        case 0x8000:
            switch (opcode & 0x000F){
                case 0x0000:
                    setRegisterToRegister(opcode);
                    break;
                case 0x0001:
                case 0x0002:
                case 0x0003:
                    setRegisterBitwise(opcode);
                    break;
                case 0x0004:
                    sumWithCarry(opcode);
                    break;
                case 0x0005:
                    subtractWithBorrow(opcode);
                    break;
                case 0x0006:
                    rightShift(opcode);
                    break;
                case 0x0007:
                    reverseSubtractWithBorrow(opcode);
                    break;
                case 0x000E:
                    leftShift(opcode);
                    break;
                default :
                    std::cout<<"Incorrect Opcode in opcode group 0x8XXX, something broke " ;     
                    stop();
                    break;
            }
            break;
        case 0x9000:
            skipNextIfNotEqualsRegister(opcode);
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
            switch (opcode & 0x00FF){
                case 0x0007:
                    setRegisterToRegister(opcode);
                    break;
                case 0x000A:
                    break;
                case 0x0002:
                    break;
                case 0x0015:
                    break;
                case 0x0018:
                    break;
                case 0x001E:
                    break;
                case 0x0029:
                    break;
                case 0x0033:
                    break;
                case 0x0055:
                    break;
                case 0x0065:
                    break;
                default :
                    std::cout<<"Incorrect Opcode in opcode group 0x8XXX, something broke " ;     
                    stop();
                    break;
            }
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
{       //TODO: test sound in quick successions

    if(!PlaySoundA("../../assets/sound/Recording.wav", NULL, SND_FILENAME | SND_ASYNC))
        {
        std::cerr << "Failed to play sound."<< GetLastError()<<"\n";
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
    uint8_t x = (opcode & 0x0F00) >>8;
    uint8_t nn = opcode & 0x00FF;
    if (v_reg[x]==nn)
        pc+=2;
}
void Chip8::skipNextIfNotEquals(uint16_t opcode){
    //this skips by incrementing pc once here , so that in the next fetch cycle its inc again and we skip one
    uint8_t x = (opcode & 0x0F00)>>8;
    uint8_t nn = opcode & 0x00FF;
    if (v_reg[x]!=nn)
        pc+=2;
} 
void Chip8::skipNextIfEqualsRegister(uint16_t opcode){
    uint8_t x = (opcode & 0x0F00)>>8;
    uint8_t y = (opcode & 0x00F0)>>4;
    if (v_reg[x]==v_reg[y])
        pc+=2;
}
void Chip8::setRegister(uint16_t opcode){
    uint8_t x = (opcode & 0x0F00)>>8;
    uint8_t nn = opcode & 0x00FF;
    v_reg[x]=nn;
}
void Chip8::incrementRegister(uint16_t opcode){
    uint8_t x = (opcode & 0x0F00)>>8;
    uint8_t nn = opcode & 0x00FF;
    v_reg[x]+=nn;
}
void Chip8::setRegisterToRegister(uint16_t opcode){
    uint8_t x = (opcode & 0x0F00)>>8;
    uint8_t y = (opcode & 0x00F0)>>4;
    v_reg[x]=v_reg[y];
}
void Chip8::setRegisterBitwise(uint16_t opcode){
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint8_t op = opcode & 0x000F;

    switch(op){
        case 0x0001: v_reg[x] |= v_reg[y]; break; // OR
        case 0x0002: v_reg[x] &= v_reg[y]; break; // AND
        case 0x0003: v_reg[x] ^= v_reg[y]; break; // XOR
    }
}
void Chip8::sumWithCarry(uint16_t opcode){
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint16_t sum = static_cast<uint16_t>(v_reg[x])+static_cast<uint16_t>(v_reg[y]);
    v_reg[x]=static_cast<uint8_t>(sum);
    v_reg[0xF]=(sum>0xFF)?1:0;
}
void Chip8::subtractWithBorrow(uint16_t opcode){
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    int16_t subtract = static_cast<int16_t>(v_reg[x])-static_cast<int16_t>(v_reg[y]);
    v_reg[x]=static_cast<uint8_t>(subtract);
    v_reg[0xF]=(subtract<0)?0:1;
}
void Chip8::rightShift(uint16_t opcode){
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t last_digit=(v_reg[x] & 0x1);
    v_reg[x]=v_reg[x]>>1;
    v_reg[0xF]=last_digit;
}
void Chip8::reverseSubtractWithBorrow(uint16_t opcode){
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    int16_t subtract = static_cast<int16_t>(v_reg[y])-static_cast<int16_t>(v_reg[x]);
    v_reg[x]=static_cast<uint8_t>(subtract);
    v_reg[0xF]=(subtract<0)?0:1;
}
void Chip8::leftShift(uint16_t opcode){
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t first_digit=(v_reg[x] & 128)?1:0;
    v_reg[x]<<=1;
    v_reg[0xF]=first_digit;
}
void Chip8::skipNextIfNotEqualsRegister(uint16_t opcode){
    uint8_t x = (opcode & 0x0F00)>>8;
    uint8_t y = (opcode & 0x00F0)>>4;
    if (v_reg[x]!=v_reg[y])
        pc+=2;
}
