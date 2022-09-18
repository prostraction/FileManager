#include "table.h"

Table::Table(QObject *parent): QAbstractTableModel(parent) {

}

QVariant Table::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
            case 0: return "Name";
            case 1: return "Size";
        }
    }
    return QVariant();
}

int Table::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return List.count();
}

int Table::columnCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return 2;
}

QVariant Table::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        if (index.row() < List.count()) {
            switch (index.column()) {
                case 0: return List.at(index.row()).VisibleName;
                case 1: {
                    int i{};
                    double mantissa = List.at(index.row()).Size;
                    for (; mantissa >= 1024.; mantissa /= 1024., ++i) { }
                    mantissa = std::ceil(mantissa * 10.) / 10.;

                    std::stringstream ss;
                    ss << mantissa << " " << "BKMGTPE"[i];
                    std::string myString = ss.str();
                    return QString::fromStdString(myString);
                }
            }
        }
        return QVariant();
    }
    return QVariant();
}

std::filesystem::path Table::nameDirAt(const QModelIndex &index) {
    if (index.row() < List.count()) {
        return List.at(index.row()).RealPath;
    }
    return "";
}

void Table::append(const Entry &t) {
    beginInsertRows(QModelIndex(), List.count(), List.count());
    List.append(t);
    endInsertRows();
}

void Table::clean() {
    beginResetModel();
    List.clear();
    endResetModel();
}
