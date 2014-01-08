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
    return QString("%1, %2 [%3]").arg(fields[School], fields[Template], fields[Locality]);
}
