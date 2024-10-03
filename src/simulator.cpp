#include "../include/simulator.h"
#include "../include/instruction.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <unordered_map>


void Simulator::scanLabels(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::string line;
    uint64_t address = 0;
    while (std::getline(file, line)) {
        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos) {
            std::string label = line.substr(0, colonPos);
            labels[label] = address;
        }
        // Assuming each line corresponds to one instruction
        address += 4;
    }
}

void Simulator::loadProgram(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    machineCode.clear();
    lineNumbers.clear();
    labels.clear();  // Clear any existing labels
    scanLabels(filename); // Scan for labels

    std::string line;
    int lineNum = 1;
    while (std::getline(file, line)) {
        // Check for labels
        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos) {
            std::string label = line.substr(0, colonPos);
            labels[label] = lineNum;
            line = line.substr(colonPos + 1);  // Remove the label from the line
        }

        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        if (!line.empty()) {
            uint32_t instruction = std::stoul(line, nullptr, 16);
            machineCode.push_back(instruction);
            lineNumbers.push_back(lineNum);
        }
        lineNum++;
    }

    pc = 0;
    currentLine = lineNumbers[0];

    // Initialize the call stack with main
    callStack.clear();
    callStack.push_back({"main", currentLine});

    // std::cout << "Loaded " << machineCode.size() << " instructions:" << std::endl;
    // for (size_t i = 0; i < machineCode.size(); ++i) {
    //     std::cout << "0x" << std::hex << std::setw(8) << std::setfill('0') << machineCode[i] << std::endl;
    // }
}

void Simulator::loadDataSection(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    uint64_t address = 0x10000; // Starting address for data section
    bool inDataSection = false;
    std::string line;

    while (std::getline(file, line)) {
        // Remove leading and trailing whitespace
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        if (line.empty() || line[0] == '#' || line[0] == ';') continue; // Skip comments and empty lines

        // Check for label definition
        if (line.back() == ':') {
            std::string label = line.substr(0, line.size() - 1);
            labels[label] = address; // Store the label with its address
            continue;
        }

        std::istringstream iss(line);
        std::string directive;
        iss >> directive;

        if (directive == ".data") {
            inDataSection = true;
            continue;
        } else if (directive == ".text") {
            inDataSection = false;
            break;
        }

        if (!inDataSection) continue;

        std::vector<uint64_t> values;
        std::string valueStr;

        while (std::getline(iss >> std::ws, valueStr, ',')) {
            valueStr.erase(0, valueStr.find_first_not_of(" \t"));
            valueStr.erase(valueStr.find_last_not_of(" \t") + 1);
            
            uint64_t value;
            if (valueStr.substr(0, 2) == "0x") {
                value = std::stoull(valueStr.substr(2), nullptr, 16);
            } else {
                value = std::stoull(valueStr, nullptr, 10);
            }
            values.push_back(value);
        }

        for (uint64_t value : values) {
            if (directive == ".byte") {
                mem.write8(address, static_cast<uint8_t>(value));
                address += 1;
            } else if (directive == ".half" || directive == ".short") {
                mem.write16(address, static_cast<uint16_t>(value));
                address += 2;
            } else if (directive == ".word" || directive == ".long") {
                mem.write32(address, static_cast<uint32_t>(value));
                address += 4;
            } else if (directive == ".dword" || directive == ".quad") {
                mem.write64(address, value);
                address += 8;
                //std::cout << "Storing value: 0x" << std::hex << value << " at address: 0x" << address - 8 << std::endl;
            } else {
                std::cerr << "Unknown directive: " << directive << std::endl;
            }
        }
    }

    //std::cout << "Data section loaded into memory." << std::endl;

    // Print the loaded data for verification
    // std::cout << "Loaded data section:" << std::endl;
    // for (uint64_t addr = 0x10000; addr < address; addr += 8) {
    //     std::cout << "0x" << std::hex << std::setw(16) << std::setfill('0') << addr << ": ";
    //     for (int i = 0; i < 8; ++i) {
    //         std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(mem.read8(addr + i)) << " ";
    //     }
    //     std::cout << std::endl;
    // }
}

void Simulator::updateCallStack(uint32_t instruction) {
    uint32_t opcode = instruction & 0x7F;
    uint32_t rd = (instruction >> 7) & 0x1F;
    uint32_t rs1 = (instruction >> 15) & 0x1F;

    if (opcode == 0x6F) { // JAL
        std::string funcName = "unknown";
        uint64_t targetAddress = pc * 4;
        for (const auto& label : labels) {
            if (label.second == targetAddress) {
                funcName = label.first;
                break;
            }
        }
        callStack.push_back({funcName, currentLine});
    } else if (opcode == 0x67 && rd == 0 && rs1 == 1) { // JALR x0, x1, 0 (return)
        if (callStack.size() > 1) {
            callStack.pop_back();
        }
    }

    // Update the line number of the current function
    if (!callStack.empty()) {
        callStack.back().line = currentLine;
    }
}

