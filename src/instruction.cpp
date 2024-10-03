#include "../include/instruction.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cstdint>

// Helper function to sign-extend a value
int64_t signExtend(uint64_t value, int bits) {
    int64_t x = (int64_t)value;
    int64_t m = 1LL << (bits - 1);
    return (x ^ m) - m;
}

std::unique_ptr<Instruction> Instruction::decode(uint32_t machineCode) {
    uint32_t opcode = machineCode & 0x7F;
    uint32_t funct3 = (machineCode >> 12) & 0x7;
    uint32_t funct7 = (machineCode >> 25) & 0x7F;

    switch(opcode) {
        case 0x03: // LOAD
            switch(funct3) {
                //case 0x0: return std::make_unique<LB>(machineCode);
                //case 0x1: return std::make_unique<LH>(machineCode);
                case 0x2: return std::make_unique<LW>(machineCode);
                case 0x3: return std::make_unique<LD>(machineCode);
                //case 0x4: return std::make_unique<LBU>(machineCode);
                //case 0x5: return std::make_unique<LHU>(machineCode);
                case 0x6: return std::make_unique<LWU>(machineCode);
            }
            break;
        case 0x13: // OP-IMM
            switch(funct3) {
                case 0x0: return std::make_unique<ADDI>(machineCode);
                case 0x1: return std::make_unique<SLLI>(machineCode);
                case 0x2: return std::make_unique<SLTI>(machineCode);
                case 0x3: return std::make_unique<SLTIU>(machineCode);
                case 0x4: return std::make_unique<XORI>(machineCode);
                case 0x5: 
                    if (funct7 == 0x00) return std::make_unique<SRLI>(machineCode);
                    if (funct7 == 0x20) return std::make_unique<SRAI>(machineCode);
                    break;
                case 0x6: return std::make_unique<ORI>(machineCode);
                case 0x7: return std::make_unique<ANDI>(machineCode);
            }
            break;
        case 0x17: return std::make_unique<AUIPC>(machineCode);
        case 0x1B: // OP-IMM-32
            switch(funct3) {
                case 0x0: return std::make_unique<ADDIW>(machineCode);
                case 0x1: return std::make_unique<SLLIW>(machineCode);
                case 0x5:
                    if (funct7 == 0x00) return std::make_unique<SRLIW>(machineCode);
                    if (funct7 == 0x20) return std::make_unique<SRAIW>(machineCode);
                    break;
            }
            break;
        case 0x23: // STORE
            switch(funct3) {
                //case 0x0: return std::make_unique<SB>(machineCode);
                //case 0x1: return std::make_unique<SH>(machineCode);
                case 0x2: return std::make_unique<SW>(machineCode);
                case 0x3: return std::make_unique<SD>(machineCode);
            }
            break;
        case 0x33: // OP
            switch(funct3) {
                case 0x0:
                    if (funct7 == 0x00) return std::make_unique<ADD>(machineCode);
                    if (funct7 == 0x20) return std::make_unique<SUB>(machineCode);
                    break;
                case 0x1: return std::make_unique<SLL>(machineCode);
                case 0x2: return std::make_unique<SLT>(machineCode);
                case 0x3: return std::make_unique<SLTU>(machineCode);
                case 0x4: return std::make_unique<XOR>(machineCode);
                case 0x5:
                    if (funct7 == 0x00) return std::make_unique<SRL>(machineCode);
                    if (funct7 == 0x20) return std::make_unique<SRA>(machineCode);
                    break;
                case 0x6: return std::make_unique<OR>(machineCode);
                case 0x7: return std::make_unique<AND>(machineCode);
            }
            break;
        case 0x37: return std::make_unique<LUI>(machineCode);
        case 0x3B: // OP-32
            switch(funct3) {
                case 0x0:
                    if (funct7 == 0x00) return std::make_unique<ADDW>(machineCode);
                    if (funct7 == 0x20) return std::make_unique<SUBW>(machineCode);
                    break;
                case 0x1: return std::make_unique<SLLW>(machineCode);
                case 0x5:
                    if (funct7 == 0x00) return std::make_unique<SRLW>(machineCode);
                    if (funct7 == 0x20) return std::make_unique<SRAW>(machineCode);
                    break;
            }
            break;
        case 0x63: // BRANCH
            switch(funct3) {
                case 0x0: return std::make_unique<BEQ>(machineCode);
                case 0x1: return std::make_unique<BNE>(machineCode);
                case 0x4: return std::make_unique<BLT>(machineCode);
                case 0x5: return std::make_unique<BGE>(machineCode);
                case 0x6: return std::make_unique<BLTU>(machineCode);
                case 0x7: return std::make_unique<BGEU>(machineCode);
            }
            break;
        case 0x67: return std::make_unique<JALR>(machineCode);
        case 0x6F: return std::make_unique<JAL>(machineCode);
    }
    throw std::runtime_error("Unknown instruction");
}

