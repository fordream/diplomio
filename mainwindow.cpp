#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "myopenworkbook.h"
#include "myparsefields.h"
#include <QFileDialog>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    schoolsFile("db.xlsx"),
    templateFile("template.ods"),
    reportsFolder("done"),
    excelBoys(NULL),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStringList subjects;

    subjects << "английскому языку (14.01.2014)";
    subjects << "астрономии (14.01.2014)";
    subjects << "мировой художественной культуре (15.01.2014)";
    subjects << "французскому языку (17.01.2014)";
    subjects << "географии (18.01.2014)";
    subjects << "физике (20.01.2014)";
    subjects << "русскому языку (20.01.2014)";
    subjects << "биологии (22.01.2014)";
    subjects << "истории (23.01.2014)";
    subjects << "основая безопасности жизнедеятельности (24.01.2014)";
    subjects << "обществознанию (27.01.2014)";
    subjects << "литературе (28.01.2014)";
    subjects << "праву (29.01.2014)";
    subjects << "химии (31.01.2014)";
    subjects << "физической культуре (01.02.2014)";
    subjects << "информатике и ИКТ (03.02.2014)";
    subjects << "математике (05.02.2014)";
    subjects << "экологии (07.02.2014)";
    subjects << "немецкому языку (07.02.2014)";
    subjects << "технологии (08.02.2014)";
    subjects << "экономике (08.02.2014)";

    ui->comboBox_2->addItems(subjects);
}

MainWindow::~MainWindow()
{
    delete ui;

    if (excelBoys)
        delete excelBoys;
}

void MainWindow::on_pushButton_clicked()
{
    loadSchools();

    QVector < QStringList > features;

    features.resize(4);
    features[0] << "ФИО" << "Фамилия имя отчество";
    features[1] << "Муниципалитет" << "Район, село" << "Город";
    features[2] << "Школа" << "Учебное заведение";
    features[3] << "Класс";

    excelBoys = new MyOpenXlsx < MySchoolboy > (QFileDialog::getOpenFileName(this, "Файл итогового протокола", QDir::currentPath(), "Microsoft Excel 2010 (*.xlsx)"), new MyOpenWorkbookHelper<MySchoolboy>(features));

    ui->listWidget->setEnabled(true);
    ui->comboBox->setEnabled(true);
    ui->comboBox_2->setEnabled(true);
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(true);
    ui->comboBox->addItems(excelBoys->getTabNames());

    prepareReportsFolder();
}

void MainWindow::updateList(int index)
{
    QVector < MySchoolboy > v = excelBoys->getTabData(index);

    foreach (MySchoolboy boy, v)
    {
        QListWidgetItem *item = new QListWidgetItem(boy.toQString(), ui->listWidget);

        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }
}

MySchool MainWindow::getSchoolByTags(QStringList tags)
{
    MyParseFields parse;

    tags.sort();

    foreach (MySchool school, schools)
    {
        QStringList schoolTags = parse.getTagsForSchool(school.getSchool());

        schoolTags << parse.getTagForLocality(school.getLocality());
        schoolTags.sort();

        if (tags == schoolTags)
            return school;
    }

    return MySchool();
}

QMap<QString, QString> MainWindow::prepareBoyForPrint(MySchoolboy boy)
{
    QMap < QString , QString > result;
    MyParseFields parse;
    QStringList tags = parse.getTagsForName(boy.getName());

    result["surname"] = tags[0];
    result["middlename"] = tags[2];
    result["name"] = tags[1];
    result["level"] = boy.getLevel();

    tags = parse.getTagsForSchool(boy.getSchool());
    tags << parse.getTagForLocality(boy.getLocality());

    tags.sort();

    foreach (QString tag, tags)
        result["tags"] += "[" + tag + "] ";

    MySchool school = getSchoolByTags(tags);
    QStringList lines = school.getTemplate().split("$");

    result["school"] = school.getSchool();

    while (lines.length() < 5)
        lines << "";

    for (int i = 0; i < lines.length(); ++i)
        result["line_" + QString::number(i)] = lines[i];

    QStringList subjectList = QRegularExpression("(.*)\\((\\d{2})\\.(\\d{2})\\.20(\\d{2})\\)").match(ui->comboBox_2->currentText()).capturedTexts();
    QStringList months;

    months << "января" << "февраля" << "марта" << "апреля" << "мая" << "июня" << "июля" << "августа" << "сентября" << "октября" << "ноября" << "декабря";

    if (subjectList[2][0] == '0')
        subjectList[2] = subjectList[2].remove(0, 1);

    result["subject"] = subjectList[1].simplified();
    result["day"] = subjectList[2];
    result["month"] = months[subjectList[3].toInt() - 1];
    result["year"] = subjectList[4];
    result["locality"] = school.getLocality();

    return result;
}

