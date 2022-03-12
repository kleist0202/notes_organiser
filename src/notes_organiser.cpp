#include <QApplication>
#include <iostream>
#include "interface.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("Notes Organiser");
    QCoreApplication::setOrganizationName("PK");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    Interface inter(".default");

    inter.show();
    return app.exec();
}