// LOAD instructions
// LB::LB(uint32_t machineCode) : Instruction(machineCode) {
//     rd = (machineCode >> 7) & 0x1F;
//     rs1 = (machineCode >> 15) & 0x1F;
//     imm = signExtend(machineCode >> 20, 12);
// }

// void LB::execute(RegisterFile& rf, Memory& mem) {
//     uint64_t addr = rf.read(rs1) + imm;
//     int8_t value = mem.read8(addr);
//     rf.write(rd, signExtend(value, 8));
// }

// std::string LB::toString() const {
//     std::stringstream ss;
//     ss << "lb x" << rd << ", " << imm << "(x" << rs1 << ")";
//     return ss.str();
// }

// LH::LH(uint32_t machineCode) : Instruction(machineCode) {
//     rd = (machineCode >> 7) & 0x1F;
//     rs1 = (machineCode >> 15) & 0x1F;
//     imm = signExtend(machineCode >> 20, 12);
// }

// void LH::execute(RegisterFile& rf, Memory& mem) {
//     uint64_t addr = rf.read(rs1) + imm;
//     int16_t value = mem.read16(addr);
//     rf.write(rd, signExtend(value, 16));
// }

// std::string LH::toString() const {
//     std::stringstream ss;
//     ss << "lh x" << rd << ", " << imm << "(x" << rs1 << ")";
//     return ss.str();
// }

LW::LW(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    imm = signExtend(machineCode >> 20, 12);
}

void LW::execute(RegisterFile& rf, Memory& mem) {
    uint64_t addr = rf.read(rs1) + imm;
    int32_t value = mem.read32(addr);
    rf.write(rd, signExtend(value, 32));
}

std::string LW::toString() const {
    std::stringstream ss;
    ss << "lw x" << rd << ", " << imm << "(x" << rs1 << ")";
    return ss.str();
}

bool LW::isJump() const {
    return false; // BEQ is a jump instruction
}

uint64_t LW::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // Return the immediate value as the jump address
}

LD::LD(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    imm = signExtend(machineCode >> 20, 12);
}

void LD::execute(RegisterFile& rf, Memory& mem) {
    uint64_t addr = rf.read(rs1) + imm;
    int64_t value = mem.read64(addr);
    rf.write(rd, value);
}

std::string LD::toString() const {
    std::stringstream ss;
    ss << "ld x" << rd << ", " << imm << "(x" << rs1 << ")";
    return ss.str();
}

bool LD::isJump() const {
    return false; // BEQ is a jump instruction
}

uint64_t LD::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // Return the immediate value as the jump address
}

// LBU::LBU(uint32_t machineCode) : Instruction(machineCode) {
//     rd = (machineCode >> 7) & 0x1F;
//     rs1 = (machineCode >> 15) & 0x1F;
//     imm = signExtend(machineCode >> 20, 12);
// }

// void LBU::execute(RegisterFile& rf, Memory& mem) {
//     uint64_t addr = rf.read(rs1) + imm;
//     uint8_t value = mem.read8(addr);
//     rf.write(rd, value);
// }

// std::string LBU::toString() const {
//     std::stringstream ss;
//     ss << "lbu x" << rd << ", " << imm << "(x" << rs1 << ")";
//     return ss.str();
// }

// LHU::LHU(uint32_t machineCode) : Instruction(machineCode) {
//     rd = (machineCode >> 7) & 0x1F;
//     rs1 = (machineCode >> 15) & 0x1F;
//     imm = signExtend(machineCode >> 20, 12);
// }

// void LHU::execute(RegisterFile& rf, Memory& mem) {
//     uint64_t addr = rf.read(rs1) + imm;
//     uint16_t value = mem.read16(addr);
//     rf.write(rd, value);
// }

// std::string LHU::toString() const {
//     std::stringstream ss;
//     ss << "lhu x" << rd << ", " << imm << "(x" << rs1 << ")";
//     return ss.str();
// }

LWU::LWU(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    imm = signExtend(machineCode >> 20, 12);
}

void LWU::execute(RegisterFile& rf, Memory& mem) {
    uint64_t addr = rf.read(rs1) + imm;
    uint32_t value = mem.read32(addr);
    rf.write(rd, value);
}

std::string LWU::toString() const {
    std::stringstream ss;
    ss << "lwu x" << rd << ", " << imm << "(x" << rs1 << ")";
    return ss.str();
}

bool LWU::isJump() const {
    return false; // BEQ is a jump instruction
}

uint64_t LWU::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // Return the immediate value as the jump address
}

// OP-IMM instructions
ADDI::ADDI(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    imm = signExtend(machineCode >> 20, 12);
}

void ADDI::execute(RegisterFile& rf, Memory& /* mem */) {
    rf.write(rd, rf.read(rs1) + imm);
}

