#pragma once

#include <string>

class IChunkProvider {
public:

    [[nodiscard]] virtual std::string GetChunk(size_t from, size_t to) = 0;

    virtual ~IChunkProvider() = default;
};
