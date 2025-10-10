#pragma once

#include "CpuDisassembler.h"

class Disassembler8080 : public CpuDisassembler {
    public:
        // Disassembes a single instruction at a given address in memory.
        DisassembledInstruction disassemble_op(const MemoryMap& memory, uint32_t pc, const SymbolMap& symbols) override;
};
