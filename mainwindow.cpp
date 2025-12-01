#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "HttpClient.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    request_headers  = {};
    response_headers = {};
    m_model = new RequestHeadersModel(request_headers);
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

void MainWindow::onAddRowClicked() const {
    m_model->addRow("New Entry", "");

    ui->requestHeaders_tableView->scrollToBottom();
}

void MainWindow::onRemoveRowClicked() const {
    QModelIndexList selected_rows = ui->requestHeaders_tableView->selectionModel()->selectedRows();

    if (!selected_rows.isEmpty()) {
        int row_to_remove = selected_rows.first().row();

        m_model->removeRows(row_to_remove, 1);
    }
}

void MainWindow::sendRequest() {
    ui->tabWidget->setCurrentIndex(1);


    auto result = main_client.sendRequest(
        ui->requestMethod_comboBox->currentText().toStdString(),
        ui->requestUrl_lineEdit->text().toStdString(),
        request_headers,
        ui->requestBody_plainTextEdit->toPlainText().toStdString()
        );

    this->response_headers = std::move(result.headers);

    ui->responseBody_plainTextEdit->setPlainText(QString::fromStdString(result.data));

    auto *newModel = new RequestHeadersModel(this->response_headers);

    auto *oldModel = ui->responseHeaders_tableView->model();
    ui->responseHeaders_tableView->setModel(newModel);

    delete oldModel;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_model;
    delete ui->responseHeaders_tableView->model();
}
