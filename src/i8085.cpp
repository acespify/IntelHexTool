#include "i8085.h"
#include <sstream>
#include <iomanip>
#include <optional>

// Helper function to get a byte from memory safely. Return 0 if address is not found.
static std::optional<uint8_t> mem_read(const MemoryMap& memory, uint32_t addr) {
    auto it = memory.find(addr);
    if (it != memory.end()) {
        return it->second;
    }
    return std::nullopt;
}


DisassembledInstruction Disassembler8085::disassemble_op(const MemoryMap& memory, uint32_t pc, const SymbolMap& symbols) {
    DisassembledInstruction instr = {pc, "???", 1};
    auto opcode_opt = mem_read(memory, pc);
    if (!opcode_opt) {
        return instr;
    }
    uint8_t opcode = *opcode_opt;

    // Check for the 8085's specific opcodes
    switch (opcode){
        case 0x20: return { pc, "RIM", 1};
        case 0x30: return { pc, "SIM", 1};
    }

    // If the opcode is not and 8085 opcode, use the parent 8080 class to handle it.
    return Disassembler8080::disassemble_op(memory, pc, symbols);
}