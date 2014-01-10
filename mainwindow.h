#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "myschool.h"
#include <QStringListModel>
#include <QMainWindow>
#include <QStandardItemModel>
#include "myopenworkbook.h"
#include <QListWidgetItem>
#include <QMap>

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
    void on_listWidget_currentRowChanged(int currentRow);
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();

private:
    QString schoolsFile;
    QString templateFile;
    QString reportsFolder;
    MyOpenWorkbook <MySchoolboy> *excelBoys;
    QVector < MySchool > schools;
    Ui::MainWindow *ui;

    void updateList(int index);
    MySchool getSchoolByTags(QStringList tags);
    QMap < QString , QString > prepareBoyForPrint(MySchoolboy boy);
    void showReport(const MySchoolboy &boy);
    void loadSchools();
    void prepareReportsFolder();
    void processSelectedBoyForPrint(const MySchoolboy &boy);
};

#endif // MAINWINDOW_H
