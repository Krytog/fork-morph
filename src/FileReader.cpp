#include "FileReader.h"

#include <stdexcept>

FileReader::FileReader(const std::string& filename)
    : filename_(filename), file_(std::ifstream(filename, std::ios::binary)) {
    if (!file_.is_open()) {
        throw std::runtime_error("FileReader exception: " + filename_ + " failed to open file");
    }
    CalculateAndSetFileSize();
}

void FileReader::CalculateAndSetFileSize() {
    const auto begin = file_.tellg();
    file_.seekg(0, std::ios::end);
    const auto end = file_.tellg();
    size_ = end - begin;
}

size_t FileReader::GetSize() const noexcept {
    return size_;
}

std::string FileReader::ReadChunk(size_t offset, size_t bytes_count) {
    file_.seekg(offset);
    if (!file_.good()) {
        throw std::runtime_error("FileReader exception: " + filename_ + std::string("failed to seekg to offset ") + std::to_string(offset));
    }

    const size_t chunk_size = std::min(bytes_count, size_ - offset);
    std::string output;
    output.resize(chunk_size);
    file_.read(output.data(), chunk_size);

    if (file_.eof()) {
        file_.clear();  // discarding eof flag so we can read the file multiple times
    }

    return output;
}
