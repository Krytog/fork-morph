#pragma once

#include <openssl/sha.h>

#include <array>
#include <vector>

namespace NManifest {

struct Chunk {
    size_t size;
    std::array<unsigned char, SHA256_DIGEST_LENGTH> hash;
};

using Manifest = std::vector<Chunk>;

}  // namespace NManifest
