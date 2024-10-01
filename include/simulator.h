#pragma once

#include "register_file.h"
#include "memory.h"
#include "instruction.h"
#include <vector>
#include <map>
#include <string>

class Simulator {
private:
    std::vector<uint32_t> machineCode;
    RegisterFile rf;
    Memory mem;
    uint64_t pc;
    std::map<int, bool> breakpoints;
    std::vector<std::string> callStack;
    int currentLine;
    size_t executedInstructions;

public:
    Simulator() : pc(0), currentLine(1), executedInstructions(0) {
        callStack.push_back("main: 1");
    }
    void loadProgram(const std::string& filename);
    void loadDataSection(const std::string& filename);
    void run();
    void step();
    void printRegs();
    void printMem(uint64_t addr, int count);
    void showStack();
    void setBreakpoint(int line);
    void deleteBreakpoint(int line);
    void updateCallStack(uint32_t instruction);

    bool isBreakpoint() const;
    void printExecutedInstruction(const std::string& instruction);
};