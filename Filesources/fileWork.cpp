#include "Filesources/fileWork.h"
//#include "Compression/txtFileCompression.h"

FileWork::FileWork()
{
    try
    {
        FileWork::inputAdress = "";
        FileWork::outputAdress = "";
        FileWork::type = "";
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

void FileWork::SaveNewFile(QFile* file)

{
    FileWork::dialogWindow->setAcceptMode(QFileDialog::AcceptSave);
    FileWork::outputAdress = FileWork::dialogWindow->QFileDialog::getSaveFileName(nullptr, "Choose file", "", "");
    if (type != "") file->rename(FileWork::outputAdress + FileWork::type);
    else file->rename(FileWork::outputAdress);
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

void FileWork::SetType(QString data)
{
    FileWork::type = "." + data;
    return;
}
