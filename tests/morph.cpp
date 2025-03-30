#include <gtest/gtest.h>

#include <Morph.h>

using namespace NMorph;
using namespace NManifest;

void ComparePlans(const std::vector<ChunkDemand>& lhs, const std::vector<ChunkDemand>& rhs) {
    ASSERT_EQ(lhs.size(), rhs.size());
    for (size_t i = 0; i < lhs.size(); ++i) {
        EXPECT_EQ(lhs[i].source, rhs[i].source);
        EXPECT_EQ(lhs[i].interval.from, rhs[i].interval.from);
        EXPECT_EQ(lhs[i].interval.to, rhs[i].interval.to);
    }
}

TEST(Morph, MorphPlan1) {
    Manifest src{
        {100, "kek"},
        {250, "lol"},
    };
    Manifest dst{
        {250, "lol"},
    };
    const auto diff = GetManifestDiff(src, dst);

    const auto plan = GetMorphPlan(diff);

    const std::vector<ChunkDemand> expected = {
        {100, 350, EDataSource::SRC},
    };
    ComparePlans(plan, expected);
}

TEST(Morph, MorphPlan2) {
    Manifest src{
        {100, "kek"},
        {250, "lol"},
    };
    Manifest dst{
        {100, "kek"},
        {250, "lol"},
        {999, "ultra kek"},
    };
    const auto diff = GetManifestDiff(src, dst);

    const auto plan = GetMorphPlan(diff);

    const std::vector<ChunkDemand> expected = {
        {0, 350, EDataSource::SRC},
        {350, 1349, EDataSource::DST},
    };

    ComparePlans(plan, expected);
}

TEST(Morph, MorphPlan3) {
    Manifest src{
        {100, "kek"},
        {250, "lol"},
        {999, "ultra kek"},
    };
    Manifest dst{
        {100, "kek"},
        {250, "cheburek"},
        {999, "ultra kek"},
    };
    const auto diff = GetManifestDiff(src, dst);

    const auto plan = GetMorphPlan(diff);

    const std::vector<ChunkDemand> expected = {
        {0, 100, EDataSource::SRC},
        {100, 350, EDataSource::DST},
        {350, 1349, EDataSource::SRC},
    };

    ComparePlans(plan, expected);
}

TEST(Morph, MorphPlan4) {
    Manifest src{
        {100500, "remove me plz"},
        {100, "kek"},
        {250, "lol"},
        {999, "ultra kek"},
    };
    Manifest dst{
        {100, "kek"},           {250, "cheburek"},      {3003, "hello world"},     {999, "ultra kek"},
        {1111, "one more kek"}, {1112, "two more kek"}, {1113, "yet another kek"},
    };
    const auto diff = GetManifestDiff(src, dst);

    const auto plan = GetMorphPlan(diff);

    const std::vector<ChunkDemand> expected = {
        {100500, 100600, EDataSource::SRC},
        {100, 3353, EDataSource::DST},
        {100850, 101849, EDataSource::SRC},
        {4352, 7688, EDataSource::DST},
    };

    ComparePlans(plan, expected);
}
