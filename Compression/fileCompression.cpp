#include "Compression/fileCompression.h"

FileCompression::FileCompression()
{

};

FileCompression::~FileCompression()
{

};

uint8_t* FileCompression::read_file(QFile* file, size_t* out_size)
{
    if ( file->exists() )
        qDebug() << "File exists";
    else
    {
        qDebug() << "File does not exist";
       // return;
    }
    if ( file->open(QIODevice::ReadOnly) )
        qDebug() << "File is openned";
    else
        qDebug() << "File is not open";

    size_t size = file->size();
    //file->seek(0);

    char chars;
    uint8_t* buf = new uint8_t[size];
    while (!file->atEnd())
    {
           file->getChar(&chars);
           buf[file->pos()-1] = (unsigned char)chars;
    }

    //QByteArray buf = file->readAll();

    file->close();

    if (out_size)
        *out_size = size;

    return buf;
};

void FileCompression::SaveFile(uint32_t rans)
{
    QFile* file = new QFile("C:/Users/donva/Desktop/compressed.txt");
    if ( file->open(QIODevice::WriteOnly) )
         //file->write(rans);
     file->QFileDevice::close();

     file->close();
     delete file;

     return;
}

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

void FileCompression::Compress(QFile* file)
{
     uint32_t prob_bits = 14;
     uint32_t prob_scale = 1 << prob_bits;

     uint8_t* in_bytes = read_file(file, &in_size);

     FileCompression::count_freqs(in_bytes, in_size);
     FileCompression::normalize_freqs(prob_scale);

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

         for (int i=0; i < 256; i++) {
             algorithm->RansEncSymbolInit(&esyms[i], FileCompression::cum_freqs[i], FileCompression::freqs[i], prob_bits);
             algorithm->RansDecSymbolInit(&dsyms[i], FileCompression::cum_freqs[i], FileCompression::freqs[i]);
         }

         // ---- regular rANS encode/decode. Typical usage.

         memset(dec_bytes, 0xcc, in_size);

         for (int run=0; run < 5; run++) {
             //double start_time = timer();
             //uint64_t enc_start_time = __rdtsc();

             RansState rans;
             algorithm->RansEncInit(&rans);

             uint8_t* ptr = out_buf + out_max_size; // *end* of output buffer
             for (size_t i=in_size; i > 0; i--) { // NB: working in reverse!
                 int s = in_bytes[i-1];
                 algorithm->RansEncPutSymbol(&rans, &ptr, &esyms[s]);
             }
             algorithm->RansEncFlush(&rans, &ptr);
             rans_begin = ptr;

            // uint64_t enc_clocks = __rdtsc() - enc_start_time;
            // double enc_time = timer() - start_time;

         }


         // try rANS decode
         for (int run=0; run < 5; run++) {
             //double start_time = timer();
             //uint64_t dec_start_time = __rdtsc();

             RansState rans;
             uint8_t* ptr = rans_begin;
             algorithm->RansDecInit(&rans, &ptr);

             for (size_t i=0; i < in_size; i++) {
                 uint32_t s = cum2sym[algorithm->RansDecGet(&rans, prob_bits)];
                 dec_bytes[i] = (uint8_t) s;
                 algorithm->RansDecAdvanceSymbol(&rans, &ptr, &dsyms[s], prob_bits);
             }

           //  uint64_t dec_clocks = __rdtsc() - dec_start_time;
           //  double dec_time = timer() - start_time;

         }

         // check decode results
         if (memcmp(in_bytes, dec_bytes, in_size) == 0)
             qDebug() << "GOOD DECODE";
         else
             qDebug() << "ERROR: bad decoder!";

         // ---- interleaved rANS encode/decode. This is the kind of thing you might do to optimize critical paths.

         memset(dec_bytes, 0xcc, in_size);

         // try interleaved rANS encode
         for (int run=0; run < 5; run++) {
             //double start_time = timer();
             //uint64_t enc_start_time = __rdtsc();

             RansState rans0, rans1;
             algorithm->RansEncInit(&rans0);
             algorithm->RansEncInit(&rans1);

             uint8_t* ptr = out_buf + out_max_size; // *end* of output buffer

             // odd number of bytes?
             if (in_size & 1) {
                 int s = in_bytes[in_size - 1];
                 algorithm->RansEncPutSymbol(&rans0, &ptr, &esyms[s]);
             }

             for (size_t i=(in_size & ~1); i > 0; i -= 2) { // NB: working in reverse!
                 int s1 = in_bytes[i-1];
                 int s0 = in_bytes[i-2];
                 algorithm->RansEncPutSymbol(&rans1, &ptr, &esyms[s1]);
                 algorithm->RansEncPutSymbol(&rans0, &ptr, &esyms[s0]);
             }
             algorithm->RansEncFlush(&rans1, &ptr);
             algorithm->RansEncFlush(&rans0, &ptr);
             rans_begin = ptr;

            // uint64_t enc_clocks = __rdtsc() - enc_start_time;
             //double enc_time = timer() - start_time;

         }


         // try interleaved rANS decode
         for (int run=0; run < 5; run++) {
             //double start_time = timer();
             //uint64_t dec_start_time = __rdtsc();

             RansState rans0, rans1;
             uint8_t* ptr = rans_begin;
             algorithm->RansDecInit(&rans0, &ptr);
             algorithm->RansDecInit(&rans1, &ptr);

             for (size_t i=0; i < (in_size & ~1); i += 2) {
                 uint32_t s0 = cum2sym[algorithm->RansDecGet(&rans0, prob_bits)];
                 uint32_t s1 = cum2sym[algorithm->RansDecGet(&rans1, prob_bits)];
                 dec_bytes[i+0] = (uint8_t) s0;
                 dec_bytes[i+1] = (uint8_t) s1;
                 algorithm->RansDecAdvanceSymbolStep(&rans0, &dsyms[s0], prob_bits);
                 algorithm->RansDecAdvanceSymbolStep(&rans1, &dsyms[s1], prob_bits);
                 algorithm->RansDecRenorm(&rans0, &ptr);
                 algorithm->RansDecRenorm(&rans1, &ptr);
             }

             // last byte, if number of bytes was odd
             if (in_size & 1) {
                 uint32_t s0 = cum2sym[algorithm->RansDecGet(&rans0, prob_bits)];
                 dec_bytes[in_size - 1] = (uint8_t) s0;
                 algorithm->RansDecAdvanceSymbol(&rans0, &ptr, &dsyms[s0], prob_bits);
             }

           //  uint64_t dec_clocks = __rdtsc() - dec_start_time;
           //  double dec_time = timer() - start_time;

         }

         // check decode results
         if (memcmp(in_bytes, dec_bytes, in_size) == 0)
             qDebug() << "GOOD DECODE";
         else
             qDebug() << "ERROR: bad decoder!";

         delete[] out_buf;
         delete[] dec_bytes;
         delete[] in_bytes;
         delete[] cum2sym;

         return;

}