std::string ADDI::toString() const {
    std::stringstream ss;
    ss << "addi x" << rd << ", x" << rs1 << ", " << imm;
    return ss.str();
}
bool ADDI::isJump() const {
    return false; // BEQ is a jump instruction
}

uint64_t ADDI::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // Return the immediate value as the jump address
}

SLTI::SLTI(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    imm = signExtend(machineCode >> 20, 12);
}

void SLTI::execute(RegisterFile& rf, Memory& /* mem */) {
    rf.write(rd, (static_cast<int64_t>(rf.read(rs1)) < static_cast<int64_t>(imm)) ? 1 : 0);
}

std::string SLTI::toString() const {
    std::stringstream ss;
    ss << "slti x" << rd << ", x" << rs1 << ", " << imm;
    return ss.str();
}

bool SLTI::isJump() const {
    return false; // SLTI is not a jump instruction
}

uint64_t SLTI::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // No jump address
}

SLTIU::SLTIU(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    imm = signExtend(machineCode >> 20, 12);
}

void SLTIU::execute(RegisterFile& rf, Memory& /* mem */) {
    rf.write(rd, (rf.read(rs1) < static_cast<uint64_t>(imm)) ? 1 : 0);
}

std::string SLTIU::toString() const {
    std::stringstream ss;
    ss << "sltiu x" << rd << ", x" << rs1 << ", " << imm;
    return ss.str();
}

bool SLTIU::isJump() const {
    return false; // SLTIU is not a jump instruction
}

uint64_t SLTIU::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // No jump address
}

XORI::XORI(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    imm = signExtend(machineCode >> 20, 12);
}

void XORI::execute(RegisterFile& rf, Memory& /* mem */) {
    rf.write(rd, rf.read(rs1) ^ imm);
}


std::string XORI::toString() const {
    std::stringstream ss;
    ss << "xori x" << rd << ", x" << rs1 << ", " << imm;
    return ss.str();
}

bool XORI::isJump() const {
    return false; // XORI is not a jump instruction
}

uint64_t XORI::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // No jump address
}

ORI::ORI(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    imm = signExtend(machineCode >> 20, 12);
}

void ORI::execute(RegisterFile& rf, Memory& /* mem */) {
    rf.write(rd, rf.read(rs1) | imm);
}

std::string ORI::toString() const {
    std::stringstream ss;
    ss << "ori x" << rd << ", x" << rs1 << ", " << imm;
    return ss.str();
}

bool ORI::isJump() const {
    return false; // ORI is not a jump instruction
}

uint64_t ORI::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // No jump address
}

ANDI::ANDI(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    imm = signExtend(machineCode >> 20, 12);
}

void ANDI::execute(RegisterFile& rf, Memory& /* mem */) {
    rf.write(rd, rf.read(rs1) & imm);
}

std::string ANDI::toString() const {
    std::stringstream ss;
    ss << "andi x" << rd << ", x" << rs1 << ", " << imm;
    return ss.str();
}

bool ANDI::isJump() const {
    return false; // ANDI is not a jump instruction
}

uint64_t ANDI::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // No jump address
}

SLLI::SLLI(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    imm = (machineCode >> 20) & 0x3F;
}

void SLLI::execute(RegisterFile& rf, Memory& /* mem */) {
    rf.write(rd, rf.read(rs1) << imm);
}

std::string SLLI::toString() const {
    std::stringstream ss;
    ss << "slli x" << rd << ", x" << rs1 << ", " << imm;
    return ss.str();
}

bool SLLI::isJump() const {
    return false; // SLLI is not a jump instruction
}

uint64_t SLLI::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // No jump address
}

SRLI::SRLI(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    imm = (machineCode >> 20) & 0x3F;
}

void SRLI::execute(RegisterFile& rf, Memory& /* mem */) {
    rf.write(rd, rf.read(rs1) >> imm);
}

std::string SRLI::toString() const {
    std::stringstream ss;
    ss << "srli x" << rd << ", x" << rs1 << ", " << imm;
    return ss.str();
}

bool SRLI::isJump() const {
    return false; // SRLI is not a jump instruction
}

uint64_t SRLI::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // No jump address
}

SRAI::SRAI(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    imm = (machineCode >> 20) & 0x3F;
}

void SRAI::execute(RegisterFile& rf, Memory& /* mem */) {
    rf.write(rd, static_cast<int64_t>(rf.read(rs1)) >> imm);
}

std::string SRAI::toString() const {
    std::stringstream ss;
    ss << "srai x" << rd << ", x" << rs1 << ", " << imm;
    return ss.str();
}

bool SRAI::isJump() const {
    return false; // SRAI is not a jump instruction
}

uint64_t SRAI::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // No jump address
}

