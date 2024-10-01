#pragma once

#include <vector>
#include <stdexcept>
#include <cstdint>

class Memory {
private:
    std::vector<uint8_t> mem;
    static const uint64_t MEM_SIZE = 0x60000; // Adjust size to include stack

public:
    Memory();
    bool isValidAddress(uint64_t address) const;
    void write64(uint64_t address, uint64_t value);
    uint64_t read64(uint64_t address) const;
    void write32(uint64_t address, uint32_t value);
    uint32_t read32(uint64_t address) const;
    void write16(uint64_t address, uint32_t value);
    uint32_t read16(uint64_t address) const;
    void write8(uint64_t address, uint32_t value);
    uint32_t read8(uint64_t address) const;

    uint64_t getStackPointer() const {
        return 0x50000; // STACK_START
    }
};