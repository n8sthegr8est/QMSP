#include "VariableLengthInt.h"
#include <stdio.h>
#include <stdlib.h>

int main(void){
	unsigned char* LogVarIntUnpack_8 ;
	unsigned char* LogVarIntUnpack_16;
	unsigned char* LogVarIntUnpack_32;
	unsigned char* LogVarIntUnpack_64;

	unsigned long long  unpacked8;
	unsigned long long unpacked16;
	unsigned long long unpacked32;
	unsigned long long unpacked64;

	unsigned char* DirVarIntUnpack_8 ;
	unsigned char* DirVarIntUnpack_16;
	unsigned char* DirVarIntUnpack_24;
	unsigned char* DirVarIntUnpack_32;

	unsigned long  dunpacked8;
	unsigned long dunpacked16;
	unsigned long dunpacked24;
	unsigned long dunpacked32;

	unsigned long long lviPackXX;
	unsigned long long lviPack64;
	unsigned long long lviPack32;
	unsigned long long lviPack16;
	unsigned long long lviPack8 ;

	unsigned long long packedLvi8 ;
	unsigned long long packedLvi16;
	unsigned long long packedLvi32;
	unsigned long long packedLvi64;
	unsigned long long packedLviXX;
	
	unsigned long dviPack32;
	unsigned long dviPack24;
	unsigned long dviPack16;
	unsigned long dviPack8 ;

	unsigned long packedDvi8 ;
	unsigned long packedDvi16;
	unsigned long packedDvi24;
	unsigned long packedDvi32;

	LogVarIntUnpack_8  = malloc(1);
	LogVarIntUnpack_16 = malloc(2);
	LogVarIntUnpack_32 = malloc(4);
	LogVarIntUnpack_64 = malloc(8);
	
	*LogVarIntUnpack_8      =  63;

	*LogVarIntUnpack_16     = 127;
	*(LogVarIntUnpack_16+1) = 255;

	*LogVarIntUnpack_32     = 191;
	*(LogVarIntUnpack_32+1) = 255;
	*(LogVarIntUnpack_32+2) = 255;
	*(LogVarIntUnpack_32+3) = 255;
	
	*LogVarIntUnpack_64     = 255;
	*(LogVarIntUnpack_64+1) = 255;
	*(LogVarIntUnpack_64+2) = 255;
	*(LogVarIntUnpack_64+3) = 255;
	*(LogVarIntUnpack_64+4) = 255;
	*(LogVarIntUnpack_64+5) = 255;
	*(LogVarIntUnpack_64+6) = 255;
	*(LogVarIntUnpack_64+7) = 255;

	unpacked8  =  unpackLogVarInt(LogVarIntUnpack_8);
	free(LogVarIntUnpack_8);
	
	unpacked16 = unpackLogVarInt(LogVarIntUnpack_16);
	free(LogVarIntUnpack_16);
	
	unpacked32 = unpackLogVarInt(LogVarIntUnpack_32);
	free(LogVarIntUnpack_32);
	
	unpacked64 = unpackLogVarInt(LogVarIntUnpack_64);
	free(LogVarIntUnpack_64);

	printf("Unpack LogVarInt Tests:\n");

	printf("Expected value unpacked8 : 0x3F              ; Actual value: 0x%llX\n", unpacked8);
	printf("Expected value unpacked16: 0x3FFF            ; Actual value: 0x%llX\n", unpacked16);
	printf("Expected value unpacked32: 0x3FFFFFFF        ; Actual value: 0x%llX\n", unpacked32);
	printf("Expected value unpacked64: 0x3FFFFFFFFFFFFFFF; Actual value: 0x%llX\n", unpacked64);
	
	DirVarIntUnpack_8  = malloc(1);
	DirVarIntUnpack_16 = malloc(2);
	DirVarIntUnpack_24 = malloc(3);
	DirVarIntUnpack_32 = malloc(4);

	*DirVarIntUnpack_8      = 255;
	
	*DirVarIntUnpack_16     = 255;
	*(DirVarIntUnpack_16+1) = 255;

	*DirVarIntUnpack_24     = 255;
	*(DirVarIntUnpack_24+1) = 255;
	*(DirVarIntUnpack_24+2) = 255;
	
	*DirVarIntUnpack_32     = 255;
	*(DirVarIntUnpack_32+1) = 255;
	*(DirVarIntUnpack_32+2) = 255;
	*(DirVarIntUnpack_32+3) = 255;
	
	dunpacked8  =  unpackDirVarInt(DirVarIntUnpack_8, 1);
	free(DirVarIntUnpack_8);
	
	dunpacked16 = unpackDirVarInt(DirVarIntUnpack_16, 2);
	free(DirVarIntUnpack_16);
	
	dunpacked24 = unpackDirVarInt(DirVarIntUnpack_24, 3);
	free(DirVarIntUnpack_24);
	
	dunpacked32 = unpackDirVarInt(DirVarIntUnpack_32, 4);
	free(DirVarIntUnpack_32);

	printf("\nUnpack DirVarInt Tests:\n");

	printf("Expected value dunpacked8 : 0xFF      ; Actual value: 0x%lX\n", dunpacked8);
	printf("Expected value dunpacked16: 0xFFFF    ; Actual value: 0x%lX\n", dunpacked16);
	printf("Expected value dunpacked24: 0xFFFFFF  ; Actual value: 0x%lX\n", dunpacked24);
	printf("Expected value dunpacked32: 0xFFFFFFFF; Actual value: 0x%lX\n", dunpacked32);

	lviPackXX = 0x4FFFFFFFFFFFFFFF;
	lviPack64 = 0x3FFFFFFFFFFFFFFF;
	lviPack32 = 0x3FFFFFFF;
	lviPack16 = 0x3FFF;
	lviPack8  = 0x3F;

	packedLvi8  = packLogVarInt(lviPack8) ;
	packedLvi16 = packLogVarInt(lviPack16);
	packedLvi32 = packLogVarInt(lviPack32);
	packedLvi64 = packLogVarInt(lviPack64);
	packedLviXX = packLogVarInt(lviPackXX);

	printf("\nPack LogVarInt Tests:\n");
	
	printf("Expected value packedLvi8 : 0x3F00000000000000; Actual value: 0x%llX\n", packedLvi8);
	printf("Expected value packedLvi16: 0x7FFF000000000000; Actual value: 0x%llX\n", packedLvi16);
	printf("Expected value packedLvi32: 0xBFFFFFFF00000000; Actual value: 0x%llX\n", packedLvi32);
	printf("Expected value packedLvi64: 0xFFFFFFFFFFFFFFFF; Actual value: 0x%llX\n", packedLvi64);
	printf("Expected value packedLviXX: 0x0               ; Actual value: 0x%llX\n", packedLviXX);
	
	dviPack32 = 0xFFFFFFFF;
	dviPack24 = 0xFFFFFF;
	dviPack16 = 0xFFFF;
	dviPack8  = 0xFF;

	packedDvi8  = packDirVarInt(dviPack8) ;
	packedDvi16 = packDirVarInt(dviPack16);
	packedDvi24 = packDirVarInt(dviPack24);
	packedDvi32 = packDirVarInt(dviPack32);

	printf("\nPack DirVarInt Tests:\n");
	
	printf("Expected value packedDvi8 : 0xFF000000; Actual value: 0x%lX\n", packedDvi8);
	printf("Expected value packedDvi16: 0xFFFF0000; Actual value: 0x%lX\n", packedDvi16);
	printf("Expected value packedDvi24: 0xFFFFFF00; Actual value: 0x%lX\n", packedDvi24);
	printf("Expected value packedDvi32: 0xFFFFFFFF; Actual value: 0x%lX\n", packedDvi32);

	printf("\nLittle Endian Check Tests:\n");
	
	//printf("Expected littleEndianVal: -1      ; Actual: %i\n",getLittleEndianVal());
	//printf("Expected litendgttarg   : \"setval\"; Actual: %s\n",getGoTo());
	printf("Result isLittleEndian() : %i\n",isLittleEndian());
	//printf("Expected littleEndianVal: 0 or 1  ; Actual: %i\n",getLittleEndianVal());
	//printf("Expected litendgttarg   : \"valset\"; Actual: %s\n",getGoTo());
}
