#include "../include/instruction.h"
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
//     ss << "LB x" << rd << ", " << imm << "(x" << rs1 << ")";
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
//     ss << "LH x" << rd << ", " << imm << "(x" << rs1 << ")";
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
    ss << "LW x" << rd << ", " << imm << "(x" << rs1 << ")";
    return ss.str();
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
    ss << "LD x" << rd << ", " << imm << "(x" << rs1 << ")";
    return ss.str();
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
//     ss << "LBU x" << rd << ", " << imm << "(x" << rs1 << ")";
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
//     ss << "LHU x" << rd << ", " << imm << "(x" << rs1 << ")";
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
    ss << "LWU x" << rd << ", " << imm << "(x" << rs1 << ")";
    return ss.str();
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
    ss << "ADDI x" << rd << ", x" << rs1 << ", " << imm;
    return ss.str();
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
    ss << "SLTI x" << rd << ", x" << rs1 << ", " << imm;
    return ss.str();
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
    ss << "SLTIU x" << rd << ", x" << rs1 << ", " << imm;
    return ss.str();
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
    ss << "XORI x" << rd << ", x" << rs1 << ", " << imm;
    return ss.str();
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
    ss << "ORI x" << rd << ", x" << rs1 << ", " << imm;
    return ss.str();
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
    ss << "ANDI x" << rd << ", x" << rs1 << ", " << imm;
    return ss.str();
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
    ss << "SLLI x" << rd << ", x" << rs1 << ", " << imm;
    return ss.str();
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
    ss << "SRLI x" << rd << ", x" << rs1 << ", " << imm;
    return ss.str();
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
    ss << "SRAI x" << rd << ", x" << rs1 << ", " << imm;
    return ss.str();
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
    ss << "ADDIW x" << rd << ", x" << rs1 << ", " << imm;
    return ss.str();
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
    ss << "SLLIW x" << rd << ", x" << rs1 << ", " << imm;
    return ss.str();
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
    ss << "SRLIW x" << rd << ", x" << rs1 << ", " << imm;
    return ss.str();
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
    ss << "SRAIW x" << rd << ", x" << rs1 << ", " << imm;
    return ss.str();
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
//     ss << "SB x" << rs2 << ", " << imm << "(x" << rs1 << ")";
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
//     ss << "SH x" << rs2 << ", " << imm << "(x" << rs1 << ")";
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
    ss << "SW x" << rs2 << ", " << imm << "(x" << rs1 << ")";
    return ss.str();
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
    ss << "SD x" << rs2 << ", " << imm << "(x" << rs1 << ")";
    return ss.str();
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
    ss << "ADD x" << rd << ", x" << rs1 << ", x" << rs2;
    return ss.str();
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
    ss << "SUB x" << rd << ", x" << rs1 << ", x" << rs2;
    return ss.str();
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
    ss << "SLL x" << rd << ", x" << rs1 << ", x" << rs2;
    return ss.str();
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
    ss << "SLT x" << rd << ", x" << rs1 << ", x" << rs2;
    return ss.str();
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
    ss << "SLTU x" << rd << ", x" << rs1 << ", x" << rs2;
    return ss.str();
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
    ss << "XOR x" << rd << ", x" << rs1 << ", x" << rs2;
    return ss.str();
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
    ss << "SRL x" << rd << ", x" << rs1 << ", x" << rs2;
    return ss.str();
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
    ss << "SRA x" << rd << ", x" << rs1 << ", x" << rs2;
    return ss.str();
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
    ss << "OR x" << rd << ", x" << rs1 << ", x" << rs2;
    return ss.str();
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
    ss << "AND x" << rd << ", x" << rs1 << ", x" << rs2;
    return ss.str();
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
    ss << "ADDW x" << rd << ", x" << rs1 << ", x" << rs2;
    return ss.str();
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
    ss << "SUBW x" << rd << ", x" << rs1 << ", x" << rs2;
    return ss.str();
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
    ss << "SLLW x" << rd << ", x" << rs1 << ", x" << rs2;
    return ss.str();
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
    ss << "SRLW x" << rd << ", x" << rs1 << ", x" << rs2;
    return ss.str();
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
    ss << "SRAW x" << rd << ", x" << rs1 << ", x" << rs2;
    return ss.str();
}

// BRANCH instructions
BEQ::BEQ(uint32_t machineCode) : Instruction(machineCode) {
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
    imm = signExtend(
        ((machineCode >> 31) << 12) |
        ((machineCode >> 7) & 0x1E) |
        ((machineCode >> 20) & 0x7E0) |
        ((machineCode >> 25) & 0x800),
        13
    );
}

void BEQ::execute(RegisterFile& rf, Memory& /* mem */) {
    if (rf.read(rs1) == rf.read(rs2)) {
        rf.write(RegisterFile::PC, rf.read(RegisterFile::PC) + imm - 4);
    }
}

std::string BEQ::toString() const {
    std::stringstream ss;
    ss << "BEQ x" << rs1 << ", x" << rs2 << ", " << imm;
    return ss.str();
}

BNE::BNE(uint32_t machineCode) : Instruction(machineCode) {
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
    imm = signExtend(
        ((machineCode >> 31) << 12) |
        ((machineCode >> 7) & 0x1E) |
        ((machineCode >> 20) & 0x7E0) |
        ((machineCode >> 25) & 0x800),
        13
    );
}

