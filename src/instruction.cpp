#include "../include/instruction.h"
#include <sstream>

std::unique_ptr<Instruction> Instruction::decode(uint32_t machineCode) {
    uint32_t opcode = machineCode & 0x7F;
    uint32_t funct3 = (machineCode >> 12) & 0x7;
    uint32_t funct7 = (machineCode >> 25) & 0x7F;

    switch(opcode) {
        case 0x13: return std::make_unique<ADDI>(machineCode);
        case 0x33: 
            if (funct3 == 0x0 && funct7 == 0x00) return std::make_unique<ADD>(machineCode);
            if (funct3 == 0x0 && funct7 == 0x01) return std::make_unique<MUL>(machineCode);
            break;
        case 0x37: return std::make_unique<LUI>(machineCode);
        case 0x23: 
            if (funct3 == 0x3) return std::make_unique<SD>(machineCode);
            if (funct3 == 0x2) return std::make_unique<SW>(machineCode);
            break;
        case 0x63: 
            if (funct3 == 0x0) return std::make_unique<BEQ>(machineCode);
            if (funct3 == 0x4) return std::make_unique<BLT>(machineCode);
            break;
        case 0x67: return std::make_unique<JALR>(machineCode);
        case 0x6F: return std::make_unique<JAL>(machineCode);
        case 0x03: return std::make_unique<LW>(machineCode);
    }
    throw std::runtime_error("Unknown instruction");
}

// ADDI implementation
ADDI::ADDI(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    imm = static_cast<int32_t>(machineCode) >> 20;
}

void ADDI::execute(RegisterFile& rf, Memory& /* mem */) {
    rf.write(rd, rf.read(rs1) + imm);
}

std::string ADDI::toString() const {
    std::stringstream ss;
    ss << "ADDI x" << rd << ", x" << rs1 << ", " << imm;
    return ss.str();
}

// ADD implementation
ADD::ADD(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
}

void ADD::execute(RegisterFile& rf, Memory& /* mem */) {
    rf.write(rd, rf.read(rs1) + rf.read(rs2));
}

std::string ADD::toString() const {
    std::stringstream ss;
    ss << "ADD x" << rd << ", x" << rs1 << ", x" << rs2;
    return ss.str();
}

// LUI implementation
LUI::LUI(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    imm = static_cast<int32_t>(machineCode & 0xFFFFF000);
}

void LUI::execute(RegisterFile& rf, Memory& /* mem */) {
    rf.write(rd, imm);
}

std::string LUI::toString() const {
    std::stringstream ss;
    ss << "LUI x" << rd << ", " << (imm >> 12);
    return ss.str();
}

// SD implementation
SD::SD(uint32_t machineCode) : Instruction(machineCode) {
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
    imm = ((machineCode >> 25) & 0x7F) << 5 | ((machineCode >> 7) & 0x1F);
}

void SD::execute(RegisterFile& rf, Memory& mem) {
    mem.write64(rf.read(rs1) + imm, rf.read(rs2));
}

std::string SD::toString() const {
    std::stringstream ss;
    ss << "SD x" << rs2 << ", " << imm << "(x" << rs1 << ")";
    return ss.str();
}

// JAL implementation
JAL::JAL(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    imm = ((machineCode >> 31) & 0x1) << 20 |
          ((machineCode >> 12) & 0xFF) << 12 |
          ((machineCode >> 20) & 0x1) << 11 |
          ((machineCode >> 21) & 0x3FF) << 1;
    imm = (imm << 11) >> 11; // Sign extend
}

void JAL::execute(RegisterFile& rf, Memory& /* mem */) {
    uint64_t pc = rf.getPC();
    rf.write(rd, pc + 4);
    rf.setPC(pc + imm);
}

std::string JAL::toString() const {
    std::stringstream ss;
    ss << "JAL x" << rd << ", " << imm;
    return ss.str();
}

// BEQ implementation
BEQ::BEQ(uint32_t machineCode) : Instruction(machineCode) {
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
    imm = ((machineCode >> 31) & 0x1) << 12 |
          ((machineCode >> 7) & 0x1) << 11 |
          ((machineCode >> 25) & 0x3F) << 5 |
          ((machineCode >> 8) & 0xF) << 1;
    imm = (imm << 19) >> 19; // Sign extend
}

void BEQ::execute(RegisterFile& rf, Memory& /* mem */) {
    if (rf.read(rs1) == rf.read(rs2)) {
        uint64_t pc = rf.getPC();
        rf.setPC(pc + imm);
    }
}

std::string BEQ::toString() const {
    std::stringstream ss;
    ss << "BEQ x" << rs1 << ", x" << rs2 << ", " << imm;
    return ss.str();
}

// JALR implementation
JALR::JALR(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    imm = static_cast<int32_t>(machineCode) >> 20;
}

void JALR::execute(RegisterFile& rf, Memory& /* mem */) {
    uint64_t pc = rf.getPC();
    uint64_t target = (rf.read(rs1) + imm) & ~1ULL;
    rf.write(rd, pc + 4);
    rf.setPC(target);
}

std::string JALR::toString() const {
    std::stringstream ss;
    ss << "JALR x" << rd << ", x" << rs1 << ", " << imm;
    return ss.str();
}

// MUL implementation
MUL::MUL(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
}

void MUL::execute(RegisterFile& rf, Memory& /* mem */) {
    rf.write(rd, rf.read(rs1) * rf.read(rs2));
}

std::string MUL::toString() const {
    std::stringstream ss;
    ss << "MUL x" << rd << ", x" << rs1 << ", x" << rs2;
    return ss.str();
}

// LW implementation
LW::LW(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    imm = static_cast<int32_t>(machineCode) >> 20;
}

void LW::execute(RegisterFile& rf, Memory& mem) {
    uint64_t addr = rf.read(rs1) + imm;
    uint32_t value = mem.read32(addr);
    rf.write(rd, static_cast<int64_t>(static_cast<int32_t>(value))); // Sign-extend to 64 bits
}

std::string LW::toString() const {
    std::stringstream ss;
    ss << "LW x" << rd << ", " << imm << "(x" << rs1 << ")";
    return ss.str();
}

// SW implementation
SW::SW(uint32_t machineCode) : Instruction(machineCode) {
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
    imm = ((machineCode >> 25) & 0x7F) << 5 | ((machineCode >> 7) & 0x1F);
}

void SW::execute(RegisterFile& rf, Memory& mem) {
    uint64_t addr = rf.read(rs1) + imm;
    uint32_t value = static_cast<uint32_t>(rf.read(rs2));
    mem.write32(addr, value);
}

std::string SW::toString() const {
    std::stringstream ss;
    ss << "SW x" << rs2 << ", " << imm << "(x" << rs1 << ")";
    return ss.str();
}

// BLT implementation
BLT::BLT(uint32_t machineCode) : Instruction(machineCode) {
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
    imm = ((machineCode >> 31) & 0x1) << 12 |
          ((machineCode >> 7) & 0x1) << 11 |
          ((machineCode >> 25) & 0x3F) << 5 |
          ((machineCode >> 8) & 0xF) << 1;
    imm = (imm << 19) >> 19; // Sign extend
}

void BLT::execute(RegisterFile& rf, Memory& /* mem */) {
    if (static_cast<int64_t>(rf.read(rs1)) < static_cast<int64_t>(rf.read(rs2))) {
        uint64_t pc = rf.getPC();
        rf.setPC(pc + imm);
    }
}

std::string BLT::toString() const {
    std::stringstream ss;
    ss << "BLT x" << rs1 << ", x" << rs2 << ", " << imm;
    return ss.str();
}