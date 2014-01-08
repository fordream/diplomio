#ifndef MYSCHOOLSDATA_H
#define MYSCHOOLSDATA_H
#include <QSql>
#include <QSqlDatabase>
#include <QString>
#include <QStringList>
#include <QSqlQuery>
#include <QVariant>

class MySchoolsData
{
private:
    QSqlDatabase db;

public:
    MySchoolsData();

    QStringList getTags() const;

};

#endif // MYSCHOOLSDATA_H
