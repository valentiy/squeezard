#include "Filesources/squeezard.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Squeezard mainwindow;
    mainwindow.show();

    return a.exec();
}
