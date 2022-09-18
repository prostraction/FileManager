#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTableView>

#include <QHeaderView>

#include <filesystem>
#include <cmath>
#include <set>
#include <bits/stdc++.h>

#include "table.h"



class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    void ShowDirs(const std::filesystem::path &dir, const bool &SizeDirs);
    uintmax_t SizeAllInDir(const std::filesystem::path &dir);

    void TableShowDir(const std::filesystem::path &entry);

    ~MainWindow();



public slots:
    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    QPushButton *test;

    QWidget*     mainwindow_layout;
    QHBoxLayout* main_layout;

    Table*  table;
    QTableView* table_view;



};
#endif // MAINWINDOW_H
