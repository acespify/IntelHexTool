#pragma once

#include "Memory.h" // For MemoryMap
#include <string>
#include <map>

// A map from a 32-bit address to its string label (e.g., 0x401A -> "L401A")
using SymbolMap = std::map<uint32_t, std::string>;

// Scans the memory and generates a map of all identified labels.
SymbolMap generate_symbols(const MemoryMap& memory);