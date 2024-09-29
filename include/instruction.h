#pragma once

#include "register_file.h"
#include "memory.h"
#include <memory>
#include <string>

class Instruction {
public:
    virtual ~Instruction() = default;
    virtual void execute(RegisterFile& rf, Memory& mem) = 0;
    virtual std::string toString() const = 0;
    static std::unique_ptr<Instruction> decode(uint32_t machineCode);
};

class ADDI : public Instruction {
public:
    ADDI(uint32_t machineCode);
    void execute(RegisterFile& rf, Memory& mem) override;
    std::string toString() const override;
private:
    int rd, rs1, imm;
};

class ADD : public Instruction {
public:
    ADD(uint32_t machineCode);
    void execute(RegisterFile& rf, Memory& mem) override;
    std::string toString() const override;
private:
    int rd, rs1, rs2;
};

class LUI : public Instruction {
public:
    LUI(uint32_t machineCode);
    void execute(RegisterFile& rf, Memory& mem) override;
    std::string toString() const override;
private:
    int rd, imm;
};

class SD : public Instruction {
public:
    SD(uint32_t machineCode);
    void execute(RegisterFile& rf, Memory& mem) override;
    std::string toString() const override;
private:
    int rs1, rs2, imm;
};

class JAL : public Instruction {
public:
    JAL(uint32_t machineCode);
    void execute(RegisterFile& rf, Memory& mem) override;
    std::string toString() const override;
private:
    int rd, imm;
};

class BEQ : public Instruction {
public:
    BEQ(uint32_t machineCode);
    void execute(RegisterFile& rf, Memory& mem) override;
    std::string toString() const override;
private:
    int rs1, rs2, imm;
};

class JALR : public Instruction {
public:
    JALR(uint32_t machineCode);
    void execute(RegisterFile& rf, Memory& mem) override;
    std::string toString() const override;
private:
    int rd, rs1, imm;
};

class MUL : public Instruction {
public:
    MUL(uint32_t machineCode);
    void execute(RegisterFile& rf, Memory& mem) override;
    std::string toString() const override;
private:
    int rd, rs1, rs2;
};

class LW : public Instruction {
public:
    LW(uint32_t machineCode);
    void execute(RegisterFile& rf, Memory& mem) override;
    std::string toString() const override;
private:
    int rd, rs1, imm;
};

class SW : public Instruction {
public:
    SW(uint32_t machineCode);
    void execute(RegisterFile& rf, Memory& mem) override;
    std::string toString() const override;
private:
    int rs1, rs2, imm;
};

class BLT : public Instruction {
public:
    BLT(uint32_t machineCode);
    void execute(RegisterFile& rf, Memory& mem) override;
    std::string toString() const override;
private:
    int rs1, rs2, imm;
};