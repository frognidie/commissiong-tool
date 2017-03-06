
#include "bitmap.h"

#define	________	0x0
#define	_______X	0x1
#define	______X_	0x2
#define	______XX	0x3
#define	_____X__	0x4
#define	_____X_X	0x5
#define	_____XX_	0x6
#define	_____XXX	0x7
#define	____X___	0x8
#define	____X__X	0x9
#define	____X_X_	0xa
#define	____X_XX	0xb
#define	____XX__	0xc
#define	____XX_X	0xd
#define	____XXX_	0xe
#define	____XXXX	0xf
#define	___X____	0x10
#define	___X___X	0x11
#define	___X__X_	0x12
#define	___X__XX	0x13
#define	___X_X__	0x14
#define	___X_X_X	0x15
#define	___X_XX_	0x16
#define	___X_XXX	0x17
#define	___XX___	0x18
#define	___XX__X	0x19
#define	___XX_X_	0x1a
#define	___XX_XX	0x1b
#define	___XXX__	0x1c
#define	___XXX_X	0x1d
#define	___XXXX_	0x1e
#define	___XXXXX	0x1f
#define	__X_____	0x20
#define	__X____X	0x21
#define	__X___X_	0x22
#define	__X___XX	0x23
#define	__X__X__	0x24
#define	__X__X_X	0x25
#define	__X__XX_	0x26
#define	__X__XXX	0x27
#define	__X_X___	0x28
#define	__X_X__X	0x29
#define	__X_X_X_	0x2a
#define	__X_X_XX	0x2b
#define	__X_XX__	0x2c
#define	__X_XX_X	0x2d
#define	__X_XXX_	0x2e
#define	__X_XXXX	0x2f
#define	__XX____	0x30
#define	__XX___X	0x31
#define	__XX__X_	0x32
#define	__XX__XX	0x33
#define	__XX_X__	0x34
#define	__XX_X_X	0x35
#define	__XX_XX_	0x36
#define	__XX_XXX	0x37
#define	__XXX___	0x38
#define	__XXX__X	0x39
#define	__XXX_X_	0x3a
#define	__XXX_XX	0x3b
#define	__XXXX__	0x3c
#define	__XXXX_X	0x3d
#define	__XXXXX_	0x3e
#define	__XXXXXX	0x3f
#define	_X______	0x40
#define	_X_____X	0x41
#define	_X____X_	0x42
#define	_X____XX	0x43
#define	_X___X__	0x44
#define	_X___X_X	0x45
#define	_X___XX_	0x46
#define	_X___XXX	0x47
#define	_X__X___	0x48
#define	_X__X__X	0x49
#define	_X__X_X_	0x4a
#define	_X__X_XX	0x4b
#define	_X__XX__	0x4c
#define	_X__XX_X	0x4d
#define	_X__XXX_	0x4e
#define	_X__XXXX	0x4f
#define	_X_X____	0x50
#define	_X_X___X	0x51
#define	_X_X__X_	0x52
#define	_X_X__XX	0x53
#define	_X_X_X__	0x54
#define	_X_X_X_X	0x55
#define	_X_X_XX_	0x56
#define	_X_X_XXX	0x57
#define	_X_XX___	0x58
#define	_X_XX__X	0x59
#define	_X_XX_X_	0x5a
#define	_X_XX_XX	0x5b
#define	_X_XXX__	0x5c
#define	_X_XXX_X	0x5d
#define	_X_XXXX_	0x5e
#define	_X_XXXXX	0x5f
#define	_XX_____	0x60
#define	_XX____X	0x61
#define	_XX___X_	0x62
#define	_XX___XX	0x63
#define	_XX__X__	0x64
#define	_XX__X_X	0x65
#define	_XX__XX_	0x66
#define	_XX__XXX	0x67
#define	_XX_X___	0x68
#define	_XX_X__X	0x69
#define	_XX_X_X_	0x6a
#define	_XX_X_XX	0x6b
#define	_XX_XX__	0x6c
#define	_XX_XX_X	0x6d
#define	_XX_XXX_	0x6e
#define	_XX_XXXX	0x6f
#define	_XXX____	0x70
#define	_XXX___X	0x71
#define	_XXX__X_	0x72
#define	_XXX__XX	0x73
#define	_XXX_X__	0x74
#define	_XXX_X_X	0x75
#define	_XXX_XX_	0x76
#define	_XXX_XXX	0x77
#define	_XXXX___	0x78
#define	_XXXX__X	0x79
#define	_XXXX_X_	0x7a
#define	_XXXX_XX	0x7b
#define	_XXXXX__	0x7c
#define	_XXXXX_X	0x7d
#define	_XXXXXX_	0x7e
#define	_XXXXXXX	0x7f
#define	X_______	0x80
#define	X______X	0x81
#define	X_____X_	0x82
#define	X_____XX	0x83
#define	X____X__	0x84
#define	X____X_X	0x85
#define	X____XX_	0x86
#define	X____XXX	0x87
#define	X___X___	0x88
#define	X___X__X	0x89
#define	X___X_X_	0x8a
#define	X___X_XX	0x8b
#define	X___XX__	0x8c
#define	X___XX_X	0x8d
#define	X___XXX_	0x8e
#define	X___XXXX	0x8f
#define	X__X____	0x90
#define	X__X___X	0x91
#define	X__X__X_	0x92
#define	X__X__XX	0x93
#define	X__X_X__	0x94
#define	X__X_X_X	0x95
#define	X__X_XX_	0x96
#define	X__X_XXX	0x97
#define	X__XX___	0x98
#define	X__XX__X	0x99
#define	X__XX_X_	0x9a
#define X__XX_XX	0x9b
#define X__XXX__	0x9c
#define X__XXX_X	0x9d
#define	X__XXXX_	0x9e
#define	X__XXXXX	0x9f
#define	X_X_____	0xa0
#define	X_X____X	0xa1
#define	X_X___X_	0xa2
#define	X_X___XX	0xa3
#define	X_X__X__	0xa4
#define	X_X__X_X	0xa5
#define	X_X__XX_	0xa6
#define	X_X__XXX	0xa7
#define	X_X_X___	0xa8
#define	X_X_X__X	0xa9
#define	X_X_X_X_	0xaa
#define	X_X_X_XX	0xab
#define	X_X_XX__	0xac
#define	X_X_XX_X	0xad
#define	X_X_XXX_	0xae
#define	X_X_XXXX	0xaf
#define	X_XX____	0xb0
#define X_XX___X	0xb1
#define	X_XX__X_	0xb2
#define	X_XX__XX	0xb3
#define	X_XX_X__	0xb4
#define	X_XX_X_X	0xb5
#define	X_XX_XX_	0xb6
#define	X_XX_XXX	0xb7
#define	X_XXX___	0xb8
#define	X_XXX__X	0xb9
#define	X_XXX_X_	0xba
#define	X_XXX_XX	0xbb
#define	X_XXXX__	0xbc
#define	X_XXXX_X	0xbd
#define	X_XXXXX_	0xbe
#define	X_XXXXXX	0xbf
#define	XX______	0xc0
#define	XX_____X	0xc1
#define	XX____X_	0xc2
#define	XX____XX	0xc3
#define	XX___X__	0xc4
#define	XX___X_X	0xc5
#define	XX___XX_	0xc6
#define	XX___XXX	0xc7
#define	XX__X___	0xc8
#define	XX__X__X	0xc9
#define	XX__X_X_	0xca
#define	XX__X_XX	0xcb
#define	XX__XX__	0xcc
#define	XX__XX_X	0xcd
#define	XX__XXX_	0xce
#define XX__XXXX	0xcf
#define	XX_X____	0xd0
#define	XX_X___X	0xd1
#define	XX_X__X_	0xd2
#define	XX_X__XX	0xd3
#define	XX_X_X__	0xd4
#define	XX_X_X_X	0xd5
#define	XX_X_XX_	0xd6
#define	XX_X_XXX	0xd7
#define	XX_XX___	0xd8
#define	XX_XX__X	0xd9
#define	XX_XX_X_	0xda
#define	XX_XX_XX	0xdb
#define	XX_XXX__	0xdc
#define	XX_XXX_X	0xdd
#define	XX_XXXX_	0xde
#define	XX_XXXXX	0xdf
#define	XXX_____	0xe0
#define	XXX____X	0xe1
#define	XXX___X_	0xe2
#define	XXX___XX	0xe3
#define	XXX__X__	0xe4
#define	XXX__X_X	0xe5
#define	XXX__XX_	0xe6
#define	XXX__XXX	0xe7
#define	XXX_X___	0xe8
#define	XXX_X__X	0xe9
#define	XXX_X_X_	0xea
#define	XXX_X_XX	0xeb
#define	XXX_XX__	0xec
#define	XXX_XX_X	0xed
#define	XXX_XXX_	0xee
#define	XXX_XXXX	0xef
#define	XXXX____	0xf0
#define	XXXX___X	0xf1
#define	XXXX__X_	0xf2
#define	XXXX__XX	0xf3
#define	XXXX_X__	0xf4
#define	XXXX_X_X	0xf5
#define	XXXX_XX_	0xf6
#define	XXXX_XXX	0xf7
#define	XXXXX___	0xf8
#define	XXXXX__X	0xf9
#define	XXXXX_X_	0xfa
#define	XXXXX_XX	0xfb
#define	XXXXXX__	0xfc
#define	XXXXXX_X	0xfd
#define	XXXXXXX_	0xfe
#define	XXXXXXXX	0xff

