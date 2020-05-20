#ifndef FILECOMPRESSION_H
#define FILECOMPRESSION_H

#include <QFile>
#include <QDebug>
#include <QByteArray>

#include "Compression/rAns.h"

class FileCompression
{

private:

    uint32_t freqs[256];
    uint32_t cum_freqs[257];
    size_t in_size;

    static const uint32_t prob_bits;
    static const uint32_t prob_scale;


public:

    FileCompression();
    ~FileCompression();
    static uint8_t* read_file(QFile* file, size_t* out_size);
    void count_freqs(uint8_t const* in, size_t nbytes);
    void calc_cum_freqs();
    void normalize_freqs(uint32_t target_total);
    void Compress(QFile*);

private:

};

#endif
