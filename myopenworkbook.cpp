#include "myopenworkbook.h"
#include <QTemporaryDir>
#include <QProcess>
#include "mysimplexmlreader.h"
#include <QPair>

MyOpenXlsx::MyOpenXlsx(const QString &fileName_):
    MyOpenWorkbook(fileName_)
{
    extractFile();
    readContentTypes();
}

MyOpenXlsx::~MyOpenXlsx()
{
}

QVector < MySchoolboy > MyOpenXlsx::processFile()
{
    return QVector < MySchoolboy >();
}

void MyOpenXlsx::extractFile()
{
    QString program("unzip");
    QStringList arguments;

    arguments << fileName;
    arguments << "-d";
    arguments << tempDir.path();

    QProcess *process = new QProcess();

    process->execute(program, arguments);


    delete process;
}

void MyOpenXlsx::readContentTypes()
{
    QString stringSharedStrings("application/vnd.openxmlformats-officedocument.spreadsheetml.sharedStrings+xml");
    QString stringWorkbook("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml");
    QString stringSheet("application/vnd.openxmlformats-officedocument.spreadsheetml.worksheet+xml");

    QVector < MyXmlTreeElement > elements = MySimpleXmlReader(tempDir.path() + "/[Content_Types].xml").getElementsByPath("/Types/Override");

    for (int i = 0; i < elements.size(); ++i)
    {
        if (elements[i].getAttr("ContentType") == stringWorkbook)
        {
            QStringList sheets;

            for (int j = 0; j < elements.size(); ++j)
                if (elements[j].getAttr("ContentType") == stringSheet)
                    sheets << elements[j].getAttr("PartName");

            readWorkBook(elements[i].getAttr("PartName"), sheets);
            continue;
        }

        if (elements[i].getAttr("ContentType") == stringSharedStrings)
        {
            readSharedStrings(elements[i].getAttr("PartName"));
            continue;
        }
    }
}

void MyOpenXlsx::readWorkBook(const QString &fileName, QStringList sheetsFileName)
{
    sheetsFileName.sort();

    QVector < MyXmlTreeElement > elements = MySimpleXmlReader(tempDir.path() + fileName).getElementsByPath("/workbook/sheets/sheet");

    for (int i = 0; i < elements.size(); ++i)
        tabNames << " ";
    //tabNames.reserve(elements.size());

    data.resize(elements.size());

    foreach (MyXmlTreeElement e, elements)
    {
        int index = e.getAttr("sheetId").toInt() - 1;

        tabNames[index] = e.getAttr("name");
        readSheet(sheetsFileName[index], index);
    }
}

void MyOpenXlsx::readSheet(const QString &fileName, int index)
{
    QVector < MyXmlTreeElement > elements = MySimpleXmlReader(tempDir.path() + fileName).getElementsByPath("/worksheet/sheetData/row/c/v");
    QVector < QPair < QString , QString > > fieldsCol(MySchoolboy::FieldsCount);

    for (int i = 0; i < elements.size(); ++i)
        if (elements[i].getAttr("t") == "s" && elements[i].getValue() != "")
            elements[i].setValue(sharedStrings[elements[i].getValue().toInt()]);

    for (int i = 0; i < elements.size(); ++i)
    {
        MySchoolboy boy;

        int field = boy.getFieldByFeature(elements[i].getValue());

        if (field != -1)
        {
            if (fieldsCol[field].first == "")
            {
                fieldsCol[field] = QPair < QString , QString >(elements[i].getValue(), elements[i].getAttr("r"));

                bool f = true;

                for (int j = 0; j < fieldsCol.size(); ++j)
                    if (fieldsCol[j] == QPair < QString , QString >())
                    {
                        f = false;
                        break;
                    }

                if (f)
                    break;
            }
        }
    }

    for (int i = 0; i < fieldsCol.size(); ++i)
        if (fieldsCol[i].first == "" || fieldsCol[i].second == "")
            return;

    QVector < MySchoolboy > boys;

    foreach (MyXmlTreeElement e, elements)
    {
        for (int i = 0; i < fieldsCol.size(); ++i)
            if (getColOfCell(fieldsCol[i].second) == getColOfCell(e.getAttr("r")))
            {
                if (getRowOfCell(fieldsCol[i].second) == getRowOfCell(e.getAttr("r")))
                    continue;

                int row = getRowOfCell(e.getAttr("r")) - 1;

                if (boys.size() <= row)
                    boys.resize(row + 1);

                boys[row].setField((MySchoolboy::Field)i, e.getValue());
            }
    }

    foreach (MySchoolboy boy, boys)
        if (boy.isComplete())
            data[index].push_back(boy);
}

void MyOpenXlsx::readSharedStrings(const QString &fileName)
{
    QVector < MyXmlTreeElement > elements = MySimpleXmlReader(tempDir.path() + fileName).getElementsByPath("/sst/si/t");

    foreach (MyXmlTreeElement e, elements)
        sharedStrings.push_back(e.getValue());
}

int MyOpenXlsx::getTabsCount() const
{
    return tabNames.size();
}

QStringList MyOpenXlsx::getTabNames() const
{
    return tabNames;
}


QVector < MySchoolboy > MyOpenXlsx::getTabData(int index) const
{
    return data[index];
}

 MySchoolboy MyOpenXlsx::getBoyFromTabData(int index, int row) const
 {
     return data[index][row];
 }
