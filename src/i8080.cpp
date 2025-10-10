#include "i8080.h"
#include <sstream>
#include <iomanip>
#include <optional>

// Helper function to get a byte from memory safely. Return 0 if address is not found.
static std::optional<uint8_t> mem_read(const MemoryMap& memory, uint32_t addr) {
    auto it = memory.find(addr);
    return(it != memory.end()) ? it->second : 0;
}

// Helper fuction for 16 bit mem_read
static std::optional<uint16_t> mem_read_word(const MemoryMap& memory, uint32_t addr) {
    auto lo = mem_read(memory, addr);
    auto hi = mem_read(memory, addr + 1);
    if (lo && hi) {
        return (*hi << 8) | *lo;
    }
    return std::nullopt;
}

DisassembledInstruction Disassembler8080::disassemble_op(const MemoryMap& memory, uint32_t pc, const SymbolMap& symbols) {
    DisassembledInstruction instr = {pc, "???", 1};
    
    auto opcode_opt = mem_read(memory, pc);
    if (!opcode_opt) {
        return instr;
    }
    uint8_t opcode = *opcode_opt;
    
    std::stringstream ss;
    ss << std::hex << std::uppercase << std::setfill('0');

    // Helper arrays for decoding register-based instructions
    const char* registers[] = {"B", "C", "D", "E", "H", "L", "M", "A"};
    const char* register_pairs[] = {"B", "D", "H", "SP"};
    const char* arith_mnemonics[] = {"ADD ", "ADC ", "SUB ", "SBB ", "ANA ", "XRA ", "ORA ", "CMP "};

    // --- Main Opcode Switch ---
    switch (opcode) {
        // --- 1-Byte Opcodes ---
        case 0x00: instr.instruction_text = "NOP"; break;
        case 0x07: instr.instruction_text = "RLC"; break;
        case 0x08: // unofficial NOP
        case 0x10: // unofficial NOP
        case 0x18: // unofficial NOP
        case 0x20: // unofficial NOP
        case 0x28: // unofficial NOP
        case 0x30: // unofficial NOP
        case 0x38: // unofficial NOP
            instr.instruction_text = "NOP*"; break;
        case 0x0F: instr.instruction_text = "RRC"; break;
        case 0x17: instr.instruction_text = "RAL"; break;
        case 0x1F: instr.instruction_text = "RAR"; break;
        case 0x27: instr.instruction_text = "DAA"; break;
        case 0x2F: instr.instruction_text = "CMA"; break;
        case 0x37: instr.instruction_text = "STC"; break;
        case 0x3F: instr.instruction_text = "CMC"; break;
        case 0x76: instr.instruction_text = "HLT"; break;
        case 0xE3: instr.instruction_text = "XTHL"; break;
        case 0xE9: instr.instruction_text = "PCHL"; break;
        case 0xEB: instr.instruction_text = "XCHG"; break;
        case 0xF3: instr.instruction_text = "DI"; break;
        case 0xF9: instr.instruction_text = "SPHL"; break;
        case 0xFB: instr.instruction_text = "EI"; break;

        // LDAX & STAX
        case 0x0A: instr.instruction_text = "LDAX B"; break;
        case 0x1A: instr.instruction_text = "LDAX D"; break;
        case 0x02: instr.instruction_text = "STAX B"; break;
        case 0x12: instr.instruction_text = "STAX D"; break;
        
        // INX, DCX, DAD
        case 0x03: case 0x13: case 0x23: case 0x33:
            ss << "INX  " << register_pairs[(opcode >> 4) & 0x03];
            instr.instruction_text = ss.str();
            break;
        case 0x0B: case 0x1B: case 0x2B: case 0x3B:
            ss << "DCX  " << register_pairs[(opcode >> 4) & 0x03];
            instr.instruction_text = ss.str();
            break;
        case 0x09: case 0x19: case 0x29: case 0x39:
            ss << "DAD  " << register_pairs[(opcode >> 4) & 0x03];
            instr.instruction_text = ss.str();
            break;

        // INR & DCR
        case 0x04: case 0x0C: case 0x14: case 0x1C: case 0x24: case 0x2C: case 0x34: case 0x3C:
            ss << "INR  " << registers[(opcode >> 3) & 0x07];
            instr.instruction_text = ss.str();
            break;
        case 0x05: case 0x0D: case 0x15: case 0x1D: case 0x25: case 0x2D: case 0x35: case 0x3D:
            ss << "DCR  " << registers[(opcode >> 3) & 0x07];
            instr.instruction_text = ss.str();
            break;

        // MOV r1, r2 (0x40 - 0x7F)
        case 0x40 ... 0x75: case 0x77 ... 0x7F:
        {
            int dst = (opcode >> 3) & 0x07;
            int src = opcode & 0x07;
            ss << "MOV  " << registers[dst] << "," << registers[src];
            instr.instruction_text = ss.str();
            break;
        }

        // Arithmetic/Logical (0x80 - 0xBF)
        case 0x80 ... 0xBF:
        {
            int op = (opcode >> 3) & 0x07;
            int reg = opcode & 0x07;
            ss << arith_mnemonics[op] << registers[reg];
            instr.instruction_text = ss.str();
            break;
        }

        // PUSH & POP
        case 0xC5: case 0xD5: case 0xE5: case 0xF5:
            ss << "PUSH " << ((opcode == 0xF5) ? "PSW" : register_pairs[(opcode >> 4) & 0x03]);
            instr.instruction_text = ss.str();
            break;
        case 0xC1: case 0xD1: case 0xE1: case 0xF1:
            ss << "POP  " << ((opcode == 0xF1) ? "PSW" : register_pairs[(opcode >> 4) & 0x03]);
            instr.instruction_text = ss.str();
            break;
            
        // Conditional RET
        case 0xC0: instr.instruction_text = "RNZ"; break;
        case 0xC8: instr.instruction_text = "RZ"; break;
        case 0xD0: instr.instruction_text = "RNC"; break;
        case 0xD8: instr.instruction_text = "RC"; break;
        case 0xE0: instr.instruction_text = "RPO"; break;
        case 0xE8: instr.instruction_text = "RPE"; break;
        case 0xF0: instr.instruction_text = "RP"; break;
        case 0xF8: instr.instruction_text = "RM"; break;
        case 0xC9: instr.instruction_text = "RET"; break;
        case 0xD9: // Unofficial RET
            instr.instruction_text = "RET*"; break;

        // RST
        case 0xC7: case 0xCF: case 0xD7: case 0xDF: case 0xE7: case 0xEF: case 0xF7: case 0xFF:
            ss << "RST  " << ((opcode >> 3) & 0x07);
            instr.instruction_text = ss.str();
            break;


        // --- 2-Byte Opcodes ---
        case 0x06: case 0x0E: case 0x16: case 0x1E:
        case 0x26: case 0x2E: case 0x36: case 0x3E:
        { // MVI r, d8
            auto byte_opt = mem_read(memory, pc + 1);
            if (byte_opt) {
                ss << "MVI  " << registers[(opcode >> 3) & 0x07] << ", #$" << std::setw(2) << static_cast<int>(*byte_opt);
                instr.instruction_text = ss.str();
            }
            instr.size = 2;
            break;
        }
        case 0xC6: case 0xCE: case 0xD6: case 0xDE:
        case 0xE6: case 0xEE: case 0xF6: case 0xFE:
        { // Immediate Arithmetic
            auto byte_opt = mem_read(memory, pc + 1);
            if (byte_opt) {
                const char* mnemonics[] = {"ADI ", "ACI ", "SUI ", "SBI ", "ANI ", "XRI ", "ORI ", "CPI "};
                ss << mnemonics[(opcode >> 3) & 0x07] << "#$" << std::setw(2) << static_cast<int>(*byte_opt);
                instr.instruction_text = ss.str();
            }
            instr.size = 2;
            break;
        }
        case 0xD3: // OUT d8
        case 0xDB: // IN d8
        {
            auto byte_opt = mem_read(memory, pc + 1);
            if (byte_opt) {
                ss << ((opcode == 0xD3) ? "OUT  " : "IN   ") << "#$" << std::setw(2) << static_cast<int>(*byte_opt);
                instr.instruction_text = ss.str();
            }
            instr.size = 2;
            break;
        }

        // --- 3-Byte Opcodes ---
        case 0x01: case 0x11: case 0x21: case 0x31:
        { // LXI rp, d16
            auto word_opt = mem_read_word(memory, pc + 1);
            if (word_opt) {
                ss << "LXI  " << register_pairs[(opcode >> 4) & 0x03] << ", #$" << std::setw(4) << *word_opt;
                instr.instruction_text = ss.str();
            }
            instr.size = 3;
            break;
        }
        case 0x22: // SHLD adr
        case 0x2A: // LHLD adr
        case 0x32: // STA adr
        case 0x3A: // LDA adr
        {
            auto word_opt = mem_read_word(memory, pc + 1);
            if (word_opt) {
                const char* mnemonic = (opcode == 0x22) ? "SHLD " : (opcode == 0x2A) ? "LHLD " : (opcode == 0x32) ? "STA  " : "LDA  ";
                ss << mnemonic << "$" << std::setw(4) << *word_opt;
                instr.instruction_text = ss.str();
            }
            instr.size = 3;
            break;
        }
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
        {
            auto word_opt = mem_read_word(memory, pc + 1);
            if (word_opt) {
                const char* mnemonics[] = {"JNZ  ", "JMP  ", "CNZ  ", "???  ", "JZ   ", "???  ", "CZ   ", "CALL ",
                                           "JNC  ", "???  ", "CNC  ", "???  ", "JC   ", "???  ", "CC   ", "???  ",
                                           "JPO  ", "???  ", "CPO  ", "???  ", "JPE  ", "???  ", "CPE  ", "???  ",
                                           "JP   ", "???  ", "CP   ", "???  ", "JM   ", "???  ", "CM   ", "???  "};
                // Decode the specific mnemonic from the complex pattern
                const char* mnemonic = mnemonics[((opcode - 0xC2) >> 1) + ((opcode & 1) * 2)];
                if (opcode == 0xC3) mnemonic = "JMP  "; if (opcode == 0xCD) mnemonic = "CALL ";

                ss << mnemonic;
                if (symbols.count(*word_opt)) {
                    ss << symbols.at(*word_opt);
                } else {
                    ss << "$" << std::setw(4) << *word_opt;
                }
                instr.instruction_text = ss.str();
            }
            instr.size = 3;
            break;
        }
        
        // Unofficial 3-byte CALL opcodes
        case 0xDD: case 0xED: case 0xFD:
        {
            auto word_opt = mem_read_word(memory, pc + 1);
            if (word_opt) {
                ss << "CALL* ";
                if (symbols.count(*word_opt)) {
                    ss << symbols.at(*word_opt);
                } else {
                    ss << "$" << std::setw(4) << *word_opt;
                }
                instr.instruction_text = ss.str();
            }
            instr.size = 3;
            break;
        }
    }
    return instr;
}