#pragma once

#include <Manifest.h>

#include <vector>

namespace NManifest {

enum class EAction {
    DROP,
    KEEP,
    INSERT,
};

struct DiffItem {
    EAction action;
    Manifest::const_iterator from;
    Manifest::const_iterator to;
};

/* DiffItems contain iterators, so Manifests have to live as long as DiffItems do */
[[nodiscard]] std::vector<DiffItem> GetManifestDiff(const Manifest& src, const Manifest& dst);

}  // namespace NManifest