const unsigned char bm_battery0[64] = {
    ________,________,________,________,
    ________,________,________,________,
    ________,XXXXXXXX,XXXXXXXX,XXX_____,
    ________,X_______,________,__X_____,
    ______XX,X_______,________,__X_____,
    ______XX,X_______,________,__X_____,
    ______XX,X_______,________,__X_____,
    ______XX,X_______,________,__X_____,
    ______XX,X_______,________,__X_____,
    ______XX,X_______,________,__X_____,
    ________,X_______,________,__X_____,
    ________,XXXXXXXX,XXXXXXXX,XXX_____,
    ________,________,________,________,
    ________,________,________,________
};

const unsigned char bm_battery1[64] = {
    ________,________,________,________,
    ________,________,________,________,
    ________,XXXXXXXX,XXXXXXXX,XXX_____,
    ________,X_______,________,__X_____,
    ______XX,X_______,____XXXX,X_X_____,
    ______XX,X_______,____XXXX,X_X_____,
    ______XX,X_______,____XXXX,X_X_____,
    ______XX,X_______,____XXXX,X_X_____,
    ______XX,X_______,____XXXX,X_X_____,
    ______XX,X_______,____XXXX,X_X_____,
    ________,X_______,________,__X_____,
    ________,XXXXXXXX,XXXXXXXX,XXX_____,
    ________,________,________,________,
    ________,________,________,________
};

