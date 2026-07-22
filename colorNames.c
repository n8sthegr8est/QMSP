#include "colorNames.h"
#include "stringCompareAnyCase.h"

int lastLen = -1;

ColorValue mapName(char* name){
    //Order could eventually matter,
    //particularly if we ever add colors like AQUAmarine or REDviolet.
    //it does raise concerns about updating this in the future.
    //for ease of modification, we may want to find a better way.
    if   (stringCaseCmpSeg(name,"WHITE",5) == 0){lastLen = 5;   return WHITE;}
    if  (stringCaseCmpSeg(name,"SILVER",6) == 0){lastLen = 6;  return SILVER;}
    if    (stringCaseCmpSeg(name,"GRAY",4) == 0){lastLen = 4;    return GRAY;}
    if   (stringCaseCmpSeg(name,"BLACK",5) == 0){lastLen = 5;   return BLACK;}
    if     (stringCaseCmpSeg(name,"RED",3) == 0){lastLen = 3;     return RED;}
    if  (stringCaseCmpSeg(name,"MAROON",6) == 0){lastLen = 6;  return MAROON;}
    if  (stringCaseCmpSeg(name,"YELLOW",6) == 0){lastLen = 6;  return YELLOW;}
    if   (stringCaseCmpSeg(name,"OLIVE",5) == 0){lastLen = 5;   return OLIVE;}
    if    (stringCaseCmpSeg(name,"LIME",4) == 0){lastLen = 4;    return LIME;}
    if   (stringCaseCmpSeg(name,"GREEN",5) == 0){lastLen = 5;   return GREEN;}
    if    (stringCaseCmpSeg(name,"AQUA",4) == 0){lastLen = 4;    return AQUA;}
    if    (stringCaseCmpSeg(name,"TEAL",4) == 0){lastLen = 4;    return TEAL;}
    if    (stringCaseCmpSeg(name,"BLUE",4) == 0){lastLen = 4;    return BLUE;}
    if    (stringCaseCmpSeg(name,"NAVY",4) == 0){lastLen = 4;    return NAVY;}
    if (stringCaseCmpSeg(name,"FUCHSIA",7) == 0){lastLen = 7; return FUCHSIA;}
    if  (stringCaseCmpSeg(name,"PURPLE",6) == 0){lastLen = 6;  return PURPLE;}

    if    (stringCaseCmpSeg(name,"CYAN",4) == 0){lastLen = 4;    return CYAN;}
    if (stringCaseCmpSeg(name,"MAGENTA",7) == 0){lastLen = 7; return MAGENTA;}

    if  (stringCaseCmpSeg(name,"ORANGE",6) == 0){lastLen = 6;  return ORANGE;}
    if  (stringCaseCmpSeg(name,"INDIGO",6) == 0){lastLen = 6;  return INDIGO;}
    if   (stringCaseCmpSeg(name,"SLATE",5) == 0){lastLen = 5;   return SLATE;}
    if    (stringCaseCmpSeg(name,"GOLD",4) == 0){lastLen = 4;    return GOLD;}
    if    (stringCaseCmpSeg(name,"PINK",4) == 0){lastLen = 4;    return PINK;}

    if (stringCaseCmpSeg(name,"DREXELBLUE",10) == 0){lastLen = 10; return DREXELBLUE;}
    if (stringCaseCmpSeg(name,"DREXELGOLD",10) == 0){lastLen = 10; return DREXELGOLD;}
    
    lastLen = -1;
    return BADCOLOR;
}

int lastColorNameLen(){
    return lastLen;
}