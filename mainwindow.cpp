#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "myopenworkbook.h"
#include "myparsefields.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete excel;
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

    /*
        this->features[0] << "ФИО" << "Фамилия имя отчество";
        this->features[1] << "Муниципалитет" << "Район, село" << "Город";
        this->features[2] << "Школа" << "Учебное заведение";
        this->features[3] << "Класс";

        this->features[0] << "Название" << "Сокращенное название";
        this->features[1] << "Муниципалитет" << "Район, село" << "Город";
        this->features[2] << "Шаблон";
     * */
    QVector < QStringList > features;

    features.resize(4);
    features[0] << "ФИО" << "Фамилия имя отчество";
    features[1] << "Муниципалитет" << "Район, село" << "Город";
    features[2] << "Школа" << "Учебное заведение";
    features[3] << "Класс";

    excel = new MyOpenXlsx < MySchoolboy > (QDir::currentPath() + "/file2.xlsx", new MyOpenWorkbookHelper<MySchoolboy>(features));

    features[0].clear();
    features[1].clear();
    features[2].clear();

    features[0] << "Название" << "Сокращенное название";
    features[1] << "Муниципалитет" << "Район, село" << "Город";
    features[2] << "Шаблон";

    features.pop_back();



    excel2 = new MyOpenXlsx < MySchool > (QDir::currentPath() + "/bd.xlsx", new MyOpenWorkbookHelper<MySchool>(features));
    ui->listWidget->setEnabled(true);
    ui->comboBox->setEnabled(true);

    QStringList list = excel2->getTabNames();

    ui->comboBox->addItems(list);


}

void MainWindow::updateList(int index)
{
    QVector < MySchool > v = excel2->getTabData(index);

    foreach (MySchool boy, v)
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
/*
    MySchoolboy boy = excel->getBoyFromTabData(ui->comboBox->currentIndex(), index.row());

    QStringList l = boy.getField(MySchoolboy::Name).split(" ");
    ui->lineEdit->setText(l[0]);
    ui->lineEdit_2->setText(l[1]);
    ui->lineEdit_3->setText(l[2]);

    MyParseFields parse;
    QString label;
    QStringList tags = parse.getTagsForSchool(boy.getField(MySchoolboy::School));

    foreach (QString tag, tags)
        label += "[" + tag + "] ";

    ui->label->setText(label);
    */
}

void MainWindow::on_listWidget_activated(const QModelIndex &index)
{
    MySchool boy = excel2->getBoyFromTabData(ui->comboBox->currentIndex(), index.row());
/*
    QStringList l = boy.getField(MySchoolboy::Name).split(" ");
    ui->lineEdit->setText(l[0]);
    ui->lineEdit_2->setText(l[1]);
    ui->lineEdit_3->setText(l[2]);
*/
    MyParseFields parse;
    QString label;
//    QStringList tags = parse.getTagsForSchool(boy.getField(MySchoolboy::School));
    QStringList tags = parse.getTagsForSchool(boy.getField(MySchool::School));

    tags << parse.getTagForLocality(boy.getField(MySchool::Locality));

    foreach (QString tag, tags)
        label += "[" + tag + "] ";

    ui->label->setText(label);
}

void MainWindow::on_pushButton_2_clicked()
{

}