void FileCompression::Compresss(QFile* file)
{
    uint32_t prob_bits = 14;
    uint32_t prob_scale = 1 << prob_bits;

    uint8_t* in_bytes = read_file(file, &in_size);

    FileCompression::count_freqs(in_bytes, in_size);
    FileCompression::normalize_freqs(prob_scale);

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

        for (int i=0; i < 256; i++) {
            algorithm->RansEncSymbolInit(&esyms[i], FileCompression::cum_freqs[i], FileCompression::freqs[i], prob_bits);
            algorithm->RansDecSymbolInit(&dsyms[i], FileCompression::cum_freqs[i], FileCompression::freqs[i]);
        }

        // ---- regular rANS encode/decode. Typical usage.

        memset(dec_bytes, 0xcc, in_size);

        for (int run=0; run < 5; run++) {
            //double start_time = timer();
            //uint64_t enc_start_time = __rdtsc();

            RansState rans;
            algorithm->RansEncInit(&rans);
            QByteArray write_in ;//= new QByteArray();

            uint8_t* ptr = out_buf + out_max_size; // *end* of output buffer
            for (size_t i=in_size; i > 0; i--) { // NB: working in reverse!
                int s = in_bytes[i-1];
                algorithm->RansEncPutSymbol(&rans, &ptr, &esyms[s]);
                write_in.append(rans);
            }
            //QDataStream* data = new QDataStream(&write_in, QIODevice::WriteOnly);
            QFile* file_compressed = new QFile("C:/Users/donva/Desktop/compressed.txt");
            if ( file_compressed->open(QIODevice::WriteOnly) )
                 file_compressed->write(write_in);
             else
             file_compressed->QFileDevice::close();

             file_compressed->close();
             delete file_compressed;

            algorithm->RansEncFlush(&rans, &ptr);
            rans_begin = ptr;

           // uint64_t enc_clocks = __rdtsc() - enc_start_time;
           // double enc_time = timer() - start_time;

        }

        qDebug() << "Compression's complete !";
        delete[] out_buf;
        delete[] dec_bytes;
        delete[] in_bytes;
        delete[] cum2sym;

        return;

}
