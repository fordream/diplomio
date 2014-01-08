#ifndef MYXMLTREEITEM_H
#define MYXMLTREEITEM_H
#include <QtXml>
#include <QMap>
#include <QString>
#include <QVector>
#include "myxmltreeelement.h"

class MyXmlTreeItem: public MyXmlTreeElement
{
private:
    QVector < MyXmlTreeItem* > childrens;
    MyXmlTreeItem *parent;

public:
    MyXmlTreeItem(MyXmlTreeItem *parent_);
    ~MyXmlTreeItem();

    MyXmlTreeItem* createNewChildren();
    void pushChildren(MyXmlTreeItem *children);
    QVector < MyXmlTreeItem* > getChildrens() const;
    int getChildrensCount() const;
    MyXmlTreeItem* getChildrenAt(int index) const;
    MyXmlTreeItem* getParent() const;
    bool isRoot() const;
};
#endif // MYXMLTREEITEM_H
