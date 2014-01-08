#ifndef MYSCHOOL_H
#define MYSCHOOL_H
#include <QString>
#include <QStringList>

class MySchool
{
public:
    enum Field {School, Locality, Template};
    static const int FieldsCount = 3;

private:
    QString fields[FieldsCount];

public:
    MySchool();

    void setField(Field field, const QString &value);

    void setField(int index, const QString &value)
    {
        setField((Field)index, value);
    }

    QString getField(Field field) const;

    QString getField(int index) const
    {
        return getField((Field)index);
    }

    QString toQString() const;
};
#endif // MYSCHOOL_H
