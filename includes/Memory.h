#pragma once

#include <cstdint>
#include <map>
#include <vector>
#include "HexParser.h" // We need the HexRecord definition

// Define our memory map as a map from a 32-bit address to an 8-bit byte.
using MemoryMap = std::map<uint32_t, uint8_t>;

// This function processes a vector of HEX records and builds the final memory map.
MemoryMap build_memory_map(const std::vector<HexRecord>& records);
