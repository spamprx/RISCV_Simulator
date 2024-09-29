#pragma once

#include <vector>
#include <cstdint>

class RegisterFile {
private:
    std::vector<uint64_t> regs;
    uint64_t pc;

public:
    RegisterFile();
    void write(int reg, uint64_t value);
    uint64_t read(int reg) const;
    void printRegs() const;
    void setPC(uint64_t value) { pc = value; }
    uint64_t getPC() const { return pc; }
};