const unsigned char bm_battery2[64] = {
    ________,________,________,________,
    ________,________,________,________,
    ________,XXXXXXXX,XXXXXXXX,XXX_____,
    ________,X_______,________,__X_____,
    ______XX,X______X,XXXXXXXX,X_X_____,
    ______XX,X______X,XXXXXXXX,X_X_____,
    ______XX,X______X,XXXXXXXX,X_X_____,
    ______XX,X______X,XXXXXXXX,X_X_____,
    ______XX,X______X,XXXXXXXX,X_X_____,
    ______XX,X______X,XXXXXXXX,X_X_____,
    ________,X_______,________,__X_____,
    ________,XXXXXXXX,XXXXXXXX,XXX_____,
    ________,________,________,________,
    ________,________,________,________
};

const unsigned char bm_battery3[64] = {
    ________,________,________,________,
    ________,________,________,________,
    ________,XXXXXXXX,XXXXXXXX,XXX_____,
    ________,X_______,________,__X_____,
    ______XX,X_XXXXXX,XXXXXXXX,X_X_____,
    ______XX,X_XXXXXX,XXXXXXXX,X_X_____,
    ______XX,X_XXXXXX,XXXXXXXX,X_X_____,
    ______XX,X_XXXXXX,XXXXXXXX,X_X_____,
    ______XX,X_XXXXXX,XXXXXXXX,X_X_____,
    ______XX,X_XXXXXX,XXXXXXXX,X_X_____,
    ________,X_______,________,__X_____,
    ________,XXXXXXXX,XXXXXXXX,XXX_____,
    ________,________,________,________,
    ________,________,________,________
};

const BitmapInfo BitmapInfo_Battery[4] = {
    {32, 16, 4, bm_battery0},
    {32, 16, 4, bm_battery1},
    {32, 16, 4, bm_battery2},
    {32, 16, 4, bm_battery3}
};

const unsigned char bm_arrow_nav[32] = {
    ________,________,
    ________,________,
    ________,________,
    ________,________,
    ________,________,
    ____XX__,________,
    _____XX_,________,
    ______XX,________,
    ______XX,________,
    _____XX_,________,
    ____XX__,________,
    ________,________,
    ________,________,
    ________,________,
    ________,________,
    ________,________
};

const unsigned char bm_arrow_nav_active[32] = {
    XXXXXXXX,XXXXXXXX,
    XXXXXXXX,XXXXXXXX,
    XXXX____,XXXXXXXX,
    XX______,__XXXXXX,
    X_______,___XXXXX,
    X___XX__,___XXXXX,
    _____XX_,____XXXX,
    ______XX,____XXXX,
    ______XX,____XXXX,
    _____XX_,____XXXX,
    X___XX__,___XXXXX,
    X_______,___XXXXX,
    XX______,__XXXXXX,
    XXXX____,XXXXXXXX,
    XXXXXXXX,XXXXXXXX,
    XXXXXXXX,XXXXXXXX
};

const BitmapInfo BitmapInfo_ArrowNav[2] = {
    {13, 16, 2, bm_arrow_nav},
    {13, 16, 2, bm_arrow_nav_active}
};

const unsigned char bm_arrow_return[] = {
    ___XXX__,
    __XXXX__,
    _XXXX___,
    XXXX____,
    XXXX____,
    XXXX____,
    _XXXX___,
    __XXXX__,
    ___XXX__
};

const BitmapInfo BitmapInfo_ArrowReturn = {
    6, 9, 1, bm_arrow_return
};

const unsigned char Bitmap_Exclamation[440] = {
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X80,0XF0,0XF0,0XE0,0X40,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XD0,0XF9,
0XFF,0XFF,0XFF,0XFF,0XFE,0XF4,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X40,0XE0,0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XF8,0X90,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X80,0XF4,0XFE,0XFF,0XFF,0XFF,0XFF,0X2F,0X2F,0X2F,0X2F,0XBF,0XFF,0XFF,0XFF,
0XFF,0XF9,0XD0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X90,0XF8,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X00,0X00,0XAA,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,
0XE0,0X40,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0XE0,0XFD,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X00,0X00,0X00,0X00,0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XE4,0X40,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X40,0XE4,
0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XA5,0X00,0X00,0X94,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0X80,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X80,0XF8,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBF,0X0B,0X07,0X07,0X0B,0XAF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF9,0XD0,0X00,0X00,0X00,0X00,
0X00,0X00,0XD0,0XF9,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFE,0XF8,0XF0,0XF0,0XF4,0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFD,0XE0,0X00,0X00,0X00,0X00,0X1F,0X2F,
0X3F,0X3F,0X3F,0X3F,0X3F,0X3F,0X3F,0X3F,0X3F,0X3F,0X3F,0X3F,0X3F,0X3F,0X3F,0X3F,
0X3F,0X3F,0X3F,0X3F,0X3F,0X3F,0X3F,0X3F,0X3F,0X3F,0X3F,0X3F,0X3F,0X3F,0X3F,0X3F,
0X3F,0X3F,0X3F,0X3F,0X2F,0X1F,0X01,0X00,};

