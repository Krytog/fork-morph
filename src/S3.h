#include <iostream>
#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <vector>
#include <string>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/auth/AWSCredentials.h>

// Функция для чтения частичных данных из S3 объекта
std::vector<uint8_t> readPartialS3Object(const std::string& url, size_t offset, size_t size) {
    // Инициализируем контейнер для результата
    std::vector<uint8_t> result;
    
    // Разбираем URL на составляющие (bucket и key)
    // Пример: s3://my-bucket/path/to/object.txt
    std::string bucket, key;
    
    if (url.substr(0, 5) == "s3://") {
        std::string s3Url = url.substr(5); // Удаляем "s3://"
        size_t slashPos = s3Url.find('/');
        
        if (slashPos != std::string::npos) {
            bucket = s3Url.substr(0, slashPos);
            key = s3Url.substr(slashPos + 1);
        } else {
            std::cerr << "Некорректный URL формат: " << url << std::endl;
            return result;
        }
    } else {
        std::cerr << "URL должен начинаться с s3://" << std::endl;
        return result;
    }
    
    // Создаем запрос к S3
    Aws::S3::Model::GetObjectRequest request;
    request.SetBucket(bucket.c_str());
    request.SetKey(key.c_str());
    
    // Устанавливаем диапазон байтов для чтения, формат: "bytes=START-END"
    std::stringstream rangeStream;
    rangeStream << "bytes=" << offset << "-" << (offset + size - 1);
    request.SetRange(rangeStream.str().c_str());
    
    // Настройка клиента S3 для работы с Minio
    Aws::Client::ClientConfiguration clientConfig;
    clientConfig.endpointOverride = "http://localhost:9000"; // Укажите ваш адрес Minio
    clientConfig.scheme = Aws::Http::Scheme::HTTP; // Или HTTPS, если настроен SSL
    clientConfig.verifySSL = false; // Отключаем проверку SSL для локального тестирования
    
    // Настройка учетных данных
    Aws::Auth::AWSCredentials credentials;
    credentials.SetAWSAccessKeyId("minioadmin"); // имя пользователя
    credentials.SetAWSSecretKey("minioadmin");    // пароль
    
    // Создаем клиента S3 с пользовательскими настройками
    Aws::S3::S3Client s3Client(credentials, clientConfig, Aws::Client::AWSAuthV4Signer::PayloadSigningPolicy::Never, false);
    
    // Выполняем запрос
    auto outcome = s3Client.GetObject(request);
    
    if (outcome.IsSuccess()) {
        // Получаем поток данных
        auto& stream = outcome.GetResult().GetBody();
        
        // Определяем размер полученных данных
        auto& contentRange = outcome.GetResult().GetContentRange();
        
        // Читаем данные
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

void example() {
    // Инициализация AWS SDK
    Aws::SDKOptions options;
    Aws::InitAPI(options);
    
    {
        // Пример использования функции
        std::string s3Url = "s3://mybucket/myfile.txt";
        size_t offset = 1;
        size_t size = 3;
        
        std::vector<uint8_t> data = readPartialS3Object(s3Url, offset, size);
        
        std::cout << "Прочитано " << data.size() << " байт из S3 объекта" << std::endl;
        
        // Дополнительная обработка данных...
    }
    
    // Очистка AWS SDK
    Aws::ShutdownAPI(options);
}
