#ifndef FONT_H
#define FONT_H

#define FONT_SINGLE_LINE_HEIGHT 16
#define FONT_CHAR_MAX_WIDTH     10

typedef struct {
    unsigned char Width;
    unsigned char Distance;
    unsigned char BytesPerLine;
    const unsigned char *paData;
} FONT_CHARINFO;

typedef struct FONT_PROP FONT_PROP;

struct FONT_PROP {
    unsigned char First;
    unsigned char Last;
    const FONT_CHARINFO *pCharInfo;
    const FONT_PROP *pNext;
};

typedef struct {
    unsigned char Height;
    unsigned char Space;
    const FONT_PROP *pProperties;
} FONT;

extern const FONT FontTrebuchetMS16;

void * FontGetCharInfo(const FONT *pFont, unsigned char asciip);

unsigned char FontGetCharWidth(const FONT *pFont, unsigned char asciip);

unsigned char FontGetNextWordWidth(const FONT *pFont, const unsigned char *pWord);

#endif
