
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void readImages(const QString& directory);

private:
    Ui::MainWindow *ui;
    QTableWidget *table;
    QPushButton *sortByNameBtn;
    QPushButton *sortBySizeBtn;
    QPushButton *sortByResolutionBtn;
    QPushButton *sortByDepthBtn;
    QPushButton *sortByCompressionBtn;
    QPushButton *sortByFormatBtn;
    QPushButton *getFilesBtn;
    QPushButton *clearTableBtn;
};

#endif // MAINWINDOW_H
