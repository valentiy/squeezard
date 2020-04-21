#ifndef SQUEEZARD_H
#define SQUEEZARD_H

#include <QDebug>
#include <QWidget>
#include <QLineEdit>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QPointer>
#include "ui_squeezard.h"
#include "Filesources/fileWork.h"

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
    static QLineEdit *inputFileAddress;
    static QLineEdit *outputFileAddress;

private:
    QPointer<FileWork> fileForCompression;

private slots:
    void returnForPressedInputFileAddress();
    void returnForPressedOutputFileAddress();

private:
    Ui::squeezard *ui;
};
#endif // SQUEEZARD_H
