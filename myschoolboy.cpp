#include "myschoolboy.h"

MySchoolboy::MySchoolboy()
{
    features[Name] << "ФИО" << "Фамилия имя отчество";
    features[Locality] << "Муниципалитет" << "Район, село" << "Город";
    features[School] << "Школа" << "Учебное заведение";
    features[Level] << "Класс";
}

bool MySchoolboy::isFieldFeature (Field field, const QString &text) const
{
    return !features[field].filter(text.simplified(), Qt::CaseInsensitive).empty();
}

int MySchoolboy::getFieldByFeature(const QString &text) const
{
    for (int i = 0; i < FieldsCount; ++i)
        if (isFieldFeature((Field)i, text))
            return i;

    return -1;
}

void MySchoolboy::setField(Field field, const QString &value)
{
    fields[field] = value;
}

QString MySchoolboy::getField(Field field) const
{
    return fields[field];
}

bool MySchoolboy::isComplete() const
{
    for (int i = 0; i < FieldsCount; ++i)
        if (fields[(Field)i] == "")
            return false;

    return true;
}

QString MySchoolboy::toQString() const
{
    return QString("%1, %2 класс, (%3, %4)").arg(fields[Name], fields[Level], fields[School], fields[Locality]);
}
