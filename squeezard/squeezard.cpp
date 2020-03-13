#include "squeezard.h"

QLineEdit *squeezard::inputFileAddress;
QLineEdit *squeezard::outputFileAddress;
QTextEdit *squeezard::readTheFile;

QString squeezard::inputAddress;
QString squeezard::outputAddress;

QFile squeezard::fileForCompression;

squeezard::squeezard(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::squeezard)
{
    ui->setupUi(this);

    QWidget::setWindowFlags(Qt::MSWindowsOwnDC);

    //выделение памяти + отлов исключений
    try
    {
        squeezard::inputFileAddress = new QLineEdit();
        squeezard::outputFileAddress = new QLineEdit();
        squeezard::readTheFile = new QTextEdit();

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

    //настройка графического интерфейса
    ui->verticalLayout->QBoxLayout::addWidget(squeezard::inputFileAddress);
    ui->verticalLayout->QBoxLayout::addWidget(squeezard::outputFileAddress);
    ui->verticalLayout->QBoxLayout::addWidget(squeezard::readTheFile);

    squeezard::inputFileAddress->QLineEdit::setPlaceholderText("choose file for compression");
    squeezard::outputFileAddress->QLineEdit::setPlaceholderText("save to: ");

    squeezard::readTheFile->QWidget::setMaximumHeight(500);
    squeezard::readTheFile->QWidget::setMinimumHeight(300);

    squeezard::readTheFile->QTextEdit::setReadOnly(true);

    QObject::connect(squeezard::inputFileAddress, SIGNAL(returnPressed()), this, SLOT(returnForPressedInputFileAddress()));
    QObject::connect(squeezard::outputFileAddress, SIGNAL(returnPressed()), this, SLOT(returnForPressedOutputFileAddress()));



    return;
}

squeezard::~squeezard()
{
    delete ui;

    return;
}

void squeezard::returnForPressedInputFileAddress()
{
    squeezard::inputAddress = squeezard::inputFileAddress->text();

    //если данны ввода пустые, то метод завершается
    if(squeezard::inputFileAddress->text() == "")
        return;

    qDebug() << squeezard::inputAddress;

    squeezard::fileForCompression.setFileName(squeezard::inputAddress);
    squeezard::fileForCompression.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream fileSText(&fileForCompression);
    while(!fileSText.atEnd())
        squeezard::readTheFile->insertPlainText(fileSText.readLine() + "\n");

    squeezard::fileForCompression.close();
    return;
}

void squeezard::returnForPressedOutputFileAddress()
{
    squeezard::outputAddress = squeezard::outputFileAddress->text() + "\n";

    //если данны ввода пустые, то метод завершается
    if(squeezard::outputFileAddress->text() == "")
        return;

    qDebug() << squeezard::outputAddress;

    return;
}
