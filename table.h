#ifndef TABLE_H
#define TABLE_H

#include <QAbstractTableModel>
#include <cmath>
#include <set>
#include <bits/stdc++.h>

struct Entry {
    QString                 VisibleName;
    std::filesystem::path   RealPath;
    uintmax_t               Size;
};

class Table : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit Table(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    std::filesystem::path nameDirAt(const QModelIndex &index);

    void clean();

    void append(const Entry &t);

private:
    QList<struct Entry> List;
    //QList <QPair<QString, uintmax_t>> FileList;
};

#endif // TABLE_H
