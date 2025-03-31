#include <Manifest.h>
#include <ManifestReader.h>
#include <ManifestDiff.h>
#include <Morph.h>
#include <DataProviders.h>

#include <memory>
#include <iostream>
#include <fstream>

const int kSrcManifestArgIndex = 1;
const int kSrcBlobArgIndex = 2;
const int kDstManifestArgIndex = 3;
const int kDstBlobArgIndex = 4;
const int kOutBloblArgIndex = 5;

namespace {

std::unique_ptr<IChunkProvider> GetSrcBlobProvider(const std::string& path) {
    return std::make_unique<FileChunkProvider>(path);
}

std::unique_ptr<IChunkProvider> GetDstBlobProvider(const std::string& path) {
    if (path.starts_with("S3@")) {
        return std::make_unique<S3ChunkProvider>(path.substr(3));
    }
    return std::make_unique<FileChunkProvider>(path);
}

}  // namespace

int main(int argc, char** argv) {
    if (argc != 6) {
        std::cout << "Usage: ./fork-morph <path-to-src-manifest> <path-to-src-blob> "
                     "<path-to-dst-manifest> <path-to-dst-blob> <path-to-output-blob>"
                  << std::endl;
        std::cout << "Path-to-dst-blob can be either local filepath or S3 url, "
                     "starting with S3 (for example, S3@http://localhost:9090/bucket/object)"
                  << std::endl;
        return 1;
    }

    try {
        const NManifest::Manifest src_manifest = NManifest::ReadManifestFromFile(argv[kSrcManifestArgIndex]);
        const NManifest::Manifest dst_manifest = NManifest::ReadManifestFromFile(argv[kDstManifestArgIndex]);

        const auto diff = NManifest::GetManifestDiff(src_manifest, dst_manifest);
        const auto plan = NMorph::GetMorphPlan(diff);

        const auto src_blob_provider = GetSrcBlobProvider(argv[kSrcBlobArgIndex]);
        const auto dst_blob_provider = GetDstBlobProvider(argv[kDstBlobArgIndex]);

        std::ofstream output(argv[kOutBloblArgIndex]);
        NMorph::MorphBlob(plan, src_blob_provider.get(), dst_blob_provider.get(), output);

    } catch (const std::exception& err) {
        std::cerr << "ERROR! WHAT: " << err.what() << std::endl;
        return 1;
    }

    return 0;
}
