#include "myschoolsdata.h"

MySchoolsData::MySchoolsData()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("db.sqlite");
    db.open();
}

QStringList MySchoolsData::getTags() const
{
    QStringList tags;
    QSqlQuery query("SELECT value FROM tags");

    while (query.next())
        tags << query.value(0).toString();

    query.exec("SELECT name FROM localities");

    while (query.next())
        tags << query.value(0).toString();

    return tags;
}
