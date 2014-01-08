#ifndef MYXMLTREEELEMENT_H
#define MYXMLTREEELEMENT_H

#include <QString>
#include <QMap>

class MyXmlTreeElement
{
public:
    typedef QMap < QString , QString > AttrsMap;

protected:
    QString value;
    QString tag;
//    QMap < QString , QString > attrs;
    AttrsMap attrs;

public:
    MyXmlTreeElement();
    MyXmlTreeElement(const MyXmlTreeElement &copy);

    void pushAttr(const QString &key, const QString &value);
    QString getAttr(const QString &key) const;
    QMap < QString , QString > getAttrs() const;
    void setValue(const QString &value_);
    QString getValue() const;
    void setTag(const QString &tag_);
    QString getTag() const;
    bool findAttr(const QString &key) const;
};

#endif // MYXMLTREEELEMENT_H
