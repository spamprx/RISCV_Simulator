#include "../include/simulator.h"
#include "../include/instruction.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iomanip>


void Simulator::loadProgram(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    machineCode.clear();
    std::string line;
    while (std::getline(file, line)) {
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        uint32_t instruction = std::stoul(line, nullptr, 16);
        machineCode.push_back(instruction);
    }

    pc = 0;
    currentLine = 1;

    std::cout << "Loaded " << machineCode.size() << " instructions:" << std::endl;
    for (size_t i = 0; i < machineCode.size(); ++i) {
        std::cout << "0x" << std::hex << std::setw(8) << std::setfill('0') << machineCode[i] << std::endl;
    }
}

void Simulator::step() {
    if (pc >= machineCode.size()) {
        std::cout << "End of program reached" << std::endl;
        return;
    }

    // Check for breakpoint before executing
    if (isBreakpoint()) {
        std::cout << "Breakpoint hit at line " << currentLine << std::endl;
        return;
    }

    uint32_t instruction = machineCode[pc];
    std::unique_ptr<Instruction> inst = Instruction::decode(instruction);
    
    updateCallStack(instruction);
    
    std::cout << "Executing: " << inst->toString() << "; PC = 0x" << std::hex << std::setw(8) << std::setfill('0') << (pc * 4) << std::endl;
    try {
        inst->execute(rf, mem);
        pc++;
        currentLine++;
        executedInstructions++;
        // Update the line number in the current stack frame
        if (!callStack.empty()) {
            size_t colonPos = callStack.back().find(':');
            if (colonPos != std::string::npos) {
                callStack.back() = callStack.back().substr(0, colonPos + 1) + " " + std::to_string(currentLine);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error executing instruction: " << e.what() << std::endl;
        std::cerr << "Stopping execution." << std::endl;
    }
}

void Simulator::run() {
    while (pc < machineCode.size()) {
        step();
        if (isBreakpoint()) {
            return;
        }
    }
}

void Simulator::printRegs() {
    rf.printRegs();
    std::cout << std::endl;
}

void Simulator::printMem(uint64_t addr, int count) {
    for (int i = 0; i < count; ++i) {
        std::cout << "Memory[0x" << std::hex << std::setw(16) << std::setfill('0') << (addr + i * 4) << "] = 0x"
                  << std::setw(8) << mem.read32(addr + i * 4) << std::endl;
    }
    std::cout << std::endl;
}

void Simulator::showStack() {
    if (callStack.empty()) {
        std::cout << "Call stack is empty." << std::endl;
    } else {
        for (auto it = callStack.rbegin(); it != callStack.rend(); ++it) {
            std::cout << *it << std::endl;
        }
    }
    std::cout << std::endl;
}

void Simulator::setBreakpoint(int line) {
    if (breakpoints.size() < 5) {
        breakpoints[line] = true;
        std::cout << "Breakpoint set at line " << line << std::endl;
    } else {
        std::cout << "Maximum number of breakpoints (5) reached" << std::endl;
    }
    std::cout << std::endl;
}

void Simulator::deleteBreakpoint(int line) {
    if (breakpoints.erase(line) > 0) {
        std::cout << "Breakpoint at line " << line << " deleted" << std::endl;
    } else {
        std::cout << "No breakpoint found at line " << line << std::endl;
    }
    std::cout << std::endl;
}

void Simulator::updateCallStack(uint32_t instruction) {
    uint32_t opcode = instruction & 0x7F;

    if (opcode == 0x6F) { // JAL
        uint32_t rd = (instruction >> 7) & 0x1F;
        if (rd == 1 || rd == 5) {  // Assuming x1 or x5 are used for return address
            callStack.push_back("function_" + std::to_string(pc) + ": " + std::to_string(currentLine));
        }
    } else if (opcode == 0x67) { // JALR
        uint32_t rd = (instruction >> 7) & 0x1F;
        uint32_t rs1 = (instruction >> 15) & 0x1F;
        if (rd == 0 && rs1 == 1) {  // Assuming this is a return instruction
            if (callStack.size() > 1) {  // Keep at least the main function in the stack
                callStack.pop_back();
            }
        } else if (rd == 1 || rd == 5) {  // Assuming this is a function call
            callStack.push_back("function_" + std::to_string(pc) + ": " + std::to_string(currentLine));
        }
    }
}

bool Simulator::isBreakpoint() const {
    return breakpoints.find(currentLine) != breakpoints.end();
}