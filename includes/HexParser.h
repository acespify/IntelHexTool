#pragma once // Prevents the file from being included multiple times

#include <string>
#include <vector>
#include <cstdint>
#include <optional>

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
