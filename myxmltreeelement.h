#ifndef MYXMLTREEELEMENT_H
#define MYXMLTREEELEMENT_H

#include <QString>
#include <QMap>
#include <algorithm>
#include <QStringList>

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

    void prependToAttr(const QString &key, const QString &value)
    {
        attrs[key] = value + " " + attrs[key];
    }

    int getCompareCount(const MyXmlTreeElement &element)
    {
        QStringList sl1 = getAttr("ufo").split(" ");
        QStringList sl2 = element.getAttr("ufo").split(" ");

        sl1.removeDuplicates();
        sl2.removeDuplicates();

        int len = std::min(sl1.size(), sl2.size());

        for (int i = 0; i < len; ++i)
            if (sl1[i] != sl2[i])
                return i;

        return len;
    }
};

#endif // MYXMLTREEELEMENT_H