#ifndef APP_BAES

const unsigned char bm_validation[32] = {
    ________,________,
    ________,________,
    _____XXX,X_______,
    ___XXX__,XXX_____,
    __XX____,__XX____,
    __X_____,___X____,
    _XX_____,___XX___,
    _X______,____X___,
    _X______,____X___,
    _XX_____,___XX___,
    __X_____,___X____,
    __XX____,__XX____,
    ___XXX__,XXX_____,
    _____XXX,X_______,
    ________,________,
    ________,________
};

const unsigned char bm_validation_active[32] = {
    ________,________,
    ________,________,
    _____XXX,X_______,
    ___XXXXX,XXX_____,
    __XXXXXX,XXXX____,
    __XXXXXX,XX_X____,
    _XXXXXXX,X__XX___,
    _XXXXXXX,__XXX___,
    _XXX__X_,_XXXX___,
    _XXXX___,XXXXX___,
    __XXXX_X,XXXX____,
    __XXXXXX,XXXX____,
    ___XXXXX,XXX_____,
    _____XXX,X_______,
    ________,________,
    ________,________
};

const BitmapInfo BitmapInfo_Validation[2] = {
    {14, 16, 2, bm_validation},
    {14, 16, 2, bm_validation_active}
};

const unsigned char bm_read_sensor[] = {
    ___X____,_XXXXXXX,X_______,
    __XX____,_XXXXXXX,X_______,
    _XXX____,_XXXXXXX,X_______,
    XXXXXX__,_XXXXXXX,X_______,
    XXXXXX__,_XXXXXXX,X_______,
    _XXX____,_XXXXXXX,X_______,
    __XX_XXX,XXXXXXXX,XXXXX___,
    ___X_XX_,________,___XX___,
    ______XX,X_______,_XXX____,
    _______X,XXX____X,XXX_____,
    ________,_XXXXXXX,X_______
};

const BitmapInfo BitmapInfo_ReadSensor = {
    21, 11, 3, bm_read_sensor
};

const unsigned char bm_write_sensor[] = {
    __X_____,_XXXXXXX,X_______,
    __XX____,_XXXXXXX,X_______,
    __XXX___,_XXXXXXX,X_______,
    XXXXXX__,_XXXXXXX,X_______,
    XXXXXX__,_XXXXXXX,X_______,
    __XXX___,_XXXXXXX,X_______,
    __XX_XXX,XXXXXXXX,XXXXX___,
    __X__XX_,________,___XX___,
    ______XX,X_______,_XXX____,
    _______X,XXX____X,XXX_____,
    ________,_XXXXXXX,X_______
};

const BitmapInfo BitmapInfo_WriteSensor = {
    21, 11, 3, bm_write_sensor
};

const unsigned char bm_read_file[] = {
    ______XX,XXX_____,________,
    ______XX,XXXXXXXX,XXX_____,
    ___X__X_,________,__X_____,
    __XX__X_,________,__X_____,
    _XXX__X_,________,__X_____,
    XXXXX_X_,________,__X_____,
    XXXXX_X_,________,__X_____,
    _XXX__X_,________,__X_____,
    __XX__X_,________,__X_____,
    ___X__X_,________,__X_____,
    ______XX,XXXXXXXX,XXX_____
};

const BitmapInfo BitmapInfo_ReadFile = {
    19, 11, 3, bm_read_file
};

const unsigned char bm_save_file[] = {
    ______XX,XXX_____,________,
    ______XX,XXXXXXXX,XXX_____,
    _X____X_,________,__X_____,
    _XX___X_,________,__X_____,
    _XXX__X_,________,__X_____,
    XXXXX_X_,________,__X_____,
    XXXXX_X_,________,__X_____,
    _XXX__X_,________,__X_____,
    _XX___X_,________,__X_____,
    _X____X_,________,__X_____,
    ______XX,XXXXXXXX,XXX_____
};

const BitmapInfo BitmapInfo_SaveFile = {
    19, 11, 3, bm_save_file
};

const unsigned char bm_delete_file[] = {
    XXXXX___,________,
    XXXXXXXX,XXXXX___,
    X_______,____X___,
    X___XX_X,X___X___,
    X___XXXX,X___X___,
    X____XXX,____X___,
    X____XXX,____X___,
    X___XXXX,X___X___,
    X___XX_X,X___X___,
    X_______,____X___,
    XXXXXXXX,XXXXX___
};

const BitmapInfo BitmapInfo_DeleteFile = {
    13, 11, 2, bm_delete_file
};

