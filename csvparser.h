#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QList>

#include "parser.h"


class CSVParser : public Parser
{
public:
    CSVParser(const QString &path);
    ~CSVParser();
    void setJsonDocument(QJsonDocument &doc);
    QList<QStringList> toStringList();

    void parse();

private:
    QJsonDocument json_doc;
    QList<QStringList> list;
};

#endif // CSVPARSER_H
