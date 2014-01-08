#ifndef MYXMLCONTENTHANDLER_H
#define MYXMLCONTENTHANDLER_H

#include <QtXml>
#include "myxmltreeitem.h"
#include <QString>
#include <QFile>

class MyXmlContentHandler : public QXmlContentHandler
{
public:
    MyXmlTreeItem *root;
    MyXmlTreeItem *current;
    QString s;

public:
    MyXmlContentHandler();

    bool startDocument();
    bool endDocument();
    void setDocumentLocator(QXmlLocator */*locator*/);
    bool startPrefixMapping(const QString &/*prefix*/, const QString &/*uri*/);
    bool endPrefixMapping(const QString &/*prefix*/);
    bool startElement(const QString &/*namespaceURI*/, const QString &/*localName*/, const QString &qName, const QXmlAttributes &atts);
    bool endElement(const QString &/*namespaceURI*/, const QString &/*localName*/, const QString &/*qName*/);
    bool characters(const QString &ch);
    bool ignorableWhitespace(const QString &/*ch*/);
    bool processingInstruction(const QString &/*target*/, const QString &/*data*/);
    bool skippedEntity(const QString &/*name*/);
    QString errorString() const;
};

#endif // MYXMLCONTENTHANDLER_H