const unsigned char bm_question_mark[] = {
    ________,
    __XXXX__,
    _XXXXXX_,
    _XX__XX_,
    ____XX__,
    ___XX___,
    ___XX___,
    ___XX___,
    ________,
    ___XX___,
    ___XX___,
};

const BitmapInfo BitmapInfo_QuestionMark = {
    8, 11, 1, bm_question_mark
};

const unsigned char Bitmap_Rectangular[540] = {
0X40,0XF0,0XF4,0XFC,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,
0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,
0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,
0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,
0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,
0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,
0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFC,0XF4,0XF0,0X40,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X01,0X0F,0X1F,0X3F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,
0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,
0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,
0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,
0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,
0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,
0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X3F,0X1F,0X07,0X01,};

const unsigned char bm_pulse[] = {
    ________,______XX,XXXXXXXX,XXXXXXXX,XX______,________,
    ________,______XX,XXXXXXXX,XXXXXXXX,XX______,________,
    ________,______XX,________,________,XX______,________,
    ________,______XX,________,________,XX______,________,
    ________,______XX,________,________,XX______,________,
    ________,______XX,________,________,XX______,________,
    ________,______XX,________,________,XX______,________,
    ________,______XX,________,________,XX______,________,
    ________,______XX,________,________,XX______,________,
    ________,______XX,________,________,XX______,________,
    ________,______XX,________,________,XX______,________,
    ________,______XX,________,________,XX______,________,
    ________,______XX,________,________,XX______,________,
    ________,______XX,________,___XXXXX,XXXXXXX_,________,
    ________,______XX,________,____XXXX,XXXXXX__,________,
    ________,______XX,________,_____XXX,XXXXX___,________,
    ________,______XX,________,______XX,XXXX____,________,
    ________,______XX,________,_______X,XXX_____,________,
    ________,______XX,________,________,XX______,________,
    ________,______XX,________,________,XX______,________,
    ________,______XX,________,________,XX______,________,
    ________,______XX,________,________,XX______,________,
    ________,______XX,________,________,XX______,________,
    ________,______XX,________,________,XX______,________,
    ________,______XX,________,________,XX______,________,
    ________,______XX,________,________,XX______,________,
    ________,______XX,________,________,XX______,________,
    ________,______XX,________,________,XX______,________,
    ________,______XX,________,________,XX______,________,
    ________,______XX,________,________,XX______,________,
    XXXXXXXX,XXXXXXXX,________,________,XXXXXXXX,XXXXXXXX,
    XXXXXXXX,XXXXXXXX,________,________,XXXXXXXX,XXXXXXXX
};

const BitmapInfo BitmapInfo_Pulse = {
    48, 32, 6, bm_pulse
};

const unsigned char bm_pulse_s[] = {
    ________,________,________,________,
    ________,___XXXXX,XXXXX___,________,
    ________,___X____,____X___,________,
    ________,___X____,____X___,________,
    ________,___X____,____X___,________,
    ________,___X____,____X___,________,
    ________,___X____,_XXXXXXX,________,
    ________,___X____,__XXXXX_,________,
    ________,___X____,___XXX__,________,
    ________,___X____,____X___,________,
    ________,___X____,____X___,________,
    ________,___X____,____X___,________,
    ________,___X____,____X___,________,
    ________,___X____,____X___,________,
    ____XXXX,XXXX____,____XXXX,XXXX____,
    ________,________,________,________,
};

const BitmapInfo BitmapInfo_Pulse_S = {
    32, 16, 4, bm_pulse_s
};

const unsigned char Bitmap_Eye[384] = {
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X40,0XC0,
0XC0,0XD0,0XE0,0XF0,0XF0,0XF0,0XF0,0XF4,0XF4,0XF0,0XF0,0XF0,0XF0,0XE0,0XD0,0XC0,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XC0,0XD0,0XF0,0XF8,0XFC,0XBF,0X3F,0X3F,0X1F,
0X0F,0X0F,0X07,0X47,0XC3,0XC3,0XC3,0XC3,0XC3,0XC3,0XC3,0X43,0X07,0X0B,0X0F,0X0F,
0X2F,0X3F,0X7F,0XFE,0XFC,0XF4,0XF0,0XC0,0X40,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0XD0,0XF0,0XFC,0XFF,0X7F,0X1F,0X0B,0X03,0X01,0X00,0X00,0X00,0XD0,
0XF4,0XFC,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0XF0,0XC0,
0X00,0X00,0X00,0X00,0X02,0X07,0X0F,0X2F,0XBF,0XFE,0XF8,0XE0,0X80,0X00,0X00,0X00,
0X80,0XF8,0XFE,0XFF,0X2F,0X07,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFD,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X83,0X42,0X83,0XFF,0XFF,0XFF,0XFF,
0XFD,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X02,0X0B,0X7F,0XFF,0XFD,0XE0,0X00,
0X02,0X1F,0XBF,0XFF,0XF8,0XD0,0X40,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XBF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XBF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X80,0XF0,0XFD,0XFF,0X3F,0X0B,0X00,
0X00,0X00,0X00,0X03,0X0F,0X3F,0XFF,0XFD,0XF4,0XD0,0X80,0X00,0X00,0X00,0X01,0X0F,
0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X2F,0X0B,
0X00,0X00,0X00,0X00,0X40,0XC0,0XF0,0XF8,0XFE,0XBF,0X2F,0X0B,0X01,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0X0B,0X1F,0X3F,0X7F,0XFD,0XFC,0XF4,0XF0,
0XE0,0XC0,0XC2,0X83,0X47,0X4B,0X0F,0X0F,0X0F,0X4B,0X47,0X83,0X82,0XC0,0XD0,0XE0,
0XF0,0XF8,0XFC,0XFE,0X7F,0X2F,0X0F,0X07,0X02,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X02,0X03,0X07,
0X0B,0X0F,0X0F,0X1F,0X2F,0X3F,0X3F,0X3F,0X3F,0X3F,0X3F,0X2F,0X1F,0X0F,0X0F,0X0B,
0X07,0X03,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};


