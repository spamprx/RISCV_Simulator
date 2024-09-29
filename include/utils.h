// utils.h
#pragma once

#include <string>
#include <sstream>
#include <iomanip>

namespace utils {

inline std::string uint64ToHex(uint64_t value) {
    std::stringstream ss;
    ss << "0x" << std::hex << std::setw(16) << std::setfill('0') << value;
    return ss.str();
}

inline std::string uint32ToHex(uint32_t value) {
    std::stringstream ss;
    ss << "0x" << std::hex << std::setw(8) << std::setfill('0') << value;
    return ss.str();
}

} // namespace utils