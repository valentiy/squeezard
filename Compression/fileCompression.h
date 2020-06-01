#ifndef FILECOMPRESSION_H
#define FILECOMPRESSION_H

#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QDataStream>
#include <QString>

#include "Compression/rAns.h"

class FileCompression : public QObject
{

    Q_OBJECT

private:

    uint32_t freqs[256];
    uint32_t cum_freqs[257];
    size_t in_size;
    QString fileType;

    static const uint32_t prob_bits;
    static const uint32_t prob_scale;

private:
    static uint8_t* read_file(QFile*, size_t*);
    uint8_t* read(QFile*, size_t*, uint32_t*);
    void count_freqs(uint8_t const* in, size_t);
    void calc_cum_freqs();
    void normalize_freqs(uint32_t);

public:

    FileCompression();
    ~FileCompression();
    void Compresss(QFile*);
    void Decompress(QFile*);

private slots:
    void GetFileType(QString);

signals:
    void SetFileType(QString);


};

#endif