const unsigned char bm_backhome[] = {
  ________,________,
  ______XX,________,
  _____XXX,X_______,
  ____XXXX,XX______,
  ___XXXXX,XXX_____,
  __XXXXXX,XXXX____,
  _XXXXXXX,XXXXX___,
  XXXXXXXX,XXXXXX__,
  _X__XXXX,X___X___,
  _X__X___,X___X___,
  _X__X___,X___X___,
  _X__X___,X___X___,
  _XXXXXXX,XXXXX___,
  ________,________,
  ________,________,
  ________,________
};
const BitmapInfo BitmapInfo_BackHome = {
    14, 16, 2, bm_backhome
};

const unsigned char bm_readtype[] = {
    ________,________,
    ________,________,
    _____XXX,X_______,
    ___XXX__,XXX_____,
    __XX____,__XX____,
    __X___XX,___X____,
    _XX_____,___XX___,
    _X____XX,____X___,
    _X____XX,____X___,
    _XX___XX,___XX___,
    __X___XX,___X____,
    __XX____,__XX____,
    ___XXX__,XXX_____,
    _____XXX,X_______,
    ________,________,
    ________,________
};

const BitmapInfo BitmapInfo_ReadType = {
    14, 16, 2, bm_readtype
};

#else

const unsigned char bm_delete_data[] = {

    ________,________,________,
    ______XX,X_____XX,X_______,
    _______X,XX___XXX,________,
    ________,XXX_XXX_,________,
    ________,_XXXXX__,________,
    ________,__XXX___,________,
    ________,__XXX___,________,
    ________,_XXXXX__,________,
    ________,XXX_XXX_,________,
    _______X,XX___XXX,________,
    ______XX,X_____XX,X_______
};

const BitmapInfo BitmapInfo_DeleteData = {
    19, 11, 3, bm_delete_data
};

const unsigned char bm_reset_data[] = {

    ________,________,________,
    ________,________,________,
    ________,________,________,
    _XXX___X,XX___XXX,___XXX__,
    X___X_X_,__X_X___,X_X___X_,
    X___X_X_,__X_X___,X_X___X_,
    X___X_X_,__X_X___,X_X___X_,
    _XXX___X,XX___XXX,___XXX__,
    ________,________,________,
    ________,________,________,
    ________,________,________
};

const BitmapInfo BitmapInfo_ResetData = {
    23, 11, 3, bm_reset_data
};

const unsigned char bm_send_data[] = {

    _____XXX,XXXXXX__,________,
    ___XXXXX,XXXXXXXX,________,
    __XXX___,______XX,X_______,
    _XXX____,_______X,XX______,
    _XX____X,XXXX____,XX______,
    _____XXX,XXXXXX__,________,
    ____XXX_,____XXX_,________,
    ___XXX__,_____XXX,________,
    ________,XXX_____,________,
    _______X,XXXX____,________,
    ______XX,___XX___,________
};

const BitmapInfo BitmapInfo_SendData = {
    19, 11, 3, bm_send_data
};

const unsigned char bm_tool[] = {

    ________,________,
    _XXXXXXX,XXXXXX__,
    _X_XXXXX,XXXX_X__,
    _X_X____,___X_X__,
    _X_X____,___X_X__,
    _X_X____,___X_X__,
    _X_XXXXX,XXXX_X__,
    _X______,_____X__,
    _X__X_X_,X_X__X__,
    _X______,_____X__,
    _X____XX,X____X__,
    _X__XXXX,XXX__X__,
    _X____XX,X____X__,
    _X______,_____X__,
    _X______,_____X__,
    __XXXXXX,XXXXX___,
};

const BitmapInfo BitmapInfo_tool = {
      14, 16, 2, bm_tool
};

/***********send******************/
const unsigned char bm_send_data_big_1[] = {
    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,______XX,XXXXXX__,________,________,________,
    ________,________,_____XXX,XXXXXXX_,________,________,________,
    ________,________,____XXXX,XXXXXXXX,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________

};

