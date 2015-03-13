#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include <QFileInfo>
#include <QStringList>

// Don't use Parse class directy!!! It's mean't to be subclassed.

class Parser
{
public:

    Parser();
    ~Parser();

    enum FileType {
        DAT=0,
        CSV,
        NoType,
    };

    void setFileType(Parser::FileType type);

    Parser::FileType fileType();
    QFileInfo exportInfo() const;
    QString exportFile() const;

    virtual void parse();

    QStringList delimiters() const;

    QStringList getFiles(const QString path);
    void setDelimiters(const QStringList list);
    void setDelimiters(const QString &delim);

    void setExportPath(const QString path);
    void setExportFile(const QString file);

private:
    Parser::FileType m_type;
    QFileInfo export_info;
    QString export_file;
    QStringList m_delims;
};

#endif // PARSER_H
