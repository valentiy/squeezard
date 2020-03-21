#include "Filesources\fileWork.h"

FileWork::FileWork()
{
    FileWork::inputAddress = "";
    FileWork::outputAddress = "";
    FileWork::workFile = nullptr;

    return;
};

FileWork::~FileWork()
{
    return;
};

void FileWork::ReadFile()
{
    FileWork::workFile->setFileName(FileWork::inputAddress);
    if ( FileWork::workFile->open(QIODevice::ReadOnly) )
        {
        qDebug() << "File is openned";
        FileWork::fileSText = workFile->readAll();
        }
    else
        qDebug() << "File is not open";

    FileWork::workFile->close();
    return;
};

void FileWork::SaveNewFile()
{
    FileWork::workFile->setFileName(FileWork::outputAddress);
    if ( FileWork::workFile->open(QIODevice::WriteOnly) )
        FileWork::workFile->write(fileSText);
    else
        qDebug() << "File was not saved";

    FileWork::workFile->close();
    return;
};

