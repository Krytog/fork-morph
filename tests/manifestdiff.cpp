#include <gtest/gtest.h>

#include <ManifestDiff.h>

using namespace NManifest;

bool IsDiffValid(const Manifest& src, const Manifest& dst, const std::vector<DiffItem>& diff) {
    auto src_iter = src.begin();
    auto dst_iter = dst.begin();
    for (const auto diff_item : diff) {
        switch (diff_item.action) {
            case EAction::DROP: {
                for (auto iter = diff_item.from; iter != diff_item.to; ++iter) {
                    if (src_iter != iter) {
                        return false;
                    }
                    ++src_iter;
                }
                break;
            }
            case EAction::KEEP: {
                for (auto iter = diff_item.from; iter != diff_item.to; ++iter) {
                    if (*src_iter != *iter || *dst_iter != *iter) {
                        return false;
                    }
                    ++src_iter;
                    ++dst_iter;
                }
                break;
            }
            case EAction::INSERT: {
                for (auto iter = diff_item.from; iter != diff_item.to; ++iter) {
                    if (dst_iter != iter) {
                        return false;
                    }
                    ++dst_iter;
                }
                break;
            }
        }
    }
    return src_iter == src.end() && dst_iter == dst.end();
}

TEST(ManifestDiff, Empty) {
    Manifest src;
    Manifest dst;

    const auto diff = GetManifestDiff(src, dst);

    EXPECT_TRUE(IsDiffValid(src, dst, diff));
}

TEST(ManifestDiff, Equal) {
    Manifest src{
        {100, "kek"},
        {250, "lol"},
    };
    Manifest dst = src;

    const auto diff = GetManifestDiff(src, dst);

    EXPECT_TRUE(IsDiffValid(src, dst, diff));
}

TEST(ManifestDiff, Drop) {
    Manifest src{
        {100, "kek"},
        {250, "lol"},
    };
    Manifest dst{
        {250, "lol"},
    };

    const auto diff = GetManifestDiff(src, dst);

    EXPECT_TRUE(IsDiffValid(src, dst, diff));
}

TEST(ManifestDiff, Insert) {
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

    EXPECT_TRUE(IsDiffValid(src, dst, diff));
}

TEST(ManifestDiff, ChangeInMiddle) {
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

    EXPECT_TRUE(IsDiffValid(src, dst, diff));
}

TEST(ManifestDiff, All) {
    Manifest src{
        {100500, "remove me plz"},
        {100, "kek"},
        {250, "lol"},
        {999, "ultra kek"},
    };
    Manifest dst{
        {100, "kek"},
        {250, "cheburek"},
        {3003, "hello world"},
        {999, "ultra kek"},
        {1111111, "one more kek"},
        {1111112, "two more kek"},
        {1111113, "yet another kek"},
    };

    const auto diff = GetManifestDiff(src, dst);

    EXPECT_TRUE(IsDiffValid(src, dst, diff));
}
