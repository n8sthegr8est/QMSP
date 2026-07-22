#include "ByteSwap.h"
#include <stdio.h>

int litend = -1;

//Checks and returns whether the system is big or little endian
int isLittleEndian(){
    if(litend < 0){
        char* ptr;
        int   tst;

        tst = 1;
        ptr = (char*) &tst;
        litend = *ptr;
    }
    return litend;
}

//Reverse the bytes of a 16-bit integer
uint16_t rev_16(uint16_t orig){
    uint8_t  hold1;
    uint8_t  hold2;
    uint16_t rever;

    rever = 0;
    hold1 = orig >> 8;
    hold2 = orig & 0xFF;

    rever  += hold2;
    rever <<= 8;
    rever  += hold1;
    
    return rever;
}

//Reverse the bytes of a 32-bit integer
uint32_t rev_32(uint32_t orig){
    uint16_t hold1;
    uint16_t hold2;
    uint32_t rever;

    rever = 0;
    hold1 = orig >> 16;
    hold2 = orig & 0xFFFF;

    rever  += rev_16(hold2);
    rever <<= 16;
    rever  += rev_16(hold1);

    return rever;
}

//Reverse the bytes of a 64-bit integer
uint64_t rev_64(uint64_t orig){
    uint32_t hold1;
    uint32_t hold2;
    uint64_t rever;

    rever = 0;
    hold1 = orig >> 32;
    hold2 = orig & 0xFFFFFFFF;

    rever  += rev_32(hold2);
    rever <<= 32;
    rever  += rev_32(hold1);

    return rever;
}

/*int main(void){
    uint16_t my16 = 0xF42D;
    uint32_t my32 = 0x3425FD2A;
    uint64_t my64 = 0xFDC23A57E2647D31;

    printf("Testing byte reverse 16:\n");
    printf("Expect: 0x2DF4\n");
    printf("Actual: 0x%X\n",rev_16(my16));
    
    printf("\nTesting byte reverse 32:\n");
    printf("Expect: 0x2AFD2534\n");
    printf("Actual: 0x%X\n",rev_32(my32));

    printf("\nTesting byte reverse 64:\n");
    printf("Expect: 0x317D64E2573AC2FD\n");
    printf("Actual: 0x%llX\n",(unsigned long long) rev_64(my64));

    printf("Is Little Endian: %i", isLittleEndian());
}*/