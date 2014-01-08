#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "myschool.h"
#include <QStringListModel>
#include <QMainWindow>
#include <QStandardItemModel>
#include "myopenworkbook.h"
#include <QListWidgetItem>
#include "myschoolsdata.h"

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

    void on_listWidget_activated(const QModelIndex &index);

    void on_pushButton_2_clicked();

private:
    QStandardItemModel *model;
    MyOpenWorkbook <MySchoolboy> *excel;
    MyOpenWorkbook <MySchool> *excel2;
    Ui::MainWindow *ui;
    MySchoolsData schoolsData;

    void updateList(int index);
};

#endif // MAINWINDOW_H
