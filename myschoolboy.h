#ifndef MYSCHOOLBOY_H
#define MYSCHOOLBOY_H
#include <QString>
#include <QStringList>

class MySchoolboy
{
public:
    enum Field {Name, Locality, School, Level};
    static const int FieldsCount = 4;

private:
    QString fields[FieldsCount];

public:
    MySchoolboy();

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

    QString getName() const
    {
        return fields[Name];
    }

    QString getLocality() const
    {
        return fields[Locality];
    }

    QString getSchool() const
    {
        return fields[School];
    }

    QString getLevel() const
    {
        return fields[Level];
    }
};

#endif // MYSCHOOLBOY_H
