#pragma once

#include <Manifest.h>

#include <vector>

namespace NManifest {

/* Struct represents an interval [from, to) */
struct Interval {
    size_t from;
    size_t to;
};

[[nodiscard]] std::vector<Interval> GetMismatchingIntervals(const Manifest& src, const Manifest& dst);

}  // namespace NManifest
