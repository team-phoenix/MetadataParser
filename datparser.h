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

class DatParser
{
public:

    enum Key {
        CRC32 = 0,
        SHA1,
        MD5,
    };

    DatParser(const QString file, DatParser::Key);
    ~DatParser();



    void parse(const QString file);
    QStringList getFiles(const QString file);

    QJsonDocument toJsonDocument() const;
    bool save();
    void setExportPath(const QString path);
    void setExportFile(const QString file);
    void setObjectKey(const DatParser::Key key);


private:
    QString getKey(QStringList &list);
    QString getValue(QStringList &list);
    QList<QStringList> getRomData(QString str);
    QString romValue(const QString &second_val, const QStringList &str_list, int i);
    bool generateKeysFile();

    QJsonDocument json_doc;
    QJsonObject json_object;
    QFileInfo export_info;
    QString export_file;
    unsigned object_key;

    unsigned unknown_doc_key_increment;

    QStringList export_keys;
};

#endif // DATPARSER_H
