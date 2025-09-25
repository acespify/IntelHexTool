#include "i8080.h"
#include <sstream>
#include <iomanip>

// Helper function to get a byte from memory safely. Return 0 if address is not found.
static uint8_t mem_read(const MemoryMap& memory, uint32_t addr) {
    auto it = memory.find(addr);
    return(it != memory.end()) ? it->second : 0;
}

DissassembeledInstruction disassemble_8080_op(const MemoryMap& memory, uint32_t pc, const SymbolMap& symbols) {
    DissassembeledInstruction instr = {pc, "???", 1};
    uint8_t opcode = mem_read(memory, pc);

    std::stringstream ss;
    ss << std::hex << std::uppercase;

    switch(opcode)
    {
        // NOP instructions for the 8080
        case 0x00: case 0x08: case 0x10: case 0x18: case 0x20: case 0x28: case 0x30: case 0x38: case 0xCB: case 0xD9: case 0xDD: case 0xED: case 0xFD: instr.instruction_text = "NOP"; instr.size = 1; break;
        // LXI B, d16
        case 0x01: { uint8_t d1 = mem_read(memory, pc + 1); uint8_t d2 = mem_read(memory, pc + 2); ss << "LXI B, #$" << std::setw(2) << std::setfill('0') << (int)d2 << std::setw(2) << (int)d1; instr.instruction_text = ss.str(); instr.size = 3;} break;
        // LXI D, d16
        case 0x11: { uint8_t d1 = mem_read(memory, pc + 1); uint8_t d2 = mem_read(memory, pc + 2); ss << "LXI D, #$" << std::setw(2) << std::setfill('0') << (int)d2 << std::setw(2) << (int)d1; instr.instruction_text = ss.str(); instr.size = 3;} break;
        // LXI H, d16
        case 0x21: { uint8_t d1 = mem_read(memory, pc + 1); uint8_t d2 = mem_read(memory, pc + 2); ss << "LXI H, #$" << std::setw(2) << std::setfill('0') << (int)d2 << std::setw(2) << (int)d1; instr.instruction_text = ss.str(); instr.size = 3;} break;
        // LXI SP, d16
        case 0x31: { uint8_t d1 = mem_read(memory, pc + 1); uint8_t d2 = mem_read(memory, pc + 2); ss << "LXI SP, #$" << std::setw(2) << std::setfill('0') << (int)d2 << std::setw(2) << (int)d1; instr.instruction_text = ss.str(); instr.size = 3;} break;
        // STAX B
        case 0x02: { instr.instruction_text = "STAX  B"; instr.size = 1; break;}
        // INX B
        case 0x03: { instr.instruction_text = "INX  B"; instr.size = 1; break; }
        // INR B
        case 0x04: { instr.instruction_text = "INR  B"; instr.size = 1; break; }
        // DCR B 
        case 0x05: { instr.instruction_text = "DCR  B"; instr.size = 1; break; }
        // MVI B,d8
        case 0x06: { uint8_t d1 = mem_read(memory, pc + 1); ss << "MVI  B, #$" << std::setw(2) << std::setfill('0') << (int)d1; instr.instruction_text = ss.str(); instr.size = 2; break; }
        // RLC
        case 0x07: { instr.instruction_text = "RLC"; instr.size = 1; break;}
        // DAD B
        case 0x09: instr.instruction_text = "DAD  B"; instr.size = 1; break;
        // LDAX B
        case 0x0A: { instr.instruction_text = "LDAX  B"; instr.size = 1; break;}
        // DCX B
        case 0x0B: { instr.instruction_text = "DCX  B"; instr.size = 1;break;}
        // INR C
        case 0x0C: { instr.instruction_text = "INR  C"; instr.size = 1;break;}
        // DCR C
        case 0x0D: instr.instruction_text = "DCR  C"; instr.size = 1; break;
        // MVI C,d8
        case 0x0E: { uint8_t d1 = mem_read(memory, pc + 1); ss << "MVI  C, #$" << std::setw(2) << std::setfill('0') << (int)d1; instr.instruction_text = ss.str(); instr.size = 2; break; }        
        // RRC
        case 0x0F: { instr.instruction_text = "RRC"; instr.size = 1; break;}
        // STAX D
        case 0x12: { instr.instruction_text = "STAX  D"; instr.size = 1;break;}
        // INX D
        case 0x13: { instr.instruction_text = "INX  D"; instr.size = 1; break; }
        // INR D
        case 0x14: { instr.instruction_text = "INR  D"; instr.size = 1; break; }
        // DCR D
        case 0x15: { instr.instruction_text = "DCR  D"; instr.size = 1; break; }
        // MVI D, d8
        case 0x16: { uint8_t d1 = mem_read(memory, pc + 1); ss << "MVI  D, #$" << std::setw(2) << std::setfill('0') << (int)d1; instr.instruction_text = ss.str(); instr.size = 2; break; }
        // RAL
        case 0x17: { instr.instruction_text = "RAL"; instr.size = 1; break; }
        // DAD D
        case 0x19: { instr.instruction_text = "DAD  D"; instr.size = 1; break; } 
        // LDAX D
        case 0x1A: { instr.instruction_text = "LDAX D"; instr.size = 1; break; }
        // DCX D
        case 0x1B: { instr.instruction_text = "DCX  D"; instr.size = 1; break; }
        // INR E
        case 0x1C: { instr.instruction_text = "INR  E"; instr.size = 1; break; }
        // DCR E
        case 0x1D: { instr.instruction_text = "DCR  E"; instr.size = 1; break; }
        // MVI E,d8
        case 0x1E: { uint8_t d1 = mem_read(memory, pc + 1); ss << "MVI  E, #$" << std::setw(2) << std::setfill('0') << (int)d1; instr.instruction_text = ss.str(); instr.size = 2; break; }
        // RAR
        case 0x1F: { instr.instruction_text = "RAR"; instr.size = 1; break; }
        // SHLD a16
        case 0x22: { uint8_t d1 = mem_read(memory, pc + 1); uint8_t d2 = mem_read(memory, pc + 2); ss << "SHLD  $" << std::setw(2) << std::setfill('0') << (int)d2 << std::setw(2) << (int)d1; instr.instruction_text = ss.str(); instr.size = 3; break; }
        // INX H
        case 0x23: { instr.instruction_text = "INX  H"; instr.size = 1; break; }
        // INR H
        case 0x24: { instr.instruction_text = "INR  H"; instr.size = 1; break; }
        // DCR H
        case 0x25: { instr.instruction_text = "DCR  H"; instr.size = 1; break; }
        // MVI H,d8
        case 0x26: { uint8_t d1 = mem_read(memory, pc + 1); ss << "MVI  H, #$" << std::setw(2) << std::setfill('0') << (int)d1; instr.instruction_text = ss.str(); instr.size = 2; break; }
        // DAA
        case 0x27: { instr.instruction_text = "DAA"; instr.size = 1; break; }
        // DAD
        case 0x29: { instr.instruction_text = "DAD  H"; instr.size = 1; break; }
        // LHLD a16
        case 0x2A: { uint8_t d1 = mem_read(memory, pc + 1); uint8_t d2 = mem_read(memory, pc + 2); ss << "LHLD  $" << std::setw(2) << std::setfill('0') << (int)d2 << std::setw(2) << (int)d1; instr.instruction_text = ss.str(); instr.size = 3; break; }
        // DCX H
        case 0x2B: { instr.instruction_text = "DCX  H"; instr.size = 1; break; } 
        // INR L
        case 0x2C: { instr.instruction_text = "INR  L"; instr.size = 1; break; }
        // DCR L
        case 0x2D: { instr.instruction_text = "DCR  L"; instr.size = 1; break; }
        // MVI L,d8
        case 0x2E: { uint8_t d1 = mem_read(memory, pc + 1); ss << "MVI  L, #$" << std::setw(2) << std::setfill('0') << (int)d1; instr.instruction_text = ss.str(); instr.size = 2; break; }
        // CMA
        case 0x2F: { instr.instruction_text = "CMA"; instr.size = 1; break; }
        // STA adr
        case 0x32: { uint8_t d1 = mem_read(memory, pc + 1); uint8_t d2 = mem_read(memory, pc + 2); ss << "STA  $" << std::setw(2) << std::setfill('0') << (int)d2 << std::setw(2) << (int)d1; instr.instruction_text = ss.str(); instr.size = 3; break; }
        // INX SP
        case 0x33: { instr.instruction_text = "INX  SP"; instr.size = 1; break; }
        // INR M
        case 0x34: { instr.instruction_text = "INR  M"; instr.size = 1; break; }
        // DCR M
        case 0x35: { instr.instruction_text = "DCR  M"; instr.size = 1; break; }
        // MVI M,d8
        case 0x36: { uint8_t d1 = mem_read(memory, pc + 1); ss << "MVI  M, #$" << std::setw(2) << std::setfill('0') << (int)d1; instr.instruction_text = ss.str(); instr.size = 2; break; }
        // STC
        case 0x37: { instr.instruction_text = "STC"; instr.size = 1; break; }
        // DAD SP
        case 0x39: { instr.instruction_text = "DAD SP"; instr.size = 1; break; }
        // LDA adr
        case 0x3A: { uint8_t d1 = mem_read(memory, pc + 1); uint8_t d2 = mem_read(memory, pc + 2); ss << "LDA  $" << std::setw(2) << std::setfill('0') << (int)d2 << std::setw(2) << (int)d1; instr.instruction_text = ss.str(); instr.size = 3; break; }
        // DCX SP
        case 0x3B: { instr.instruction_text = "DCX  SP"; instr.size = 1; break; }
        // INR A
        case 0x3C: { instr.instruction_text = "INR  A"; instr.size = 1; break; }
        // DCR A
        case 0x3D: { instr.instruction_text = "DCR  A"; instr.size = 1; break; }
        // MVI A,d8
        case 0x3E: { uint8_t d1 = mem_read(memory, pc + 1); ss << "MVI  A, #$" << std::setw(2) << std::setfill('0') << (int)d1; instr.instruction_text = ss.str(); instr.size = 2; break; }
        // CMC
        case 0x3F: { instr.instruction_text = "CMC"; instr.size = 1; break; }
        // MOV B,B
        case 0x40: { instr.instruction_text = "MOV  B,B"; instr.size = 1; break; }
        // MOV B,C
        case 0x41: { instr.instruction_text = "MOV  B,C"; instr.size = 1; break; }
        // MOV B,D
        case 0x42: { instr.instruction_text = "MOV  B,D"; instr.size = 1; break; }
        // MOV B,E
        case 0x43: { instr.instruction_text = "MOV  B,E"; instr.size = 1; break; }
        // MOV B,H
        case 0x44: { instr.instruction_text = "MOV  B,H"; instr.size = 1; break; }
        // MOV B,L
        case 0x45: { instr.instruction_text = "MOV  B,L"; instr.size = 1; break; }
        // MOV B,M
        case 0x46: { instr.instruction_text = "MOV  B,M"; instr.size = 1; break; }
        // MOV B,A
        case 0x47: { instr.instruction_text = "MOV  B,A"; instr.size = 1; break; }
        // MOV C,B
        case 0x48: { instr.instruction_text = "MOV  C,B"; instr.size = 1; break; }
        // MOV C,C
        case 0x49: { instr.instruction_text = "MOV  C,C"; instr.size = 1; break; }
        // MOV C,D
        case 0x4A: { instr.instruction_text = "MOV  C,D"; instr.size = 1; break; }
        // MOV C,E
        case 0x4B: { instr.instruction_text = "MOV  C,E"; instr.size = 1; break; }
        // MOV C,H
        case 0x4C: { instr.instruction_text = "MOV  C,H"; instr.size = 1; break; }
        // MOV C,L
        case 0x4D: { instr.instruction_text = "MOV  C,L"; instr.size = 1; break; }
        // MOV C,M
        case 0x4E: { instr.instruction_text = "MOV  C,M"; instr.size = 1; break; }
        // MOV C,A
        case 0x4F: { instr.instruction_text = "MOV  C,A"; instr.size = 1; break; }
        // MOV D,B
        case 0x50: { instr.instruction_text = "MOV  D,B"; instr.size = 1; break; }
        // MOV D,C
        case 0x51: { instr.instruction_text = "MOV  D,C"; instr.size = 1; break; }
        // MOV D,D
        case 0x52: { instr.instruction_text = "MOV  D,D"; instr.size = 1; break; }
        // MOV D,E
        case 0x53: { instr.instruction_text = "MOV  D,E"; instr.size = 1; break; }
        // MOV D,H
        case 0x54: { instr.instruction_text = "MOV  D,H"; instr.size = 1; break; }
        // MOV D,L
        case 0x55: { instr.instruction_text = "MOV  D,L"; instr.size = 1; break; }
        // MOV D,M
        case 0x56: { instr.instruction_text = "MOV  D,M"; instr.size = 1; break; }
        // MOV D,A
        case 0x57: { instr.instruction_text = "MOV  D,A"; instr.size = 1; break; }
        // MOV E,B
        case 0x58: { instr.instruction_text = "MOV  E,B"; instr.size = 1; break; }
        // MOV E,C
        case 0x59: { instr.instruction_text = "MOV  E,C"; instr.size = 1; break; }
        // MOV E,D
        case 0x5A: { instr.instruction_text = "MOV  E,D"; instr.size = 1; break; }
        // MOV E,E
        case 0x5B: { instr.instruction_text = "MOV  E,E"; instr.size = 1; break; }
        // MOV E,H
        case 0x5C: { instr.instruction_text = "MOV  E,H"; instr.size = 1; break; }
        // MOV E,L
        case 0x5D: { instr.instruction_text = "MOV  E,L"; instr.size = 1; break; }
        // MOV E,M
        case 0x5E: { instr.instruction_text = "MOV  E,M"; instr.size = 1; break; }
        // MOV E,A
        case 0x5F: { instr.instruction_text = "MOV  E,A"; instr.size = 1; break; }
        // MOV H,B
        case 0x60: { instr.instruction_text = "MOV  H,B"; instr.size = 1; break; }
        // MOV H,C
        case 0x61: { instr.instruction_text = "MOV  H,C"; instr.size = 1; break; }
        // MOV H,D
        case 0x62: { instr.instruction_text = "MOV  H,D"; instr.size = 1; break; }
        // MOV H,E
        case 0x63: { instr.instruction_text = "MOV  H,E"; instr.size = 1; break; }
        // MOV H,H
        case 0x64: { instr.instruction_text = "MOV  H,H"; instr.size = 1; break; }
        // MOV H,L
        case 0x65: { instr.instruction_text = "MOV  H,L"; instr.size = 1; break; }
        // MOV H,M
        case 0x66: { instr.instruction_text = "MOV  H,M"; instr.size = 1; break; }
        // MOV H,A
        case 0x67: { instr.instruction_text = "MOV  H,A"; instr.size = 1; break; }
        // MOV L,B
        case 0x68: { instr.instruction_text = "MOV  L,B"; instr.size = 1; break; }
        // MOV L,C
        case 0x69: { instr.instruction_text = "MOV  L,C"; instr.size = 1; break; }
        // MOV L,D
        case 0x6A: { instr.instruction_text = "MOV  L,D"; instr.size = 1; break; }
        // MOV L,E
        case 0x6B: { instr.instruction_text = "MOV  L,E"; instr.size = 1; break; }
        // MOV L,H
        case 0x6C: { instr.instruction_text = "MOV  L,H"; instr.size = 1; break; }
        // MOV L,L
        case 0x6D: { instr.instruction_text = "MOV  L,L"; instr.size = 1; break; }
        // MOV L,M
        case 0x6E: { instr.instruction_text = "MOV  L,M"; instr.size = 1; break; }
        // MOV L,A
        case 0x6F: { instr.instruction_text = "MOV  L,A"; instr.size = 1; break; }
        // MOV M,B
        case 0x70: { instr.instruction_text = "MOV  M,B"; instr.size = 1; break; }
        // MOV M,C
        case 0x71: { instr.instruction_text = "MOV  M,C"; instr.size = 1; break; }
        // MOV M,D
        case 0x72: { instr.instruction_text = "MOV  M,D"; instr.size = 1; break; }
        // MOV M,E
        case 0x73: { instr.instruction_text = "MOV  M,E"; instr.size = 1; break; }
        // MOV M,H
        case 0x74: { instr.instruction_text = "MOV  M,H"; instr.size = 1; break; }
        // MOV M,L
        case 0x75: { instr.instruction_text = "MOV  M,L"; instr.size = 1; break; }
        // HLT
        case 0x76: { instr.instruction_text = "HLT"; instr.size = 1; break; }
        // MOV M,A
        case 0x77: { instr.instruction_text = "MOV  M,A"; instr.size = 1; break; }
        // MOV A,B
        case 0x78: { instr.instruction_text = "MOV  A,B"; instr.size = 1; break; }
        // MOV A,C
        case 0x79: { instr.instruction_text = "MOV  A,C"; instr.size = 1; break; }
        // MOV A,D
        case 0x7A: { instr.instruction_text = "MOV  A,D"; instr.size = 1; break; }
        // MOV A,E
        case 0x7B: { instr.instruction_text = "MOV  A,E"; instr.size = 1; break; }
        // MOV A,H
        case 0x7C: { instr.instruction_text = "MOV  A,H"; instr.size = 1; break; }
        // MOV A,L
        case 0x7D: { instr.instruction_text = "MOV  A,L"; instr.size = 1; break; }
        // MOV A,M
        case 0x7E: { instr.instruction_text = "MOV  A,M"; instr.size = 1; break; }
        // MOV A,A
        case 0x7F: { instr.instruction_text = "MOV  A,A"; instr.size = 1; break; }
        // ADD B
        case 0x80: { instr.instruction_text = "ADD  B"; instr.size = 1; break; }
        // ADD C
        case 0x81: { instr.instruction_text = "ADD  C"; instr.size = 1; break; }
        // ADD D
        case 0x82: { instr.instruction_text = "ADD  D"; instr.size = 1; break; }
        // ADD E
        case 0x83: { instr.instruction_text = "ADD  E"; instr.size = 1; break; }
        // ADD H
        case 0x84: { instr.instruction_text = "ADD  H"; instr.size = 1; break; }
        // ADD L
        case 0x85: { instr.instruction_text = "ADD  L"; instr.size = 1; break; }
        // ADD M
        case 0x86: { instr.instruction_text = "ADD  M"; instr.size = 1; break; }
        // ADD A
        case 0x87: { instr.instruction_text = "ADD  A"; instr.size = 1; break; }
        // ADC B
        case 0x88: { instr.instruction_text = "ADC  B"; instr.size = 1; break; }
        // ADC C
        case 0x89: { instr.instruction_text = "ADC  C"; instr.size = 1; break; }
        // ADC D
        case 0x8A: { instr.instruction_text = "ADC  D"; instr.size = 1; break; }
        // ADC E
        case 0x8B: { instr.instruction_text = "ADC  E"; instr.size = 1; break; }
        // ADC H
        case 0x8C: { instr.instruction_text = "ADC  H"; instr.size = 1; break; }
        // ADC L
        case 0x8D: { instr.instruction_text = "ADC  L"; instr.size = 1; break; }
        // ADC M
        case 0x8E: { instr.instruction_text = "ADC  M"; instr.size = 1; break; }
        // ADC A
        case 0x8F: { instr.instruction_text = "ADC  A"; instr.size = 1; break; }
        // SUB B
        case 0x90: { instr.instruction_text = "SUB  B"; instr.size = 1; break; }
        // SUB C
        case 0x91: { instr.instruction_text = "SUB  C"; instr.size = 1; break; }
        // SUB D
        case 0x92: { instr.instruction_text = "SUB  D"; instr.size = 1; break; }
        // SUB E
        case 0x93: { instr.instruction_text = "SUB  E"; instr.size = 1; break; }
        // SUB H
        case 0x94: { instr.instruction_text = "SUB  H"; instr.size = 1; break; }
        // SUB L
        case 0x95: { instr.instruction_text = "SUB  L"; instr.size = 1; break; }
        // SUB M
        case 0x96: { instr.instruction_text = "SUB  M"; instr.size = 1; break; }
        // SUB A
        case 0x97: { instr.instruction_text = "SUB  A"; instr.size = 1; break; }
        // SBB B
        case 0x98: { instr.instruction_text = "SBB  B"; instr.size = 1; break; }
        // SBB C
        case 0x99: { instr.instruction_text = "SBB  C"; instr.size = 1; break; }
        // SBB D
        case 0x9A: { instr.instruction_text = "SBB  D"; instr.size = 1; break; }
        // SBB E
        case 0x9B: { instr.instruction_text = "SBB  E"; instr.size = 1; break; }
        // SBB H
        case 0x9C: { instr.instruction_text = "SBB  H"; instr.size = 1; break; }
        // SBB L
        case 0x9D: { instr.instruction_text = "SBB  L"; instr.size = 1; break; }
        // SBB M
        case 0x9E: { instr.instruction_text = "SBB  M"; instr.size = 1; break; }
        // SBB A
        case 0x9F: { instr.instruction_text = "SBB  A"; instr.size = 1; break; }
        // ANA B
        case 0xA0: { instr.instruction_text = "ANA  B"; instr.size = 1; break; }
        // ANA C
        case 0xA1: { instr.instruction_text = "ANA  C"; instr.size = 1; break; }
        // ANA D
        case 0xA2: { instr.instruction_text = "ANA  D"; instr.size = 1; break; }
        // ANA E
        case 0xA3: { instr.instruction_text = "ANA  E"; instr.size = 1; break; }
        // ANA H
        case 0xA4: { instr.instruction_text = "ANA  H"; instr.size = 1; break; }
        // ANA L
        case 0xA5: { instr.instruction_text = "ANA  L"; instr.size = 1; break; }
        // ANA M
        case 0xA6: { instr.instruction_text = "ANA  M"; instr.size = 1; break; }
        // ANA A
        case 0xA7: { instr.instruction_text = "ANA  A"; instr.size = 1; break; }
        // XRA B
        case 0xA8: { instr.instruction_text = "XRA  B"; instr.size = 1; break; }
        // XRA C
        case 0xA9: { instr.instruction_text = "XRA  C"; instr.size = 1; break; }
        // XRA D
        case 0xAA: { instr.instruction_text = "XRA  D"; instr.size = 1; break; }
        // XRA E
        case 0xAB: { instr.instruction_text = "XRA  E"; instr.size = 1; break; }
        // XRA H
        case 0xAC: { instr.instruction_text = "XRA  H"; instr.size = 1; break; }
        // XRA L
        case 0xAD: { instr.instruction_text = "XRA  L"; instr.size = 1; break; }
        // XRA M
        case 0xAE: { instr.instruction_text = "XRA  M"; instr.size = 1; break; }
        // XRA A
        case 0xAF: { instr.instruction_text = "XRA  A"; instr.size = 1; break; }
        // ORA B
        case 0xB0: { instr.instruction_text = "ORA  B"; instr.size = 1; break; }
        // ORA C
        case 0xB1: { instr.instruction_text = "ORA  C"; instr.size = 1; break; }
        // ORA D
        case 0xB2: { instr.instruction_text = "ORA  D"; instr.size = 1; break; }
        // ORA E
        case 0xB3: { instr.instruction_text = "ORA  E"; instr.size = 1; break; }
        // ORA H
        case 0xB4: { instr.instruction_text = "ORA  H"; instr.size = 1; break; }
        // ORA L
        case 0xB5: { instr.instruction_text = "ORA  L"; instr.size = 1; break; }
        // ORA M
        case 0xB6: { instr.instruction_text = "ORA  M"; instr.size = 1; break; }
        // ORA A
        case 0xB7: { instr.instruction_text = "ORA  A"; instr.size = 1; break; }
        // CMP B
        case 0xB8: { instr.instruction_text = "CMP  B"; instr.size = 1; break; }
        // CMP C
        case 0xB9: { instr.instruction_text = "CMP  C"; instr.size = 1; break; }
        // CMP D
        case 0xBA: { instr.instruction_text = "CMP  D"; instr.size = 1; break; }
        // CMP E
        case 0xBB: { instr.instruction_text = "CMP  E"; instr.size = 1; break; }
        // CMP H
        case 0xBC: { instr.instruction_text = "CMP  H"; instr.size = 1; break; }
        // CMP L
        case 0xBD: { instr.instruction_text = "CMP  L"; instr.size = 1; break; }
        // CMP M
        case 0xBE: { instr.instruction_text = "CMP  M"; instr.size = 1; break; }
        // CMP A
        case 0xBF: { instr.instruction_text = "CMP  A"; instr.size = 1; break; }
        // RNZ
        case 0xC0: { instr.instruction_text = "RNZ"; instr.size = 1; break; }
        // POP B
        case 0xC1: { instr.instruction_text = "POP  B"; instr.size = 1; break; }
        // JNZ a16
        case 0xC2: { uint8_t d1 = mem_read(memory, pc + 1); uint8_t d2 = mem_read(memory, pc + 2); uint16_t addr = (d2 << 8) | d1; if (symbols.count(addr)) { ss << "JNZ  " << symbols.at(addr);} else { ss << "JNZ  $" << std::setw(4) << std::setfill('0') << addr;} instr.instruction_text = ss.str(); instr.size = 3; break; }
        // JMP a16
        case 0xC3: { uint8_t d1 = mem_read(memory, pc + 1); uint8_t d2 = mem_read(memory, pc + 2); uint16_t addr = (d2 << 8) | d1; if (symbols.count(addr)) { ss << "JMP  " << symbols.at(addr);} else { ss << "JMP  $" << std::setw(4) << std::setfill('0') << addr;} instr.instruction_text = ss.str(); instr.size = 3; break; }
        // CNZ a16
        case 0xC4: { uint8_t d1 = mem_read(memory, pc + 1); uint8_t d2 = mem_read(memory, pc + 2); uint16_t addr = (d2 << 8) | d1; if (symbols.count(addr)) { ss << "CNZ  " << symbols.at(addr);} else { ss << "CNZ  $" << std::setw(4) << std::setfill('0') << addr;} instr.instruction_text = ss.str(); instr.size = 3; break; }
        // PUSH B
        case 0xC5: { instr.instruction_text = "PUSH B"; instr.size = 1; break; }
        // ADI d8
        case 0xC6: { uint8_t d1 = mem_read(memory, pc + 1); ss << "ADI  #$" << std::setw(2) << std::setfill('0') << (int)d1; instr.instruction_text = ss.str(); instr.size = 2; break; } 
        // RST 0
        case 0xC7: { instr.instruction_text = "RST  0"; instr.size = 1; break; }
        // RZ
        case 0xC8: { instr.instruction_text = "RZ"; instr.size = 1; break; }
        // RET
        case 0xC9: { instr.instruction_text = "RET"; instr.size = 1; break; }
        // JZ a16
        case 0xCA: { uint8_t d1 = mem_read(memory, pc + 1); uint8_t d2 = mem_read(memory, pc + 2); uint16_t addr = (d2 << 8) | d1; if (symbols.count(addr)) { ss << "JZ  " << symbols.at(addr);} else { ss << "JZ  $" << std::setw(4) << std::setfill('0') << addr;} instr.instruction_text = ss.str(); instr.size = 3; break; }
        // CZ a16
        case 0xCC: { uint8_t d1 = mem_read(memory, pc + 1); uint8_t d2 = mem_read(memory, pc + 2); uint16_t addr = (d2 << 8) | d1; if (symbols.count(addr)) { ss << "CZ  " << symbols.at(addr);} else { ss << "CZ  $" << std::setw(4) << std::setfill('0') << addr;} instr.instruction_text = ss.str(); instr.size = 3; break; }
        // CALL a16
        case 0xCD: { uint8_t d1 = mem_read(memory, pc + 1); uint8_t d2 = mem_read(memory, pc + 2); uint16_t addr = (d2 << 8) | d1; if (symbols.count(addr)) { ss << "CALL  " << symbols.at(addr);} else { ss << "CALL  $" << std::setw(4) << std::setfill('0') << addr;} instr.instruction_text = ss.str(); instr.size = 3; break; }
        // ACI d8
        case 0xCE: { uint8_t d1 = mem_read(memory, pc + 1); ss << "ACI  #$" << std::setw(2) << std::setfill('0') << (int)d1; instr.instruction_text = ss.str(); instr.size = 2; break; } 
        // RST 1
        case 0xCF: { instr.instruction_text = "RST  1"; instr.size = 1; break; }
        // RNC
        case 0xD0: { instr.instruction_text = "RNC"; instr.size = 1; break; }
        // POP D
        case 0xD1: { instr.instruction_text = "POP  D"; instr.size = 1; break; }
        // JNC a16
        case 0xD2: { uint8_t d1 = mem_read(memory, pc + 1); uint8_t d2 = mem_read(memory, pc + 2); uint16_t addr = (d2 << 8) | d1; if (symbols.count(addr)) { ss << "JNC  " << symbols.at(addr);} else { ss << "JNC  $" << std::setw(4) << std::setfill('0') << addr;} instr.instruction_text = ss.str(); instr.size = 3; break; }
        // OUT d8
        case 0xD3: { uint8_t d1 = mem_read(memory, pc + 1); ss << "OUT  #$" << std::setw(2) << std::setfill('0') << (int)d1; instr.instruction_text = ss.str(); instr.size = 2; break; }
        // CNC a16
        case 0xD4: { uint8_t d1 = mem_read(memory, pc + 1); uint8_t d2 = mem_read(memory, pc + 2); uint16_t addr = (d2 << 8) | d1; if (symbols.count(addr)) { ss << "CNC  " << symbols.at(addr);} else { ss << "CNC  $" << std::setw(4) << std::setfill('0') << addr;} instr.instruction_text = ss.str(); instr.size = 3; break; }
        // PUSH D
        case 0xD5: { instr.instruction_text = "PUSH D"; instr.size = 1; break;}
        // SUI d8
        case 0xD6: { uint8_t d1 = mem_read(memory, pc + 1); ss << "SUI  #$" << std::setw(2) << std::setfill('0') << (int)d1; instr.instruction_text = ss.str(); instr.size = 2; break; } 
        // RST 2
        case 0xD7: { instr.instruction_text = "RST  2"; instr.size = 1; break; }
        // RC
        case 0xD8: { instr.instruction_text = "RC"; instr.size = 1; break; }
        // JC a16
        case 0xDA: { uint8_t d1 = mem_read(memory, pc + 1); uint8_t d2 = mem_read(memory, pc + 2); uint16_t addr = (d2 << 8) | d1; if (symbols.count(addr)) { ss << "JC  " << symbols.at(addr);} else { ss << "JC  $" << std::setw(4) << std::setfill('0') << addr;} instr.instruction_text = ss.str(); instr.size = 3; break; }
        // IN d8
        case 0xDB: { uint8_t d1 = mem_read(memory, pc + 1); ss << "IN  #$" << std::setw(2) << std::setfill('0') << (int)d1; instr.instruction_text = ss.str(); instr.size = 2; break; }
        // CC a16
        case 0xDC: { uint8_t d1 = mem_read(memory, pc + 1); uint8_t d2 = mem_read(memory, pc + 2); uint16_t addr = (d2 << 8) | d1; if (symbols.count(addr)) { ss << "CC  " << symbols.at(addr);} else { ss << "CC  $" << std::setw(4) << std::setfill('0') << addr;} instr.instruction_text = ss.str(); instr.size = 3; break; }
        // SBI d8
        case 0xDE: { uint8_t d1 = mem_read(memory, pc + 1); ss << "SBI  #$" << std::setw(2) << std::setfill('0') << (int)d1; instr.instruction_text = ss.str(); instr.size = 2; break; } 
        // RST 3
        case 0xDF: { instr.instruction_text = "RST  3"; instr.size = 1; break; }
        // RPO
        case 0xE0: { instr.instruction_text = "RPO"; instr.size = 1; break; }
        // POP H
        case 0xE1: { instr.instruction_text = "POP  H"; instr.size = 1; break; }
        // JPO a16
        case 0xE2: { uint8_t d1 = mem_read(memory, pc + 1); uint8_t d2 = mem_read(memory, pc + 2); uint16_t addr = (d2 << 8) | d1; if (symbols.count(addr)) { ss << "JPO  " << symbols.at(addr);} else { ss << "JPO  $" << std::setw(4) << std::setfill('0') << addr;} instr.instruction_text = ss.str(); instr.size = 3; break; }
        // XTHL
        case 0xE3: { instr.instruction_text = "XTHL"; instr.size = 1; break; }
        // CPO a16
        case 0xE4: { uint8_t d1 = mem_read(memory, pc + 1); uint8_t d2 = mem_read(memory, pc + 2); uint16_t addr = (d2 << 8) | d1; if (symbols.count(addr)) { ss << "CPO  " << symbols.at(addr);} else { ss << "CPO  $" << std::setw(4) << std::setfill('0') << addr;} instr.instruction_text = ss.str(); instr.size = 3; break; }
        // PUSH H
        case 0xE5: { instr.instruction_text = "PUSH  H"; instr.size = 1; break; }
        // ANI d8
        case 0xE6: { uint8_t d1 = mem_read(memory, pc + 1); ss << "ANI  #$" << std::setw(2) << std::setfill('0') << (int)d1; instr.instruction_text = ss.str(); instr.size = 2; break; }
        // RST 4
        case 0xE7: { instr.instruction_text = "RST  4"; instr.size = 1; break; }
        // RPE
        case 0xE8: { instr.instruction_text = "RPE"; instr.size = 1; break; }
        // JPE a16
        case 0xEA: { uint8_t d1 = mem_read(memory, pc + 1); uint8_t d2 = mem_read(memory, pc + 2); uint16_t addr = (d2 << 8) | d1; if (symbols.count(addr)) { ss << "JPE  " << symbols.at(addr);} else { ss << "JPE  $" << std::setw(4) << std::setfill('0') << addr;} instr.instruction_text = ss.str(); instr.size = 3; break; }
        // XCHG
        case 0xEB: { instr.instruction_text = "XCHG"; instr.size = 1; break; }
        // CPE a16
        case 0xEC: { uint8_t d1 = mem_read(memory, pc + 1); uint8_t d2 = mem_read(memory, pc + 2); uint16_t addr = (d2 << 8) | d1; if (symbols.count(addr)) { ss << "CPE  " << symbols.at(addr);} else { ss << "CPE  $" << std::setw(4) << std::setfill('0') << addr;} instr.instruction_text = ss.str(); instr.size = 3; break; }
        // XRI d8
        case 0xEE: { uint8_t d1 = mem_read(memory, pc + 1); ss << "XRI  #$" << std::setw(2) << std::setfill('0') << (int)d1; instr.instruction_text = ss.str(); instr.size = 2; break; }
        // RST 5
        case 0xEF: { instr.instruction_text = "RST  5"; instr.size = 1; break; }
        // RP
        case 0xF0: { instr.instruction_text = "RP"; instr.size = 1; break; }
        // POP PSW
        case 0xF1: { instr.instruction_text = "POP PSW"; instr.size = 1; break; }
        // JP a16
        case 0xF2: { uint8_t d1 = mem_read(memory, pc + 1); uint8_t d2 = mem_read(memory, pc + 2); uint16_t addr = (d2 << 8) | d1; if (symbols.count(addr)) { ss << "JP  " << symbols.at(addr);} else { ss << "JP  $" << std::setw(4) << std::setfill('0') << addr;} instr.instruction_text = ss.str(); instr.size = 3; break; }
        // DI
        case 0xF3: { instr.instruction_text = "DI"; instr.size = 1; break; }
        // CP a16
        case 0xF4: { uint8_t d1 = mem_read(memory, pc + 1); uint8_t d2 = mem_read(memory, pc + 2); uint16_t addr = (d2 << 8) | d1; if (symbols.count(addr)) { ss << "CP  " << symbols.at(addr);} else { ss << "CP  $" << std::setw(4) << std::setfill('0') << addr;} instr.instruction_text = ss.str(); instr.size = 3; break; }
        // PUSH PSW
        case 0xF5: { instr.instruction_text = "PUSH PSW"; instr.size = 1; break; }
        // ORI d8
        case 0xF6: { uint8_t d1 = mem_read(memory, pc + 1); ss << "ORI  #$" << std::setw(2) << std::setfill('0') << (int)d1; instr.instruction_text = ss.str(); instr.size = 2; break; }
        // RST 6
        case 0xF7: { instr.instruction_text = "RST  6"; instr.size = 1; break; }
        // RM
        case 0xF8: { instr.instruction_text = "RM"; instr.size = 1; break; }
        // SPHL
        case 0xF9: { instr.instruction_text = "SPHL"; instr.size = 1; break; }
        // JM a16
        case 0xFA: { uint8_t d1 = mem_read(memory, pc + 1); uint8_t d2 = mem_read(memory, pc + 2); uint16_t addr = (d2 << 8) | d1; if (symbols.count(addr)) { ss << "JM  " << symbols.at(addr);} else { ss << "JM  $" << std::setw(4) << std::setfill('0') << addr;} instr.instruction_text = ss.str(); instr.size = 3; break; }
        // EI
        case 0xFB: { instr.instruction_text = "EI"; instr.size = 1; break; } 
        // CM a16
        case 0xFC: { uint8_t d1 = mem_read(memory, pc + 1); uint8_t d2 = mem_read(memory, pc + 2); uint16_t addr = (d2 << 8) | d1; if (symbols.count(addr)) { ss << "CM  " << symbols.at(addr);} else { ss << "CM  $" << std::setw(4) << std::setfill('0') << addr;} instr.instruction_text = ss.str(); instr.size = 3; break; }
        // CPI d8
        case 0xFE: { uint8_t d1 = mem_read(memory, pc + 1); ss << "CPI  #$" << std::setw(2) << std::setfill('0') << (int)d1; instr.instruction_text = ss.str(); instr.size = 2; break; }
        // RST 7
        case 0xFF: { instr.instruction_text = "RST  7"; instr.size = 1; break; }
    }

    return instr;
}