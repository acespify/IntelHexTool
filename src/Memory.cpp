#include "Memory.h"

MemoryMap build_memory_map(const std::vector<HexRecord>& records) {
    MemoryMap memory;
    uint32_t high_address = 0; // Stores the upper 16 bits of the address

    for (const auto& record : records) {
        switch (record.record_type) {
            case 0x00: { // Data Record
                uint32_t current_address = high_address + record.address;
                for (uint8_t byte : record.data) {
                    memory[current_address++] = byte;
                }
                break;
            }
            case 0x02: { // Extended Segment Address Record (rarely used, but good to have)
                high_address = ((record.data[0] << 8) | record.data[1]) << 4;
                break;
            }
            case 0x04: { // Extended Linear Address Record
                high_address = ((record.data[0] << 8) | record.data[1]) << 16;
                break;
            }
            // Other record types (01, 03, 05) don't contain data for the memory map
            default:
                break;
        }
    }
    return memory;
}
