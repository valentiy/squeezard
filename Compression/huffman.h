#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <QDebug>
#include <QByteArray>
#include <string>
#include <cstdlib>

class AlgorithmHuf
{

private:
    unsigned char data;
    unsigned char minimum;
    unsigned int frequency;

    AlgorithmHuf * leftChar;
    AlgorithmHuf * rightChar;

public:
    AlgorithmHuf();
    AlgorithmHuf(const AlgorithmHuf &tfc);
    AlgorithmHuf(unsigned char d, unsigned int f) : data(d),  minimum(d), frequency(f){}
    AlgorithmHuf(AlgorithmHuf *, AlgorithmHuf *);
    ~AlgorithmHuf();

    bool operator> (const AlgorithmHuf &);
    void FillingCodeBook(std::string *, std::string &);

};

class Alphabet
{

private:
    AlgorithmHuf **minAlphabet;
    int alphabetSize;

public:
    Alphabet();
    ~Alphabet();

    void Push(AlgorithmHuf*);
    void Pop();
    int Size();
    AlgorithmHuf * Top();
};


#endif // HUFFMAN_H
