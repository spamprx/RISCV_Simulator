#pragma once

#include "instruction.h"
#include "memory.h"
#include "register_file.h"
#include <vector>
#include <map>
#include <string>

class Simulator {
public:
    Simulator();
    void loadProgram(const std::string& filename);
    void step();
    void run();
    void printRegs();
    void printMem(uint64_t addr, int count);
    void showStack();
    void setBreakpoint(int line);
    void deleteBreakpoint(int line);

private:
    std::vector<uint32_t> machineCode;
    RegisterFile rf;
    Memory mem;
    uint64_t pc;
    std::vector<std::string> callStack;
    std::map<int, bool> breakpoints;
    int currentLine;

    void updateCallStack();
    bool isBreakpoint();
    void printExecutedInstruction(const std::string& instruction);
};