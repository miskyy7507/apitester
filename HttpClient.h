#ifndef APITESTER_HTTPCLIENT_H
#define APITESTER_HTTPCLIENT_H

#include <string>
#include <vector>
#include <curl/curl.h>

class HttpClient {
public:
    HttpClient();
    ~HttpClient();

    struct HttpResponse {
        long httpCode = 0;
        std::string data;
        std::string error;
        std::vector<std::pair<std::string, std::string>> headers;
    };

    HttpResponse sendRequest(
        const std::string& method,
        const std::string& url,
        const std::vector<std::pair<std::string, std::string>>& headers,
        const std::string& postData = ""
    );

private:
    CURL* curl_handle;

    /**
     * @brief Static callback function required by libcurl to write received data.
     *
     * @param contents Pointer to the data buffer.
     * @param size Size of one data chunk.
     * @param nmemb Number of chunks.
     * @param userp Pointer to the string where data will be appended (our HttpResponse::data).
     * @return The total number of bytes processed.
     */
    static size_t write_callback(const char* contents, size_t size, size_t nmemb, void* userp);
};


#endif //APITESTER_HTTPCLIENT_H