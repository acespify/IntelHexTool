#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include "Memory.h"  // Needed for MemoryMap
#include "Symbols.h" // Needed for SymbolMap

struct DissassembeledInstruction {
    uint32_t address;
    std::string instruction_text;
    uint8_t size;  // The number of bytes the instruction occupies (1,2, or 3)
};

// Creating an abstract class the defines what the disassembler must be capable of doing.
class CpuDisassembler {
    public:
        virtual ~CpuDisassembler() = default; // a virtual destructor

        // A pure virtual function that any class cn inherit from.
        // The must provide an implementation for this function.
        virtual DissassembeledInstruction disassemble_op(const MemoryMap& memory, uint32_t pc, const SymbolMap& symbols) = 0;
};