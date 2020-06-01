#include "Filesources/fileWork.h"
//#include "Compression/txtFileCompression.h"

FileWork::FileWork()
{
    try
    {
        FileWork::inputAdress = "";
        FileWork::outputAdress = "";
        FileWork::workFile = nullptr;
        FileWork::dialogWindow = new QFileDialog();
    }
    catch(std::bad_alloc &exp)
    {
        #ifndef Q_DEBUG
        qCritical() << "Exception caught: " << exp.std::bad_alloc::what();
        #endif
        abort();
    }
    catch(...)
    {
        #ifndef Q_DEBUG
        qCritical() << "Some exception caught";
        #endif
        abort();
    }

    FileWork::dialogWindow->QFileDialog::setDirectory(QDir::rootPath());
    return;
}

FileWork::~FileWork()
{
    delete FileWork::workFile;
    delete FileWork::dialogWindow;
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
    FileWork::workFile = new QFile(FileWork::inputAdress);
    return workFile;
}

void FileWork::ReadFile()
{
    //TxtFileCompression squeeze;
    FileWork::workFile = new QFile(FileWork::inputAdress);
   // if ( FileWork::workFile->exists() )
     //   qDebug() << "File exists";
   // else
   // {
     //   qDebug() << "File does not exist";
       // return;
   // }

    //if ( FileWork::workFile->open(QIODevice::ReadOnly) )
    //{
      //  qDebug() << "File is openned";
       // squeeze.Compress();
        //char fuck;
        //FileWork::fileSText = workFile->read(1);
        //FileWork::workFile->getChar(&fuck);
        //qDebug() << fuck;
    //}

   // FileWork::workFile->setFileName(FileWork::inputAddress);
   // if ( FileWork::workFile->open(QIODevice::ReadOnly) )
   //     {
   //     qDebug() << "File is openned";
   //     FileWork::fileSText = workFile->readAll();
   //     }

//    else
  //      qDebug() << "File is not open";

    //FileWork::workFile->close();
    delete FileWork::workFile;

    return;
}

void FileWork::SaveNewFile()
{
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

void FileWork::ChooseFile()
{
    FileWork::inputAdress = FileWork::dialogWindow->QFileDialog::getOpenFileName(nullptr, "Choose file", "", "");

    return;
}

QString FileWork::GetAdress()
{
    return FileWork::inputAdress;
}

QString FileWork::GetType()
{
    QString::const_iterator iter = inputAdress.end();
    QString buffer = "";

    do
    {
        buffer.push_front(*iter);
        --iter;

    }while(*iter != ".");

    buffer.chop(1);
    return buffer;
}
