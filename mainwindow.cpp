#include "mainwindow.h"

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index) {
    std::filesystem::path entry = table->nameDirAt(index);
    TableShowDir(entry);
}

void MainWindow::TableShowDir(const std::filesystem::path &entry) {
    if (std::filesystem::is_directory(entry)) {
        table->clean();
        ShowDirs(entry, false);
        for (int i = 0; i < table->rowCount(); i++) {
            table_view->setRowHeight(i, 24);
        }
    }
    else {
        ;
    }
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    this->resize(1280, 720);

    mainwindow_layout = new QWidget();
    this->setCentralWidget(mainwindow_layout);
    main_layout = new QHBoxLayout();
    mainwindow_layout->setLayout(main_layout);

    table = new Table();
    table_view = new QTableView();
    table_view->setModel(table);
    main_layout->addWidget(table_view);

    Table* table1 = new Table();
    QTableView* table_view1 = new QTableView();
    table_view1->setModel(table1);
    //main_layout->addWidget(table_view1);

    table_view->setColumnWidth(0, 2*table_view->width()-110);
    table_view->setColumnWidth(1, 60);

    connect(table_view, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(on_tableView_doubleClicked(const QModelIndex &)));
    table->clean();

    QHeaderView *VH = new QHeaderView(Qt::Vertical, table_view);
    VH->setMinimumSectionSize(24);
    VH->setMaximumSectionSize(24);
    VH->sectionResizeMode(QHeaderView::Fixed);
    table_view->setVerticalHeader(VH);

    TableShowDir("/");

    //std::filesystem::canonical(dir_entry.path());
}

void MainWindow::ShowDirs(const std::filesystem::path &dir, const bool &SizeDirs) {
    std::filesystem::path p {dir};
    QList<struct Entry> List;

    for (auto const& dir_entry : std::filesystem::directory_iterator{p, std::filesystem::directory_options::skip_permission_denied}) {
        try {
            if (dir_entry.is_regular_file() && dir_entry.path() != "/dev/core" && dir_entry.path() != "/proc/kcore") {
                List.append((struct Entry){QString::fromStdString(dir_entry.path().filename()), dir_entry.path(), dir_entry.file_size()});
            }
            else {
                if (SizeDirs) {
                    List.append((struct Entry){QString::fromStdString(dir_entry.path().filename()), dir_entry.path(), SizeAllInDir(dir_entry.path())});
                }
                else {
                    List.append((struct Entry){QString::fromStdString(dir_entry.path().filename()), dir_entry.path(), 0});
                }

            }
        }  catch (std::filesystem::filesystem_error& e) {
            fprintf(stderr, "Skipped: %s\n", dir_entry.path().c_str());
        }
    }

    std::sort(List.begin(), List.end(), [] (const Entry& l, const Entry &r)
                 {return l.Size > r.Size;});

    table->append((struct Entry){QString::fromStdString(".."), p.parent_path(), 0});

    for (auto it = List.begin(); it != List.end(); ++it) {
        table->append(*it);
    }
}

uintmax_t MainWindow::SizeAllInDir(const std::filesystem::path &dir) {
    std::filesystem::path p {dir};
    uintmax_t max_val = 0;

    for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{p, std::filesystem::directory_options::skip_permission_denied}) {
        try {
            if (dir_entry.is_regular_file() && dir_entry.path() != "/dev/core" && dir_entry.path() != "/proc/kcore") {
                max_val += dir_entry.file_size();
            }
        }  catch (std::filesystem::filesystem_error& e) {
            fprintf(stderr, "Skipped: %s\n", dir_entry.path().c_str());
        }
    }
    return max_val;
}

MainWindow::~MainWindow() {

}

