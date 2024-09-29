#include "../include/memory.h"

Memory::Memory() : mem(MEM_SIZE, 0) {}

bool Memory::isValidAddress(uint64_t address) const {
    return address < MEM_SIZE;
}

void Memory::write64(uint64_t address, uint64_t value) {
    if (!isValidAddress(address) || !isValidAddress(address + 7)) {
        throw std::out_of_range("Memory write out of bounds");
    }
    for (int i = 0; i < 8; ++i) {
        mem[address + i] = (value >> (i * 8)) & 0xFF;
    }
}

uint64_t Memory::read64(uint64_t address) const {
    if (!isValidAddress(address) || !isValidAddress(address + 7)) {
        throw std::out_of_range("Memory read out of bounds");
    }
    uint64_t value = 0;
    for (int i = 0; i < 8; ++i) {
        value |= static_cast<uint64_t>(mem[address + i]) << (i * 8);
    }
    return value;
}

void Memory::write32(uint64_t address, uint32_t value) {
    if (!isValidAddress(address) || !isValidAddress(address + 3)) {
        throw std::out_of_range("Memory write out of bounds");
    }
    for (int i = 0; i < 4; ++i) {
        mem[address + i] = (value >> (i * 8)) & 0xFF;
    }
}

uint32_t Memory::read32(uint64_t address) const {
    if (!isValidAddress(address) || !isValidAddress(address + 3)) {
        throw std::out_of_range("Memory read out of bounds");
    }
    uint32_t value = 0;
    for (int i = 0; i < 4; ++i) {
        value |= static_cast<uint32_t>(mem[address + i]) << (i * 8);
    }
    return value;
}