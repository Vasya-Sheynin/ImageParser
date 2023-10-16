
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    table = new QTableWidget(0,6);
    sortByNameBtn = new QPushButton("Sort by name");
    sortBySizeBtn = new QPushButton("Sort by size");
    sortByResolutionBtn = new QPushButton("Sort by resolution");
    sortByDepthBtn = new QPushButton("Sort by color depth");
    sortByFormatBtn = new QPushButton("Sort by format");
    sortByCompressionBtn = new QPushButton("Sort by compression");
    getFilesBtn = new QPushButton("Select images");
    clearTableBtn = new QPushButton("Clear table");

    QStringList lst;
    lst << "Name" << "Format" << "Size" << "Resolution" << "Color depth" << "Compression";
    table->setHorizontalHeaderLabels(lst);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    QGridLayout *grid = new QGridLayout;

    grid->addWidget(getFilesBtn, 2, 0, 1, 3);
    grid->addWidget(clearTableBtn, 2, 3, 1, 3);
    grid->addWidget(sortByNameBtn, 1, 0, 1, 2);
    grid->addWidget(sortByFormatBtn, 1, 2, 1, 2);
    grid->addWidget(sortBySizeBtn, 1, 4, 1, 2);
    grid->addWidget(sortByResolutionBtn, 1, 6, 1, 2);
    grid->addWidget(sortByDepthBtn, 1, 8, 1, 2);
    grid->addWidget(sortByCompressionBtn, 1, 10, 1, 2);
    grid->addWidget(table, 0, 0, 1, 12);

    ui->centralwidget->setLayout(grid);

    connect(getFilesBtn, &QPushButton::clicked, this, [=](){
        QFileDialog dialog;
        dialog.setFileMode(QFileDialog::ExistingFiles);
        dialog.setDirectory(QDir::homePath());
        dialog.setNameFilter("All Files (*);;JPG Files (*.jpg);;GIF Files (*.gif);;TIFF Files (*.tiff);;BMP Files (*.bmp);;PNG Files (*.png);;PCX Files (*.pcx)");

        QStringList fileNames;
        if (dialog.exec())
        {
            fileNames = dialog.selectedFiles();
            foreach (QString fileName, fileNames)
            {
                readImages(fileName);
            }
        }
    });

    connect(clearTableBtn, &QPushButton::clicked, this, [=](){ table->setRowCount(0); });

    connect(sortByCompressionBtn, &QPushButton::clicked, this, [=](){
        table->sortItems(5);
    });

    connect(sortByDepthBtn, &QPushButton::clicked, this, [=](){
        table->sortItems(4);
    });

    connect(sortByNameBtn, &QPushButton::clicked, this, [=](){
        table->sortItems(0);
    });

    connect(sortByFormatBtn, &QPushButton::clicked, this, [=](){
        for (int i = 0; i < table->rowCount() - 1; i++)
        {
            for (int j = 0; j < table->rowCount() - 1; j++)
            {
                if (table->item(j, 1)->text().toLower() > table->item(j + 1, 1)->text().toLower())
                {
                    for (int k = 0; k < table->columnCount(); k++)
                    {
                        QString tmp = table->item(j, k)->text();
                        table->item(j, k)->setText(table->item(j + 1, k)->text());
                        table->item(j + 1, k)->setText(tmp);
                    }
                }
            }
        }
    });

    connect(sortByResolutionBtn, &QPushButton::clicked, this, [=](){
        for (int i = 0; i < table->rowCount() - 1; i++)
        {
            for (int j = 0; j < table->rowCount() - 1; j++)
            {

                int res = table->item(j, 3)->text().remove(table->item(j, 3)->text().length() - 4, 4).toInt();
                int resNext = table->item(j + 1, 3)->text().remove(table->item(j + 1, 3)->text().length() - 4, 4).toInt();

                if (res > resNext)
                {
                    for (int k = 0; k < table->columnCount(); k++)
                    {
                        QString tmp = table->item(j, k)->text();
                        table->item(j, k)->setText(table->item(j + 1, k)->text());
                        table->item(j + 1, k)->setText(tmp);
                    }
                }
            }
        }
    });

    connect(sortBySizeBtn, &QPushButton::clicked, this, [=](){
        for (int i = 0; i < table->rowCount() - 1; i++)
        {
            for (int j = 0; j < table->rowCount() - 1; j++)
            {
                QString x = table->item(j, 2)->text().mid(0, table->item(j, 2)->text().toStdString().find_first_of(" px"));
                QString y = table->item(j, 2)->text().remove(0, 6 + x.length());
                y = y.mid(0, y.toStdString().find_first_of(" px"));

                QString xNext = table->item(j + 1, 2)->text().mid(0, table->item(j + 1, 2)->text().toStdString().find_first_of(" px"));
                QString yNext = table->item(j + 1, 2)->text().remove(0, 6 + xNext.length());
                yNext = yNext.mid(0, yNext.toStdString().find_first_of(" px"));

                if (x.toInt() > xNext.toInt() || (x.toInt() == xNext.toInt() && y.toInt() > yNext.toInt()))
                {
                    for (int k = 0; k < table->columnCount(); k++)
                    {
                        QString tmp = table->item(j, k)->text();
                        table->item(j, k)->setText(table->item(j + 1, k)->text());
                        table->item(j + 1, k)->setText(tmp);
                    }
                }
            }
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readImages(const QString &directory)
{
    QDir dir(directory);
    QStringList filters;

    QImageReader reader(directory);
    QImage image = reader.read();

    QString fileName = QFileInfo(dir.absolutePath()).fileName();
    QSize imageSize = image.size();

    int dpiX = qRound(image.dotsPerMeterX() / 39.3701);
    int dpiY = qRound(image.dotsPerMeterY() / 39.3701);
    int dpi = (dpiX + dpiY) / 2;

    int depth = image.bitPlaneCount();
    int compression = QImageWriter(directory).compression();

    int dotPos = fileName.toStdString().find_last_of(".");
    QString fmt = QString::fromStdString(fileName.toStdString().substr(dotPos + 1, fileName.length() - dotPos - 1));

    table->insertRow(table->rowCount());
    table->setItem(table->rowCount() - 1, 0, new QTableWidgetItem(fileName));
    table->setItem(table->rowCount() - 1, 1, new QTableWidgetItem(fmt));
    table->setItem(table->rowCount() - 1, 2, new QTableWidgetItem(QString::number(imageSize.height()) + " px × " + QString::number(imageSize.width()) + " px"));
    table->setItem(table->rowCount() - 1, 3, new QTableWidgetItem(QString::number(dpi) + " dpi"));
    table->setItem(table->rowCount() - 1, 4, new QTableWidgetItem(QString::number(depth)));
    table->setItem(table->rowCount() - 1, 5, new QTableWidgetItem(compression < 0 ? "None" : QString::number(compression)));

    for (int i = 0; i < table->columnCount(); i++)
    {
        table->item(table->rowCount() - 1, i)->setTextAlignment(Qt::AlignCenter);
    }
}


