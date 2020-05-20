#include "Filesources/squeezard.h"

QLineEdit *Squeezard::inputFileAdress;
QLineEdit *Squeezard::outputFileAdress;

Squeezard::Squeezard(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::squeezard)
{
    ui->setupUi(this);

    QWidget::setWindowFlags(Qt::MSWindowsOwnDC);

    //memory allocation & exceptions catch
    try
    {
        Squeezard::inputFileAdress = new QLineEdit();
        Squeezard::inputFileButton = new QPushButton();
    //    Squeezard::outputFileAdress = new QLineEdit();
    //    Squeezard::outputFileButton = new QPushButton();
        Squeezard::actionButton = new QPushButton();
        Squeezard::selectedFiles = new QListWidget();
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
    ui->horizontalLayoutForINput->QBoxLayout::addWidget(Squeezard::inputFileAdress);
    ui->horizontalLayoutForINput->QBoxLayout::addWidget(Squeezard::inputFileButton);
    ui->horizontalLayoutForOutput->QBoxLayout::addWidget(Squeezard::selectedFiles);
   // ui->horizontalLayoutForOutput->QBoxLayout::addWidget(Squeezard::outputFileAdress);
   // ui->horizontalLayoutForOutput->QBoxLayout::addWidget(Squeezard::outputFileButton);
    ui->horizontalLayoutActionButtons->QBoxLayout::addWidget(Squeezard::actionButton);

    Squeezard::inputFileAdress->QLineEdit::setPlaceholderText("choose file for compression");
   // Squeezard::outputFileAdress->QLineEdit::setPlaceholderText("save to: ");

    Squeezard::inputFileButton->QPushButton::setText("browse");
  //  Squeezard::outputFileButton->QPushButton::setText("browse");
    Squeezard::actionButton->QPushButton::setText("compress");

    Squeezard::selectedFiles->QAbstractItemView::setSelectionMode(QAbstractItemView::SingleSelection);
    Squeezard::selectedFiles->QAbstractItemView::setSelectionBehavior(QAbstractItemView::SelectRows);

    QObject::connect(Squeezard::inputFileAdress, SIGNAL(returnPressed()), this, SLOT(ReturnForPressedInputFileAdress()));
  //  QObject::connect(Squeezard::outputFileAdress, SIGNAL(returnPressed()), this, SLOT(ReturnForPressedOutputFileAdress()));

    QObject::connect(Squeezard::inputFileAdress, SIGNAL(returnPressed()), Squeezard::inputFileAdress, SLOT(clear()));
  //  QObject::connect(Squeezard::outputFileAdress, SIGNAL(returnPressed()), Squeezard::outputFileAdress, SLOT(clear()));

    Squeezard::fileForCompression = nullptr;
    Squeezard::txtAlgorithm = nullptr;

    try
    {
        Squeezard::fileForCompression = new FileWork();
        Squeezard::txtAlgorithm = new FileCompression();
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

    QObject::connect(Squeezard::inputFileButton, SIGNAL(clicked(bool)), Squeezard::fileForCompression, SLOT(ChooseFile()));
    QObject::connect(Squeezard::inputFileButton, SIGNAL(clicked(bool)), this, SLOT(CheckingFiles()));
    QObject::connect(Squeezard::selectedFiles, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(DeleteFileFromQueue()));
   // QObject::connect(Squeezard::outputFileButton, SIGNAL(clicked(bool)), Squeezard::fileForCompression, SLOT(ChooseFile()));
    QObject::connect(Squeezard::actionButton, SIGNAL(clicked(bool)), this, SLOT(Squeeze()));

    return;
}

Squeezard::~Squeezard()
{
    delete Squeezard::ui;
    delete Squeezard::inputFileAdress;
    delete Squeezard::selectedFiles;
    delete Squeezard::inputFileButton;
    delete Squeezard::actionButton;
    delete Squeezard::fileForCompression;
    delete Squeezard::txtAlgorithm;

    return;
}

void Squeezard::ReturnForPressedInputFileAdress()
{
    if(Squeezard::inputFileAdress->text() == "")
        return;

    Squeezard::fileForCompression->SetInputPath(Squeezard::inputFileAdress->text());

    return;
}

void Squeezard::ReturnForPressedOutputFileAdress()
{
    if(Squeezard::outputFileAdress->text() == "")
        return;

    Squeezard::fileForCompression->SetOutputPath(Squeezard::outputFileAdress->text());

    return;
}

void Squeezard::CheckingFiles()
{
    Squeezard::selectedFiles->QListWidget::addItem(Squeezard::fileForCompression->GetAdress(true));
    return;
}

void Squeezard::DeleteFileFromQueue()
{
    Squeezard::selectedFiles->removeItemWidget(Squeezard::selectedFiles->currentItem());
}

void Squeezard::Squeeze()
{
    Squeezard::txtAlgorithm->Compresss(Squeezard::fileForCompression->GetFile());
    return;
}
