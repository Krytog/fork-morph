#pragma once

#include <IChunkProvider.h>
#include <FileReader.h>
#include <S3.h>

#include <string>

class FileChunkProvider : public IChunkProvider {
public:
    explicit FileChunkProvider(const std::string& filename);

    [[nodiscard]] std::string GetChunk(size_t from, size_t to) override;

private:
    FileReader file_reader_;
};


class S3ChunkProvider : public IChunkProvider {
public:
    explicit S3ChunkProvider(const std::string& url);

    [[nodiscard]] std::string GetChunk(size_t from, size_t to) override;

private:
    S3Fetcher fetcher_;
};
