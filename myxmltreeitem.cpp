#include "myxmltreeitem.h"

MyXmlTreeItem::~MyXmlTreeItem()
{
    for (int i = 0; i < childrens.size(); ++i)
        delete childrens[i];

    parent = NULL;
}

MyXmlTreeItem::MyXmlTreeItem(MyXmlTreeItem *parent_):
    parent(parent_)
{
}

MyXmlTreeItem* MyXmlTreeItem::createNewChildren()
{
    MyXmlTreeItem *item = new MyXmlTreeItem(this);

    pushChildren(item);
    return item;
}

void MyXmlTreeItem::pushChildren(MyXmlTreeItem *children)
{
    childrens.push_back(children);
}

QVector < MyXmlTreeItem* > MyXmlTreeItem::getChildrens() const
{
    return childrens;
}

int MyXmlTreeItem::getChildrensCount() const
{
    return childrens.size();
}

MyXmlTreeItem* MyXmlTreeItem::getChildrenAt(int index) const
{
    return childrens[index];
}

MyXmlTreeItem* MyXmlTreeItem::getParent() const
{
    return parent;
}

bool MyXmlTreeItem::isRoot() const
{
    return parent == NULL;
}
