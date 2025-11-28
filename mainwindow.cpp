#include "mainwindow.h"

#include <iostream>

#include "./ui_mainwindow.h"
#include "HttpClient.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    m_model = new RequestHeadersModel({});
    ui->setupUi(this);

    // ui->requestHeaders_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->requestHeaders_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->requestHeaders_tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->responseHeaders_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->responseHeaders_tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(ui->requestAddHeader_pushButton,
        &QPushButton::clicked,
        this,
        &MainWindow::onAddRowClicked);

    connect(ui->requestRemoveHeader_pushButton,
        &QPushButton::clicked,
        this,
        &MainWindow::onRemoveRowClicked);

    connect(ui->requestSend_pushButton,
        &QPushButton::clicked,
        this,
        &MainWindow::sendRequest);

    ui->requestHeaders_tableView->setModel(m_model);

}

void MainWindow::onAddRowClicked() {
    m_model->addRow("New Entry", "");

    ui->requestHeaders_tableView->scrollToBottom();
}

void MainWindow::onRemoveRowClicked() {
    QModelIndexList selected_rows = ui->requestHeaders_tableView->selectionModel()->selectedRows();

    if (!selected_rows.isEmpty()) {
        int row_to_remove = selected_rows.first().row();

        m_model->removeRows(row_to_remove, 1);
    }
}

void MainWindow::sendRequest() {
    std::cout << "Method: " << ui->requestMethod_comboBox->currentText().toStdString() << '\n';
    std::cout << "URL: " <<  ui->requestUrl_lineEdit->text().toStdString() << '\n';
    auto headers = m_model->get_data();

    for (const auto& [value, name] : headers) {
        std::cout << value << ": " << name << '\n';
    }

    std::cout << "Body: " << ui->requestBody_plainTextEdit->toPlainText().toStdString() << '\n';

    ui->tabWidget->setCurrentIndex(1);

    HttpClient client;
    auto result = client.sendRequest(
        ui->requestMethod_comboBox->currentText().toStdString(),
        ui->requestUrl_lineEdit->text().toStdString(),
        m_model->get_data(),
        ui->requestBody_plainTextEdit->toPlainText().toStdString()
        );

    std::cout << result.httpCode << std::endl;
    std::cout << result.data << std::endl;

    ui->responseBody_plainTextEdit->setPlainText(QString::fromStdString(result.data));

    const QItemSelectionModel *m = ui->responseHeaders_tableView->selectionModel();
    ui->responseHeaders_tableView->setModel(new RequestHeadersModel(result.headers));
    delete m;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_model;
}
