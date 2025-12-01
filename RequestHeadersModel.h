#ifndef APITESTER_REQUESTHEADERSMODEL_H
#define APITESTER_REQUESTHEADERSMODEL_H
#include <qabstractitemmodel.h>


class RequestHeadersModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit RequestHeadersModel(
        std::vector<std::pair<std::string, std::string>> &m_data,
        QObject *parent = nullptr
        );

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    // Methods for adding/removing rows
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    void addRow(const std::string &data1, const std::string &data2); // A helper function

private:
    std::vector<std::pair<std::string, std::string>> &m_data;
};


#endif //APITESTER_REQUESTHEADERSMODEL_H