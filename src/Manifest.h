#pragma once

#include <openssl/sha.h>

#include <array>
#include <vector>

namespace NManifest {

struct Chunk {
    size_t size;
    std::array<unsigned char, SHA256_DIGEST_LENGTH> hash;

    bool operator==(const Chunk& other) const {
        if (size != other.size) {
            return false;
        }
        for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
            if (hash[i] != other.hash[i]) {
                return false;
            }
        }
        return true;
    }
};

using Manifest = std::vector<Chunk>;

}  // namespace NManifest
