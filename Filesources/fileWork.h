#ifndef FILEWORK_H
#define FILEWORK_H

#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QByteArray>
#include <QString>

class FileWork : public QFile //read about inheritance!!
{
    Q_OBJECT

    private:
        QFile *workFile;
        QByteArray fileSText;

    public:
        FileWork();
        ~FileWork();

    public:

        QString inputAddress;
        QString outputAddress;

    public slots:
        void ReadFile();
        void SaveNewFile();
};

#endif // FILEWORK_H
