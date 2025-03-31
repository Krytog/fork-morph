#pragma once

#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/auth/AWSCredentials.h>

#include <iostream>
#include <string>

inline std::string ReadPartialS3Object(const std::string& url, size_t offset, size_t size) {
    std::string result;
    
    std::string bucket, key;
    
    Aws::S3::Model::GetObjectRequest request;
    request.SetBucket(bucket.c_str());
    request.SetKey(key.c_str());
    
    std::stringstream rangeStream;
    rangeStream << "bytes=" << offset << "-" << (offset + size - 1);
    request.SetRange(rangeStream.str().c_str());
    
    Aws::Client::ClientConfiguration clientConfig;
    clientConfig.endpointOverride = url;
    clientConfig.scheme = Aws::Http::Scheme::HTTP;
    clientConfig.verifySSL = false;
    
    Aws::Auth::AWSCredentials credentials;
    credentials.SetAWSAccessKeyId("minioadmin");
    credentials.SetAWSSecretKey("minioadmin");
    
    Aws::S3::S3Client s3Client(credentials, clientConfig, Aws::Client::AWSAuthV4Signer::PayloadSigningPolicy::Never, false);
    
    auto outcome = s3Client.GetObject(request);
    
    if (outcome.IsSuccess()) {
        auto& stream = outcome.GetResult().GetBody();
        
        auto& contentRange = outcome.GetResult().GetContentRange();
        
        char buffer[4096];
        while (stream.good()) {
            stream.read(buffer, sizeof(buffer));
            auto bytesRead = stream.gcount();
            if (bytesRead > 0) {
                result.insert(result.end(), buffer, buffer + bytesRead);
            }
        }
    } else {
        auto error = outcome.GetError();
        std::cerr << "Ошибка при чтении S3 объекта: " 
                  << error.GetExceptionName() << ": " 
                  << error.GetMessage() << std::endl;
    }
    
    return result;
}

class S3Fetcher {
public:
    S3Fetcher(const std::string& url) : url_(url) {
        Aws::InitAPI(options_);
    }

    std::string ReadChunk(size_t offset, size_t size) {
        return ReadPartialS3Object(url_, offset, size);
    }

    ~S3Fetcher() {
        Aws::ShutdownAPI(options_);
    }

private:
    const std::string url_;
    Aws::SDKOptions options_;
};
