#include "../include/simulator.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

int main() {
    Simulator sim;
    std::string command;

    // Automatically load the input file at startup
    std::string inputFile = "input/input.hex";
    std::ifstream file(inputFile);
    if (file.good()) {
        sim.loadProgram(inputFile);
        std::cout << "Loaded program: " << inputFile << std::endl;
    } else {
        std::cerr << "Error: Could not open input file: " << inputFile << std::endl;
    }

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);
        std::istringstream iss(command);
        std::string cmd;
        iss >> cmd;

        if (cmd == "run") {
            sim.run();
        } else if (cmd == "step") {
            sim.step();
        } else if (cmd == "regs") {
            sim.printRegs();
        } else if (cmd == "mem") {
            uint64_t addr;
            int count;
            iss >> std::hex >> addr >> std::dec >> count;
            sim.printMem(addr, count);
        } else if (cmd == "show-stack") {
            sim.showStack();
        } else if (cmd == "break") {
            int line;
            iss >> line;
            sim.setBreakpoint(line);
        } else if (cmd == "del") {
            std::string subCmd;
            iss >> subCmd;
            if (subCmd == "break") {
                int line;
                iss >> line;
                sim.deleteBreakpoint(line);
            } else {
                std::cout << "Unknown command" << std::endl;
            }
        } else if (cmd == "exit") {
            std::cout << "Exiting the simulator" << std::endl;
            break;
        } else {
            std::cout << "Unknown command" << std::endl;
        }
        
        std::cout << std::endl;
    }

    return 0;
}