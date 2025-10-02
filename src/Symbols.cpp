#include "Symbols.h"
#include <set>
#include <sstream>
#include <iomanip>

// Helper to safely read a 16-bit word from memory.
static uint16_t mem_read_word(const MemoryMap& memory, uint32_t addr) {
    auto it_lo = memory.find(addr);
    auto it_hi = memory.find(addr + 1);
    if (it_lo == memory.end() || it_hi == memory.end()) {
        return 0;
    }
    return (it_hi->second << 8) | it_lo->second;
}

SymbolMap generate_symbols(const MemoryMap& memory) {
    if (memory.empty()) {
        return {};
    }

    std::set<uint32_t> label_addresses;
    uint32_t pc = memory.begin()->first;
    uint32_t end_addr = memory.rbegin()->first;

    // First Pass: Scan for all JMP/CALL targets
    while (pc <= end_addr) {
        if (memory.find(pc) == memory.end()) {
            pc++;
            continue;
        }
        uint8_t opcode = memory.at(pc);
        uint8_t size = 1;

        // Check for 3-byte JMP and CALL opcodes
        switch (opcode) {
            case 0xC3: // JMP
            case 0xC2: // JNZ
            case 0xCA: // JZ
            case 0xD2: // JNC
            case 0xDA: // JC
            case 0xE2: // JPO
            case 0xEA: // JPE
            case 0xF2: // JP
            case 0xFA: // JM
            case 0xCD: // CALL
            case 0xC4: // CNZ
            case 0xCC: // CZ
            case 0xD4: // CNC
            case 0xDC: // CC
            case 0xE4: // CPO
            case 0xEC: // CPE
            case 0xF4: // CP
            case 0xFC: // CM
                label_addresses.insert(mem_read_word(memory, pc + 1));
                size = 3;
                break;
            // All other opcodes have sizes of 1 or 2 bytes
            case 0x06: case 0x0E: case 0x16: case 0x1E: case 0x26: 
            case 0x2E: case 0x36: case 0x3E: case 0xC6: case 0xCE:
            case 0xD6: case 0xDE: case 0xE6: case 0xEE: case 0xF6:
            case 0xFE: case 0xD3: case 0xDB:
                size = 2;
                break;
            case 0x01: case 0x11: case 0x21: case 0x31: case 0x22:
            case 0x2A: case 0x32: case 0x3A:
                size = 3;
                break;
        }
        pc += size;
    }

    // Second Pass: Generate names for the found addresses
    SymbolMap symbols;
    for (uint32_t addr : label_addresses) {
        std::stringstream ss;
        ss << "L" << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << addr;
        symbols[addr] = ss.str();
    }
    
    return symbols;
}