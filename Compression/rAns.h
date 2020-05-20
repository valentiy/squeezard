#ifndef RANS_H
#define RANS_H

#include <stdint.h>
#include <cstdlib>

#include <QDebug>

#ifdef assert
#define RansAssert assert
#else
#define RansAssert(x)
#endif

typedef uint32_t RansState;

class RAns
{
public:

    struct RansEncSymbol
    {
        uint32_t x_max;     // (Exclusive) upper bound of pre-normalization interval
        uint32_t rcp_freq;  // Fixed-point reciprocal frequency
        uint32_t bias;      // Bias
        uint16_t cmpl_freq; // Complement of frequency: (1 << scale_bits) - freq
        uint16_t rcp_shift; // Reciprocal shift
    };

    struct RansDecSymbol
    {
        uint16_t start;     // Start of range.
        uint16_t freq;      // Symbol frequency.
    };

public:

    RAns();
    ~RAns();
    static void RansEncInit(RansState* r);
    static RansState RansEncRenorm(RansState x, uint8_t** pptr, uint32_t freq, uint32_t scale_bits);
    static void RansEncPut(RansState* r, uint8_t** pptr, uint32_t start, uint32_t freq, uint32_t scale_bits);
    static void RansEncFlush(RansState* r, uint8_t** pptr);
    static void RansDecInit(RansState* r, uint8_t** pptr);
    static uint32_t RansDecGet(RansState* r, uint32_t scale_bits);
    static void RansDecAdvance(RansState* r, uint8_t** pptr, uint32_t start, uint32_t freq, uint32_t scale_bits);
    static void RansEncSymbolInit(RansEncSymbol* s, uint32_t start, uint32_t freq, uint32_t scale_bits);
    static void RansDecSymbolInit(RansDecSymbol* s, uint32_t start, uint32_t freq);
    static void RansEncPutSymbol(RansState* r, uint8_t** pptr, RansEncSymbol const* sym);
    static void RansDecAdvanceSymbol(RansState* r, uint8_t** pptr, RansDecSymbol const* sym, uint32_t scale_bits);
    static void RansDecAdvanceStep(RansState* r, uint32_t start, uint32_t freq, uint32_t scale_bits);
    static void RansDecAdvanceSymbolStep(RansState* r, RansDecSymbol const* sym, uint32_t scale_bits);
    static void RansDecRenorm(RansState* r, uint8_t** pptr);


};

#endif // RANS_H
