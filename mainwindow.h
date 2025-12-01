#ifndef APITESTER_MAINWINDOW_HPP
#define APITESTER_MAINWINDOW_HPP

#include <QMainWindow>

#include "HttpClient.h"
#include "RequestHeadersModel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void onAddRowClicked() const;
    void onRemoveRowClicked() const;

    void sendRequest();

private:
    Ui::MainWindow *ui;
    RequestHeadersModel *m_model;
    HttpClient main_client;
    std::vector<std::pair<std::string, std::string>> request_headers;
    std::vector<std::pair<std::string, std::string>> response_headers;
};

#endif //APITESTER_MAINWINDOW_HPP