const unsigned char bm_send_data_big_2[] = {

    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,____XXXX,XXXXXXXX,________,________,________,
    ________,________,__XXXXXX,XXXXXXXX,XX______,________,________,
    ________,________,XXXXXXXX,XXXXXXXX,XXXX____,________,________,
    ________,______XX,XXXXX___,_______X,XXXXXX__,________,________,
    ________,_____XXX,XXX_____,________,_XXXXXX_,________,________,
    ________,_____XXX,X_______,________,___XXXX_,________,________,
    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,______XX,XXXXXX__,________,________,________,
    ________,________,_____XXX,XXXXXXX_,________,________,________,
    ________,________,____XXXX,XXXXXXXX,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________
};

const unsigned char bm_send_data_big_3[] = {

    ________,________,__XXXXXX,XXXXXXXX,________,________,________,
    ________,_______X,XXXXXXXX,XXXXXXXX,XXXX____,________,________,
    ________,_____XXX,XXXXXXXX,XXXXXXXX,XXXXXXX_,________,________,
    ________,__XXXXXX,XXX_____,________,_XXXXXXX,________,________,
    ________,_XXXXXXX,________,________,___XXXXX,XX______,________,
    ________,XXXXXX__,________,________,_____XXX,XXX_____,________,
    _______X,XXX_____,________,________,_______X,XXXX____,________,
    ______XX,XX______,________,________,________,_XXXX___,________,
    _____XXX,X_______,________,________,________,__XXXX__,________,
    ________,________,____XXXX,XXXXXXXX,________,________,________,
    ________,________,__XXXXXX,XXXXXXXX,XX______,________,________,
    ________,________,XXXXXXXX,XXXXXXXX,XXXX____,________,________,
    ________,______XX,XXXXX___,_______X,XXXXXX__,________,________,
    ________,_____XXX,XXX_____,________,_XXXXXX_,________,________,
    ________,_____XXX,X_______,________,___XXXX_,________,________,
    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,______XX,XXXXXX__,________,________,________,
    ________,________,_____XXX,XXXXXXX_,________,________,________,
    ________,________,____XXXX,XXXXXXXX,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________

};

const BitmapInfo BitmapInfo_SendData_big[3] = {
    {50, 23, 7, bm_send_data_big_1},
    {50, 23, 7, bm_send_data_big_2},
    {50, 23, 7, bm_send_data_big_3}
};

/***********delete******************/
const unsigned char bm_delete_data_big_1[] = {

    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,________,________,XXX_____,________,________,
    ________,________,________,_______X,XXX_____,________,________,
    ________,________,________,______XX,XXX_____,________,________,
    ________,________,________,_____XXX,XX______,________,________,
    ________,________,________,____XXXX,X_______,________,________,
    ________,________,________,___XXXXX,________,________,________,
    ________,________,________,__XXXXX_,________,________,________,
    ________,________,________,XXXXXX__,________,________,________,
    ________,________,________,XXXXX___,________,________,________,
    ________,________,_______X,XXXX____,________,________,________,
    ________,________,______XX,XXX_____,________,________,________,
    ________,________,_____XXX,XX______,________,________,________,
    ________,________,____XXXX,X_______,________,________,________,
    ________,________,___XXXXX,________,________,________,________,
    ________,________,__XXXXX_,________,________,________,________,
    ________,________,_XXXXX__,________,________,________,________,
    ________,________,XXXXX___,________,________,________,________,
    ________,_______X,XXXX____,________,________,________,________,
    ________,_______X,XXX_____,________,________,________,________,
    ________,_______X,XX______,________,________,________,________,
    ________,________,________,________,________,________,________


};

const unsigned char bm_delete_data_big_2[] = {

    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,XXX_____,________,________,________,________,
    ________,________,XXXX____,________,________,________,________,
    ________,________,XXXXX___,________,________,________,________,
    ________,________,_XXXXX__,________,________,________,________,
    ________,________,__XXXXX_,________,________,________,________,
    ________,________,___XXXXX,________,________,________,________,
    ________,________,____XXXX,X_______,________,________,________,
    ________,________,_____XXX,XX______,________,________,________,
    ________,________,______XX,XXX_____,________,________,________,
    ________,________,_______X,XXXX____,________,________,________,
    ________,________,_______X,XXXXX___,________,________,________,
    ________,________,________,XXXXXX__,________,________,________,
    ________,________,________,__XXXXX_,________,________,________,
    ________,________,________,___XXXXX,________,________,________,
    ________,________,________,____XXXX,X_______,________,________,
    ________,________,________,_____XXX,XX______,________,________,
    ________,________,________,______XX,XXX_____,________,________,
    ________,________,________,_______X,XXXX____,________,________,
    ________,________,________,________,XXXX____,________,________,
    ________,________,________,________,_XXX____,________,________,
    ________,________,________,________,________,________,________
};

