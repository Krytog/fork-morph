#pragma once

#include <Manifest.h>
#include <ManifestDiff.h>
#include <IChunkProvider.h>

#include <cstddef>
#include <vector>
#include <ostream>

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

void MorphBlob(const std::vector<ChunkDemand>& plan, IChunkProvider* src, IChunkProvider* dst,
               std::ostream& out);

}  // namespace NMorph
