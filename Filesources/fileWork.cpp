#include "Filesources\fileWork.h"

FileWork::FileWork()
{
    FileWork::inputAddress = "";
    FileWork::outputAddress = "";
    FileWork::workFile = nullptr;

    return;
}

FileWork::~FileWork()
{
    return;
}

void FileWork::ReadFile()
{

    FileWork::workFile = new QFile(FileWork::inputAddress);
    if ( FileWork::workFile->exists() )
        qDebug() << "File exists";
    else
    {
        qDebug() << "File does not exist";
        return;
    }

    if ( FileWork::workFile->open(QIODevice::ReadOnly) )
    {
        qDebug() << "File is openned";
        FileWork::fileSText = workFile->readAll();
        qDebug() << FileWork::fileSText;
    }

   // FileWork::workFile->setFileName(FileWork::inputAddress);
    if ( FileWork::workFile->open(QIODevice::ReadOnly) )
        {
        qDebug() << "File is openned";
        FileWork::fileSText = workFile->readAll();
        }

    else
        qDebug() << "File is not open";

    FileWork::workFile->close();
    delete FileWork::workFile;

    return;
}

void FileWork::SaveNewFile()
{

    //add possibility to name files
    //not sure for .bin
    //compressedfile's type depends on compression algorithm
    FileWork::workFile = new QFile(FileWork::outputAddress + "/compressedfile.bin");

  //  FileWork::workFile->setFileName(FileWork::outputAddress);

    if ( FileWork::workFile->open(QIODevice::WriteOnly) )
        FileWork::workFile->write(fileSText);
    else
        qDebug() << "File was not saved";

    FileWork::workFile->QFileDevice::close();

    FileWork::workFile->close();
    delete FileWork::workFile;

    return;
}

