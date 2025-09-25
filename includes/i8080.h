#pragma once

#include "Symbols.h"
#include "Memory.h"
#include <string>

struct DissassembeledInstruction {
    uint32_t address;
    std::string instruction_text;
    uint8_t size;  // The number of bytes the instruction occupies (1,2, or 3)
};

// Disassembes a single instruction at a given address in memory.
DissassembeledInstruction disassemble_8080_op(const MemoryMap& memory, uint32_t pc, const SymbolMap& symbols);
