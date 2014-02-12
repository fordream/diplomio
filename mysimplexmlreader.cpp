#include "mysimplexmlreader.h"

MySimpleXmlReader::MySimpleXmlReader(QString filename_):
    filename(filename_)
{
    QXmlSimpleReader xmlReader;
    QXmlContentHandler *handler = new MyXmlContentHandler();

    xmlReader.setContentHandler(handler);
    QXmlInputSource *source = new QXmlInputSource(new QFile(filename));

    if (!xmlReader.parse(source))
        root = NULL;
    else
        root = ((MyXmlContentHandler*)handler)->root;
}

MySimpleXmlReader::~MySimpleXmlReader()
{
    delete root;
}

MyXmlTreeItem* MySimpleXmlReader::getRootItem() const
{
    return root;
}

QVector < MyXmlTreeElement > MySimpleXmlReader::getItemsWithTag(const QString &tag)
{
    QVector < MyXmlTreeElement > result;

    checkAllItems(result, root, MyCheckTag(tag));
    return result;
}

QVector < MyXmlTreeElement > MySimpleXmlReader::getElementsByPath(const QString &path)
{
    QVector < MyXmlTreeElement > result;
    QStringList sl = path.split("/");

    getElementsByPathRec(result, sl, root, 0);

    return result;
}

void MySimpleXmlReader::getElementsByPathRec(QVector < MyXmlTreeElement > &result, const QStringList &sl, MyXmlTreeItem *current, int pos)
{
    if (pos == sl.length())
        return;

    if (current == NULL)
        return;

    if (current->getTag() == sl[pos])
    {
        if (pos + 1 == sl.length()/* && current->getChildrensCount() == 0*/)
        {
            MyXmlTreeElement currentElement((MyXmlTreeElement)(*current));

            for (MyXmlTreeItem *item = current; !item->isRoot(); item = item->getParent())
            {
                QMap < QString , QString > attrs(item->getAttrs());

                for (MyXmlTreeElement::AttrsMap::const_iterator it = attrs.constBegin(); it != attrs.constEnd(); ++it)
                {
                    if (!currentElement.findAttr(it.key()))
                        currentElement.pushAttr(it.key(), it.value());

                    currentElement.prependToAttr("ufo", QString::number((long long int)item));
                }
            }

            result.push_back(currentElement);
            return;
        }

        for (int i = 0; i < current->getChildrensCount(); ++i)
            getElementsByPathRec(result, sl, current->getChildrenAt(i), pos + 1);
    }
}

template < class CheckFunctor >
void MySimpleXmlReader::checkAllItems(QVector < MyXmlTreeElement > &result, MyXmlTreeItem *current, CheckFunctor check)
{
    MyXmlTreeElement element(*current);

    if (check(element))
        result.push_back(element);

    for (int i = 0; i < current->getChildrensCount(); ++i)
        checkAllItems(result, current->getChildrenAt(i), check);
}
