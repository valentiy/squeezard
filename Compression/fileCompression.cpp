#include "Compression/fileCompression.h"

FileCompression::FileCompression()
{
    msgBox.setMinimumSize(500,500);
    msgBox.setInformativeText("Do you want to save new file?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    QObject::connect(this, &FileCompression::SetFileType, this, &FileCompression::GetFileType);
    return;
};

FileCompression::~FileCompression()
{

};

void FileCompression::GetFileType(QString type)
{
    FileCompression::fileType = type;
    return;
}

uint8_t* FileCompression::read_file(QFile* file, size_t* out_size)
{
    if ( file->exists() )
        qDebug() << "File exists";
    else
    {
        qDebug() << "File does not exist";
    }
    if ( file->open(QIODevice::ReadOnly) )
        qDebug() << "File is openned";
    else
        qDebug() << "File is not open";

    size_t size = file->size();

    char chars;
    uint8_t* buf = new uint8_t[size];
    while (!file->atEnd())
    {
           file->getChar(&chars);
           buf[file->pos()-1] = (unsigned char)chars;
    }

    file->close();

    if (out_size)
        *out_size = size;

    return buf;
};

uint8_t* FileCompression::read(QFile* file, size_t* in_size, uint32_t* freqs)
{
    if ( file->exists() )
        qDebug() << "File exists";
    else
    {
        qDebug() << "File does not exist";
    }
    if ( file->open(QIODevice::ReadOnly) )
        qDebug() << "File is openned";
    else
        qDebug() << "File is not open";

    size_t size = file->size();

    uint8_t* buf = new uint8_t[size];

    QDataStream data(file);

    data >> FileCompression::fileType;
    data >> *in_size;
    qDebug() << "size :" << *in_size;

    for(int i = 0; i < 256; i++)
    {
        data >> freqs[i];
    }

   int iterator = 0;
    while (!data.atEnd())
    {
           data >> buf[iterator];
           iterator++;
    }

    file->close();
    qDebug() << "end reading";
    return buf;
};



void FileCompression::count_freqs(uint8_t const* in, size_t nbytes)
{
    for (int i=0; i < 256; i++)
        FileCompression::freqs[i] = 0;

    for (size_t i=0; i < nbytes; i++)
        FileCompression::freqs[in[i]]++;
}

void FileCompression::calc_cum_freqs()
{
    FileCompression::cum_freqs[0] = 0;
    for (int i=0; i < 256; i++)
        FileCompression::cum_freqs[i+1] = cum_freqs[i] + freqs[i];
}

void FileCompression::normalize_freqs(uint32_t target_total)
{
    assert(target_total >= 256);

    FileCompression::calc_cum_freqs();
    uint32_t cur_total = cum_freqs[256];

    // resample distribution based on cumulative freqs
    for (int i = 1; i <= 256; i++)
        FileCompression::cum_freqs[i] = ((uint64_t)target_total * cum_freqs[i])/cur_total;

    // if we nuked any non-0 frequency symbol to 0, we need to steal
    // the range to make the frequency nonzero from elsewhere.
    //
    // this is not at all optimal, i'm just doing the first thing that comes to mind.
    for (int i=0; i < 256; i++) {
        if (FileCompression::freqs[i] && FileCompression::cum_freqs[i+1] == FileCompression::cum_freqs[i]) {
            // symbol i was set to zero freq

            // find best symbol to steal frequency from (try to steal from low-freq ones)
            uint32_t best_freq = ~0u;
            int best_steal = -1;
            for (int j=0; j < 256; j++) {
                uint32_t freq = FileCompression::cum_freqs[j+1] - FileCompression::cum_freqs[j];
                if (freq > 1 && freq < best_freq) {
                    best_freq = freq;
                    best_steal = j;
                }
            }
            assert(best_steal != -1);

            // and steal from it!
            if (best_steal < i) {
                for (int j = best_steal + 1; j <= i; j++)
                    FileCompression::cum_freqs[j]--;
            } else {
                assert(best_steal > i);
                for (int j = i + 1; j <= best_steal; j++)
                    FileCompression::cum_freqs[j]++;
            }
        }
    }

    // calculate updated freqs and make sure we didn't screw anything up
    assert(FileCompression::cum_freqs[0] == 0 && FileCompression::cum_freqs[256] == target_total);
    for (int i=0; i < 256; i++) {
        if (freqs[i] == 0)
            assert(FileCompression::cum_freqs[i+1] == FileCompression::cum_freqs[i]);
        else
            assert(FileCompression::cum_freqs[i+1] > FileCompression::cum_freqs[i]);

        // calc updated freq
        FileCompression::freqs[i] = FileCompression::cum_freqs[i+1] - FileCompression::cum_freqs[i];
    }
}

void FileCompression::Compresss(QFile* file)
{
    uint32_t prob_bits = 14;
    uint32_t prob_scale = 1 << prob_bits;

    uint8_t* in_bytes = read_file(file, &in_size);

    FileCompression::count_freqs(in_bytes, in_size);
    FileCompression::normalize_freqs(prob_scale);

    for (int i=0; i < 256; i++)
    {
        qDebug() << FileCompression::freqs[i];
    }

    uint8_t* cum2sym = new uint8_t[prob_scale];

        for (int s=0; s < 256; s++)
            for (uint32_t i= FileCompression::cum_freqs[s]; i < FileCompression::cum_freqs[s+1]; i++)
                cum2sym[i] = s;

        static size_t out_max_size = 32<<20; // 32MB
        uint8_t* out_buf = new uint8_t[out_max_size];
        uint8_t* dec_bytes = new uint8_t[in_size];

        // try rANS encode
        uint8_t *rans_begin;
        RAns* algorithm = new RAns();
        RAns::RansEncSymbol esyms[256];
        RAns::RansDecSymbol dsyms[256];

        for (int i=0; i < 256; i++)
        {
            algorithm->RansEncSymbolInit(&esyms[i], FileCompression::cum_freqs[i], FileCompression::freqs[i], prob_bits);
            algorithm->RansDecSymbolInit(&dsyms[i], FileCompression::cum_freqs[i], FileCompression::freqs[i]);
        }

        // ---- regular rANS encode/decode. Typical usage.

        memset(dec_bytes, 0xcc, in_size);
        QFile* file_compressed = new QFile("compressed");
        file_compressed->open(QIODevice::WriteOnly);

        QDataStream put_data(file_compressed);

        for (int run=0; run < 5; run++)
        {

            RansState rans;
            algorithm->RansEncInit(&rans);

            uint8_t* ptr = out_buf + out_max_size; // *end* of output buffer
            for (size_t i=in_size; i > 0; i--)
            { // NB: working in reverse!
                int s = in_bytes[i-1];
                algorithm->RansEncPutSymbol(&rans, &ptr, &esyms[s]);
            }

            algorithm->RansEncFlush(&rans, &ptr);
            rans_begin = ptr;

        }

        put_data << FileCompression::fileType;

        put_data << in_size;
        qDebug() << "size :" << in_size;

        for (int i=0; i < 256; i++)
        {
            qDebug() << FileCompression::freqs[i];
            put_data << FileCompression::freqs[i];
        }

        uint8_t* iterator = rans_begin;
        while(iterator <= out_buf + out_max_size)
        {
            put_data << *iterator;
            iterator++;
        }

        int result = (int)(out_buf + out_max_size - rans_begin);

        msgBox.setText("Compression's complete !\n to " + QString::number(result) + " bytes");
        msgBox.exec();
        int ret = msgBox.exec();

        switch (ret)
        {
            case QMessageBox::Save: emit FileCompression::SaveFlag(file_compressed); break;
            case QMessageBox::Cancel:  break;
        }

         file_compressed->QFileDevice::close();
        delete file_compressed;
        qDebug() << "Compression's complete !";

        qDebug() << "to " << (int)(out_buf + out_max_size - rans_begin) << "bytes";

        delete[] out_buf;
        delete[] dec_bytes;
        delete[] in_bytes;
        delete[] cum2sym;

        return;

}

void FileCompression::Decompress(QFile* file)
{
    uint32_t prob_bits = 14;
    uint32_t prob_scale = 1 << prob_bits;

    uint8_t* in_bytes = read(file, &in_size, FileCompression::freqs);
    for(int i =0; i<256; i++)
    {
        qDebug() << FileCompression::freqs[i];
    }

    FileCompression::normalize_freqs(prob_scale);

    uint8_t* cum2sym = new uint8_t[prob_scale];
    for (int s=0; s < 256; s++)
        for (uint32_t i= FileCompression::cum_freqs[s]; i < FileCompression::cum_freqs[s+1]; i++)
            cum2sym[i] = s;

    uint8_t* dec_bytes = new uint8_t[in_size];

    RAns::RansDecSymbol dsyms[256];

    QFile* new_file =  new QFile("decompressed");

    new_file->open(QIODevice::WriteOnly);
    QDataStream put_data(new_file);

    RAns * algorithm = new RAns();

    for (int i=0; i < 256; i++)
    {
        algorithm->RansDecSymbolInit(&dsyms[i], FileCompression::cum_freqs[i], FileCompression::freqs[i]);
    }

    for (int run=0; run < 5; run++)
    {

        RansState rans;
        uint8_t* ptr = in_bytes;//rans_begin;
        algorithm->RansDecInit(&rans, &ptr);

        for (size_t i=0; i < in_size; i++)
        {
            uint32_t s = cum2sym[algorithm->RansDecGet(&rans, prob_bits)];
            dec_bytes[i] = (uint8_t) s;
            algorithm->RansDecAdvanceSymbol(&rans, &ptr, &dsyms[s], prob_bits);
            if (run == 4) put_data << dec_bytes[i];
        }

    }

    msgBox.setText("Decompression complete!");
    msgBox.exec();
    int ret = msgBox.exec();

    switch (ret)
    {
        case QMessageBox::Save: emit FileCompression::SendType(fileType); emit FileCompression::SaveFlag(new_file); break;
        case QMessageBox::Cancel:  break;
    }

    new_file->QFileDevice::close();

    new_file->close();
    delete new_file;

    delete[] dec_bytes;
    delete[] in_bytes;
    delete[] cum2sym;
    qDebug() << "end decode";
    return;


}
