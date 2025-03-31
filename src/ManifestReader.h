#pragma once

#include <Manifest.h>

#include <string>

namespace NManifest {

[[nodiscard]] Manifest ReadManifestFromFile(const std::string& filename);

}
