#pragma once

#include <Manifest.h>
#include <ManifestDiff.h>

#include <cstddef>
#include <vector>

namespace NMorph {

/* Represents an interval [from, to) */
struct Interval {
    size_t from;
    size_t to;
};

enum class EDataSource {
    SRC,
    DST,
};

struct ChunkDemand {
    Interval interval;
    EDataSource source;
};

[[nodiscard]] std::vector<ChunkDemand> GetMorphPlan(const std::vector<NManifest::DiffItem>& diff);

}  // namespace NMorph
