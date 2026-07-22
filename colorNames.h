typedef enum {
    //HTML standard
    WHITE   = 0xFFFFFF,
    SILVER  = 0xC0C0C0,
    GRAY    = 0x808080,
    BLACK   = 0x000000,
    RED     = 0xFF0000,
    MAROON  = 0x800000,
    YELLOW  = 0xFFFF00,
    OLIVE   = 0x808000,
    LIME    = 0x00FF00,
    GREEN   = 0x008000,
    AQUA    = 0x00FFFF,
    TEAL    = 0x008080,
    BLUE    = 0x0000FF,
    NAVY    = 0x000080,
    FUCHSIA = 0xFF00FF,
    PURPLE  = 0x800080,
    
    //Additional Names
    CYAN    = 0X00FFFF,
    MAGENTA = 0xFF00FF,

    //Logical Extras
    ORANGE    = 0xFFA500,
    INDIGO    = 0x4B0082,
    //LIGHTGRAY = 0xD3D3D3,
    SLATE     = 0x708090,
    GOLD      = 0xFFD700,
    PINK      = 0xFFC0CB,

    //Extended Colors
    REDVIOLET     = 0xC71585,
    DEEPPINK      = 0xFF1493,
    PALEREDVIOLET = 0xDB7093,
    HOTPINK       = 0xFF69B4,

    //Silly Fun Colors
    //UPENNRED     = 0x990000,
    //BADASSGREEN  = 0xBADA55,
    DREXELBLUE   = 0x07294D,
    DREXELGOLD   = 0xFFC600,
    //TEMPLECHERRY = 0xA32036,
    //UPENNBLUE    = 0x011F5B,

    //Sentinel for invalid
    BADCOLOR = -1
} ColorValue;

ColorValue mapName(char*);

int lastColorNameLen();