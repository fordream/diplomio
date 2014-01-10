#include "myschool.h"


MySchool::MySchool()
{
}

void MySchool::setField(Field field, const QString &value)
{
    fields[field] = value;
}

QString MySchool::getField(Field field) const
{
    return fields[field];
}

QString MySchool::toQString() const
{
    if (fields[School] == "" || fields[Locality] == "")
        return "";
    else
        return QString("%1, %2").arg(fields[School], fields[Locality]);
}
