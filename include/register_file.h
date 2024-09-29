#pragma once

#include <vector>
#include <cstdint>

class RegisterFile {
public:
    RegisterFile();
    void write(int reg, uint64_t value);
    uint64_t read(int reg) const;
    void printRegs() const;
    uint64_t getPC() const { return pc; }
    void setPC(uint64_t newPC) { pc = newPC; }
    void incrementPC() { pc += 4; }

private:
    std::vector<uint64_t> regs;
    uint64_t pc;
};