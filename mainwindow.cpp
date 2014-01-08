#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "myopenworkbook.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
/*    QListWidgetItem *item = new QListWidgetItem("ewe", ui->listWidget);

    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(Qt::Unchecked);


    ui->comboBox->addItem("fsdffadsdfsd");

    for (int i = 0; i < ui->listWidget->count(); ++i)
    {
        QListWidgetItem *item = ui->listWidget->item(i);
        qDebug() << item->flags() << Qt::Checked << item->text() << item->checkState();
    }*/
    ui->listWidget->clear();
    ui->comboBox->clear();
    ui->pushButton->setEnabled(false);
   excel = new MyOpenXlsx(QDir::currentPath() + "/file2.xlsx");

    ui->listWidget->setEnabled(true);
    ui->comboBox->setEnabled(true);

    QStringList list = excel->getTabNames();

    ui->comboBox->addItems(list);


}

void MainWindow::updateList(int index)
{
    QVector < MySchoolboy > v = excel->getTabData(index);

    foreach (MySchoolboy boy, v)
    {
        QListWidgetItem *item = new QListWidgetItem(boy.toQString(), ui->listWidget);

        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);

    }
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    ui->listWidget->clear();
    updateList(index);
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{/*
    QString text = item->text();
    QStringList l = text.split(" ");
    ui->lineEdit->setText(l[0]);
    ui->lineEdit_2->setText(l[1]);
    ui->lineEdit_3->setText(l[2]);*/
}

void MainWindow::on_listWidget_clicked(const QModelIndex &index)
{
    MySchoolboy boy = excel->getBoyFromTabData(ui->comboBox->currentIndex(), index.row());

    QStringList l = boy.getField(MySchoolboy::Name).split(" ");
    ui->lineEdit->setText(l[0]);
    ui->lineEdit_2->setText(l[1]);
    ui->lineEdit_3->setText(l[2]);
}