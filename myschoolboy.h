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
    QStringList features[FieldsCount];
    QString fields[FieldsCount];

public:
    MySchoolboy();

    bool isFieldFeature (Field field, const QString &text) const;
    int getFieldByFeature(const QString &text) const;
    void setField(Field field, const QString &value);
    QString getField(Field field) const;
    bool isComplete() const;
    QString toQString() const;
};

#endif // MYSCHOOLBOY_H
