#ifndef FILEWORK_H
#define FILEWORK_H

#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <QByteArray>
#include <QString>

class FileWork  : public QFile//read about inheritance!!
{
    Q_OBJECT

private:
    QByteArray fileSText;
    QFileDialog *dialogWindow;
    QFile *workFile;
    QString inputAdress;
    QString outputAdress;

public:
    FileWork();
    ~FileWork();
    void SetInputPath(QString);
    void SetOutputPath(QString);
    QFile * GetFile();
    QString GetAdress();
    QString GetType();

public slots:
    void ReadFile();
    void SaveNewFile();
    void ChooseFile();

};

#endif // FILEWORK_H
