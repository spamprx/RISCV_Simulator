#pragma once

#include "register_file.h"
#include "memory.h"
#include "instruction.h"
#include <vector>
#include <map>
#include <string>
#include <unordered_map>

class Simulator {
private:
    std::vector<uint32_t> machineCode;
    RegisterFile rf;
    Memory mem;
    uint64_t pc;
    std::map<int, bool> breakpoints;
    std::vector<int> lineNumbers;
    int currentLine;
    size_t executedInstructions;
    std::unordered_map<std::string, uint64_t> labels;


    struct CallStackFrame {
    std::string functionName;
    int line;
};

    std::vector<CallStackFrame> callStack;
    std::unordered_map<uint64_t, std::string> addressToLabel;
    void scanLabels(const std::string& filename);

public:
    Simulator() : pc(0), currentLine(1), executedInstructions(0) {
        rf.write(RegisterFile::PC, 0);
    }
    void loadProgram(const std::string& filename);
    void loadDataSection(const std::string& filename);
    void run();
    void step();
    void printRegs();
    void printMem(uint64_t addr, int count);
    void showStack() const;
    void setBreakpoint(int line);
    void deleteBreakpoint(int line);
    void updateCallStack(uint32_t instruction);
    void listBreakpoints() const;

    bool isBreakpoint() const;

    void printTextSection() const;
    void printDataSection() const;

    void showHelp() const;

};