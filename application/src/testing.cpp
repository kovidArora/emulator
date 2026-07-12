#include <iostream>
#include <cstdint>

int main()
{
    uint16_t opcode;

    std::cout << "Enter opcode (hex): ";
    std::cin >> std::hex >> opcode;

    uint8_t x = (opcode & 0x0F00)>>8 ;
    uint8_t nn = opcode & 0x00FF;
    uint8_t v_reg[5] = {1,2,3,4,55};

    uint16_t pc = 0x200;
    std::cout <<v_reg[x-1]<<"\n";
    if (v_reg[x-1] == nn)
        pc += 2;
    
    std::cout << x << nn<<"\n";

    std::cout << "PC = 0x" << std::hex << pc << '\n';
}