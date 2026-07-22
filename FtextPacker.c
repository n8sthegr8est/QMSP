#include "FtextPacker.h"
#include "colorNames.h"
#include <stdlib.h>
#include <string.h>

//Formatting in raw:
//Size:      <s32>text</>
//Color:    <cred>text</> or <cblue>text</> etc.
//Color: <#4213FF>text</>
//Bold:        <b>text</>
//Italic:      <i>text</>
//Underline:   <u>text</>
//Strikethru:  <x>text</>
//Superscript: <^>text</>
//Subscript:   <v>text</>
//multiple: <bis32cred>text</>
//one to another: <b>text<bu>text<b>text</>
//don't format: /<x> /</>

//Bold:         **text**
//Italic:        *text*
//Bold&Italic: ***text***
//Underline:    __text__
//Strikethru:   ~~text~~
//Superscript:  ^^text^^
//Subscript:   ^^^text^^^
//Cancel form:\_\_text\_\_

//packed formatting: {text bytes} 0x08 0xXX {text bytes} 0x08 {text bytes}

//formatting id: xxxx xxxx
//formatting index: scbi ux^v (spoi nts#) (cred #### | cgre en## | cblu e###)

int colorLen(char* color){
    if(*color == "#"){
        return 7;//#XXXXXX
    }
    else{
        return lastColorNameLen();
    }
}

int isValidColor(char* color){
    if(*color == "#"){
        for(int i = 1; i < 7; i++){
            if(*(color+i) < '0' || (*(color+i) > '9' && *(color+i) < 'A') || (*(color+i) > 'F' && *(color+i) < 'a') || *(color+i) > 'f'){
                return 0;
            }
        }
        return 1;
    }
    else{
        return (mapName(color) > -1);
    }
}

int isValidFormatCmd(char* copypoint){
    char*           copypnt;
    uint64_t sizeColorCheck;
    char*    nameColorCheck;
    char               bold;
    char               ital;
    char               ulin;
    char               stru;
    char               sups;
    char               subs;
    char               size;
    char               colr;

    copypnt = copypoint;
    
    bold = 0;
    ital = 0;
    ulin = 0;
    stru = 0;
    sups = 0;
    subs = 0;
    size = 0;
    colr = 0;
    while(*copypnt != '>'){
        switch(*copypnt){
            case '/':
                if(copypnt == copypoint && *(copypnt+1) == '>'){
                    return 1;
                }
                return 0;
            case 'b':
                if(bold == 1){
                    return 0;
                }
                bold = 1;
                copypnt++;
                break;
            case 'i':
                if(ital == 1){
                    return 0;
                }
                ital = 1;
                copypnt++;
                break;
            case 'u':
                if(ulin == 1){
                    return 0;
                }
                ulin = 1;
                copypnt++;
                break;
            case 'x':
                if(stru == 1){
                    return 0;
                }
                stru = 1;
                copypnt++;
                break;
            case '^':
                if(subs == 1 || sups == 1){
                    return 0;
                }
                sups = 1;
                copypnt++;
                break;
            case 'v':
                if(subs == 1 || sups == 1){
                    return 0;
                }
                subs = 1;
                copypnt++;
                break;
            case 's':
                if(size == 1){
                    return 0;
                }
                size = 1;
                copypnt++;
                if((*copypnt - '0') < 0 || (*copypnt - '0') >= 10){
                    return 0;
                }
                sizeColorCheck = 0;
                while((*copypnt - '0') >= 0 && (*copypnt - '0') < 10){
                    sizeColorCheck *= 10;
                    sizeColorCheck += *copypnt - '0';
                    copypnt++;
                }
                if(sizeColorCheck > 255){
                    return 0;
                }
                break;
            case 'c':
                if(colr == 1){
                    return 0;
                }
                colr = 1;
                if(isValidColor(copypnt+1) < 0){
                    return 0;
                }
                //
                break;
            default:
                return 0;
        }
    }
    return 1;
}

int matchesIndex(char* fmt, struct FormattingIndex* fdx){
    if((fdx->SimpleFormat & 0x20) && (strchr(fmt,'b') == NULL && strchr(fmt,'B') == NULL)){
        return 0;
    }
}

struct FormattingIndex* createFormattingIndex(){

}

//frees all allocated formatting indexes if we run out of memory
void freeAllFormattingIndexesOnOutOfMem(int requisiteCount, struct FormattingIndex** requisites){
    for(int i = 0; i < requisiteCount; i++){

    }
}

//Packs text with formatting into a condensed string for an FText Frame and loads a passed array with needed formatting indices.
char* packFText(char* rawText, struct FormattingIndex** requisites){
    char*          newText;
    unsigned char* copypnt;
    int            charlen;
    unsigned char* copydst;
    char*          tempory;
    int            inFrmat;
    int            requcnt;
    char*          ftagstr;
    int            ftaglen;

    copypnt = rawText;
    inFrmat = 0;
    charlen = 0;
    newText = NULL;
    requcnt = 0;

    while(*copypnt != 0x00){
        if(*copypnt == '<' && ((copypnt == rawText) ? 1 : (*(copypnt-1) != '/')) && isValidFormatCmd(copypnt+1)){
            inFrmat = 1;
            charlen += 2;
            tempory = realloc(newText,charlen);
            if(tempory == NULL){
                freeAllFormattingIndexesOnOutOfMem(requcnt,&requisites);
                free(requisites);
                free(newText);
                return NULL;
            }
            newText = tempory;
            newText[charlen-2] = 0x08;

            ftaglen = strcspn(copypnt,'>');
            ftagstr = malloc(ftaglen+1);
            ftagstr = strncpy(ftagstr,copypnt,ftaglen);
            ftagstr[ftaglen] = 0x00;

            int i = 0;

            for(i = 0; i < requcnt; i++){
                if(matchesIndex(ftagstr,requisites[i])){
                    newText[charlen-1] = i;
                    break;
                }
            }
            if(i==requcnt && requcnt < 255){
                requcnt++;
                tempory = realloc(requisites,sizeof(char*) * requcnt);
                if(tempory == NULL){
                    freeAllFormattingIndexesOnOutOfMem(requcnt,&requisites);
                    free(requisites);
                    free(newText);
                    return NULL;
                }
            }
        }
    }
}

/*struct FTextFrame* packFText(char* text,struct FormattingFrame fFrames){
    struct FTextFrame* packedText;

    packedText = malloc(sizeof(struct FTextFrame));
    packedText

    return packedText;
}*/



/*char* unpackFText(struct FTextFrame*, uint8_t, struct FormattingFrame*);*/