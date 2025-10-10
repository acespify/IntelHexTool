#pragma once // Prevents the file from being included multiple times

#include <string>
#include <vector>
#include <cstdint>
#include <optional>
#include <map>

// Represents a single parsed line from an Intel HEX file.
struct HexRecord {
    uint8_t byte_count;
    uint16_t address;
    uint8_t record_type;
    std::vector<uint8_t> data;
    uint8_t checksum;
};

// Parses a single line of text into a HexRecord.
std::optional<HexRecord> parse_hex_record(const std::string& line);

// Parses an entire file and returns a vector of valid records.
std::vector<HexRecord> parse_hex_file(const std::string& file_path);

// A new funciton to parse raw binary files
std::map<uint32_t, uint8_t> parse_binary_file(const std::string& file_path, uint32_t base_address);

// Finds the offset of the first non-zero byte in a file.
uint32_t find_rom_start_offset(const std::string& file_path);