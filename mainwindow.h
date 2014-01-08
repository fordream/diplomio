#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QStringListModel>
#include <QMainWindow>
#include <QStandardItemModel>
#include "myopenworkbook.h"
#include <QListWidgetItem>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_listWidget_clicked(const QModelIndex &index);

private:
    QStandardItemModel *model;
     MyOpenWorkbook *excel;
    Ui::MainWindow *ui;

    void updateList(int index);
};

#endif // MAINWINDOW_H
