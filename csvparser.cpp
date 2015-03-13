#include "csvparser.h"

#include <QDebug>

CSVParser::CSVParser(const QString &path)
{
    setFileType(Parser::CSV);
    setExportPath(path);
    setDelimiters("*.csv");
}

CSVParser::~CSVParser()
{

}

void CSVParser::parse()
{
    qDebug() << exportInfo().absoluteFilePath();

    auto str_list = getFiles(exportInfo().absoluteFilePath());

    for (auto &dat : str_list) {
        QFile fopen(dat);
        if (!fopen.open(QIODevice::Text | QIODevice::ReadOnly)) {
            qDebug() << "Error with opening " << exportInfo().absoluteFilePath();
            return;
        }

        while (!fopen.atEnd()) {
            //"Title","CoverRegionName","CoverFrontURL","CoverBackURL","EnglishTitle","ReferenceURL"

            QString line = fopen.readLine();
            QStringList str_list = line.split(",");

            QString title;
            QString cover_region_name;
            QString cover_front_url;
            QString cover_back_url;
            QString cover_english_title;
            QString reference_url;

            // Don't update to c++11 iterator! i value is needed for switch statement.
            for (int i=0; i < str_list.length(); ++i) {

                QString val = str_list.at(i);
                val = val.remove('"');

                switch (i) {
                    case 0:
                        title = val;
                        break;
                    case 1:
                        cover_region_name = val;
                        break;
                    case 2:
                        cover_front_url = val;
                        break;
                    case 3:
                        cover_back_url = val;
                        break;
                    case 4:
                        cover_english_title = val;
                        break;
                    case 5:
                        reference_url = val;
                        break;
                    default:
                        //qDebug() << "Unexpected value, update the parser!";
                        break;
                }
            }

            QStringList temp_list{title, cover_region_name, cover_front_url,
                                  cover_english_title, reference_url, };

            list.append(temp_list);

        }

        fopen.close();
    }
}

QList<QStringList> CSVParser::toStringList()
{
    return list;
}
