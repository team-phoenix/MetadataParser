#include "parser.h"

#include <QDebug>
#include <QDir>
#include <QDirIterator>

Parser::Parser()
    : m_type{Parser::NoType}
{
}

Parser::~Parser()
{

}

Parser::FileType Parser::fileType()
{
    return m_type;
}

void Parser::setFileType(Parser::FileType type)
{
    m_type = type;
}

void Parser::setExportPath(const QString path)
{
    export_info.setFile(path);
}

void Parser::setExportFile(const QString file)
{
    export_file = file;
}

void Parser::setDelimiters(const QStringList list)
{
    m_delims = list;
}

void Parser::setDelimiters(const QString &delim)
{
    m_delims.append(delim);
}

QStringList Parser::delimiters() const
{
    return m_delims;
}

QString Parser::exportFile() const
{
    return export_file;
}

QFileInfo Parser::exportInfo() const
{
    return export_info;
}

void Parser::parse()
{
    return;
}

QStringList Parser::getFiles(const QString path)
{
    qDebug() << "Dir exists? " << QDir(path).exists();

    QStringList list;
    QDirIterator iter(path, delimiters(), QDir::Files, QDirIterator::NoIteratorFlags);

    while (iter.hasNext()) {
        list.append(iter.next());
    }
    return list;

}
