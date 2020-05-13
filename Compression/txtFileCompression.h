#ifndef TXTFILECOMPRESSION_H
#define TXTFILECOMPRESSION_H

#include "Compression/huffman.h"
#include <QFile>
#include <cstring>

class TxtFileCompression : public QFile
{
    Q_OBJECT

private:
    unsigned int frequences[256] = {0};
    std::string codebook[256];

public:
    TxtFileCompression();
    ~TxtFileCompression();

public:
    void Compress(QFile* file);
    void Decompress(QFile* file);
    void Pop(QFile* file);
    AlgorithmHuf* BiuldAlphabet();
};

/*
#include <QDebug>
#include <QByteArray>
#include <string>
#include <cstdlib>

class TxtFileCompression
{

private:
    unsigned char data;
    unsigned char minimum;
    unsigned int frequency;

    TxtFileCompression * leftChar;
    TxtFileCompression * rightChar;

public:
    TxtFileCompression();
    TxtFileCompression(const TxtFileCompression &tfc);
    TxtFileCompression(unsigned char d, unsigned int f) : data(d),  minimum(d), frequency(f){}
    TxtFileCompression(TxtFileCompression *, TxtFileCompression *);
    ~TxtFileCompression();

    bool operator> (const TxtFileCompression &);
    void FillingCodeBook(std::string *, std::string &);

};

class Alphabet
{

private:
    TxtFileCompression **minAlphabet;
    int alphabetSize;

public:
    Alphabet();
    ~Alphabet();

    void Push(TxtFileCompression*);
    void Pop();
    int Size();
    TxtFileCompression * Top();
};
*/
#endif // TXTFILECOMPRESSION_H
