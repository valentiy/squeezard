#ifndef SQUEEZARD_H
#define SQUEEZARD_H

#include <QDebug>
#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QFile>
#include <QTextStream>
#include <QString>
#include "ui_squeezard.h"

//QT_BEGIN_NAMESPACE
//namespace Ui { class squeezard; }
//QT_END_NAMESPACE

class squeezard : public QWidget, private Ui::squeezard
{
    Q_OBJECT

public:
    squeezard(QWidget *parent = nullptr);
    ~squeezard();
public:
    static QLineEdit *inputFileAddress;
    static QLineEdit *outputFileAddress;

    static QTextEdit *readTheFile;

public:
    static QString inputAddress;
    static QString outputAddress;

    static QFile fileForCompression;

private slots:
    void returnForPressedInputFileAddress();
    void returnForPressedOutputFileAddress();

private:
    Ui::squeezard *ui;
};
#endif // SQUEEZARD_H
