#include "Filesources/squeezard.h"

QLineEdit *Squeezard::inputFileAddress;
QLineEdit *Squeezard::outputFileAddress;
QTextEdit *Squeezard::readTheFile;

Squeezard::Squeezard(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::squeezard)
{
    ui->setupUi(this);

    QWidget::setWindowFlags(Qt::MSWindowsOwnDC);

    //memory allocation & exceptions catch
    try
    {
        Squeezard::inputFileAddress = new QLineEdit();
        Squeezard::outputFileAddress = new QLineEdit();
        Squeezard::readTheFile = new QTextEdit();

    }
    catch (std::bad_alloc &exp)
    {
        #ifndef Q_DEBUG
        qDebug() << "Exception caught: " << exp.std::bad_alloc::what();
        #endif
        abort();
    }
    catch(...)
    {
        #ifndef Q_DEBUG
        qDebug() << "Some exception caught";
        #endif
        abort();
    }

    //set ui
    ui->verticalLayout->QBoxLayout::addWidget(Squeezard::inputFileAddress);
    ui->verticalLayout->QBoxLayout::addWidget(Squeezard::outputFileAddress);
    ui->verticalLayout->QBoxLayout::addWidget(Squeezard::readTheFile);

    Squeezard::inputFileAddress->QLineEdit::setPlaceholderText("choose file for compression");
    Squeezard::outputFileAddress->QLineEdit::setPlaceholderText("save to: ");

    Squeezard::readTheFile->QWidget::setMaximumHeight(500);
    Squeezard::readTheFile->QWidget::setMinimumHeight(300);

    Squeezard::readTheFile->QTextEdit::setReadOnly(true);

    QObject::connect(Squeezard::inputFileAddress, SIGNAL(returnPressed()), this, SLOT(returnForPressedInputFileAddress()));
    QObject::connect(Squeezard::outputFileAddress, SIGNAL(returnPressed()), this, SLOT(returnForPressedOutputFileAddress()));

    Squeezard::fileForCompression = nullptr;
    try
    {
        Squeezard::fileForCompression = new FileWork();
    }
    catch (std::bad_alloc &exp)
    {
        #ifndef Q_DEBUG
        qDebug() << "Exception caught: " << exp.std::bad_alloc::what();
        #endif
        abort();
    }
    catch(...)
    {
        #ifndef Q_DEBUG
        qDebug() << "Some exception caught";
        #endif
        abort();
    }
    return;
}

Squeezard::~Squeezard()
{
    delete Squeezard::ui;
    delete Squeezard::fileForCompression;

    return;
}

void Squeezard::returnForPressedInputFileAddress()
{
    if(Squeezard::inputFileAddress->text() == "")
        return;

    Squeezard::fileForCompression->inputAddress = Squeezard::inputFileAddress->text();

    qDebug() << Squeezard::fileForCompression->inputAddress;

    Squeezard::fileForCompression->ReadFile();
    /*Squeezard::fileForCompression.setFileName(Squeezard::inputAddress);
    if(Squeezard::fileForCompression.open(QIODevice::ReadOnly | QIODevice::Text))
    {
    QTextStream fileSText(&fileForCompression);
    while(!fileSText.atEnd())
        Squeezard::readTheFile->insertPlainText(fileSText.readLine() + "\n");
    }
    else
    {
        qDebug() << "File is not open";
    }

    Squeezard::fileForCompression.close();
    */
    return;
}

void Squeezard::returnForPressedOutputFileAddress()
{
    if(Squeezard::outputFileAddress->text() == "")
        return;

    Squeezard::fileForCompression->outputAddress = Squeezard::outputFileAddress->text();

    qDebug() << Squeezard::fileForCompression->outputAddress;

    Squeezard::fileForCompression->SaveNewFile();

    return;
}
