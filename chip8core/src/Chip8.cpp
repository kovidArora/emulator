#include "chip8.h"
#include <algorithm>
#include <iostream>
#include <cstdlib>
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
            updateIRegister(opcode);
            break;
        case 0xB000:
            jumpToSum(opcode);
            break;
        case 0xC000:
            random(opcode);
            break;
        case 0xD000:
            drawSprite(opcode);
            break;
        case 0xE000:
            switch(opcode & 0x00FF){
                case 0x009E:
                    skipIfKeyPressed(opcode);
                    break;
                case 0x00A1:
                    skipIfKeyNotPressed(opcode);
                    break;
                default :
                    std::cout<<"Incorrect Opcode in opcode group 0xEXXX, something broke " ;     
                    stop();
                    break;
            }
            break;
        case 0xF000:
            switch (opcode & 0x00FF){
                case 0x0007:
                    storeDelayTimer(opcode);
                    break;
                case 0x000A:
                    waitForKeyPress(opcode);
                    break;
                case 0x0015:
                    updateDelayTimer(opcode);
                    break;
                case 0x0018:
                    updateSoundTimer(opcode);
                    break;
                case 0x001E:
                    incrementIRegister(opcode);
                    break;
                case 0x0029:
                    setIToFont(opcode);
                    break;
                case 0x0033:
                    storeBinaryCodedDecimalToI(opcode);
                    break;
                case 0x0055:
                    storeRegisterToRAM(opcode);
                    break;
                case 0x0065:
                    loadRAMtoRegister(opcode);
                    break;
                default :
                    std::cout<<"Incorrect Opcode in opcode group 0xFXXX, something broke " ;     
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
void Chip8::updateIRegister(uint16_t opcode){
    uint16_t nnn = opcode & 0xFFF;
    i_reg=nnn;
}
void Chip8::jumpToSum(uint16_t opcode){
    uint16_t nnn = opcode & 0xFFF;
    pc = v_reg[0] + nnn;
}
void Chip8::random(uint16_t opcode){
    std::uniform_int_distribution<int> dist(0, 255);

    uint8_t r = dist(rng);
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t nn = opcode & 0x00FF;

    v_reg[x] = r & nn;
}
void Chip8::skipIfKeyPressed(uint16_t opcode){
    uint8_t x = (opcode & 0x0F00) >> 8;
    bool key = keys[v_reg[x]];
    if(key)
        pc+=2;
}
void Chip8::skipIfKeyNotPressed(uint16_t opcode){
    uint8_t x = (opcode & 0x0F00) >> 8;
    bool key = keys[v_reg[x]];
    if(!key)
        pc+=2;
}
void Chip8::drawSprite(uint16_t opcode){
    //store the sprite location and height
    uint8_t x = v_reg[(opcode & 0x0F00)>>8];
    uint8_t y = v_reg[(opcode & 0x00F0)>>4];
    uint8_t num_rows = opcode & 0x000F;
    bool flipped = false;
    for(uint16_t row=0; row <num_rows; row++){
        //i_reg stores sprite address in memory , so here we are going row by row , byte by byte of that address
        uint16_t address= i_reg + row;
        uint8_t pixels = ram[address];
        for (uint16_t column =0; column<8;column++){
            //checking each column , each pixel in the row aka each bit
            if(pixels & (0b10000000 >> column)){
                //get the pixel cords 
                uint16_t x_cord= (x+column)%SCREEN_WIDTH;
                uint16_t y_cord= (y+row)%SCREEN_HEIGHT;
                //since screen is stored in a single contiguous array , we get the location of the pixel and check if it has been flipped
                uint16_t idx= x_cord+SCREEN_WIDTH*y_cord;
                flipped  |= screen[idx];
                screen[idx] ^= true;
                //This is the collision case — the sprite is XOR-erasing something that was already there.
            }
        }
    }
     if (flipped) 
        v_reg[0xF] = 1;
    else 
        v_reg[0xF] = 0;
}

void Chip8::waitForKeyPress(uint16_t opcode){
    uint8_t x = (opcode & 0x0F00) >> 8;
    bool pressed = false;
    for (int i =0; i <NUM_KEYS;i++){
        if(keys[i]) {
            v_reg[x] = i ;
            pressed = true;
            break;
        }
    }
    if (!pressed){
        pc-=2;
    }
}
void Chip8::storeDelayTimer(uint16_t opcode){
    uint8_t x = (opcode & 0x0F00) >> 8;
    v_reg[x]=delay_timer;
}
void Chip8::updateDelayTimer(uint16_t opcode){
    uint8_t x = (opcode & 0x0F00) >> 8;
    delay_timer=v_reg[x];
}
void Chip8::updateSoundTimer(uint16_t opcode){
    uint8_t x = (opcode & 0x0F00) >> 8;
    sound_timer=v_reg[x];
}
void Chip8::incrementIRegister(uint16_t opcode){
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint16_t sum= i_reg+ v_reg[x];
    i_reg=sum & 0x0FFF;
}
void Chip8::setIToFont(uint16_t opcode){
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t c = v_reg[x];
    i_reg=c*5;
}
void Chip8::storeBinaryCodedDecimalToI(uint16_t opcode){
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t value = v_reg[x];

    ram[i_reg]     = value / 100;
    ram[i_reg + 1] = (value % 100) / 10;
    ram[i_reg + 2] = value % 10;

}
void Chip8::storeRegisterToRAM(uint16_t opcode){
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint16_t i = i_reg;
    for (size_t idx=0 ; idx <=x ;idx++){
        ram[i+idx]=v_reg[idx];
    }
}
void Chip8::loadRAMtoRegister(uint16_t opcode){
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint16_t i = i_reg;
    for (size_t idx=0 ; idx <=x ;idx++){
        v_reg[idx]=ram[i+idx];
    }
}
const bool* Chip8::get_display() const{
    return screen;
}
void Chip8::keypress(size_t idx , bool pressed){
    keys[idx]=pressed;
}
void Chip8::ROMLoad(std::vector<uint8_t>& data){
    uint16_t start=START_ADDR;
    for (size_t i = 0; i < data.size(); i++){
        ram[start + i] = data[i];
    }

}