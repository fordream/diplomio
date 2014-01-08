#ifndef MYOPENWORKBOOK_H
#define MYOPENWORKBOOK_H

#include <QString>
#include <QVector>
#include <QTemporaryDir>
#include <myschoolboy.h>
#include <qdebug.h>

class MyOpenWorkbook
{
protected:
    QString fileName;
    QStringList tabNames;
    QVector < QVector < MySchoolboy > > data;


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
    MyOpenWorkbook(const QString &fileName_)
    {
        fileName = fileName_;
    }

    virtual MySchoolboy getBoyFromTabData(int index, int row) const = 0;
    virtual QVector < MySchoolboy > getTabData(int index) const = 0;
    virtual int getTabsCount() const = 0;
    virtual QStringList getTabNames() const = 0;
    virtual QVector < MySchoolboy > processFile() = 0;
    virtual ~MyOpenWorkbook()
    {
    }

};

class MyOpenXlsx: public MyOpenWorkbook
{
private:
    QTemporaryDir tempDir;
    QVector < QString > sharedStrings;

    void extractFile();

public:
    MyOpenXlsx(const QString &fileName_);
    ~MyOpenXlsx();

    QVector < MySchoolboy > processFile();


    MySchoolboy getBoyFromTabData(int index, int row) const;
    QVector < MySchoolboy > getTabData(int index) const;
    int getTabsCount() const;
    QStringList getTabNames() const;
    void readContentTypes();
    void readWorkBook(const QString &fileName, QStringList sheetsFileName);
    void readSheet(const QString &fileName, int index);
    void readSharedStrings(const QString &fileName);
};

#endif // MYOPENWORKBOOK_H
