#pragma once

#include "register_file.h"
#include "memory.h"
#include <memory>
#include <string>

class Instruction {
protected:
    uint32_t machineCode;

public:
    Instruction(uint32_t mc) : machineCode(mc) {}
    virtual ~Instruction() = default;
    virtual void execute(RegisterFile& rf, Memory& mem) = 0;
    virtual std::string toString() const = 0;
    static std::unique_ptr<Instruction> decode(uint32_t machineCode);
};

#define DECLARE_INSTRUCTION(name) \
class name : public Instruction { \
protected: \
    int rd, rs1, rs2; \
    int64_t imm; \
public: \
    name(uint32_t machineCode); \
    void execute(RegisterFile& rf, Memory& mem) override; \
    std::string toString() const override; \
};

// LOAD instructions
//DECLARE_INSTRUCTION(LB)
//DECLARE_INSTRUCTION(LH)
DECLARE_INSTRUCTION(LW)
DECLARE_INSTRUCTION(LD)
//DECLARE_INSTRUCTION(LBU)
//DECLARE_INSTRUCTION(LHU)
DECLARE_INSTRUCTION(LWU)

// OP-IMM instructions
DECLARE_INSTRUCTION(ADDI)
DECLARE_INSTRUCTION(SLTI)
DECLARE_INSTRUCTION(SLTIU)
DECLARE_INSTRUCTION(XORI)
DECLARE_INSTRUCTION(ORI)
DECLARE_INSTRUCTION(ANDI)
DECLARE_INSTRUCTION(SLLI)
DECLARE_INSTRUCTION(SRLI)
DECLARE_INSTRUCTION(SRAI)

// OP-IMM-32 instructions
DECLARE_INSTRUCTION(ADDIW)
DECLARE_INSTRUCTION(SLLIW)
DECLARE_INSTRUCTION(SRLIW)
DECLARE_INSTRUCTION(SRAIW)

// STORE instructions
//DECLARE_INSTRUCTION(SB)
//DECLARE_INSTRUCTION(SH)
DECLARE_INSTRUCTION(SW)
DECLARE_INSTRUCTION(SD)

// OP instructions
DECLARE_INSTRUCTION(ADD)
DECLARE_INSTRUCTION(SUB)
DECLARE_INSTRUCTION(SLL)
DECLARE_INSTRUCTION(SLT)
DECLARE_INSTRUCTION(SLTU)
DECLARE_INSTRUCTION(XOR)
DECLARE_INSTRUCTION(SRL)
DECLARE_INSTRUCTION(SRA)
DECLARE_INSTRUCTION(OR)
DECLARE_INSTRUCTION(AND)

// OP-32 instructions
DECLARE_INSTRUCTION(ADDW)
DECLARE_INSTRUCTION(SUBW)
DECLARE_INSTRUCTION(SLLW)
DECLARE_INSTRUCTION(SRLW)
DECLARE_INSTRUCTION(SRAW)

// BRANCH instructions
DECLARE_INSTRUCTION(BEQ)
DECLARE_INSTRUCTION(BNE)
DECLARE_INSTRUCTION(BLT)
DECLARE_INSTRUCTION(BGE)
DECLARE_INSTRUCTION(BLTU)
DECLARE_INSTRUCTION(BGEU)

// JALR instruction
DECLARE_INSTRUCTION(JALR)

// JAL instruction
DECLARE_INSTRUCTION(JAL)

// LUI instruction
DECLARE_INSTRUCTION(LUI)

// AUIPC instruction
DECLARE_INSTRUCTION(AUIPC)

#undef DECLARE_INSTRUCTION