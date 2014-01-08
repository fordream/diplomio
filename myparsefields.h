#ifndef MYPARSEFIELDS_H
#define MYPARSEFIELDS_H

#include <QString>
#include <QStringList>
#include <myschoolboy.h>
#include <QMap>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

class MyParseFields
{
private:
    QString getSchoolName(QString text)
    {
        text = text.replace("\"", " ").replace("«", " ").replace("»", " ");

        QStringList leftWords;
        QStringList rightWords;

        leftWords << "МБОУ" << "МОБУ" << "МОУ" << "МКОУ" << "МАОУ";
        rightWords << "СОШ" << "ООШ" << "средняя" << "гимназия" << "лицей" << "школа";

        QStringList words = text.split(" ");

        if (words.length() < 3)
            return "";

        if (leftWords.contains(words[0]) && rightWords.contains(words[2]))
        {
            QStringList tags = getTagsForSchool(words[2]);

            if (tags.length() > 0 && tags[0] == "школа")
                return words[1];
        }

        return "";
    }

public:
    MyParseFields();

    QStringList getTagsForSchool(const QString &text)
    {
        QMap < QString , QString > pairs;

        pairs["ООШ"] = "школа";
        pairs["СОШ"] = "школа";
        pairs["школа"] = "школа";
        pairs["гимназия"] = "гимназия";
        pairs["лицей"] = "лицей";
        pairs["КУГ"] = "гимназия";
        pairs["МУК"] = "МУК";
        pairs["ЦО"] = "ЦО";
        pairs["кадетский"] = "кадетский";
        pairs["Мариинская"] = "Мариинская";

        QStringList tags;

        for (QMap < QString , QString >::const_iterator it = pairs.constBegin(); it != pairs.constEnd(); ++it)
        {
            if (text.indexOf(it.key(), 0, Qt::CaseInsensitive) != -1)
                tags << it.value();
        }

        QString schoolNumber = QRegularExpression("[№](\\s*)([0-9]+)").match(text).captured(2);

        if (schoolNumber != "")
            tags << "№ " + schoolNumber;

        QString schoolName = getSchoolName(text);

        if (schoolName != "")
            tags << schoolName;

        return tags;
    }

    QString getTagForLocality (const QString &text)
    {
        QString myText = text;

        QMap < QString , QString > pairs;

        pairs["г."] = "город ";
        pairs["р-он"] = "район";
        pairs["р-н"] = "район";
        pairs[","] = " ";

        for (QMap < QString , QString >::const_iterator it = pairs.constBegin(); it != pairs.constEnd(); ++it)
            myText = myText.replace(it.key(), it.value(), Qt::CaseInsensitive);

        myText = myText.simplified();

        if (myText.toLower() == "кадеты")
            return "Кадеты";

        if (myText.toLower() == "школа космонавтики")
            return "Школа космонавтики";

        QStringList words = myText.split(" ");

        if (words.length() < 2)
            return "";

        if (words[0] == "город")
            return "город " + words[1];

        if (words[1] == "район")
            return words[0] + " район";

        if (words[0] == "ЗАТО")
            return "ЗАТО " + words[1];

        return "";
    }
};

#endif // MYPARSEFIELDS_H
