#pragma once

#include <string>
#include <fstream>

class FileReader {
public:
    explicit FileReader(const std::string& filename);

    [[nodiscard]] size_t GetSize() const noexcept;

    [[nodiscard]] std::string ReadChunk(size_t offset, size_t bytes_count);

private:
    void CalculateAndSetFileSize();

    std::string filename_;
    std::ifstream file_;
    size_t size_;
};
