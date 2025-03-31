#include "DataProviders.h"

// #include <S3.h>

#include <utility>
#include <cstring>

namespace {

std::pair<size_t, size_t> GetSizeAndOffset(size_t from, size_t to) {
    return {from, to - from};
}

}

FileChunkProvider::FileChunkProvider(const std::string& filename) : file_reader_(filename) {
}

std::string FileChunkProvider::GetChunk(size_t from, size_t to) {
    const auto [offset, size] = GetSizeAndOffset(from, to);
    return file_reader_.ReadChunk(offset, size);
}

/*

S3ChunkProvider::S3ChunkProvider(const std::string& url) : url_(url) {
}

std::string S3ChunkProvider::GetChunk(size_t from, size_t to) {
    const auto [offset, size] = GetSizeAndOffset(from, to);
    const auto chunk = readPartialS3Object(url_, offset, size);
    std::string output;
    output.resize(chunk.size());
    std::memcpy(chunk.data(), output.data(), chunk.size());
    return output;
}

*/
