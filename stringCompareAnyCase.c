#include "stringCompareAnyCase.h"
#include <stdio.h>

int stringCaseCmp(char* lefty, char* right){
    char* leftypt;
    char* rightpt;
    
    char hold;
    char hord;
    
    leftypt = lefty;
    rightpt = right;
    while(1){
        if(!*leftypt && !*rightpt){
            return 0;
        }
        hold = *leftypt;
        hord = *rightpt;
        
        //capitalize lowercase letters
        if(hold >= 'a' && hold <= 'z'){
            hold &= 0x5F;
        }
        if(hord >= 'a' && hord <= 'z'){
            hord &= 0x5F;
        }

        //If they're different characters, return the difference between them
        if(hold-hord){
            return hold-hord;
        }

        leftypt++;
        rightpt++;
    }
}

int stringCaseCmpSeg(char* lefty, char* right, int len){
    char* leftypt;
    char* rightpt;
    
    char hold;
    char hord;

    int checked;

    leftypt = lefty;
    rightpt = right;
    checked = 0;
    while(1){
        if((!*leftypt && !*rightpt) || checked == len){
            return 0;
        }
        hold = *leftypt;
        hord = *rightpt;
        
        //capitalize lowercase letters
        if(hold >= 'a' && hold <= 'z'){
            hold &= 0x5F;
        }
        if(hord >= 'a' && hord <= 'z'){
            hord &= 0x5F;
        }

        //If they're different characters, return the difference between them
        if(hold-hord){
            return hold-hord;
        }

        leftypt++;
        rightpt++;
        checked++;
    }
}