void Simulator::step() {
    if (pc >= machineCode.size()) {
        std::cout << "Nothing to step" << std::endl;
        return;
    }

    currentLine = lineNumbers[pc];

    if (breakpoints.find(currentLine) != breakpoints.end()) {
        std::cout << "Breakpoint hit at line " << std::dec << currentLine << std::endl;
        return;
    }

    uint32_t instruction = machineCode[pc];
    std::unique_ptr<Instruction> inst = Instruction::decode(instruction);
    
    updateCallStack(instruction);
    
    std::cout << "Executed: " << inst->toString() << "; PC = 0x" << std::hex << std::setw(8) << std::setfill('0') << (pc * 4) << std::endl;
    
    uint64_t old_pc = rf.read(RegisterFile::PC);
    inst->execute(rf, mem);
    uint64_t new_pc = rf.read(RegisterFile::PC);
    
    if (new_pc == old_pc) {
        rf.write(RegisterFile::PC, old_pc + 4);
        pc++;
    } else {
        pc = new_pc / 4;
    }
    
    executedInstructions++;
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
    for (int i = 0; i < 32; ++i) {
        uint64_t regValue = rf.read(i); // Assuming rf is an instance of RegisterFile

        // Check if the register index is single-digit or double-digit
        if (i < 10) {
            std::cout << "x" << std::dec << i << "  = 0x" << std::dec << std::noshowbase << std::hex<< regValue << std::endl;
        } else {
            std::cout << "x" <<  std::dec << i << " = 0x" << std::dec << std::noshowbase << std::hex<< regValue << std::endl;
        }
    }
    std::cout << std::endl;
}

void Simulator::printMem(uint64_t addr, int count) {
    for (int i = 0; i < count; ++i) {
        std::cout << "Memory[0x" << std::hex << std::noshowbase << (addr + i) << "] = 0x"
                  << std::hex << std::noshowbase << static_cast<int>(mem.read8(addr + i)) << std::endl;
    }
    std::cout << std::endl;
}

void Simulator::setBreakpoint(int line) {
    if (breakpoints.size() < 5) {
        breakpoints[line] = true;
        std::cout << "Breakpoint set at line " << std::dec << line << std::endl;
    } else {
        std::cout << "Maximum number of breakpoints (5) reached" << std::endl;
    }
    std::cout << std::endl;
}

void Simulator::deleteBreakpoint(int line) {
    if (breakpoints.erase(line) > 0) {
        //std::cout << "Breakpoint at line " << std::dec << line << " deleted" << std::endl;
    } else {
        std::cout << "No breakpoint found at line " << std::dec << line << std::endl;
    }
    std::cout << std::endl;
}

bool Simulator::isBreakpoint() const {
    return breakpoints.find(currentLine) != breakpoints.end();
}

void Simulator::showStack() const {
    if (callStack.empty()) {
        std::cout << "Call stack is empty." << std::endl;
    } else {
        std::cout << "Call stack:" << std::endl;
        for (const auto& frame : callStack) {
            std::cout << frame.functionName << ":" << std::dec << frame.line << std::endl;
        }
    }
}

void Simulator::listBreakpoints() const {
    if (breakpoints.empty()) {
        std::cout << "No breakpoints set." << std::endl;
    } else {
        std::cout << "Current breakpoints:" << std::endl;
        for (const auto& bp : breakpoints) {
            std::cout << "Line: " << std::dec << bp.first << std::endl;
        }
    }
}

void Simulator::printTextSection() const {
    std::cout << "Text Section:" << std::endl;
    for (size_t i = 0; i < machineCode.size(); ++i) {
        std::cout << "0x" << std::hex << std::setw(8) << std::setfill('0') << (i * 4)
                  << ": 0x" << std::setw(8) << machineCode[i] << " ";
        
        std::unique_ptr<Instruction> inst = Instruction::decode(machineCode[i]);
        std::cout << inst->toString() << std::endl;
    }
    std::cout << std::dec << std::endl;
}

void Simulator::printDataSection() const {
    std::cout << "Data Section (non-zero values):" << std::endl;
    uint64_t dataStart = 0x10000; // Assuming data section starts at 0x10000
    uint64_t dataEnd = 0x11000;   // Adjust this based on your actual data section size

    bool hasData = false;
    for (uint64_t addr = dataStart; addr < dataEnd; addr += 8) {
        uint64_t value = mem.read64(addr);
        if (value != 0) {
            hasData = true;
            std::cout << "0x" << std::hex << std::setw(16) << std::setfill('0') << addr << ": ";
            for (int i = 0; i < 8; ++i) {
                std::cout << std::setw(2) << static_cast<int>(mem.read8(addr + i)) << " ";
            }
            std::cout << std::endl;
        }
    }

    if (!hasData) {
        std::cout << "No non-zero data found in the data section." << std::endl;
    }
    std::cout << std::dec << std::endl;
}

void Simulator::showHelp() const {
    std::cout << "Available commands:" << std::endl;
    std::cout << "  load input.s       - Load the input assembly file." << std::endl;
    std::cout << "  run                 - Execute the loaded program." << std::endl;
    std::cout << "  step                - Execute the next instruction." << std::endl;
    std::cout << "  regs                - Display the current register values." << std::endl;
    std::cout << "  mem <addr> <count>  - Display memory contents starting from <addr>." << std::endl;
    std::cout << "  show-stack          - Show the current call stack." << std::endl;
    std::cout << "  break <line>       - Set a breakpoint at the specified line." << std::endl;
    std::cout << "  del break <line>   - Delete a breakpoint at the specified line." << std::endl;
    std::cout << "  list-breaks        - List all current breakpoints." << std::endl;
    std::cout << "  help                - Show this help message." << std::endl;
    std::cout <<"   text                - Show the text section." << std::endl;
    std::cout <<"   data                - Show the data section." << std::endl;
    std::cout << "  exit                - Exit the simulator." << std::endl;
}