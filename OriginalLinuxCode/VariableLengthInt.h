#ifndef VAR_INTS
#define VAR_INTS

unsigned long long unpackLogVarInt(unsigned char* bytes)            ;
unsigned long      unpackDirVarInt(unsigned char* bytes, int length);

unsigned long long packLogVarInt(unsigned long long value);
unsigned long      packDirVarInt(unsigned long      value);

int getSizeOfLVIToPack(unsigned long long lvi);
int getSizeOfDVIToPack(unsigned long      dvi);

int isLittleEndian();
#endif
