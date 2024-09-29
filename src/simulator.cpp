#include "../include/simulator.h"
#include "../include/instruction.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iomanip>

Simulator::Simulator() : pc(0) {}

void Simulator::loadProgram(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    machineCode.clear();
    std::string line;
    while (std::getline(file, line)) {
        // Remove any whitespace from the line
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

        // Convert the hex string to uint32_t
        uint32_t instruction = std::stoul(line, nullptr, 16);
        machineCode.push_back(instruction);
    }

    pc = 0;

    std::cout << "Loaded " << machineCode.size() << " instructions:" << std::endl;
    for (size_t i = 0; i < machineCode.size(); ++i) {
        std::cout << "0x" << std::hex << std::setw(8) << std::setfill('0') << machineCode[i] << std::endl;
    }
}

void Simulator::step() {
    if (pc < machineCode.size()) {
        uint32_t instruction = machineCode[pc];
        std::unique_ptr<Instruction> inst = Instruction::decode(instruction);
        std::cout << "Executed: " << inst->toString() << "; PC = 0x" << std::hex << std::setw(8) << std::setfill('0') << (pc * 4) << std::endl;
        try {
            inst->execute(rf, mem);
            pc++;
        } catch (const std::exception& e) {
            std::cerr << "Error executing instruction: " << e.what() << std::endl;
            std::cerr << "Stopping execution." << std::endl;
            return;
        }
    } else {
        std::cout << "Nothing to step" << std::endl;
    }
}

void Simulator::run() {
    while (pc < machineCode.size()) {
        try {
            step();
        } catch (const std::exception& e) {
            std::cerr << "Error during execution: " << e.what() << std::endl;
            std::cerr << "Stopping execution." << std::endl;
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
        std::cout << "Memory[0x" << std::hex << std::setw(8) << std::setfill('0') << (addr + i) << "] = 0x"
                  << std::setw(2) << static_cast<int>(mem.read64(addr + i) & 0xFF) << std::endl;
    }
    std::cout << std::endl;
}

void Simulator::showStack() {
    for (auto it = callStack.rbegin(); it != callStack.rend(); ++it) {
        std::cout << *it << std::endl;
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

void Simulator::updateCallStack() {
    // Implement logic to update call stack based on function calls and returns
}

bool Simulator::isBreakpoint() {
    return breakpoints.find(currentLine) != breakpoints.end();
}

void Simulator::printExecutedInstruction(const std::string& instruction) {
    std::cout << "Executed " << instruction << "; PC=0x" << std::hex << std::setw(8) << std::setfill('0') << (pc * 4) << std::endl;
}