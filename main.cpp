#include <iostream>

#include "mainwindow.h"

#include <curl/curl.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
        std::cerr << "Failed to initialize libcurl. Exiting.\n";
        curl_global_cleanup();
        return 1;
    }
    curl_version_info_data *curl_data = curl_version_info(CURLVERSION_NOW);
    std::cout << "libcurl " << curl_data->version << " initialized\n";

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    int qt_app_return = a.exec();

    curl_global_cleanup();
    return qt_app_return;
}
