#include "Compression/huffman.h"

AlgorithmHuf::AlgorithmHuf()
{

}

AlgorithmHuf::AlgorithmHuf(const AlgorithmHuf &tfc)
{
    AlgorithmHuf::data = tfc.data;
    AlgorithmHuf::frequency = tfc.frequency;
    AlgorithmHuf::leftChar = tfc.leftChar;
    AlgorithmHuf::rightChar = tfc.rightChar;
}

AlgorithmHuf::AlgorithmHuf( AlgorithmHuf * right, AlgorithmHuf * left)
{
    AlgorithmHuf::frequency = right->frequency + left->frequency;
    AlgorithmHuf::leftChar = left;
    AlgorithmHuf::rightChar = right;
    AlgorithmHuf::minimum = (right->minimum < left->minimum) ? right->minimum : left->minimum;
}

AlgorithmHuf::~AlgorithmHuf()
{

}

void AlgorithmHuf::FillingCodeBook(std::string * codebook, std::string &code)
{
    qDebug() << "Start filling";
    if (!AlgorithmHuf::leftChar && !AlgorithmHuf::rightChar)
    {
        codebook[AlgorithmHuf::data] = code;
        qDebug() << "first condition";
        qDebug() << &codebook[AlgorithmHuf::data];
        return;
    }

    if (AlgorithmHuf::leftChar)
    {
        qDebug() << "second condition";
        code += '0';
        AlgorithmHuf::leftChar->FillingCodeBook(codebook, code);
        code.erase(code.end()-1);
    }

    if (AlgorithmHuf::rightChar)
    {
        qDebug() << "third condition";
        code += '1';
        AlgorithmHuf::rightChar->FillingCodeBook(codebook, code);
        code.erase(code.end()-1);
    }
}

bool AlgorithmHuf::operator>( const AlgorithmHuf &tfc)
{
    if (AlgorithmHuf::frequency > tfc.frequency)
        return true;
    if (AlgorithmHuf::frequency < tfc.frequency)
        return false;
    if (AlgorithmHuf::frequency == tfc.frequency)
        if (AlgorithmHuf::minimum > tfc.minimum)
            return true;
    return false;
}

Alphabet::Alphabet()
{
    Alphabet::alphabetSize = 0;
    Alphabet::minAlphabet = new AlgorithmHuf*[257];
}

Alphabet::~Alphabet()
{

}

void Alphabet::Push(AlgorithmHuf* newTxt)
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

    AlgorithmHuf* lastTxt = Alphabet::minAlphabet[Alphabet::alphabetSize];
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

AlgorithmHuf* Alphabet::Top()
{
    return Alphabet::minAlphabet[1];
}
