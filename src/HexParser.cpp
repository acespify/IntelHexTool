#include "HexParser.h" // Our header file
#include <fstream>
#include <numeric>
#include <iostream>

// Implementation of the single-line parser.
std::optional<HexRecord> parse_hex_record(const std::string& line) {
    if (line.empty() || line[0] != ':') {
        return std::nullopt;
    }

    std::string hex_string = line.substr(1);
    std::vector<uint8_t> bytes_list;
    try {
        for (size_t i = 0; i < hex_string.length(); i += 2) {
            bytes_list.push_back(static_cast<uint8_t>(std::stoul(hex_string.substr(i, 2), nullptr, 16)));
        }
    } catch (...) {
        return std::nullopt; // Invalid hex characters
    }

    if (bytes_list.size() < 5) {
        return std::nullopt; // Record is too short
    }
    
    // The sum of all bytes, including the checksum, should have its lower 8 bits as zero.
    uint8_t checksum_verify = std::accumulate(bytes_list.begin(), bytes_list.end(), 0);
    if (checksum_verify != 0) {
        std::cerr << "Warning: Checksum mismatch on line: " << line << std::endl;
        // We can still parse it but warn the user.
    }
    
    HexRecord record;
    record.byte_count = bytes_list[0];

    // Check if the actual data length matches the byte count.
    if (bytes_list.size() != (size_t)record.byte_count + 5) {
        return std::nullopt; // Malformed record
    }

    record.address = (bytes_list[1] << 8) | bytes_list[2]; // Combine high and low address bytes
    record.record_type = bytes_list[3];
    record.checksum = bytes_list.back();
    
    // Extract the data payload.
    record.data.assign(bytes_list.begin() + 4, bytes_list.end() - 1);

    return record;
}

// Implementation of the full-file parser.
std::vector<HexRecord> parse_hex_file(const std::string& file_path) {
    std::vector<HexRecord> records;
    std::ifstream file(file_path);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << file_path << std::endl;
        return records;
    }

    while (std::getline(file, line)) {
        // Trim whitespace just in case
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (auto record = parse_hex_record(line)) {
            records.push_back(*record);
        }
    }

    return records;
}

// A new function to parse raw binary files
std::map<uint32_t, uint8_t> parse_binary_file(const std::string& file_path, uint32_t base_address) {
    std::map<uint32_t, uint8_t> data_map;
    
    // Open the file in binary mode at the end to get its size
    std::ifstream file(file_path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open binary file: " << file_path << std::endl;
        return data_map; 
    }

    //std::streamsize size = file.tellg();
    file.seekg(base_address, std::ios::beg);

    char byte_buffer = 0;
    uint32_t current_address = base_address;
    while (file.get(byte_buffer)){
        data_map[current_address] = static_cast<uint8_t>(byte_buffer);
        current_address++;
    }
    /*
    // Read the whole file into a buffer
    std::vector<char> buffer(size);
    if (file.read(buffer.data(), size)) {
        // Populate the memory map with the file data
        for (int i = 0; i < size; ++i) {
            data_map[base_address + i] = static_cast<uint8_t>(buffer[i]);
        }
    }*/
    
    return data_map;
}

uint32_t find_rom_start_offset(const std::string& file_path) {
    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open()) {
        return 0; // Return 0 if file can't be opened.
    }

    uint32_t offset = 0;
    char byte = 0;
    while (file.get(byte)) {
        if (byte != 0x00) {
            // Found the first non-zero byte, this is our starting offset.
            return offset;
        }
        offset++;
    }

    // If the file is all zeros ore empty, return 0.
    return 0;
}