void MainWindow::showReport(const MySchoolboy &boy)
{
    QMap < QString , QString > report =  prepareBoyForPrint(boy);

    ui->label->setText(report["tags"]);
    ui->label_2->setText(QString("Школа: \"%1\" (из %2)").arg(report["school"], QString::number(schools.size())));
    ui->lineEdit->setText(report["surname"]);
    ui->lineEdit_2->setText(report["name"]);
    ui->lineEdit_3->setText(report["middlename"]);
    ui->lineEdit_4->setText(report["level"]);
    ui->lineEdit_5->setText(report["line_0"]);
    ui->lineEdit_6->setText(report["line_1"]);
    ui->lineEdit_7->setText(report["line_2"]);
    ui->lineEdit_8->setText(report["line_3"]);
    ui->lineEdit_9->setText(report["line_4"]);
    ui->lineEdit_10->setText(report["day"]);
    ui->lineEdit_11->setText(report["month"]);
    ui->lineEdit_12->setText(report["year"]);
    ui->lineEdit_13->setText(report["subject"]);
}

void MainWindow::loadSchools()
{
    QVector < QStringList > features;

    features.resize(3);
    features[0] << "Название" << "Сокращенное название";
    features[1] << "Муниципалитет" << "Район, село" << "Город";
    features[2] << "Шаблон";

    schools = MyOpenXlsx < MySchool >(QDir::currentPath() + "/" + schoolsFile, new MyOpenWorkbookHelper<MySchool>(features)).getTabData(0);
}

void MainWindow::prepareReportsFolder()
{
    QDir dir;

    if (dir.cd(reportsFolder))
        dir.removeRecursively();

    dir = QDir();
    dir.mkdir(reportsFolder);
    dir.cd(reportsFolder);

    QStringList tabs = excelBoys->getTabNames();

    foreach (QString tab, tabs)
        dir.mkdir(tab);
}

void MainWindow::processSelectedBoyForPrint(const MySchoolboy &boy)
{
    QTemporaryDir tempDir;

    QString program("unzip");
    QStringList arguments;

    arguments << templateFile;
    arguments << "-d";
    arguments << tempDir.path();

    QProcess *process = new QProcess();

    process->execute(program, arguments);

    QMap < QString , QString > report = prepareBoyForPrint(boy);
    QFile file(tempDir.path() + "/content.xml");

    file.open(QFile::ReadOnly);

    QString text = QTextStream(&file).readAll();

    for (QMap < QString , QString >::const_iterator it = report.constBegin(); it != report.constEnd(); ++it)
        text = text.replace("%%" + it.key() + "%%", it.value());

    file.close();
    file.open(QFile::WriteOnly | QFile::Truncate);

    QTextStream out(&file);

    out << text;
    file.close();

    QString outputFilename = QString("%1 %2 %3.ods").arg(report["surname"], report["name"], report["middlename"]);
    QString outputFile = QString("%1/%2/%3/%4").arg(QDir::currentPath(), reportsFolder, ui->comboBox->currentText(), outputFilename);

    process->execute("sh", QStringList() << "-c" << "cd " + tempDir.path() + " && zip -r \"" + outputFile + "\" .");

    delete process;
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    ui->listWidget->clear();
    updateList(index);
}

void MainWindow::on_listWidget_currentRowChanged(int currentRow)
{
    MySchoolboy boy;

    if (currentRow != -1)
        boy = excelBoys->getBoyFromTabData(ui->comboBox->currentIndex(), currentRow);

    showReport(boy);
}

void MainWindow::on_pushButton_3_clicked()
{
    loadSchools();
}

void MainWindow::on_pushButton_2_clicked()
{
    for (int i = 0; i < ui->listWidget->count(); ++i)
    {
        QListWidgetItem *item = ui->listWidget->item(i);

        if (item->checkState() == Qt::Checked)
            processSelectedBoyForPrint(excelBoys->getBoyFromTabData(ui->comboBox->currentIndex(), i));
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    QFile file(QDir::currentPath() + "/" + ui->comboBox->currentText() + ".txt");
    QVector < MySchoolboy > boys = excelBoys->getTabData(ui->comboBox->currentIndex());

    file.open(QFile::WriteOnly | QFile::Truncate);

    QTextStream stream(&file);

    foreach (MySchoolboy boy, boys)
    {
        QMap < QString , QString > report = prepareBoyForPrint(boy);

        stream << report["surname"] << " " << report["name"] << " "
                                    << report["middlename"] << "\t" <<
                                       report["locality"] << "\t" << report["school"] << "\t\t" << report["level"] << "\n";
    }

    stream << "\n---\n";

    MyParseFields parse;

    foreach (MySchool school, schools)
    {
        stream << "1. " << school.getSchool() << "\n2. " << school.getLocality() << "\n3. " << school.getTemplate() << "\n4. ";
        QStringList items = parse.getTagsForSchool(school.getSchool()) << parse.getTagForLocality(school.getLocality());
        items.sort();

        foreach (QString item, items)
        {
            stream << "[" << item << "] ";
        }

        stream << "\n\n";
    }

    file.close();
}

void MainWindow::on_pushButton_5_clicked()
{
    for (int i = 0; i < ui->listWidget->count(); ++i)
    {
        QListWidgetItem *item = ui->listWidget->item(i);

        if (item->checkState() == Qt::Unchecked)
            item->setCheckState(Qt::Checked);
    }

}
