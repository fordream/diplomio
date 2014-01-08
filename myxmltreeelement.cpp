#include "myxmltreeelement.h"

MyXmlTreeElement::MyXmlTreeElement()
{
}

MyXmlTreeElement::MyXmlTreeElement(const MyXmlTreeElement &copy)
{
    tag = copy.tag;
    value = copy.value;
    attrs = copy.attrs;
}

void MyXmlTreeElement::pushAttr(const QString &key, const QString &value)
{
    attrs[key] = value;
}

QString MyXmlTreeElement::getAttr(const QString &key) const
{
    return attrs[key];
}

QMap < QString , QString > MyXmlTreeElement::getAttrs() const
{
    return attrs;
}

void MyXmlTreeElement::setValue(const QString &value_)
{
    value = value_;
}

QString MyXmlTreeElement::getValue() const
{
    return value;
}

void MyXmlTreeElement::setTag(const QString &tag_)
{
    tag = tag_;
}

QString MyXmlTreeElement::getTag() const
{
    return tag;
}

bool MyXmlTreeElement::findAttr(const QString &key) const
{
    return (attrs.find(key) != attrs.end());
}
