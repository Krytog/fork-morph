#include "ManifestReader.h"

#include <stdexcept>
#include <sstream>
#include <fstream>
#include <iostream>

namespace NManifest {

Manifest ReadManifestFromFile(const std::string& filename) {
    Manifest manifest;
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.front() == '[' || line.front() == ']') {
            continue;
        }

        std::erase(line, '"');
        std::erase(line, ',');

        const auto separator_pos = line.find_first_of(':');
        if (separator_pos == std::string::npos) {
            throw std::runtime_error("No separator in line: " + line);
        }
        std::string size_as_str = line.substr(0, separator_pos);
        std::string hash_as_str = line.substr(separator_pos + 1);

        Chunk chunk;
        chunk.size = std::stoull(size_as_str);
        if (hash_as_str.length() != SHA256_DIGEST_LENGTH * 2) {
            throw std::runtime_error("Invalid hash length in line: " + line);
        }

        for (size_t i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
            std::istringstream hexByte(hash_as_str.substr(i * 2, 2));
            int byte;
            hexByte >> std::hex >> byte;
            chunk.hash[i] = static_cast<unsigned char>(byte);
        }

        manifest.push_back(chunk);
    }

    return manifest;
}

}  // namespace NManifest
