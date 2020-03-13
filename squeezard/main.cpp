#include "squeezard.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    squeezard mainwindow;
    mainwindow.show();

    return a.exec();
}
