#ifndef VAR_INTS
#define VAR_INTS
#include "ByteSwap.h"

uint64_t unpackLogVarInt(unsigned char* bytes)            ;
uint32_t unpackDirVarInt(unsigned char* bytes, int length);

uint64_t packLogVarInt(uint64_t value);
uint32_t packDirVarInt(uint32_t value);

int getSizeOfLVIToPack(uint64_t lvi);
int getSizeOfDVIToPack(uint32_t dvi);
#endif
