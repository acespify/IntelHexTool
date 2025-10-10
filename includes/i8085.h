/**********************************************************
 * This class inherits from the i8080, this is due to 
 * the 8085 is a subset of the 8080. 
 * Author: Andrew Young
 * Date: September 26, 2025
 * 
 * This tool was created to be able to disassemble hex 
 * and text files that are in a intel hex format in
 * order to properly configure the 8080/85 emulator
 * that I am working on. AceSpify
 *********************************************************/
#pragma once

#include "i8080.h"

class Disassembler8085 : public Disassembler8080 {
    public:
        DisassembledInstruction disassemble_op(const MemoryMap& memory, uint32_t pc, const SymbolMap& symbols) override;
};
