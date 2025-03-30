#include "ManifestDiff.h"

#include <diff/dmp_diff.hpp>

namespace NManifest {

EAction CastToAction(Operation operation) {
    switch (operation) {
        case Operation::DELETE: {
            return EAction::DROP;
        }
        case Operation::EQUAL: {
            return EAction::KEEP;
        }
        case Operation::INSERT: {
            return EAction::INSERT;
        }
    }
    return EAction::KEEP;
}

std::vector<DiffItem> GetManifestDiff(const Manifest& src, const Manifest& dst) {
    MyersDiff<Manifest> diff(src, dst);
    std::vector<DiffItem> output;
    output.reserve(diff.end() - diff.begin());
    for (const auto& entry : diff) {
        output.emplace_back(CastToAction(entry.operation), entry.text.from, entry.text.till);
    }
    return output;
}

}  // namespace NManifest
