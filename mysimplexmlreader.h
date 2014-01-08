#ifndef MYSIMPLEXMLREADER_H
#define MYSIMPLEXMLREADER_H

#include <QFile>
#include <QtXml>
#include "myxmltreeitem.h"
#include "myxmlcontenthandler.h"
#include <QDebug>

class MyCheckTag
{
private:
    QString tag;

public:
    MyCheckTag(const QString &tag_):
        tag(tag_)
    {
    }

    bool operator()(const MyXmlTreeElement &element) const
    {
        return (element.getTag() == tag);
    }
};

class MyCheckAttr
{
private:
    QString key;
    QString value;

public:
    MyCheckAttr(const QString &key_, const QString &value_):
        key(key_),
        value(value_)
    {
    }

    bool operator()(const MyXmlTreeItem &element) const
    {
        return element.getAttr(key) == value;
    }
};

class MySimpleXmlReader
{
private:
    QString filename;
    MyXmlTreeItem *root;

    template < class CheckFunctor >
    void checkAllItems(QVector < MyXmlTreeElement > &result, MyXmlTreeItem *current, CheckFunctor check);
    void getElementsByPathRec(QVector < MyXmlTreeElement > &result, const QStringList &sl, MyXmlTreeItem *current, int pos);

public:
    MySimpleXmlReader(QString filename_);
    ~MySimpleXmlReader();

    MyXmlTreeItem* getRootItem() const;
    QVector < MyXmlTreeElement > getItemsWithTag(const QString &tag);
    QVector < MyXmlTreeElement > getElementsByPath(const QString &path);
};

#endif // MYSIMPLEXMLREADER_H
