#ifndef APITESTER_MAINWINDOW_HPP
#define APITESTER_MAINWINDOW_HPP

#include <QMainWindow>

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
    ~MainWindow();

    void onAddRowClicked();
    void onRemoveRowClicked();

    void sendRequest();

private:
    Ui::MainWindow *ui;
    RequestHeadersModel *m_model;
};

#endif //APITESTER_MAINWINDOW_HPP