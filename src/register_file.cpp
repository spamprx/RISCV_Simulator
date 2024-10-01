#include "../include/register_file.h"
#include <iostream>
#include <iomanip>

RegisterFile::RegisterFile() : regs(33, 0) {}  // 32 general-purpose registers + PC

void RegisterFile::write(int reg, uint64_t value) {
    if (reg != 0) {  // x0 is always 0
        regs[reg] = value;
    }
}

uint64_t RegisterFile::read(int reg) const {
    return regs[reg];
}

void RegisterFile::printRegs() const {
    for (int i = 0; i < 33; ++i) {
        if (i == 32) {
            continue;
        } else {
            std::cout << "x" << std::dec << std::setfill('0') << std::setw(2) << i << " = ";
        }
        if (regs[i] == 0) {
            std::cout << "0x0";
        } else {
            std::cout << "0x" << std::hex << std::setfill('0') << std::setw(16) << regs[i];
        }
        std::cout << std::endl;
    }
}