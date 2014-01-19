#ifndef MYPARSEFIELDS_H
#define MYPARSEFIELDS_H

#include <QString>
#include <QStringList>
#include <myschoolboy.h>
#include <QMap>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDebug>

class MyParseFields
{
private:
    QString getSchoolName(QString text)
    {
        text = text.replace("\"", " ").replace("«", " ").replace("»", " ").replace("\"", " ").replace("ё", "е").simplified();

        QStringList leftWords;
        QStringList rightWords;

        leftWords << "МБОУ" << "МОБУ" << "МОУ" << "МКОУ" << "МАОУ" << "учреждение";
        rightWords << "СОШ" << "ООШ" << "средняя" << "гимназия" << "лицей" << "школа" << "основная" << "образовательная" << "общеобразовательная";

        QStringList words = text.split(" ");

        if (words.length() < 3)
            return "";

        for (int i = 0; i < words.size() - 2; ++i)
        {
            if (leftWords.contains(words[i], Qt::CaseInsensitive) && rightWords.contains(words[i + 2], Qt::CaseInsensitive))
            {
                QString s;

                for (int j = i + 1; j < words.size(); ++j)
                    s += words[j] + " ";

                QStringList tags = getTagsForSchool(s);

                if (tags.contains("школа", Qt::CaseInsensitive) && !leftWords.contains(words[i + 1], Qt::CaseInsensitive) && !rightWords.contains(words[i + 1], Qt::CaseInsensitive))
                    return words[i + 1];
            }
        }
        return "";
    }

    QStringList checkIsSoldats(const QString &text)
    {
        QStringList tags;
        QStringList names;

        names << "Кедровый" << "Ачинский" << "Канский" << "Красноярский" << "Лесосибирский" << "Минусинский" << "Норильский" << "Шарыповский";

        if (text.contains("кадетский", Qt::CaseInsensitive) && text.contains("корпус", Qt::CaseInsensitive))
        {
            foreach (QString name, names)
                if (text.contains(name, Qt::CaseInsensitive))
                {
                    tags << name << "кадетский корпус";
                    break;
                }
        }

        if (tags.length() != 0)
            return tags;

        names.clear();
        names << "Ачинская" << "Красноярская";

        if (text.contains("Мариинская", Qt::CaseInsensitive) && text.contains("гимназия", Qt::CaseInsensitive))
        {
            foreach (QString name, names)
            {
                if (text.contains(name, Qt::CaseInsensitive))
                {
                    tags << name << "Мариинская гимназия";
                }
            }
        }

        return tags;
    }

public:
    MyParseFields();

    QStringList getTagsForSchool(const QString &text)
    {
        QStringList tags = checkIsSoldats(text.toLower().replace("мжги", "Мариинская женская гимназия-интернат"));

        if (tags.length() != 0)
            return tags;

        QMap < QString , QString > pairs;

        pairs["средняя"] = "школа";
        pairs["ООШ"] = "школа";
        pairs["СОШ"] = "школа";
        pairs["школа"] = "школа";
        pairs["гимназия"] = "гимназия";
        pairs["лицей"] = "лицей";
        pairs["КУГ"] = "гимназия";
        pairs["МУК"] = "МУК";
        pairs["ЦО"] = "ЦО";

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

        tags.removeDuplicates();

        return tags;
    }

    QString getTagForLocality(const QString &text)
    {
        QString myText = text;
        QMap < QString , QString > pairs;

        pairs["г."] = "город ";
        pairs["р-он"] = "район";
        pairs["р-н"] = "район";
        pairs[","] = " ";
        pairs["ё"] = "е";

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

        for (int i = 0; i < words.size(); ++i)
            if (words[i] == "район")
            {
                QString locality;

                for (int j = 0; j < i; ++j)
                    locality += words[j] + " ";

                return locality + "район";
            }

        if (words[1] == "район")
            return words[0] + " район";

        if (words[0] == "ЗАТО")
            return "ЗАТО " + words[1];

        return "";
    }

    QStringList getTagsForName(const QString &text)
    {
        QStringList tags = text.simplified().split(" ");

        while (tags.length() > 3)
        {
            QString s = tags.back();

            tags.pop_back();
            tags.back() += " " + s;
        }

        while (tags.length() < 3)
            tags << "";

        return tags;
    }
};

#endif // MYPARSEFIELDS_H