// OP-IMM-32 instructions
ADDIW::ADDIW(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    imm = signExtend(machineCode >> 20, 12);
}

void ADDIW::execute(RegisterFile& rf, Memory& /* mem */) {
    int32_t result = static_cast<int32_t>(rf.read(rs1)) + static_cast<int32_t>(imm);
    rf.write(rd, signExtend(result, 32));
}

std::string ADDIW::toString() const {
    std::stringstream ss;
    ss << "addiw x" << rd << ", x" << rs1 << ", " << imm;
    return ss.str();
}

bool ADDIW::isJump() const {
    return false; // LUI is not a jump instruction
}

uint64_t ADDIW::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // No jump address
}


SLLIW::SLLIW(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    imm = (machineCode >> 20) & 0x1F;
}

void SLLIW::execute(RegisterFile& rf, Memory& /* mem */) {
    uint32_t result = static_cast<uint32_t>(rf.read(rs1)) << imm;
    rf.write(rd, signExtend(result, 32));
}

std::string SLLIW::toString() const {
    std::stringstream ss;
    ss << "slliw x" << rd << ", x" << rs1 << ", " << imm;
    return ss.str();
}

bool SLLIW::isJump() const {
    return false; // LUI is not a jump instruction
}

uint64_t SLLIW::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // No jump address
}


SRLIW::SRLIW(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    imm = (machineCode >> 20) & 0x1F;
}

void SRLIW::execute(RegisterFile& rf, Memory& /* mem */) {
    uint32_t result = static_cast<uint32_t>(rf.read(rs1)) >> imm;
    rf.write(rd, signExtend(result, 32));
}

std::string SRLIW::toString() const {
    std::stringstream ss;
    ss << "srliw x" << rd << ", x" << rs1 << ", " << imm;
    return ss.str();
}

bool SRLIW::isJump() const {
    return false; // LUI is not a jump instruction
}

uint64_t SRLIW::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // No jump address
}


SRAIW::SRAIW(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    imm = (machineCode >> 20) & 0x1F;
}

void SRAIW::execute(RegisterFile& rf, Memory& /* mem */) {
    int32_t result = static_cast<int32_t>(rf.read(rs1)) >> imm;
    rf.write(rd, signExtend(result, 32));
}

std::string SRAIW::toString() const {
    std::stringstream ss;
    ss << "sraiw x" << rd << ", x" << rs1 << ", " << imm;
    return ss.str();
}

bool SRAIW::isJump() const {
    return false; // LUI is not a jump instruction
}

uint64_t SRAIW::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // No jump address
}

// STORE instructions
// SB::SB(uint32_t machineCode) : Instruction(machineCode) {
//     rs1 = (machineCode >> 15) & 0x1F;
//     rs2 = (machineCode >> 20) & 0x1F;
//     imm = signExtend(((machineCode >> 7) & 0x1F) | ((machineCode >> 25) << 5), 12);
// }

// void SB::execute(RegisterFile& rf, Memory& mem) {
//     uint64_t addr = rf.read(rs1) + imm;
//     mem.write8(addr, rf.read(rs2) & 0xFF);
// }

// std::string SB::toString() const {
//     std::stringstream ss;
//     ss << "sb x" << rs2 << ", " << imm << "(x" << rs1 << ")";
//     return ss.str();
// }

// SH::SH(uint32_t machineCode) : Instruction(machineCode) {
//     rs1 = (machineCode >> 15) & 0x1F;
//     rs2 = (machineCode >> 20) & 0x1F;
//     imm = signExtend(((machineCode >> 7) & 0x1F) | ((machineCode >> 25) << 5), 12);
// }

// void SH::execute(RegisterFile& rf, Memory& mem) {
//     uint64_t addr = rf.read(rs1) + imm;
//     mem.write16(addr, rf.read(rs2) & 0xFFFF);
// }

// std::string SH::toString() const {
//     std::stringstream ss;
//     ss << "sh x" << rs2 << ", " << imm << "(x" << rs1 << ")";
//     return ss.str();
// }

SW::SW(uint32_t machineCode) : Instruction(machineCode) {
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
    imm = signExtend(((machineCode >> 7) & 0x1F) | ((machineCode >> 25) << 5), 12);
}

void SW::execute(RegisterFile& rf, Memory& mem) {
    uint64_t addr = rf.read(rs1) + imm;
    mem.write32(addr, rf.read(rs2) & 0xFFFFFFFF);
}

std::string SW::toString() const {
    std::stringstream ss;
    ss << "sw x" << rs2 << ", " << imm << "(x" << rs1 << ")";
    return ss.str();
}

bool SW::isJump() const {
    return false; // SW is not a jump instruction
}

uint64_t SW::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // No jump address
}

SD::SD(uint32_t machineCode) : Instruction(machineCode) {
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
    imm = signExtend(((machineCode >> 7) & 0x1F) | ((machineCode >> 25) << 5), 12);
}

