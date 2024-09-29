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
private: \
    int rd, rs1, rs2; \
    int32_t imm; \
public: \
    name(uint32_t machineCode); \
    void execute(RegisterFile& rf, Memory& mem) override; \
    std::string toString() const override; \
};

DECLARE_INSTRUCTION(ADDI)
DECLARE_INSTRUCTION(ADD)
DECLARE_INSTRUCTION(LUI)
DECLARE_INSTRUCTION(SD)
DECLARE_INSTRUCTION(JAL)
DECLARE_INSTRUCTION(BEQ)
DECLARE_INSTRUCTION(JALR)
DECLARE_INSTRUCTION(MUL)
DECLARE_INSTRUCTION(LW)
DECLARE_INSTRUCTION(SW)
DECLARE_INSTRUCTION(BLT)

#undef DECLARE_INSTRUCTION