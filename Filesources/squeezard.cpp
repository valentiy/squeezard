#include "Filesources/squeezard.h"

QLineEdit *Squeezard::inputFileAddress;
QLineEdit *Squeezard::outputFileAddress;

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

    Squeezard::inputFileAddress->QLineEdit::setPlaceholderText("choose file for compression");
    Squeezard::outputFileAddress->QLineEdit::setPlaceholderText("save to: ");

    QObject::connect(Squeezard::inputFileAddress, SIGNAL(returnPressed()), this, SLOT(returnForPressedInputFileAddress()));
    QObject::connect(Squeezard::outputFileAddress, SIGNAL(returnPressed()), this, SLOT(returnForPressedOutputFileAddress()));

    QObject::connect(Squeezard::inputFileAddress, SIGNAL(returnPressed()), Squeezard::inputFileAddress, SLOT(clear()));
    QObject::connect(Squeezard::outputFileAddress, SIGNAL(returnPressed()), Squeezard::outputFileAddress, SLOT(clear()));

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
