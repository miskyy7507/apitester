#include "HttpClient.h"

#include <sstream>
#include <curl/curl.h>

HttpClient::HttpClient() {
    curl_handle = curl_easy_init();
    if (!curl_handle) {
        throw std::runtime_error("Failed to initialize curl handle.");
    }
}

HttpClient::~HttpClient() {
    curl_easy_cleanup(curl_handle);
}

HttpClient::HttpResponse HttpClient::sendRequest(
    const std::string &method, const std::string &url,
    const std::vector<std::pair<std::string, std::string>> &headers,
    const std::string &postData
) {
    HttpResponse response;
    curl_slist *headers_slist = nullptr;

    // enable verbose logging for debugging
    curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
    // This function handles the response body data
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response.data);

    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());

    // set method options
    curl_easy_setopt(curl_handle, CURLOPT_CUSTOMREQUEST, method.c_str());

    // set post data, if present
    if (!postData.empty()) {
        curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, postData.c_str());
        curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE, postData.length());
    }

    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "apitester/0.1.0");

    // set headers
    for (const auto& header : headers) {
        std::string headerString = header.first + ": " + header.second;
        headers_slist = curl_slist_append(headers_slist, headerString.c_str());
    }
    curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers_slist);

    // send the request
    CURLcode res = curl_easy_perform(curl_handle);

    // process result
    if (res != CURLE_OK) {
        std::ostringstream errorStream;
        errorStream << "curl_easy_perform() failed: " << curl_easy_strerror(res);
        response.error = errorStream.str();
    } else {
        curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &response.httpCode);

        curl_header *header;
        curl_header *header_prev = nullptr;

        while ((header = curl_easy_nextheader(curl_handle, CURLH_HEADER, -1, header_prev))) {
            response.headers.emplace_back(header->name, header->value);
            header_prev = header;
        }
    }

    // cleanup
    curl_slist_free_all(headers_slist);
    curl_easy_reset(curl_handle);

    return response;
}

size_t HttpClient::write_callback(char *contents, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    static_cast<std::string*>(userp)->append(contents, total_size);
    return total_size;
}
