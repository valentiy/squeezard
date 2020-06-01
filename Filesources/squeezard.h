#ifndef SQUEEZARD_H
#define SQUEEZARD_H

#include <QDebug>
#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QFile>
#include <QString>

#include "ui_squeezard.h"

#include "Filesources/fileWork.h"
#include "Compression/fileCompression.h"

//QT_BEGIN_NAMESPACE
//namespace Ui { class squeezard; }
//QT_END_NAMESPACE

class Squeezard : public QWidget, private Ui::squeezard
{
    Q_OBJECT

public:
    Squeezard(QWidget *parent = nullptr);
    ~Squeezard();

public:
    static QLineEdit *inputFileAdress;
    static QLineEdit *outputFileAdress;
    QPushButton *inputFileButton;
    QPushButton *outputFileButton;
    QPushButton *actionButton;
    QPushButton *deActionButton;
    QListWidget *selectedFiles;

private:
    FileWork * fileForCompression;
    FileCompression * txtAlgorithm;

private slots:
    void ReturnForPressedInputFileAdress();
    void ReturnForPressedOutputFileAdress();
    void CheckingFiles();
    void DeleteFileFromQueue();
    void Squeeze();
    void DeSqueeze();

private:
    Ui::squeezard *ui;
};
#endif // SQUEEZARD_H
