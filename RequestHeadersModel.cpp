#include "RequestHeadersModel.h"

#include <iostream>
#include <qcolor.h>

RequestHeadersModel::RequestHeadersModel(std::vector<std::pair<std::string, std::string>> &m_data,
                                         QObject *parent)
        : QAbstractTableModel(parent)
        , m_data(m_data)
{}

int RequestHeadersModel::rowCount(const QModelIndex &) const {
    return static_cast<int>(m_data.size());
}

int RequestHeadersModel::columnCount(const QModelIndex &) const {
    return 2; // two columns in std::pair
}

QVariant RequestHeadersModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_data.size())
        return QVariant();

    const auto &row_data = m_data.at(index.row());

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        // const auto &row_data = m_data.at(index.row());
        switch (index.column()) {
            case 0: return QString::fromStdString(row_data.first);
            case 1: return QString::fromStdString(row_data.second);
            default: break;
        }
    }
    if (role == Qt::BackgroundRole) {
        if (row_data.second.empty()) {
            return QColor::fromRgb(255, 0, 0);
        }
    }
    return QVariant();
}

QVariant RequestHeadersModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0: return "Name";
            case 1: return "Value";
            default: break;
        }
    }
    return QVariant();
}

Qt::ItemFlags RequestHeadersModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return QAbstractTableModel::flags(index);

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

bool RequestHeadersModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();
        int col = index.column();

        if (row >= 0 && row < m_data.size()) {
            auto &row_data = m_data[row];
            bool success = false;

            if (col == 0) {
                row_data.first = value.toString().toStdString();
                success = true;
            } else if (col == 1) {
                row_data.second = value.toString().toStdString();
                success = true;
            }

            if (success) {
                // Notify the view that data has changed
                emit dataChanged(index, index, {Qt::EditRole});
                return true;
            }
        }
    }

    return false;
}

// *** For Adding Rows ***
bool RequestHeadersModel::insertRows(int row, int count, const QModelIndex &parent) {
    Q_UNUSED(parent);
    if (row < 0 || row > m_data.size() || count <= 0)
        return false;

    beginInsertRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        // Insert default data
        m_data.insert(m_data.begin() + row, {"foo", "bar"});
    }
    endInsertRows();
    return true;
}

void RequestHeadersModel::addRow(const std::string &data1, const std::string &data2) {
    int new_row = m_data.size();
    if (insertRows(new_row, 1)) {
        // The insertRows implementation above adds a default row.
        // We now update it with the specific values.
        m_data[new_row] = {data1, data2};
        emit dataChanged(index(new_row, 0), index(new_row, 1), {Qt::DisplayRole, Qt::EditRole});
    }
}

// *** For Removing Rows ***
bool RequestHeadersModel::removeRows(int row, int count, const QModelIndex &parent) {
    Q_UNUSED(parent);
    if (row < 0 || row + count > m_data.size() || count <= 0)
        return false;

    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        m_data.erase(m_data.begin() + row); // removeAt adjusts for subsequent removals
    }
    endRemoveRows();
    return true;
}