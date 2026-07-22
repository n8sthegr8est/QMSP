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

    /*char* shortLefty;
    char* shortRight;
    int       answer;

    shortLefty = malloc(len+1);
    shortRight = malloc(len+1);

    strncpy(shortLefty,lefty,len);
    strncpy(shortRight,right,len);
    shortLefty[len] = 0x00;
    shortRight[len] = 0x00;

    answer = stringCaseCmp(shortLefty,shortRight);
    free(shortLefty);
    free(shortRight);

    return answer;*/
}

/*int main(void){
   char* myfirstTest1 = "Someone";
   char* myfirstTest2 = "Someone";
   char* myseconTest1 = "moronitude";
   char* myseconTest2 = "MORONITUDE";
   char* mythirdTest1 = "short";
   char* mythirdTest2 = "shorter";
   char* myfourtTest1 = "SHORTER";
   char* myfourtTest2 = "short";
   char* myfifthTest1 = "@nope";
   char* myfifthTest2 = "`nope";
   char* mysixthTest1 = "[nope]";
   char* mysixthTest2 = "{nope}";
   char* mysevenTest1 = "\nnope";
   char* mysevenTest2 = "*nope";
   char* myeightTest1 = "|nope";
   char* myeightTest2 = "\\nope";
   char* myninthTest1 = "^nope";
   char* myninthTest2 = "~nope";
   char* mytenthTest1 = "IamTHESaMe";
   char* mytenthTest2 = "iAMthesAmE";
   char* mylevenTest1 = "the quick brown fox jumps over the lazy dog";
   char* mylevenTest2 = "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG";
   char* mytwelvTest1 = "rgiw9";
   char* mytwelvTest2 = "RgIw)";

   printf("Test 1  should be = 0, and it is %i\n",stringCaseCmp(myfirstTest1,myfirstTest2));
   printf("Test 2  should be = 0, and it is %i\n",stringCaseCmp(myseconTest1,myseconTest2));
   printf("Test 3  should be < 0, and it is %i\n",stringCaseCmp(mythirdTest1,mythirdTest2));
   printf("Test 4  should be > 0, and it is %i\n",stringCaseCmp(myfourtTest1,myfourtTest2));
   printf("Test 5  should be < 0, and it is %i\n",stringCaseCmp(myfifthTest1,myfifthTest2));
   printf("Test 6  should be < 0, and it is %i\n",stringCaseCmp(mysixthTest1,mysixthTest2));
   printf("Test 7  should be < 0, and it is %i\n",stringCaseCmp(mysevenTest1,mysevenTest2));
   printf("Test 8  should be > 0, and it is %i\n",stringCaseCmp(myeightTest1,myeightTest2));
   printf("Test 9  should be < 0, and it is %i\n",stringCaseCmp(myninthTest1,myninthTest2));
   printf("Test 10 should be = 0, and it is %i\n",stringCaseCmp(mytenthTest1,mytenthTest2));
   printf("Test 11 should be = 0, and it is %i\n",stringCaseCmp(mylevenTest1,mylevenTest2));
   printf("Test 12 should be > 0, and it is %i\n",stringCaseCmp(mytwelvTest1,mytwelvTest2));
}*/