#ifndef DATPARSER_H
#define DATPARSER_H

#include <QFile>
#include <QString>
#include <QStringList>
#include <QList>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFileInfo>

#include "parser.h"

class DatParser : public Parser
{
public:

    enum Key {
        CRC32 = 0,
        SHA1,
        MD5,
    };

    DatParser(const QString file);
    ~DatParser();


    void parse();

    QJsonDocument toJsonDocument() const;
    bool save();

    void setObjectKey(const DatParser::Key key);


private:
    QString getKey(QStringList &list);
    QString getValue(QStringList &list);
    QList<QStringList> getRomData(QString str);
    QString romValue(const QString &second_val, const QStringList &str_list, int i);
    bool generateKeysFile();
    void parse(const QString file);

    QJsonDocument json_doc;
    QJsonObject json_object;

    unsigned object_key;

    unsigned unknown_doc_key_increment;

    QStringList export_keys;
};

#endif // DATPARSER_H