void SD::execute(RegisterFile& rf, Memory& mem) {
    uint64_t addr = rf.read(rs1) + imm;
    mem.write64(addr, rf.read(rs2));
}

std::string SD::toString() const {
    std::stringstream ss;
    ss << "sd x" << rs2 << ", " << imm << "(x" << rs1 << ")";
    return ss.str();
}

bool SD::isJump() const {
    return false; // SD is not a jump instruction
}

uint64_t SD::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // No jump address
}

// OP instructions
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
    ss << "add x" << rd << ", x" << rs1 << ", x" << rs2;
    return ss.str();
}

bool ADD::isJump() const {
    return false; // ADD is not a jump instruction
}

uint64_t ADD::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // No jump address
}

SUB::SUB(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
}

void SUB::execute(RegisterFile& rf, Memory& /* mem */) {
    rf.write(rd, rf.read(rs1) - rf.read(rs2));
}

std::string SUB::toString() const {
    std::stringstream ss;
    ss << "sub x" << rd << ", x" << rs1 << ", x" << rs2;
    return ss.str();
}

bool SUB::isJump() const {
    return false; // SUB is not a jump instruction
}

uint64_t SUB::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // No jump address
}

SLL::SLL(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
}

void SLL::execute(RegisterFile& rf, Memory& /* mem */) {
    rf.write(rd, rf.read(rs1) << (rf.read(rs2) & 0x3F));
}

std::string SLL::toString() const {
    std::stringstream ss;
    ss << "sll x" << rd << ", x" << rs1 << ", x" << rs2;
    return ss.str();
}

bool SLL::isJump() const {
    return false; // SLL is not a jump instruction
}

uint64_t SLL::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // No jump address
}

SLT::SLT(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
}

void SLT::execute(RegisterFile& rf, Memory& /* mem */) {
    rf.write(rd, (static_cast<int64_t>(rf.read(rs1)) < static_cast<int64_t>(rf.read(rs2))) ? 1 : 0);
}

std::string SLT::toString() const {
    std::stringstream ss;
    ss << "slt x" << rd << ", x" << rs1 << ", x" << rs2;
    return ss.str();
}

bool SLT::isJump() const {
    return false; // SLT is not a jump instruction
}

uint64_t SLT::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // No jump address
}

SLTU::SLTU(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
}

void SLTU::execute(RegisterFile& rf, Memory& /* mem */) {
    rf.write(rd, (rf.read(rs1) < rf.read(rs2)) ? 1 : 0);
}

std::string SLTU::toString() const {
    std::stringstream ss;
    ss << "sltu x" << rd << ", x" << rs1 << ", x" << rs2;
    return ss.str();
}

bool SLTU::isJump() const {
    return false; // SLTU is not a jump instruction
}

uint64_t SLTU::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // No jump address
}


XOR::XOR(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
}

void XOR::execute(RegisterFile& rf, Memory& /* mem */) {
    rf.write(rd, rf.read(rs1) ^ rf.read(rs2));
}

std::string XOR::toString() const {
    std::stringstream ss;
    ss << "xor x" << rd << ", x" << rs1 << ", x" << rs2;
    return ss.str();
}

bool XOR::isJump() const {
    return false; // XOR is not a jump instruction
}

uint64_t XOR::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // No jump address
}

SRL::SRL(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
}

void SRL::execute(RegisterFile& rf, Memory& /* mem */) {
    rf.write(rd, rf.read(rs1) >> (rf.read(rs2) & 0x3F));
}

std::string SRL::toString() const {
    std::stringstream ss;
    ss << "srl x" << rd << ", x" << rs1 << ", x" << rs2;
    return ss.str();
}

bool SRL::isJump() const {
    return false; // SRL is not a jump instruction
}

uint64_t SRL::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // No jump address
}

SRA::SRA(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
}

void SRA::execute(RegisterFile& rf, Memory& /* mem */) {
    rf.write(rd, static_cast<int64_t>(rf.read(rs1)) >> (rf.read(rs2) & 0x3F));
}

std::string SRA::toString() const {
    std::stringstream ss;
    ss << "sra x" << rd << ", x" << rs1 << ", x" << rs2;
    return ss.str();
}

bool SRA::isJump() const {
    return false; // SRA is not a jump instruction
}

uint64_t SRA::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // No jump address
}

OR::OR(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
}

void OR::execute(RegisterFile& rf, Memory& /* mem */) {
    rf.write(rd, rf.read(rs1) | rf.read(rs2));
}

std::string OR::toString() const {
    std::stringstream ss;
    ss << "or x" << rd << ", x" << rs1 << ", x" << rs2;
    return ss.str();
}

bool OR::isJump() const {
    return false; // OR is not a jump instruction
}

uint64_t OR::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // No jump address
}

AND::AND(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
}

