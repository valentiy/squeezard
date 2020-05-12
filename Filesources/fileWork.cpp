#include "Filesources/fileWork.h"
//#include "Compression/txtFileCompression.h"

FileWork::FileWork()
{
    FileWork::inputAdress = "";
    FileWork::outputAdress = "";
    FileWork::workFile = nullptr;
    return;
}

FileWork::~FileWork()
{
    return;
}

void FileWork::SetInputPath(QString adress)
{
    FileWork::inputAdress = adress;
    qDebug() <<  FileWork::inputAdress;

    return;
}

void FileWork::SetOutputPath(QString adress)
{
    FileWork::outputAdress = adress;
    return;
}

QFile * FileWork::GetFile()
{
    return workFile;
}

void FileWork::ReadFile()
{
    //TxtFileCompression squeeze;
    FileWork::workFile = new QFile(FileWork::inputAdress);
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
       // squeeze.Compress();
        char fuck;
        FileWork::fileSText = workFile->read(1);
        FileWork::workFile->getChar(&fuck);
        qDebug() << fuck;
    }

   // FileWork::workFile->setFileName(FileWork::inputAddress);
   // if ( FileWork::workFile->open(QIODevice::ReadOnly) )
   //     {
   //     qDebug() << "File is openned";
   //     FileWork::fileSText = workFile->readAll();
   //     }

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
    FileWork::workFile = new QFile(FileWork::outputAdress + "/compressedfile.txt");

    if ( FileWork::workFile->open(QIODevice::WriteOnly) )
        FileWork::workFile->write(fileSText);
    else
        qDebug() << "File was not saved";

    FileWork::workFile->QFileDevice::close();

    FileWork::workFile->close();
    delete FileWork::workFile;

    return;
}

void FileWork::GetFileAdress()
{

}