void BNE::execute(RegisterFile& rf, Memory& /* mem */) {
    if (rf.read(rs1) != rf.read(rs2)) {
        rf.write(RegisterFile::PC, rf.read(RegisterFile::PC) + imm - 4);
    }
}

std::string BNE::toString() const {
    std::stringstream ss;
    ss << "BNE x" << rs1 << ", x" << rs2 << ", " << imm;
    return ss.str();
}

BLT::BLT(uint32_t machineCode) : Instruction(machineCode) {
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
    imm = signExtend(
        ((machineCode >> 31) << 12) |
        ((machineCode >> 7) & 0x1E) |
        ((machineCode >> 20) & 0x7E0) |
        ((machineCode >> 25) & 0x800),
        13
    );
}

void BLT::execute(RegisterFile& rf, Memory& /* mem */) {
    if (static_cast<int64_t>(rf.read(rs1)) < static_cast<int64_t>(rf.read(rs2))) {
        rf.write(RegisterFile::PC, rf.read(RegisterFile::PC) + imm - 4);
    }
}

std::string BLT::toString() const {
    std::stringstream ss;
    ss << "BLT x" << rs1 << ", x" << rs2 << ", " << imm;
    return ss.str();
}

BGE::BGE(uint32_t machineCode) : Instruction(machineCode) {
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
    imm = signExtend(
        ((machineCode >> 31) << 12) |
        ((machineCode >> 7) & 0x1E) |
        ((machineCode >> 20) & 0x7E0) |
        ((machineCode >> 25) & 0x800),
        13
    );
}

void BGE::execute(RegisterFile& rf, Memory& /* mem */) {
    if (static_cast<int64_t>(rf.read(rs1)) >= static_cast<int64_t>(rf.read(rs2))) {
        rf.write(RegisterFile::PC, rf.read(RegisterFile::PC) + imm - 4);
    }
}

std::string BGE::toString() const {
    std::stringstream ss;
    ss << "BGE x" << rs1 << ", x" << rs2 << ", " << imm;
    return ss.str();
}

BLTU::BLTU(uint32_t machineCode) : Instruction(machineCode) {
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
    imm = signExtend(
        ((machineCode >> 31) << 12) |
        ((machineCode >> 7) & 0x1E) |
        ((machineCode >> 20) & 0x7E0) |
        ((machineCode >> 25) & 0x800),
        13
    );
}

void BLTU::execute(RegisterFile& rf, Memory& /* mem */) {
    if (rf.read(rs1) < rf.read(rs2)) {
        rf.write(RegisterFile::PC, rf.read(RegisterFile::PC) + imm - 4);
    }
}

std::string BLTU::toString() const {
    std::stringstream ss;
    ss << "BLTU x" << rs1 << ", x" << rs2 << ", " << imm;
    return ss.str();
}

BGEU::BGEU(uint32_t machineCode) : Instruction(machineCode) {
    rs1 = (machineCode >> 15) & 0x1F;
    rs2 = (machineCode >> 20) & 0x1F;
    imm = signExtend(
        ((machineCode >> 31) << 12) |
        ((machineCode >> 7) & 0x1E) |
        ((machineCode >> 20) & 0x7E0) |
        ((machineCode >> 25) & 0x800),
        13
    );
}

void BGEU::execute(RegisterFile& rf, Memory& /* mem */) {
    if (rf.read(rs1) >= rf.read(rs2)) {
        rf.write(RegisterFile::PC, rf.read(RegisterFile::PC) + imm - 4);
    }
}

std::string BGEU::toString() const {
    std::stringstream ss;
    ss << "BGEU x" << rs1 << ", x" << rs2 << ", " << imm;
    return ss.str();
}

// JALR instruction
JALR::JALR(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    rs1 = (machineCode >> 15) & 0x1F;
    imm = signExtend(machineCode >> 20, 12);
}

void JALR::execute(RegisterFile& rf, Memory& /* mem */) {
    uint64_t temp = rf.read(RegisterFile::PC);
    rf.write(RegisterFile::PC, (rf.read(rs1) + imm) & ~1ULL);
    rf.write(rd, temp + 4);
}

std::string JALR::toString() const {
    std::stringstream ss;
    ss << "JALR x" << rd << ", x" << rs1 << ", " << imm;
    return ss.str();
}

// JAL instruction
JAL::JAL(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    imm = signExtend(
        ((machineCode >> 31) << 20) |
        ((machineCode >> 12) & 0xFF000) |
        ((machineCode >> 20) & 0x7FE) |
        ((machineCode >> 21) & 0x800),
        21
    );
}

void JAL::execute(RegisterFile& rf, Memory& /* mem */) {
    rf.write(rd, rf.read(RegisterFile::PC) + 4);
    rf.write(RegisterFile::PC, rf.read(RegisterFile::PC) + imm - 4);
}

std::string JAL::toString() const {
    std::stringstream ss;
    ss << "JAL x" << rd << ", " << imm;
    return ss.str();
}

// LUI instruction
LUI::LUI(uint32_t machineCode) : Instruction(machineCode) {
    rd = (machineCode >> 7) & 0x1F;
    imm = machineCode & 0xFFFFF000;
}

void LUI::execute(RegisterFile& rf, Memory& /* mem */) {
    rf.write(rd, imm);
}

std::string LUI::toString() const {
    std::stringstream ss;
    ss << "LUI x" << rd << ", " << (imm >> 12);
    return ss.str();
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
    ss << "AUIPC x" << rd << ", " << (imm >> 12);
    return ss.str();
}
