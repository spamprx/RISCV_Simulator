#ifndef REGISTER_FILE_H
#define REGISTER_FILE_H

#include <vector>
#include <cstdint>

class RegisterFile {
public:
    static const int PC = 32;  // Program Counter is treated as the 33rd register

    RegisterFile();
    void write(int reg, uint64_t value);
    uint64_t read(int reg) const;
    void printRegs() const;

private:
    std::vector<uint64_t> regs;
};

#endif // REGISTER_FILE_H