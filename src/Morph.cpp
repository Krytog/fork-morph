#include "Morph.h"

namespace NMorph {

std::vector<ChunkDemand> GetOptimizedPlan(const std::vector<ChunkDemand>& plan) {
    std::vector<ChunkDemand> output;
    for (size_t i = 0; i < plan.size(); ++i) {
        size_t ptr = i + 1;
        while (ptr < plan.size() && plan[ptr - 1].source == plan[ptr].source &&
               plan[ptr - 1].interval.to == plan[ptr].interval.from) {
            ++ptr;
        }
        output.emplace_back(Interval{plan[i].interval.from, plan[ptr - 1].interval.to}, plan[i].source);
        i = ptr - 1;
    }
    return output;
}

std::vector<ChunkDemand> GetMorphPlan(const std::vector<NManifest::DiffItem>& diff) {
    std::vector<ChunkDemand> plan;
    size_t src_offset = 0;
    size_t dst_offset = 0;
    for (const auto diff_item : diff) {
        switch (diff_item.action) {
            case NManifest::EAction::DROP: {
                for (auto iter = diff_item.from; iter != diff_item.to; ++iter) {
                    src_offset += iter->size;
                }
                break;
            }
            case NManifest::EAction::KEEP: {
                const auto from = src_offset;
                for (auto iter = diff_item.from; iter != diff_item.to; ++iter) {
                    src_offset += iter->size;
                    dst_offset += iter->size;
                }
                plan.emplace_back(Interval{from, src_offset}, EDataSource::SRC);
                break;
            }
            case NManifest::EAction::INSERT: {
                const auto from = dst_offset;
                for (auto iter = diff_item.from; iter != diff_item.to; ++iter) {
                    dst_offset += iter->size;
                }
                plan.emplace_back(Interval{from, dst_offset}, EDataSource::DST);
                break;
            }
        }
    }
    const auto optimized_plan = GetOptimizedPlan(plan);
    return optimized_plan;
}

void MorphBlob(const std::vector<ChunkDemand>& plan, IChunkProvider* src, IChunkProvider* dst,
               std::ostream& out) {
    for (const auto item : plan) {
        IChunkProvider* data_provider = nullptr;
        switch (item.source) {
            case EDataSource::SRC: {
                data_provider = src;
                break;
            }
            case EDataSource::DST: {
                data_provider = dst;
                break;
            }
        }
        const auto data = data_provider->GetChunk(item.interval.from, item.interval.to);
        out << data;
    }
}

}  // namespace NMorph