void AND::execute(RegisterFile& rf, Memory& /* mem */) {
    rf.write(rd, rf.read(rs1) & rf.read(rs2));
}

std::string AND::toString() const {
    std::stringstream ss;
    ss << "and x" << rd << ", x" << rs1 << ", x" << rs2;
    return ss.str();
}

bool AND::isJump() const {
    return false; // AND is not a jump instruction
}

uint64_t AND::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // No jump address
}

// OP-32 instructions
ADDW::ADDW(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
}

void ADDW::execute(RegisterFile& rf, Memory& /* mem */) {
    int32_t result = static_cast<int32_t>(rf.read(rs1)) + static_cast<int32_t>(rf.read(rs2));
    rf.write(rd, signExtend(result, 32));
}

std::string ADDW::toString() const {
    std::stringstream ss;
    ss << "addw x" << rd << ", x" << rs1 << ", x" << rs2;
    return ss.str();
}

bool ADDW::isJump() const {
    return false; // ADDW is not a jump instruction
}

uint64_t ADDW::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // No jump address
}

SUBW::SUBW(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
}

void SUBW::execute(RegisterFile& rf, Memory& /* mem */) {
    int32_t result = static_cast<int32_t>(rf.read(rs1)) - static_cast<int32_t>(rf.read(rs2));
    rf.write(rd, signExtend(result, 32));
}

std::string SUBW::toString() const {
    std::stringstream ss;
    ss << "subw x" << rd << ", x" << rs1 << ", x" << rs2;
    return ss.str();
}

bool SUBW::isJump() const {
    return false; // SUBW is not a jump instruction
}

uint64_t SUBW::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // No jump address
}

SLLW::SLLW(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
}

void SLLW::execute(RegisterFile& rf, Memory& /* mem */) {
    uint32_t result = static_cast<uint32_t>(rf.read(rs1)) << (rf.read(rs2) & 0x1F);
    rf.write(rd, signExtend(result, 32));
}

std::string SLLW::toString() const {
    std::stringstream ss;
    ss << "sllw x" << rd << ", x" << rs1 << ", x" << rs2;
    return ss.str();
}

bool SLLW::isJump() const {
    return false; // SLLW is not a jump instruction
}

uint64_t SLLW::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // No jump address
}

SRLW::SRLW(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
}

void SRLW::execute(RegisterFile& rf, Memory& /* mem */) {
    uint32_t result = static_cast<uint32_t>(rf.read(rs1)) >> (rf.read(rs2) & 0x1F);
    rf.write(rd, signExtend(result, 32));
}

std::string SRLW::toString() const {
    std::stringstream ss;
    ss << "srlw x" << rd << ", x" << rs1 << ", x" << rs2;
    return ss.str();
}

bool SRLW::isJump() const {
    return false; // SRLW is not a jump instruction
}

uint64_t SRLW::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // No jump address
}

SRAW::SRAW(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
}

void SRAW::execute(RegisterFile& rf, Memory& /* mem */) {
    int32_t result = static_cast<int32_t>(rf.read(rs1)) >> (rf.read(rs2) & 0x1F);
    rf.write(rd, signExtend(result, 32));
}

std::string SRAW::toString() const {
    std::stringstream ss;
    ss << "sraw x" << rd << ", x" << rs1 << ", x" << rs2;
    return ss.str();
}

bool SRAW::isJump() const {
    return false; // SRAW is not a jump instruction
}

uint64_t SRAW::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // No jump address
}

// BRANCH instructions
JAL::JAL(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    
    // Unpack the immediate components
    uint32_t imm20 = (machineCode >> 31) & 0x1;
    uint32_t imm10_1 = (machineCode >> 21) & 0x3FF;
    uint32_t imm11 = (machineCode >> 20) & 0x1;
    uint32_t imm19_12 = (machineCode >> 12) & 0xFF;
    
    // Assemble the immediate
    imm = signExtend(
        (imm20 << 20) |
        (imm19_12 << 12) |
        (imm11 << 11) |
        (imm10_1 << 1),
        21
    );
    
    // Add debug output
    // std::cout << "JAL Immediate components:" << std::endl;
    // std::cout << "  imm20: " << imm20 << std::endl;
    // std::cout << "  imm19_12: 0x" << std::hex << imm19_12 << std::endl;
    // std::cout << "  imm11: " << std::dec << imm11 << std::endl;
    // std::cout << "  imm10_1: 0x" << std::hex << imm10_1 << std::endl;
    // std::cout << "  Final imm: 0x" << std::hex << imm << std::endl;
}