const unsigned char bm_delete_data_big_3[] = {

    ________,________,________,________,________,________,________,
    ________,________,________,________,________,________,________,
    ________,________,XXX_____,________,XXX_____,________,________,
    ________,________,XXXX____,_______X,XXX_____,________,________,
    ________,________,XXXXX___,______XX,XXX_____,________,________,
    ________,________,_XXXXX__,_____XXX,XX______,________,________,
    ________,________,__XXXXX_,____XXXX,X_______,________,________,
    ________,________,___XXXXX,___XXXXX,________,________,________,
    ________,________,____XXXX,X_XXXXX_,________,________,________,
    ________,________,_____XXX,XXXXXX__,________,________,________,
    ________,________,______XX,XXXXX___,________,________,________,
    ________,________,_______X,XXXX____,________,________,________,
    ________,________,______XX,XXXXX___,________,________,________,
    ________,________,_____XXX,XXXXXX__,________,________,________,
    ________,________,____XXXX,X_XXXXX_,________,________,________,
    ________,________,___XXXXX,___XXXXX,________,________,________,
    ________,________,__XXXXX_,____XXXX,X_______,________,________,
    ________,________,_XXXXX__,_____XXX,XX______,________,________,
    ________,________,XXXXX___,______XX,XXX_____,________,________,
    ________,_______X,XXXX____,_______X,XXXX____,________,________,
    ________,_______X,XXX_____,________,XXXX____,________,________,
    ________,_______X,XX______,________,_XXX____,________,________,
    ________,________,________,________,________,________,________
};

const BitmapInfo BitmapInfo_DeleteData_big[3] = {
      {50, 23, 7, bm_delete_data_big_1},
      {50, 23, 7, bm_delete_data_big_2},
      {50, 23, 7, bm_delete_data_big_3}

};

const unsigned char dig_0[] = {
    ________,
    ________,
    ________,
    ________,           //     v
    ________,           //     ^
    ________,
    ________,
    ________,
};

const unsigned char dig_1[] = {
    ________,
    XXXXXXXX,
    XXXXXXXX,
    XXXXXXXX,               //    <
    XXXXXXXX,               //   ^
    XXXXXXXX,
    XXXXXXXX,
    ________,
};
const unsigned char dig_2[] = {
    _XXXXXX_,
    _XXXXXX_,
    _XXXXXX_,
    _XXXXXX_,               //    <
    _XXXXXX_,               //   ^
    _XXXXXX_,
    _XXXXXX_,
    _XXXXXX_,
};
const unsigned char dig_3[] = {
    _XXXXXX_,
    __XXXX__,
    ___XX___,
    ________,           //     v
    ________,           //     ^
    ________,
    ________,
    ________,
};

const unsigned char dig_4[] = {
    ________,
    _______X,
    ______XX,
    _____XXX,               //    <
    _____XXX,               //   ^
    ______XX,
    _______X,
    ________,
};

const unsigned char dig_5[] = {
    ________,
    ________,
    ________,
    ________,               //    <
    ________,               //   ^
    ___XX___,
    __XXXX__,
    _XXXXXX_,
};

const unsigned char dig_6[] = {
    ________,
    X_______,
    XX______,
    XXX_____,             //         >
    XXX_____,             //          ^
    XX______,
    X_______,
    ________,
};

const unsigned char dig_7[] = {
    _XXXXXX_,
    __XXXX__,
    ___XX___,
    ________,           //     v
    ________,           //     ^
    ___XX___,
    __XXXX__,
    _XXXXXX_,
};

const unsigned char dig_8[] = {
    ________,
    X______X,
    XX____XX,
    XXX__XXX,
    XXX__XXX,
    XX____XX,
    X______X,
    ________,
};

const unsigned char dig_9[] = {
    _XXXXXX_,
    X_XXXX__,
    XX_XX___,
    XXX_____,
    XXX_____,
    XX______,
    X_______,
    ________,
};

const unsigned char dig_10[] = {
    _XXXXXX_,
    __XXXX_X,
    ___XX_XX,
    _____XXX,
    _____XXX,
    ______XX,
    _______X,
    ________,
};
const unsigned char dig_11[] = {
    ________,
    _______X,
    ______XX,
    _____XXX,
    _____XXX,
    ___XX_XX,
    __XXXX_X,
    _XXXXXX_,
};

const unsigned char dig_12[] = {
    ________,
    X_______,
    XX______,
    XXX_____,
    XXX_____,
    XX_XX___,
    X_XXXX__,
    _XXXXXX_,
};

const unsigned char dig_13[] = {
    _XXXXXX_,
    X_XXXX__,
    XX_XX___,
    XXX_____,
    XXX_____,
    XX_XX___,
    X_XXXX__,
    _XXXXXX_,
};

const unsigned char dig_14[] = {
    _XXXXXX_,
    __XXXX_X,
    ___XX_XX,
    _____XXX,
    _____XXX,
    ___XX_XX,
    __XXXX_X,
    _XXXXXX_,
};


const BitmapInfo BitmapInfo_Digit[15] = {
    {8, 8, 1, dig_0},
    {8, 8, 1, dig_1},
    {8, 8, 1, dig_2},
    {8, 8, 1, dig_3},
    {8, 8, 1, dig_4},
    {8, 8, 1, dig_5},
    {8, 8, 1, dig_6},
    {8, 8, 1, dig_7},
    {8, 8, 1, dig_8},
    {8, 8, 1, dig_9},
    {8, 8, 1, dig_10},
    {8, 8, 1, dig_11},
    {8, 8, 1, dig_12},
    {8, 8, 1, dig_13},
    {8, 8, 1, dig_14}
};

#endif  // #ifndef APP_BAES
