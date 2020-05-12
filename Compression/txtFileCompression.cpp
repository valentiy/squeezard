#include "Compression/txtFileCompression.h"

TxtFileCompression::TxtFileCompression()
{

}

TxtFileCompression::~TxtFileCompression()
{

}

void TxtFileCompression::Compress(QFile* file)
{
    qDebug() << "We entered compression!";
    char nextChar;

    while ( file->atEnd() )
        {
            qDebug() << "Getting char";
            file->getChar(&nextChar);
            TxtFileCompression::frequences[(unsigned char)nextChar]++;
        }


    AlgorithmHuf * root = TxtFileCompression::BiuldAlphabet();
    std::string code;

    qDebug() << "Filling the codebook";
    root->FillingCodeBook(TxtFileCompression::codebook,code);

    TxtFileCompression::Pop(file);
}

void TxtFileCompression::Decompress(QFile* file)
{

}

void TxtFileCompression::Pop(QFile* file)
{
    qDebug() << "We entered Poping!!";

    char nextChar;
    char nextByte = 0;
    unsigned char nextCharUC;
    int bitCounter = 0;

    file->seek(0);
    while( !file->atEnd() )
    {
        qDebug() << "Entered wtite_in_file";
        file->getChar(&nextChar);
        nextCharUC = (unsigned char)nextChar;
        for(unsigned int i = 0; i < codebook[nextCharUC].size(); i++, bitCounter++)
        {
            if( bitCounter == 8 )
            {
                nextByte = 0;
                bitCounter = 0;
            }

            if(codebook[nextCharUC][i] == '1')
                nextByte = nextByte | (0x01 << bitCounter);
        }

    }

}

AlgorithmHuf* TxtFileCompression::BiuldAlphabet()
{
    qDebug() << "We entered Building of alphabet!!";
    Alphabet minimum;
    AlgorithmHuf* next;

    for (unsigned int i = 0; i < 256; i++)
    {
        qDebug() << "cicle for the building alphabet";
        if (TxtFileCompression::frequences[i])
        {
            next = new AlgorithmHuf(i, TxtFileCompression::frequences[i]);
            minimum.Push(next);

        }
    }

    AlgorithmHuf* first;
    AlgorithmHuf* second;
    AlgorithmHuf* merged;

    while(minimum.Size() > 1)
    {
        first = minimum.Top();
        minimum.Pop();
        second = minimum.Top();
        minimum.Pop();
        merged = new AlgorithmHuf(first, second);
        minimum.Push(merged);
    }

    return minimum.Top();
}
/*
TxtFileCompression::TxtFileCompression()
{

}

TxtFileCompression::TxtFileCompression(const TxtFileCompression &tfc)
{
    TxtFileCompression::data = tfc.data;
    TxtFileCompression::frequency = tfc.frequency;
    TxtFileCompression::leftChar = tfc.leftChar;
    TxtFileCompression::rightChar = tfc.rightChar;
}

TxtFileCompression::TxtFileCompression( TxtFileCompression * right, TxtFileCompression * left)
{
    TxtFileCompression::frequency = right->frequency + left->frequency;
    TxtFileCompression::leftChar = left;
    TxtFileCompression::rightChar = right;
    TxtFileCompression::minimum = (right->minimum < left->minimum) ? right->minimum : left->minimum;
}

TxtFileCompression::~TxtFileCompression()
{

}

void TxtFileCompression::FillingCodeBook(std::string * codebook, std::string & code)
{
    if (!TxtFileCompression::leftChar && !TxtFileCompression::rightChar)
    {
        codebook[TxtFileCompression::data] = code;
        return;
    }

    if (TxtFileCompression::leftChar)
    {
        code += '0';
        TxtFileCompression::leftChar->FillingCodeBook(codebook, code);
        code.erase(code.end()-1);
    }

    if (TxtFileCompression::rightChar)
    {
        code += '1';
        TxtFileCompression::rightChar->FillingCodeBook(codebook, code);
        code.erase(code.end()-1);
    }
}

bool TxtFileCompression::operator>( const TxtFileCompression &tfc)
{
    if (TxtFileCompression::frequency > tfc.frequency)
        return true;
    if (TxtFileCompression::frequency < tfc.frequency)
        return false;
    if (TxtFileCompression::frequency == tfc.frequency)
        if (TxtFileCompression::minimum > tfc.minimum)
            return true;
    return false;
}

Alphabet::Alphabet()
{
    Alphabet::alphabetSize = 0;
    Alphabet::minAlphabet = new TxtFileCompression*[257];
}

Alphabet::~Alphabet()
{

}

void Alphabet::Push(TxtFileCompression* newTxt)
{
    int currentLocation = ++Alphabet::alphabetSize;

    while (currentLocation != 1 && *Alphabet::minAlphabet[currentLocation / 2] > *newTxt)
    {
        Alphabet::minAlphabet[currentLocation] = Alphabet::minAlphabet[currentLocation / 2];
        currentLocation = currentLocation / 2;
    }

    Alphabet::minAlphabet[currentLocation] = newTxt;
}

void Alphabet::Pop()
{
   int currentLocation = 1;
   int child = 2;

    TxtFileCompression* lastTxt = Alphabet::minAlphabet[Alphabet::alphabetSize];
    Alphabet::minAlphabet[alphabetSize--] = Alphabet::minAlphabet[1];

    while(child <= Alphabet::alphabetSize)
    {
        if (child < Alphabet::alphabetSize && *Alphabet::minAlphabet[child] > *Alphabet::minAlphabet[child+1])
            child++;

        if (*Alphabet::minAlphabet[child] > *lastTxt)
            break;

        Alphabet::minAlphabet[currentLocation] = Alphabet::minAlphabet[child];
        currentLocation = child;
        child *= 2;

        Alphabet::minAlphabet[currentLocation] = lastTxt;
    }
}

int Alphabet::Size()
{
    return Alphabet::alphabetSize;
}

TxtFileCompression* Alphabet::Top()
{
    return Alphabet::minAlphabet[1];
}
*/
