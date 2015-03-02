#include <QCoreApplication>
#include <QDebug>
#include "datparser.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    DatParser parser("/Users/lee/Desktop/nointro/", DatParser::SHA1);
    parser.save();


    return a.exec();
}
