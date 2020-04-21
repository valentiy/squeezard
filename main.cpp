#include "Filesources/squeezard.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile *file = new QFile(":/Styles/style.css");
    file->QIODevice::open(QFile::ReadOnly);
    a.QApplication::setStyleSheet(file->QIODevice::readAll());

    Squeezard mainwindow;
    mainwindow.show();

    return a.exec();
}
