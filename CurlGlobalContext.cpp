#include "CurlGlobalContext.h"

#include <iostream>
#include <stdexcept>
#include <curl/curl.h>

CurlGlobalContext::CurlGlobalContext() {
    auto c = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (c != CURLE_OK) {
        throw std::runtime_error("Failed to initialize libcurl");
    }

    curl_version_info_data *curl_data = curl_version_info(CURLVERSION_NOW);
    std::cout << "libcurl " << curl_data->version << " initialized\n";
}

CurlGlobalContext::~CurlGlobalContext() {
    curl_global_cleanup();
}