void JAL::execute(RegisterFile& rf, Memory& /* mem */) {
    uint64_t current_pc = rf.read(RegisterFile::PC);
    uint64_t return_address = current_pc + 4;
    uint64_t jump_address = current_pc + imm;

    rf.write(rd, return_address);
    rf.write(RegisterFile::PC, jump_address);

    // std::cout << "JAL Debug:" << std::endl;
    // std::cout << "  Current PC: 0x" << std::hex << current_pc << std::endl;
    // std::cout << "  Return Address: 0x" << std::hex << return_address << std::endl;
    // std::cout << "  Jump Address: 0x" << std::hex << jump_address << std::endl;
    // std::cout << "  Updated PC: 0x" << std::hex << rf.read(RegisterFile::PC) << std::endl;
}

std::string JAL::toString() const {
    std::stringstream ss;
    ss << "jal x" << rd << ", " << (static_cast<int32_t>(imm));
    return ss.str();
}

bool JAL::isJump() const {
    return true; // JAL is a jump instruction
}

uint64_t JAL::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return imm; // Return the immediate value as the jump address
}

// JALR instruction implementation
JALR::JALR(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    imm = signExtend(machineCode >> 20, 12);
}

void JALR::execute(RegisterFile& rf, Memory& /* mem */) {
    uint64_t current_pc = rf.read(RegisterFile::PC);
    uint64_t return_address = current_pc + 4;
    uint64_t jump_address = (rf.read(rs1) + imm) & ~1ULL;  // Clear least significant bit

    rf.write(rd, return_address);
    rf.write(RegisterFile::PC, jump_address);
}

std::string JALR::toString() const {
    std::stringstream ss;
    ss << "jalr x" << rd << ", x" << rs1 << ", " << imm;
    return ss.str();
}

bool JALR::isJump() const {
    return true; // JALR is a jump instruction
}

uint64_t JALR::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return rf.read(rs1) + imm; // Return the calculated address
}

// BEQ instruction implementation
BEQ::BEQ(uint32_t machineCode) : Instruction(machineCode) {
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
    imm = signExtend(
        ((machineCode >> 31) & 0x1) << 12 |
        ((machineCode >> 25) & 0x3F) << 5 |
        ((machineCode >> 8) & 0xF) << 1 |
        ((machineCode >> 7) & 0x1) << 11,
        13
    );
}

void BEQ::execute(RegisterFile& rf, Memory& /* mem */) {
    uint64_t current_pc = rf.read(RegisterFile::PC);
    if (rf.read(rs1) == rf.read(rs2)) {
        rf.write(RegisterFile::PC, current_pc + imm);
    } else {
        rf.write(RegisterFile::PC, current_pc + 4);
    }
}

std::string BEQ::toString() const {
    std::stringstream ss;
    ss << "beq x" << rs1 << ", x" << rs2 << ", " << (static_cast<int32_t>(imm));
    return ss.str();
}

bool BEQ::isJump() const {
    return true; // BEQ is a jump instruction
}

uint64_t BEQ::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return imm; // Return the immediate value as the jump address
}

// BNE instruction implementation
BNE::BNE(uint32_t machineCode) : Instruction(machineCode) {
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
    imm = signExtend(
        ((machineCode >> 31) & 0x1) << 12 |
        ((machineCode >> 25) & 0x3F) << 5 |
        ((machineCode >> 8) & 0xF) << 1 |
        ((machineCode >> 7) & 0x1) << 11,
        13
    );
}

void BNE::execute(RegisterFile& rf, Memory& /* mem */) {
    uint64_t current_pc = rf.read(RegisterFile::PC);
    if (rf.read(rs1) != rf.read(rs2)) {
        rf.write(RegisterFile::PC, current_pc + imm);
    } else {
        rf.write(RegisterFile::PC, current_pc + 4);
    }
}

std::string BNE::toString() const {
    std::stringstream ss;
    ss << "bne x" << rs1 << ", x" << rs2 << ", " << imm;
    return ss.str();
}

bool BNE::isJump() const {
    return true; // BNE is a jump instruction
}

uint64_t BNE::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return imm; // Return the immediate value as the jump address
}

// BLT instruction implementation
BLT::BLT(uint32_t machineCode) : Instruction(machineCode) {
   rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
    imm = signExtend(
        ((machineCode >> 31) & 0x1) << 12 |
        ((machineCode >> 25) & 0x3F) << 5 |
        ((machineCode >> 8) & 0xF) << 1 |
        ((machineCode >> 7) & 0x1) << 11,
        13
    );
}

void BLT::execute(RegisterFile& rf, Memory& /* mem */) {
    uint64_t current_pc = rf.read(RegisterFile::PC);
    if (static_cast<int64_t>(rf.read(rs1)) < static_cast<int64_t>(rf.read(rs2))) {
        rf.write(RegisterFile::PC, current_pc + imm);
    } else {
        rf.write(RegisterFile::PC, current_pc + 4);
    }
}

std::string BLT::toString() const {
    std::stringstream ss;
    ss << "blt x" << rs1 << ", x" << rs2 << ", " << imm;
    return ss.str();
}

