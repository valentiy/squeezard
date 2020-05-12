#ifndef SQUEEZARD_H
#define SQUEEZARD_H

#include <QDebug>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QPointer>
#include "ui_squeezard.h"
#include "Filesources/fileWork.h"
#include "Compression/txtFileCompression.h"

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

private:
    FileWork * fileForCompression;
    TxtFileCompression * txtAlgorithm;

private slots:
    void ReturnForPressedInputFileAdress();
    void ReturnForPressedOutputFileAdress();
    void Squeeze();

private:
    Ui::squeezard *ui;
};
#endif // SQUEEZARD_H
