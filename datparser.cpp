#include "datparser.h"

#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QJsonArray>

DatParser::DatParser(const QString file, DatParser::Key key)
    : unknown_doc_key_increment{0}
{
    setObjectKey(key);
    setExportPath(file);
    setExportFile("nointrodb.json");

    auto str_list = getFiles(file);
    for (auto &dat : str_list)
        parse(dat);
    json_doc.setObject(json_object);

}

DatParser::~DatParser()
{
}

void DatParser::setObjectKey(const DatParser::Key key)
{
    object_key = key;
}

void DatParser::parse(const QString file)
{

    QFile fopen(file);

    if (!fopen.open(QIODevice::Text | QIODevice::ReadOnly)) {
        qDebug() << file << " was not opened";
    }

    QJsonObject system_obj;
    QString doc_key;

    while (!fopen.atEnd()) {

        QString line = fopen.readLine();

        if (line.contains("clrmamepro")) {

            while (true) {
                line = fopen.readLine();
                if (line.contains(')')) {
                    break;
                }

                QStringList list = line.split(" ");
                QString key = getKey(list);
                QString val;
                if (key == "name") {
                    val = getValue(list);
                    doc_key = "system" + val;
                }


                else if (key == "version") {
                    val = getValue(list);
                    system_obj.insert(key, val);
                    break;
                }
            }
        }

        if (line.contains("game")) {

            QJsonObject temp_obj;
            QString obj_key_val;

            while (true) {
                line = fopen.readLine();

                QStringList list = line.split(" ");
                QString key = getKey(list);
                QString val = getValue(list);

                if (key.contains("rom")) {
                    auto list = getRomData(val);
                    for (auto &str_list : list) {
                        QString rom_key = str_list.at(0);
                        QString rom_value = str_list.at(1);
                        temp_obj.insert(rom_key, rom_value);
                        switch (object_key) {
                            case DatParser::CRC32:
                                if (rom_key.contains("romcrc"))
                                    obj_key_val = rom_value;
                                break;
                            case DatParser::SHA1:
                                if (rom_key.contains("romsha1"))
                                    obj_key_val = rom_value;
                                break;
                            case DatParser::MD5:
                                if (rom_key.contains("rommd5"))
                                    obj_key_val = rom_value;
                                break;
                            default:
                                break;
                        }
                        if (obj_key_val == "")
                            obj_key_val = rom_key;

                    }
                    break;
                }

                temp_obj.insert(key, val);

            }

            system_obj.insert(obj_key_val, temp_obj);
        }
    }

    if (doc_key == "") {
        doc_key = "systemUnknown" + QString::number(unknown_doc_key_increment);
        unknown_doc_key_increment++;
    }


    json_object.insert(doc_key, system_obj);
    export_keys.append(doc_key);

    fopen.close();

}

QStringList DatParser::getFiles(const QString file)
{
    qDebug() << "Dir exists? " << QDir(file).exists();

    QStringList list;
    QDirIterator iter(file, QStringList() << "*.dat", QDir::Files, QDirIterator::NoIteratorFlags);
    //qDebug() << iter.filePath();
    while (iter.hasNext()) {
        list.append(iter.next());
    }
    return list;

}

QString DatParser::getKey(QStringList &list)
{
    return list.takeAt(0).simplified();
}

QString DatParser::getValue(QStringList &list)
{
    QString val;
    int i=0;
    for (auto &str : list) {
        auto temp = str.remove('"').simplified();
        if (i==0)
            val += temp;
        else
            val = val + " " + temp;
        i++;
    }
    return val;
}

QList<QStringList> DatParser::getRomData(QString str)
{
    QList<QStringList> data;
    QStringList str_list = str.split(" ");
    for (int i=0; i < str_list.length(); ++i) {
        QString key = str_list.at(i);
        if (key.contains("("))
            continue;

        if (key.contains("name")) {
            data.append(QStringList{"rom" + key, romValue("size", str_list, i)});
        }
        else if (key.contains("size")) {
            data.append(QStringList{"rom" + key, romValue("crc", str_list, i)});
        }
        else if (key.contains("crc")) {
            data.append(QStringList{"rom" + key, romValue("md5", str_list, i)});
        }

        else if (key.contains("md5"))  {
            data.append(QStringList{"rom" + key, romValue("sha1", str_list, i)});
        }

        else if (key.contains("sha1")) {
            data.append(QStringList{"rom" + key, romValue(")", str_list, i)});
        }
    }
    return data;
}

QString DatParser::romValue(const QString &second_val, const QStringList &str_list, int i)
{

    int index = str_list.indexOf(second_val);
    QStringList temp_list;
    for (int j=i+1; j < index; ++j) {
        temp_list.append(str_list.at(j));
    }

    return getValue(temp_list);
}

QJsonDocument DatParser::toJsonDocument() const
{
    return json_doc;
}

void DatParser::setExportPath(const QString path)
{
    export_info.setFile(path);
}

void DatParser::setExportFile(const QString file)
{
    export_file = file;
}

bool DatParser::save()
{

    if (!json_doc.isEmpty() && !json_doc.isNull()) {

        QString path = export_info.absoluteFilePath() + "/" + export_file;
        QFile fopen(path);

        if (!fopen.open(QIODevice::WriteOnly)) {
            qDebug() << path << " could not be opened for save.";
            return false;
        }
        int status = fopen.write(json_doc.toJson(QJsonDocument::Indented));
        if (status == -1)
            return false;
        qDebug() << export_file << " saved to " << export_info.absoluteFilePath();
        fopen.close();
        generateKeysFile();

        return true;

    }

    qDebug() << "Json document isn't valid";
    return false;
}

bool DatParser::generateKeysFile()
{
    bool result = true;
    QFile fopen(export_info.absoluteFilePath() + "/" + "nointrodb_keys.txt");

    if (!fopen.open(QIODevice::Text | QIODevice::WriteOnly)) {
        qDebug() << "nointrodb_key.txt file could not be created. The json document may still be valid!";
        return false;
    }

    for (auto &str : export_keys) {
        str += "\n";
        QByteArray array(str.toStdString().c_str());

        if (fopen.write(array) == -1) {
            qDebug() << "write error";
            result = false;
        }

    }

    fopen.close();

     return result;

}
