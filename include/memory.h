#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>

class Memory {
public:
    Memory();
    void write64(uint64_t address, uint64_t value);
    uint64_t read64(uint64_t address) const;
    void write32(uint64_t address, uint32_t value);
    uint32_t read32(uint64_t address) const;

private:
    static const uint64_t MEM_SIZE = 0x100000; // 1 MB
    std::vector<uint8_t> mem;
    bool isValidAddress(uint64_t address) const;
};