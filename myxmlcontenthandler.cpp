#include "myxmlcontenthandler.h"

MyXmlContentHandler::MyXmlContentHandler()
{
    root = new MyXmlTreeItem(NULL);
    current = root;
}

bool MyXmlContentHandler::startDocument()
{
    return true;
}

bool MyXmlContentHandler::endDocument()
{
    return true;
}

void MyXmlContentHandler::setDocumentLocator(QXmlLocator */*locator*/)
{
}

bool MyXmlContentHandler::startPrefixMapping(const QString &/*prefix*/, const QString &/*uri*/)
{
    return true;
}

bool MyXmlContentHandler::endPrefixMapping(const QString &/*prefix*/)
{
    return true;
}

bool MyXmlContentHandler::startElement(const QString &/*namespaceURI*/, const QString &/*localName*/, const QString &qName, const QXmlAttributes &atts)
{
    current = current->createNewChildren();
    current->setTag(qName);

    for (int i = 0; i < atts.count(); ++i)
        current->pushAttr(atts.qName(i), atts.value(i));

    return true;
}

bool MyXmlContentHandler::endElement(const QString &/*namespaceURI*/, const QString &/*localName*/, const QString &/*qName*/)
{
    current = current->getParent();
    return true;
}

bool MyXmlContentHandler::characters(const QString &ch)
{
    current->setValue(ch);
    return true;
}

bool MyXmlContentHandler::ignorableWhitespace(const QString &/*ch*/)
{
    return true;
}

bool MyXmlContentHandler::processingInstruction(const QString &/*target*/, const QString &/*data*/)
{
    return true;
}

bool MyXmlContentHandler::skippedEntity(const QString &/*name*/)
{
    return false;
}

QString MyXmlContentHandler::errorString() const
{
    return "";
}
