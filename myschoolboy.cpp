#include "myschoolboy.h"

MySchoolboy::MySchoolboy()
{
}

void MySchoolboy::setField(Field field, const QString &value)
{
    fields[field] = value;
}

QString MySchoolboy::getField(Field field) const
{
    return fields[field];
}

QString MySchoolboy::toQString() const
{
    return QString("%1, %2 класс, (%3, %4)").arg(fields[Name], fields[Level], fields[School], fields[Locality]);
}
