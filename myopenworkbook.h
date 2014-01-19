#ifndef MYOPENWORKBOOK_H
#define MYOPENWORKBOOK_H

#include <QString>
#include <QVector>
#include <QTemporaryDir>
#include <myschoolboy.h>
#include <qdebug.h>

template < class T >
class MyOpenWorkbookHelper
{
protected:
    int fieldsCount;
    QVector < QStringList > features;

public:
    MyOpenWorkbookHelper(const QVector < QStringList > &features_):
        features(features_)
    {
    }

    bool isFieldFeature(int index, const QString &text) const
    {
        return !this->features[index].filter(text.simplified(), Qt::CaseInsensitive).empty();
    }

    int getFieldByFeature(const QString &text) const
    {
        for (int i = 0; i < features.size(); ++i)
            if (isFieldFeature(i, text))
                return i;

        return -1;
    }

    bool isComplete(const T &t) const
    {
        for (int i = 0; i < features.size(); ++i)
            if (t.getField(i)  == "")
                return false;

        return true;
    }

    int getFieldsCount() const
    {
        return features.size();
    }
};

template < class T >
class MyOpenWorkbook
{
protected:
    QString fileName;
    QStringList tabNames;
    QVector < QVector < T > > data;
    MyOpenWorkbookHelper < T > *helper;

protected:
    int getRowOfCell(const QString &cell) const
    {
        QString row;

        foreach (QChar ch, cell)
            if (ch.isDigit())
                row.push_back(ch);

        return row.toInt();
    }

    QString getColOfCell(const QString &cell) const
    {
        QString column;

        foreach (QChar ch, cell)
            if (ch.isLetter())
                column.push_back(ch);

        return column;
    }
public:
    MyOpenWorkbook(const QString &fileName_, MyOpenWorkbookHelper < T > *helper_)
    {
        fileName = fileName_;
        helper = helper_;
    }

    virtual T getBoyFromTabData(int index, int row) const = 0;
    virtual QVector < T > getTabData(int index) const = 0;
    virtual int getTabsCount() const = 0;
    virtual QStringList getTabNames() const = 0;

    virtual ~MyOpenWorkbook()
    {
        delete helper;
    }

};

template < class T >
class MyOpenXlsx: public MyOpenWorkbook<T>
{
private:
    QTemporaryDir tempDir;
    QVector < QString > sharedStrings;

    void extractFile();

public:
    MyOpenXlsx(const QString &fileName_, MyOpenWorkbookHelper < T > *helper_);
    ~MyOpenXlsx();

    T getBoyFromTabData(int index, int row) const;
    QVector < T > getTabData(int index) const;
    int getTabsCount() const;
    QStringList getTabNames() const;
    void readContentTypes();
    void readWorkBook(const QString &fileName, QStringList sheetsFileName);
    void readSheet(const QString &fileName, int index);
    void readSharedStrings(const QString &fileName);
};

/*******************/
#include "myopenworkbook.h"
#include <QTemporaryDir>
#include <QProcess>
#include "mysimplexmlreader.h"
#include <QPair>

template < class T >
MyOpenXlsx<T>::MyOpenXlsx(const QString &fileName_, MyOpenWorkbookHelper < T > *helper_):
    MyOpenWorkbook<T>(fileName_, helper_)
{
    extractFile();
    readContentTypes();
}

template < class T >
MyOpenXlsx<T>::~MyOpenXlsx()
{
}

template < class T >
void MyOpenXlsx<T>::extractFile()
{
    QString program("unzip");
    QStringList arguments;

    arguments << this->fileName;
    arguments << "-d";
    arguments << tempDir.path();

    QProcess *process = new QProcess();

    process->execute(program, arguments);

    delete process;
}

template < class T >
void MyOpenXlsx<T>::readContentTypes()
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

template < class T >
void MyOpenXlsx<T>::readWorkBook(const QString &fileName, QStringList sheetsFileName)
{
    sheetsFileName.sort();

    QVector < MyXmlTreeElement > elements = MySimpleXmlReader(tempDir.path() + fileName).getElementsByPath("/workbook/sheets/sheet");

    for (int i = 0; i < elements.size(); ++i)
        this->tabNames << " ";

    this->data.resize(elements.size());

    foreach (MyXmlTreeElement e, elements)
    {
        int index = e.getAttr("sheetId").toInt() - 1;

        this->tabNames[index] = e.getAttr("name");
        readSheet(sheetsFileName[index], index);
    }
}

template < class T >
void MyOpenXlsx<T>::readSheet(const QString &fileName, int index)
{
    QVector < MyXmlTreeElement > elements = MySimpleXmlReader(tempDir.path() + fileName).getElementsByPath("/worksheet/sheetData/row/c/v");
    QVector < QPair < QString , QString > > fieldsCol(this->helper->getFieldsCount());

    qDebug() << elements.size() << sharedStrings.size();
    for (int i = 0; i < elements.size(); ++i)
    {
        if (elements[i].getAttr("t") == "s" && elements[i].getValue() != "")
        {
            if (elements[i].getValue().toInt() < sharedStrings.size())
                elements[i].setValue(sharedStrings[elements[i].getValue().toInt()]);
        }
    }

    qDebug() << "olo";
    for (int i = 0; i < elements.size(); ++i)
    {
        int field = this->helper->getFieldByFeature(elements[i].getValue());

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

    QVector < T > boys;

    foreach (MyXmlTreeElement e, elements)
    {
        for (int i = 0; i < fieldsCol.size(); ++i)
            if (this->getColOfCell(fieldsCol[i].second) == this->getColOfCell(e.getAttr("r")))
            {
                if (this->getRowOfCell(fieldsCol[i].second) == this->getRowOfCell(e.getAttr("r")))
                    continue;

                int row = this->getRowOfCell(e.getAttr("r")) - 1;

                if (boys.size() <= row)
                    boys.resize(row + 1);

                boys[row].setField(i, e.getValue());
            }
    }

    foreach (T boy, boys)
        if (this->helper->isComplete(boy))
            this->data[index].push_back(boy);
}

template < class T >
void MyOpenXlsx<T>::readSharedStrings(const QString &fileName)
{
    QVector < MyXmlTreeElement > elements = MySimpleXmlReader(tempDir.path() + fileName).getElementsByPath("/sst/si/t");

    foreach (MyXmlTreeElement e, elements)
        sharedStrings.push_back(e.getValue());
}

template < class T >
int MyOpenXlsx<T>::getTabsCount() const
{
    return this->tabNames.size();
}

template < class T >
QStringList MyOpenXlsx<T>::getTabNames() const
{
    return this->tabNames;
}

template < class T >
QVector < T > MyOpenXlsx<T>::getTabData(int index) const
{
    return this->data[index];
}


template < class T >
T MyOpenXlsx<T>::getBoyFromTabData(int index, int row) const
{
    return this->data[index][row];
}



#endif // MYOPENWORKBOOK_H