bool BLT::isJump() const {
    return true; // BEQ is a jump instruction
}

uint64_t BLT::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return imm; // Return the immediate value as the jump address
}

// BGE instruction implementation
BGE::BGE(uint32_t machineCode) : Instruction(machineCode) {
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
    imm = signExtend(
        ((machineCode >> 31) & 0x1) << 12 |
        ((machineCode >> 25) & 0x3F) << 5 |
        ((machineCode >> 8) & 0xF) << 1 |
        ((machineCode >> 7) & 0x1) << 11,
        13
    );
}

void BGE::execute(RegisterFile& rf, Memory& /* mem */) {
    uint64_t current_pc = rf.read(RegisterFile::PC);
    if (static_cast<int64_t>(rf.read(rs1)) >= static_cast<int64_t>(rf.read(rs2))) {
        rf.write(RegisterFile::PC, current_pc + imm);
    } else {
        rf.write(RegisterFile::PC, current_pc + 4);
    }
}

std::string BGE::toString() const {
    std::stringstream ss;
    ss << "bge x" << rs1 << ", x" << rs2 << ", " << imm;
    return ss.str();
}

bool BGE::isJump() const {
    return true; // BEQ is a jump instruction
}

uint64_t BGE::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return imm; // Return the immediate value as the jump address
}

// BLTU instruction implementation
BLTU::BLTU(uint32_t machineCode) : Instruction(machineCode) {
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
    imm = signExtend(
        ((machineCode >> 31) & 0x1) << 12 |
        ((machineCode >> 25) & 0x3F) << 5 |
        ((machineCode >> 8) & 0xF) << 1 |
        ((machineCode >> 7) & 0x1) << 11,
        13
    );
}

void BLTU::execute(RegisterFile& rf, Memory& /* mem */) {
    uint64_t current_pc = rf.read(RegisterFile::PC);
    if (rf.read(rs1) < rf.read(rs2)) {
        rf.write(RegisterFile::PC, current_pc + imm);
    } else {
        rf.write(RegisterFile::PC, current_pc + 4);
    }
}

std::string BLTU::toString() const {
    std::stringstream ss;
    ss << "bltu x" << rs1 << ", x" << rs2 << ", " << imm;
    return ss.str();
}

bool BLTU::isJump() const {
    return true; // BEQ is a jump instruction
}

uint64_t BLTU::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return imm; // Return the immediate value as the jump address
}

// BGEU instruction implementation
BGEU::BGEU(uint32_t machineCode) : Instruction(machineCode) {
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
    imm = signExtend(
        ((machineCode >> 31) & 0x1) << 12 |
        ((machineCode >> 25) & 0x3F) << 5 |
        ((machineCode >> 8) & 0xF) << 1 |
        ((machineCode >> 7) & 0x1) << 11,
        13
    );
}

void BGEU::execute(RegisterFile& rf, Memory& /* mem */) {
    uint64_t current_pc = rf.read(RegisterFile::PC);
    if (rf.read(rs1) >= rf.read(rs2)) {
        rf.write(RegisterFile::PC, current_pc + imm);
    } else {
        rf.write(RegisterFile::PC, current_pc + 4);
    }
}

std::string BGEU::toString() const {
    std::stringstream ss;
    ss << "bgeu x" << rs1 << ", x" << rs2 << ", " << imm;
    return ss.str();
}

bool BGEU::isJump() const {
    return true; // BEQ is a jump instruction
}

uint64_t BGEU::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return imm; // Return the immediate value as the jump address
}

// LUI instruction
LUI::LUI(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
        imm = (machineCode >> 12) & 0xFFFFF;// Get the upper 20 bits
}

void LUI::execute(RegisterFile& rf, Memory& /* mem */) {
      uint64_t value = static_cast<uint64_t>(imm) << 12;
        rf.write(rd, value);
}

std::string LUI::toString() const {
    std::stringstream ss;
        ss << "lui x" << rd << ", 0x" << std::hex << imm << std::dec;
        return ss.str();
}

bool LUI::isJump() const {
    return false; // LUI is not a jump instruction
}

uint64_t LUI::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // No jump address
}

// AUIPC instruction
AUIPC::AUIPC(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    imm = machineCode & 0xFFFFF000;
}

void AUIPC::execute(RegisterFile& rf, Memory& /* mem */) {
    rf.write(rd, rf.read(RegisterFile::PC) + imm - 4);
}

std::string AUIPC::toString() const {
    std::stringstream ss;
    ss << "auipc x" << rd << ", " << (imm >> 12);
    return ss.str();
}

bool AUIPC::isJump() const {
    return false; // LUI is not a jump instruction
}

uint64_t AUIPC::getJumpAddress(const std::unordered_map<std::string, uint64_t>& /* labels */) const {
    return 0; // No jump address
}
