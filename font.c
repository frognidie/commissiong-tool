
#include "font.h"

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

const unsigned char acFontTrebuchetMS16_0x20[ 16] = { /* code 0020 */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x21[ 16] = { /* code 0021 */
  ________,
  ________,
  ________,
  ________,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  ________,
  _X______,
  _X______,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x22[ 16] = { /* code 0022 */
  ________,
  ________,
  ________,
  ________,
  X_X_____,
  X_X_____,
  X_X_____,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x23[ 16] = { /* code 0023 */
  ________,
  ________,
  ________,
  ________,
  __X_X___,
  __X_X___,
  XXXXXX__,
  _X_X____,
  _X_X____,
  XXXXX___,
  X_X_____,
  X_X_____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x24[ 16] = { /* code 0024 */
  ________,
  ________,
  ________,
  ___X____,
  __XXX___,
  _X______,
  _X______,
  __X_____,
  ___X____,
  ____X___,
  ____X___,
  _XXX____,
  ___X____,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x25[ 16] = { /* code 0025 */
  ________,
  ________,
  ________,
  ________,
  _X____X_,
  X_X__X__,
  X_X_X___,
  _X_X____,
  ___X_X__,
  __X_X_X_,
  _X__X_X_,
  X____X__,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x26[ 16] = { /* code 0026 */
  ________,
  ________,
  ________,
  ________,
  __XXX___,
  _X__X___,
  _X___X__,
  __XXXXX_,
  _X___X__,
  _X___X__,
  _X___X__,
  __XXX_XX,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x27[ 16] = { /* code 0027 */
  ________,
  ________,
  ________,
  ________,
  X_______,
  X_______,
  X_______,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x28[ 16] = { /* code 0028 */
  ________,
  ________,
  ________,
  ________,
  __X_____,
  __X_____,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  __X_____,
  __X_____,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x29[ 16] = { /* code 0029 */
  ________,
  ________,
  ________,
  ________,
  _X______,
  _X______,
  __X_____,
  __X_____,
  __X_____,
  __X_____,
  __X_____,
  __X_____,
  _X______,
  _X______,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x2A[ 16] = { /* code 002A */
  ________,
  ________,
  ________,
  ________,
  _X______,
  XXXX____,
  _X______,
  X_X_____,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x2B[ 16] = { /* code 002B */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  __X_____,
  __X_____,
  XXXXX___,
  __X_____,
  __X_____,
  ________,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x2C[ 16] = { /* code 002C */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  _XX_____,
  __X_____,
  __X_____,
  _X______,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x2D[ 16] = { /* code 002D */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  XXXXX___,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x2E[ 16] = { /* code 002E */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  _XX_____,
  _XX_____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x2F[ 16] = { /* code 002F */
  ________,
  ________,
  ________,
  ________,
  ____X___,
  ___X____,
  ___X____,
  ___X____,
  __X_____,
  __X_____,
  __X_____,
  _X______,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x30[ 16] = { /* code 0030 */
  ________,
  ________,
  ________,
  ________,
  _XXX____,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  _XXX____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x31[ 16] = { /* code 0031 */
  ________,
  ________,
  ________,
  ________,
  ___X____,
  __XX____,
  _X_X____,
  ___X____,
  ___X____,
  ___X____,
  ___X____,
  ___X____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x32[ 16] = { /* code 0032 */
  ________,
  ________,
  ________,
  ________,
  _XXX____,
  X___X___,
  ____X___,
  ___X____,
  ___X____,
  __X_____,
  _X______,
  XXXXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x33[ 16] = { /* code 0033 */
  ________,
  ________,
  ________,
  ________,
  _XXX____,
  X___X___,
  ____X___,
  ___X____,
  ____X___,
  ____X___,
  X___X___,
  _XXX____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x34[ 16] = { /* code 0034 */
  ________,
  ________,
  ________,
  ________,
  ____X___,
  ___XX___,
  __X_X___,
  _X__X___,
  X___X___,
  XXXXXX__,
  ____X___,
  ____X___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x35[ 16] = { /* code 0035 */
  ________,
  ________,
  ________,
  ________,
  _XXXX___,
  _X______,
  _X______,
  _XXX____,
  ____X___,
  ____X___,
  ____X___,
  _XXX____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x36[ 16] = { /* code 0036 */
  ________,
  ________,
  ________,
  ________,
  ___X____,
  __X_____,
  _X______,
  XXXX____,
  X___X___,
  X___X___,
  X___X___,
  _XXX____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x37[ 16] = { /* code 0037 */
  ________,
  ________,
  ________,
  ________,
  XXXXX___,
  ____X___,
  ___X____,
  ___X____,
  __X_____,
  __X_____,
  _X______,
  _X______,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x38[ 16] = { /* code 0038 */
  ________,
  ________,
  ________,
  ________,
  _XXX____,
  X___X___,
  X___X___,
  _XXX____,
  X___X___,
  X___X___,
  X___X___,
  _XXX____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x39[ 16] = { /* code 0039 */
  ________,
  ________,
  ________,
  ________,
  _XXX____,
  X___X___,
  X___X___,
  X___X___,
  _XXXX___,
  ___X____,
  ___X____,
  __X_____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x3A[ 16] = { /* code 003A */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  _XX_____,
  _XX_____,
  ________,
  ________,
  _XX_____,
  _XX_____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x3B[ 16] = { /* code 003B */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  _XX_____,
  _XX_____,
  ________,
  ________,
  _XX_____,
  _XX_____,
  XX______,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x3C[ 16] = { /* code 003C */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ____X___,
  __XX____,
  _X______,
  __XX____,
  ____X___,
  ________,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x3D[ 16] = { /* code 003D */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  _XXXX___,
  ________,
  _XXXX___,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x3E[ 16] = { /* code 003E */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  _X______,
  __XX____,
  ____X___,
  __XX____,
  _X______,
  ________,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x3F[ 16] = { /* code 003F */
  ________,
  ________,
  ________,
  ________,
  XXX_____,
  ___X____,
  __X_____,
  _X______,
  _X______,
  ________,
  _X______,
  _X______,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x40[ 32] = { /* code 0040 */
  ________,________,
  ________,________,
  ________,________,
  __XXXX__,________,
  _X____X_,________,
  X_XX___X,________,
  X___X__X,________,
  X__XX__X,________,
  X_X_X__X,________,
  X_X_X__X,________,
  X_XXXXX_,________,
  _X______,________,
  __XXXX__,________,
  ________,________,
  ________,________,
  ________,________};

const unsigned char acFontTrebuchetMS16_0x41[ 16] = { /* code 0041 */
  ________,
  ________,
  ________,
  ________,
  __X_____,
  __X_____,
  _X_X____,
  _X_X____,
  _X_X____,
  XXXXX___,
  X___X___,
  X___X___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x42[ 16] = { /* code 0042 */
  ________,
  ________,
  ________,
  ________,
  XXXX____,
  X___X___,
  X___X___,
  XXXX____,
  X___X___,
  X___X___,
  X___X___,
  XXXX____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x43[ 16] = { /* code 0043 */
  ________,
  ________,
  ________,
  ________,
  __XXX___,
  _X___X__,
  X_______,
  X_______,
  X_______,
  X_______,
  _X___X__,
  __XXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x44[ 16] = { /* code 0044 */
  ________,
  ________,
  ________,
  ________,
  XXXX____,
  X___X___,
  X____X__,
  X____X__,
  X____X__,
  X____X__,
  X___X___,
  XXXX____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x45[ 16] = { /* code 0045 */
  ________,
  ________,
  ________,
  ________,
  XXXXX___,
  X_______,
  X_______,
  XXXX____,
  X_______,
  X_______,
  X_______,
  XXXXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x46[ 16] = { /* code 0046 */
  ________,
  ________,
  ________,
  ________,
  XXXXX___,
  X_______,
  X_______,
  XXXX____,
  X_______,
  X_______,
  X_______,
  X_______,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x47[ 16] = { /* code 0047 */
  ________,
  ________,
  ________,
  ________,
  __XXX___,
  _X___X__,
  X_______,
  X_______,
  X___XX__,
  X____X__,
  _X___X__,
  __XXXX__,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x48[ 16] = { /* code 0048 */
  ________,
  ________,
  ________,
  ________,
  X____X__,
  X____X__,
  X____X__,
  XXXXXX__,
  X____X__,
  X____X__,
  X____X__,
  X____X__,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x49[ 16] = { /* code 0049 */
  ________,
  ________,
  ________,
  ________,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x4A[ 16] = { /* code 004A */
  ________,
  ________,
  ________,
  ________,
  ___X____,
  ___X____,
  ___X____,
  ___X____,
  ___X____,
  ___X____,
  X__X____,
  _XX_____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x4B[ 16] = { /* code 004B */
  ________,
  ________,
  ________,
  ________,
  X___X___,
  X__X____,
  X_X_____,
  XXX_____,
  X_X_____,
  X__X____,
  X__X____,
  X___X___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x4C[ 16] = { /* code 004C */
  ________,
  ________,
  ________,
  ________,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  _XXXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x4D[ 16] = { /* code 004D */
  ________,
  ________,
  ________,
  ________,
  _X___X__,
  _X___X__,
  _X___X__,
  _XX_XX__,
  X_X_X_X_,
  X_X_X_X_,
  X__X__X_,
  X__X__X_,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x4E[ 16] = { /* code 004E */
  ________,
  ________,
  ________,
  ________,
  X____X__,
  XX___X__,
  X_X__X__,
  X_X__X__,
  X__X_X__,
  X__X_X__,
  X___XX__,
  X____X__,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x4F[ 16] = { /* code 004F */
  ________,
  ________,
  ________,
  ________,
  __XXX___,
  _X___X__,
  X_____X_,
  X_____X_,
  X_____X_,
  X_____X_,
  _X___X__,
  __XXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x50[ 16] = { /* code 0050 */
  ________,
  ________,
  ________,
  ________,
  XXXX____,
  X___X___,
  X___X___,
  X___X___,
  XXXX____,
  X_______,
  X_______,
  X_______,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x51[ 16] = { /* code 0051 */
  ________,
  ________,
  ________,
  ________,
  __XXX___,
  _X___X__,
  X_____X_,
  X_____X_,
  X_____X_,
  X_____X_,
  _X___X__,
  __XXX___,
  ____XX__,
  ______XX,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x52[ 16] = { /* code 0052 */
  ________,
  ________,
  ________,
  ________,
  XXXX____,
  X___X___,
  X___X___,
  X___X___,
  XXXX____,
  X__X____,
  X___X___,
  X____X__,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x53[ 16] = { /* code 0053 */
  ________,
  ________,
  ________,
  ________,
  _XXX____,
  X_______,
  X_______,
  _X______,
  __X_____,
  ___X____,
  ___X____,
  XXX_____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x54[ 16] = { /* code 0054 */
  ________,
  ________,
  ________,
  ________,
  XXXXX___,
  __X_____,
  __X_____,
  __X_____,
  __X_____,
  __X_____,
  __X_____,
  __X_____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x55[ 16] = { /* code 0055 */
  ________,
  ________,
  ________,
  ________,
  X____X__,
  X____X__,
  X____X__,
  X____X__,
  X____X__,
  X____X__,
  X____X__,
  _XXXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x56[ 16] = { /* code 0056 */
  ________,
  ________,
  ________,
  ________,
  X_____X_,
  _X___X__,
  _X___X__,
  _X___X__,
  __X_X___,
  __X_X___,
  __X_X___,
  ___X____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x57[ 32] = { /* code 0057 */
  ________,________,
  ________,________,
  ________,________,
  ________,________,
  X___X___,X_______,
  X___X___,X_______,
  _X_X_X_X,________,
  _X_X_X_X,________,
  _X_X_X_X,________,
  _X_X_X_X,________,
  __X___X_,________,
  __X___X_,________,
  ________,________,
  ________,________,
  ________,________,
  ________,________};

const unsigned char acFontTrebuchetMS16_0x58[ 16] = { /* code 0058 */
  ________,
  ________,
  ________,
  ________,
  X____X__,
  _X__X___,
  _X__X___,
  __XX____,
  __XX____,
  _X__X___,
  _X__X___,
  X____X__,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x59[ 16] = { /* code 0059 */
  ________,
  ________,
  ________,
  ________,
  X_____X_,
  _X___X__,
  __X_X___,
  ___X____,
  ___X____,
  ___X____,
  ___X____,
  ___X____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x5A[ 16] = { /* code 005A */
  ________,
  ________,
  ________,
  ________,
  XXXXX___,
  ___X____,
  ___X____,
  __X_____,
  _X______,
  _X______,
  X_______,
  XXXXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x5B[ 16] = { /* code 005B */
  ________,
  ________,
  ________,
  ________,
  __XX____,
  __X_____,
  __X_____,
  __X_____,
  __X_____,
  __X_____,
  __X_____,
  __X_____,
  __X_____,
  __XX____,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x5C[ 16] = { /* code 005C */
  ________,
  ________,
  ________,
  ________,
  X_______,
  _X______,
  _X______,
  _X______,
  __X_____,
  __X_____,
  __X_____,
  ___X____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x5D[ 16] = { /* code 005D */
  ________,
  ________,
  ________,
  ________,
  XX______,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  XX______,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x5E[ 16] = { /* code 005E */
  ________,
  ________,
  ________,
  ________,
  ___X____,
  __X_X___,
  _X___X__,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x5F[ 16] = { /* code 005F */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  XXXXXX__,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x60[ 16] = { /* code 0060 */
  ________,
  ________,
  ________,
  __X_____,
  ___X____,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x61[ 16] = { /* code 0061 */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  _XXX____,
  ____X___,
  _XXXX___,
  X___X___,
  X___X___,
  _XXXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x62[ 16] = { /* code 0062 */
  ________,
  ________,
  ________,
  ________,
  X_______,
  X_______,
  XXXX____,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  XXXX____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x63[ 16] = { /* code 0063 */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  _XXX____,
  X___X___,
  X_______,
  X_______,
  X___X___,
  _XXX____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x64[ 16] = { /* code 0064 */
  ________,
  ________,
  ________,
  ________,
  ____X___,
  ____X___,
  _XXXX___,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  _XXXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x65[ 16] = { /* code 0065 */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  _XXX____,
  X___X___,
  XXXXX___,
  X_______,
  X___X___,
  _XXX____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x66[ 16] = { /* code 0066 */
  ________,
  ________,
  ________,
  ________,
  __XX____,
  _X______,
  XXX_____,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x67[ 16] = { /* code 0067 */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  _XXXX___,
  X___X___,
  X___X___,
  _XXX____,
  X_______,
  _XXXX___,
  X____X__,
  _XXXX___,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x68[ 16] = { /* code 0068 */
  ________,
  ________,
  ________,
  ________,
  X_______,
  X_______,
  XXXX____,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x69[ 16] = { /* code 0069 */
  ________,
  ________,
  ________,
  ________,
  _X______,
  ________,
  XX______,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x6A[ 16] = { /* code 006A */
  ________,
  ________,
  ________,
  ________,
  __X_____,
  ________,
  _XX_____,
  __X_____,
  __X_____,
  __X_____,
  __X_____,
  __X_____,
  __X_____,
  XX______,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x6B[ 16] = { /* code 006B */
  ________,
  ________,
  ________,
  ________,
  X_______,
  X_______,
  X__X____,
  X_X_____,
  XX______,
  X_X_____,
  X__X____,
  X___X___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x6C[ 16] = { /* code 006C */
  ________,
  ________,
  ________,
  ________,
  X_______,
  X_______,
  X_______,
  X_______,
  X_______,
  X_______,
  X_______,
  X_______,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x6D[ 16] = { /* code 006D */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  XXXXXX__,
  X__X__X_,
  X__X__X_,
  X__X__X_,
  X__X__X_,
  X__X__X_,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x6E[ 16] = { /* code 006E */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  XXXX____,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x6F[ 16] = { /* code 006F */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  _XXX____,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  _XXX____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x70[ 16] = { /* code 0070 */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  XXXX____,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  XXXX____,
  X_______,
  X_______,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x71[ 16] = { /* code 0071 */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  _XXXX___,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  _XXXX___,
  ____X___,
  ____X___,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x72[ 16] = { /* code 0072 */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  _X_X____,
  _XX_____,
  _X______,
  _X______,
  _X______,
  _X______,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x73[ 16] = { /* code 0073 */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  _XXX____,
  X_______,
  _X______,
  __X_____,
  ___X____,
  XXX_____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x74[ 16] = { /* code 0074 */
  ________,
  ________,
  ________,
  ________,
  _X______,
  _X______,
  XXXX____,
  _X______,
  _X______,
  _X______,
  _X______,
  __XX____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x75[ 16] = { /* code 0075 */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  _XXXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x76[ 16] = { /* code 0076 */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  X___X___,
  X___X___,
  _X_X____,
  _X_X____,
  _X_X____,
  __X_____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x77[ 16] = { /* code 0077 */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  X__X__X_,
  X__X__X_,
  X_X_X_X_,
  X_X_X_X_,
  X_X_X_X_,
  _X___X__,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x78[ 16] = { /* code 0078 */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  X___X___,
  _X_X____,
  __X_____,
  __X_____,
  _X_X____,
  X___X___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x79[ 16] = { /* code 0079 */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  X___X___,
  X___X___,
  _X_X____,
  _X_X____,
  _X_X____,
  __X_____,
  __X_____,
  XX______,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x7A[ 16] = { /* code 007A */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  XXXXX___,
  ____X___,
  ___X____,
  __X_____,
  _X______,
  XXXXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x7B[ 16] = { /* code 007B */
  ________,
  ________,
  ________,
  ________,
  _XX_____,
  _X______,
  _X______,
  _X______,
  X_______,
  _X______,
  _X______,
  _X______,
  _X______,
  _XX_____,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x7C[ 16] = { /* code 007C */
  ________,
  ________,
  ________,
  ________,
  __X_____,
  __X_____,
  __X_____,
  __X_____,
  __X_____,
  __X_____,
  __X_____,
  __X_____,
  __X_____,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x7D[ 16] = { /* code 007D */
  ________,
  ________,
  ________,
  ________,
  XX______,
  _X______,
  _X______,
  _X______,
  __X_____,
  _X______,
  _X______,
  _X______,
  _X______,
  XX______,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x7E[ 16] = { /* code 007E */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  XX__X___,
  X_XXX___,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0x7F[ 16] = { /* code 007F */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xA0[ 16] = { /* code 00A0 */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xA1[ 16] = { /* code 00A1 */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  _X______,
  _X______,
  ________,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xA2[ 16] = { /* code 00A2 */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ____X___,
  __XXX___,
  _X_X____,
  _X_X____,
  _XXX____,
  __XXX___,
  __X_____,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xA3[ 16] = { /* code 00A3 */
  ________,
  ________,
  ________,
  ________,
  __XXX___,
  _X______,
  _X______,
  XXX_____,
  _X______,
  _X______,
  _X__X___,
  XXXX____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xA4[ 16] = { /* code 00A4 */
  ________,
  ________,
  ________,
  ________,
  ________,
  XXXXX___,
  X___X___,
  X___X___,
  X___X___,
  XXXXX___,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xA5[ 16] = { /* code 00A5 */
  ________,
  ________,
  ________,
  ________,
  X_____X_,
  _X___X__,
  __X_X___,
  ___X____,
  ___X____,
  __XXX___,
  ___X____,
  ___X____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xA6[ 16] = { /* code 00A6 */
  ________,
  ________,
  ________,
  ________,
  ___X____,
  ___X____,
  ___X____,
  ___X____,
  ________,
  ___X____,
  ___X____,
  ___X____,
  ___X____,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xA7[ 16] = { /* code 00A7 */
  ________,
  ________,
  ________,
  ________,
  _XXX____,
  X_______,
  XX______,
  X_XX____,
  XX_X____,
  __XX____,
  ___X____,
  XXX_____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xA8[ 16] = { /* code 00A8 */
  ________,
  ________,
  ________,
  ________,
  _X_X____,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xA9[ 16] = { /* code 00A9 */
  ________,
  ________,
  ________,
  ________,
  __XXXX__,
  _X____X_,
  X__XX__X,
  X_X____X,
  X_X____X,
  X__XX__X,
  _X____X_,
  __XXXX__,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xAA[ 16] = { /* code 00AA */
  ________,
  ________,
  ________,
  ________,
  XXX_____,
  __X_____,
  XXX_____,
  X_X_____,
  XXX_____,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xAB[ 16] = { /* code 00AB */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  __X_X___,
  _X_X____,
  __X_X___,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xAC[ 16] = { /* code 00AC */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  _XXXX___,
  ____X___,
  ____X___,
  ________,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xAD[ 16] = { /* code 00AD */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  XXX_____,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xAE[ 16] = { /* code 00AE */
  ________,
  ________,
  ________,
  ________,
  __XXXX__,
  _X____X_,
  X_XXX__X,
  X_X_X__X,
  X_XX___X,
  X_X_X__X,
  _X____X_,
  __XXXX__,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xAF[ 16] = { /* code 00AF */
  ________,
  ________,
  ________,
  XXXXXX__,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xB0[ 16] = { /* code 00B0 */
  ________,
  ________,
  ________,
  ________,
  _XXX____,
  _X_X____,
  _XXX____,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xB1[ 16] = { /* code 00B1 */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  __X_____,
  __X_____,
  XXXXX___,
  __X_____,
  __X_____,
  ________,
  XXXXX___,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xB2[ 16] = { /* code 00B2 */
  ________,
  ________,
  ________,
  ________,
  _XXX____,
  ___X____,
  __X_____,
  _X______,
  _XXX____,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xB3[ 16] = { /* code 00B3 */
  ________,
  ________,
  ________,
  ________,
  _XXX____,
  ___X____,
  __XX____,
  ___X____,
  _XXX____,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xB4[ 16] = { /* code 00B4 */
  ________,
  ________,
  ________,
  ___X____,
  __X_____,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xB5[ 16] = { /* code 00B5 */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  XXXXX___,
  X_______,
  X_______,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xB6[ 16] = { /* code 00B6 */
  ________,
  ________,
  ________,
  ________,
  _XXXX___,
  XXX_X___,
  XXX_X___,
  XXX_X___,
  _XX_X___,
  __X_X___,
  __X_X___,
  __X_X___,
  __X_X___,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xB7[ 16] = { /* code 00B7 */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  _XX_____,
  _XX_____,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xB8[ 16] = { /* code 00B8 */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  __X_____,
  __X_____,
  _X______,
  ________};

const unsigned char acFontTrebuchetMS16_0xB9[ 16] = { /* code 00B9 */
  ________,
  ________,
  ________,
  ________,
  __X_____,
  _XX_____,
  __X_____,
  __X_____,
  __X_____,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xBA[ 16] = { /* code 00BA */
  ________,
  ________,
  ________,
  ________,
  _XX_____,
  X__X____,
  X__X____,
  X__X____,
  _XX_____,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xBB[ 16] = { /* code 00BB */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  _X_X____,
  __X_X___,
  _X_X____,
  ________,
  ________,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xBC[ 32] = { /* code 00BC */
  ________,________,
  ________,________,
  ________,________,
  ________,________,
  _X____X_,________,
  XX___X__,________,
  _X__X___,________,
  _X__X__X,________,
  _X_X__XX,________,
  __X__X_X,________,
  __X_XXXX,X_______,
  _X_____X,________,
  ________,________,
  ________,________,
  ________,________,
  ________,________};

const unsigned char acFontTrebuchetMS16_0xBD[ 32] = { /* code 00BD */
  ________,________,
  ________,________,
  ________,________,
  ________,________,
  _X____X_,________,
  XX___X__,________,
  _X__X___,________,
  _X__XXXX,________,
  _X_X___X,________,
  __X___X_,________,
  __X__X__,________,
  _X___XXX,________,
  ________,________,
  ________,________,
  ________,________,
  ________,________};

const unsigned char acFontTrebuchetMS16_0xBE[ 32] = { /* code 00BE */
  ________,________,
  ________,________,
  ________,________,
  ________,________,
  XXX___X_,________,
  __X__X__,________,
  _X__X___,________,
  __X_X__X,________,
  XXXX__XX,________,
  __X__X_X,________,
  __X_XXXX,X_______,
  _X_____X,________,
  ________,________,
  ________,________,
  ________,________,
  ________,________};

const unsigned char acFontTrebuchetMS16_0xBF[ 16] = { /* code 00BF */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  __X_____,
  __X_____,
  ________,
  __X_____,
  __X_____,
  _X______,
  X_______,
  _XXX____,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xC0[ 16] = { /* code 00C0 */
  ________,
  _X______,
  __X_____,
  ________,
  __X_____,
  __X_____,
  _X_X____,
  _X_X____,
  _X_X____,
  XXXXX___,
  X___X___,
  X___X___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xC1[ 16] = { /* code 00C1 */
  ________,
  ___X____,
  __X_____,
  ________,
  __X_____,
  __X_____,
  _X_X____,
  _X_X____,
  _X_X____,
  XXXXX___,
  X___X___,
  X___X___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xC2[ 16] = { /* code 00C2 */
  ________,
  __X_____,
  _X_X____,
  ________,
  __X_____,
  __X_____,
  _X_X____,
  _X_X____,
  _X_X____,
  XXXXX___,
  X___X___,
  X___X___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xC3[ 16] = { /* code 00C3 */
  ________,
  _XX_X___,
  _X_XX___,
  ________,
  __X_____,
  __X_____,
  _X_X____,
  _X_X____,
  _X_X____,
  XXXXX___,
  X___X___,
  X___X___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xC4[ 16] = { /* code 00C4 */
  ________,
  ________,
  _X_X____,
  ________,
  __X_____,
  __X_____,
  _X_X____,
  _X_X____,
  _X_X____,
  XXXXX___,
  X___X___,
  X___X___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xC5[ 16] = { /* code 00C5 */
  ________,
  ________,
  _XXX____,
  _X_X____,
  _XXX____,
  __X_____,
  _X_X____,
  _X_X____,
  _X_X____,
  XXXXX___,
  X___X___,
  X___X___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xC6[ 32] = { /* code 00C6 */
  ________,________,
  ________,________,
  ________,________,
  ________,________,
  ___XXXXX,X_______,
  ___XX___,________,
  __X_X___,________,
  __X_XXXX,________,
  _X__X___,________,
  _XXXX___,________,
  _X__X___,________,
  X___XXXX,X_______,
  ________,________,
  ________,________,
  ________,________,
  ________,________};

const unsigned char acFontTrebuchetMS16_0xC7[ 16] = { /* code 00C7 */
  ________,
  ________,
  ________,
  ________,
  __XXX___,
  _X___X__,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  _X___X__,
  __XXX___,
  ___X____,
  ___X____,
  __X_____,
  ________};

const unsigned char acFontTrebuchetMS16_0xC8[ 16] = { /* code 00C8 */
  ________,
  _X______,
  __X_____,
  ________,
  XXXXX___,
  X_______,
  X_______,
  XXXX____,
  X_______,
  X_______,
  X_______,
  XXXXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xC9[ 16] = { /* code 00C9 */
  ________,
  ___X____,
  __X_____,
  ________,
  XXXXX___,
  X_______,
  X_______,
  XXXX____,
  X_______,
  X_______,
  X_______,
  XXXXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xCA[ 16] = { /* code 00CA */
  ________,
  __X_____,
  _X_X____,
  ________,
  XXXXX___,
  X_______,
  X_______,
  XXXX____,
  X_______,
  X_______,
  X_______,
  XXXXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xCB[ 16] = { /* code 00CB */
  ________,
  ________,
  _X_X____,
  ________,
  XXXXX___,
  X_______,
  X_______,
  XXXX____,
  X_______,
  X_______,
  X_______,
  XXXXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xCC[ 16] = { /* code 00CC */
  ________,
  X_______,
  _X______,
  ________,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xCD[ 16] = { /* code 00CD */
  ________,
  __X_____,
  _X______,
  ________,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xCE[ 16] = { /* code 00CE */
  ________,
  _X______,
  X_X_____,
  ________,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xCF[ 16] = { /* code 00CF */
  ________,
  ________,
  X_X_____,
  ________,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xD0[ 16] = { /* code 00D0 */
  ________,
  ________,
  ________,
  ________,
  XXXX____,
  X___X___,
  X____X__,
  XX___X__,
  X____X__,
  X____X__,
  X___X___,
  XXXX____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xD1[ 16] = { /* code 00D1 */
  ________,
  _XX_X___,
  _X_XX___,
  ________,
  X____X__,
  XX___X__,
  X_X__X__,
  X_X__X__,
  X__X_X__,
  X__X_X__,
  X___XX__,
  X____X__,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xD2[ 16] = { /* code 00D2 */
  ________,
  __X_____,
  ___X____,
  ________,
  __XXX___,
  _X___X__,
  X_____X_,
  X_____X_,
  X_____X_,
  X_____X_,
  _X___X__,
  __XXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xD3[ 16] = { /* code 00D3 */
  ________,
  ____X___,
  ___X____,
  ________,
  __XXX___,
  _X___X__,
  X_____X_,
  X_____X_,
  X_____X_,
  X_____X_,
  _X___X__,
  __XXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xD4[ 16] = { /* code 00D4 */
  ________,
  ___X____,
  __X_X___,
  ________,
  __XXX___,
  _X___X__,
  X_____X_,
  X_____X_,
  X_____X_,
  X_____X_,
  _X___X__,
  __XXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xD5[ 16] = { /* code 00D5 */
  ________,
  __XX_X__,
  __X_XX__,
  ________,
  __XXX___,
  _X___X__,
  X_____X_,
  X_____X_,
  X_____X_,
  X_____X_,
  _X___X__,
  __XXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xD6[ 16] = { /* code 00D6 */
  ________,
  ________,
  __X_X___,
  ________,
  __XXX___,
  _X___X__,
  X_____X_,
  X_____X_,
  X_____X_,
  X_____X_,
  _X___X__,
  __XXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xD7[ 16] = { /* code 00D7 */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  X___X___,
  _X_X____,
  __X_____,
  _X_X____,
  X___X___,
  ________,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xD8[ 16] = { /* code 00D8 */
  ________,
  ________,
  ________,
  ________,
  __XXXX__,
  _X__X___,
  X__X_X__,
  X__X_X__,
  X_X__X__,
  X_X__X__,
  _X__X___,
  XXXX____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xD9[ 16] = { /* code 00D9 */
  ________,
  __X_____,
  ___X____,
  ________,
  X____X__,
  X____X__,
  X____X__,
  X____X__,
  X____X__,
  X____X__,
  X____X__,
  _XXXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xDA[ 16] = { /* code 00DA */
  ________,
  ____X___,
  ___X____,
  ________,
  X____X__,
  X____X__,
  X____X__,
  X____X__,
  X____X__,
  X____X__,
  X____X__,
  _XXXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xDB[ 16] = { /* code 00DB */
  ________,
  ___X____,
  __X_X___,
  ________,
  X____X__,
  X____X__,
  X____X__,
  X____X__,
  X____X__,
  X____X__,
  X____X__,
  _XXXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xDC[ 16] = { /* code 00DC */
  ________,
  ________,
  __X_X___,
  ________,
  X____X__,
  X____X__,
  X____X__,
  X____X__,
  X____X__,
  X____X__,
  X____X__,
  _XXXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xDD[ 16] = { /* code 00DD */
  ________,
  ____X___,
  ___X____,
  ________,
  X_____X_,
  _X___X__,
  __X_X___,
  ___X____,
  ___X____,
  ___X____,
  ___X____,
  ___X____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xDE[ 16] = { /* code 00DE */
  ________,
  ________,
  ________,
  ________,
  X_______,
  XXXX____,
  X___X___,
  X___X___,
  X___X___,
  XXXX____,
  X_______,
  X_______,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xDF[ 16] = { /* code 00DF */
  ________,
  ________,
  ________,
  ________,
  _XX_____,
  X__X____,
  X__X____,
  X_X_____,
  X__X____,
  X___X___,
  X___X___,
  X_XX____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xE0[ 16] = { /* code 00E0 */
  ________,
  ________,
  ________,
  _X______,
  __X_____,
  ________,
  _XXX____,
  ____X___,
  _XXXX___,
  X___X___,
  X___X___,
  _XXXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xE1[ 16] = { /* code 00E1 */
  ________,
  ________,
  ________,
  ___X____,
  __X_____,
  ________,
  _XXX____,
  ____X___,
  _XXXX___,
  X___X___,
  X___X___,
  _XXXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xE2[ 16] = { /* code 00E2 */
  ________,
  ________,
  ________,
  __X_____,
  _X_X____,
  ________,
  _XXX____,
  ____X___,
  _XXXX___,
  X___X___,
  X___X___,
  _XXXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xE3[ 16] = { /* code 00E3 */
  ________,
  ________,
  ________,
  _XX_X___,
  _X_XX___,
  ________,
  _XXX____,
  ____X___,
  _XXXX___,
  X___X___,
  X___X___,
  _XXXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xE4[ 16] = { /* code 00E4 */
  ________,
  ________,
  ________,
  ________,
  _X_X____,
  ________,
  _XXX____,
  ____X___,
  _XXXX___,
  X___X___,
  X___X___,
  _XXXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xE5[ 16] = { /* code 00E5 */
  ________,
  ________,
  _XXX____,
  _X_X____,
  _XXX____,
  ________,
  _XXX____,
  ____X___,
  _XXXX___,
  X___X___,
  X___X___,
  _XXXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xE6[ 32] = { /* code 00E6 */
  ________,________,
  ________,________,
  ________,________,
  ________,________,
  ________,________,
  ________,________,
  _XXX_XXX,________,
  ____X___,X_______,
  _XXXXXXX,X_______,
  X___X___,________,
  X___X___,X_______,
  _XXXXXXX,________,
  ________,________,
  ________,________,
  ________,________,
  ________,________};

const unsigned char acFontTrebuchetMS16_0xE7[ 16] = { /* code 00E7 */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  _XXX____,
  X___X___,
  X_______,
  X_______,
  X___X___,
  _XXX____,
  ___X____,
  ___X____,
  __X_____,
  ________};

const unsigned char acFontTrebuchetMS16_0xE8[ 16] = { /* code 00E8 */
  ________,
  ________,
  ________,
  _X______,
  __X_____,
  ________,
  _XXX____,
  X___X___,
  XXXXX___,
  X_______,
  X___X___,
  _XXX____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xE9[ 16] = { /* code 00E9 */
  ________,
  ________,
  ________,
  ___X____,
  __X_____,
  ________,
  _XXX____,
  X___X___,
  XXXXX___,
  X_______,
  X___X___,
  _XXX____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xEA[ 16] = { /* code 00EA */
  ________,
  ________,
  ________,
  __X_____,
  _X_X____,
  ________,
  _XXX____,
  X___X___,
  XXXXX___,
  X_______,
  X___X___,
  _XXX____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xEB[ 16] = { /* code 00EB */
  ________,
  ________,
  ________,
  ________,
  _X_X____,
  ________,
  _XXX____,
  X___X___,
  XXXXX___,
  X_______,
  X___X___,
  _XXX____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xEC[ 16] = { /* code 00EC */
  ________,
  ________,
  ________,
  X_______,
  _X______,
  ________,
  XX______,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xED[ 16] = { /* code 00ED */
  ________,
  ________,
  ________,
  __X_____,
  _X______,
  ________,
  XX______,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xEE[ 16] = { /* code 00EE */
  ________,
  ________,
  ________,
  _X______,
  X_X_____,
  ________,
  XX______,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xEF[ 16] = { /* code 00EF */
  ________,
  ________,
  ________,
  ________,
  X_X_____,
  ________,
  XX______,
  _X______,
  _X______,
  _X______,
  _X______,
  _X______,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xF0[ 16] = { /* code 00F0 */
  ________,
  ________,
  ________,
  _X______,
  XXXX____,
  ___X____,
  _XXX____,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  _XXX____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xF1[ 16] = { /* code 00F1 */
  ________,
  ________,
  ________,
  _XX_X___,
  _X_XX___,
  ________,
  XXXX____,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xF2[ 16] = { /* code 00F2 */
  ________,
  ________,
  ________,
  _X______,
  __X_____,
  ________,
  _XXX____,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  _XXX____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xF3[ 16] = { /* code 00F3 */
  ________,
  ________,
  ________,
  ___X____,
  __X_____,
  ________,
  _XXX____,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  _XXX____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xF4[ 16] = { /* code 00F4 */
  ________,
  ________,
  ________,
  __X_____,
  _X_X____,
  ________,
  _XXX____,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  _XXX____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xF5[ 16] = { /* code 00F5 */
  ________,
  ________,
  ________,
  _XX_X___,
  _X_XX___,
  ________,
  _XXX____,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  _XXX____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xF6[ 16] = { /* code 00F6 */
  ________,
  ________,
  ________,
  ________,
  _X_X____,
  ________,
  _XXX____,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  _XXX____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xF7[ 16] = { /* code 00F7 */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  __X_____,
  ________,
  XXXXX___,
  ________,
  __X_____,
  ________,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xF8[ 16] = { /* code 00F8 */
  ________,
  ________,
  ________,
  ________,
  ________,
  ________,
  _XXXX___,
  X__XX___,
  X_X_X___,
  X_X_X___,
  XX__X___,
  XXXX____,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xF9[ 16] = { /* code 00F9 */
  ________,
  ________,
  ________,
  _X______,
  __X_____,
  ________,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  _XXXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xFA[ 16] = { /* code 00FA */
  ________,
  ________,
  ________,
  ___X____,
  __X_____,
  ________,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  _XXXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xFB[ 16] = { /* code 00FB */
  ________,
  ________,
  ________,
  __X_____,
  _X_X____,
  ________,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  _XXXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xFC[ 16] = { /* code 00FC */
  ________,
  ________,
  ________,
  ________,
  _X_X____,
  ________,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  _XXXX___,
  ________,
  ________,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xFD[ 16] = { /* code 00FD */
  ________,
  ________,
  ________,
  ___X____,
  __X_____,
  ________,
  X___X___,
  X___X___,
  _X_X____,
  _X_X____,
  _X_X____,
  __X_____,
  __X_____,
  XX______,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xFE[ 16] = { /* code 00FE */
  ________,
  ________,
  ________,
  ________,
  X_______,
  X_______,
  XXXX____,
  X___X___,
  X___X___,
  X___X___,
  X___X___,
  XXXX____,
  X_______,
  X_______,
  ________,
  ________};

const unsigned char acFontTrebuchetMS16_0xFF[ 16] = { /* code 00FF */
  ________,
  ________,
  ________,
  ________,
  _X_X____,
  ________,
  X___X___,
  X___X___,
  _X_X____,
  _X_X____,
  _X_X____,
  __X_____,
  __X_____,
  XX______,
  ________,
  ________};

const FONT_CHARINFO FontTrebuchetMS16_CharInfo[192] = {
   {   3,   3,  1, (void *)&acFontTrebuchetMS16_0x20 } /* code 0020 */
  ,{   4,   4,  1, (void *)&acFontTrebuchetMS16_0x21 } /* code 0021 */
  ,{   4,   4,  1, (void *)&acFontTrebuchetMS16_0x22 } /* code 0022 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x23 } /* code 0023 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x24 } /* code 0024 */
  ,{   7,   7,  1, (void *)&acFontTrebuchetMS16_0x25 } /* code 0025 */
  ,{   8,   8,  1, (void *)&acFontTrebuchetMS16_0x26 } /* code 0026 */
  ,{   2,   2,  1, (void *)&acFontTrebuchetMS16_0x27 } /* code 0027 */
  ,{   4,   4,  1, (void *)&acFontTrebuchetMS16_0x28 } /* code 0028 */
  ,{   4,   4,  1, (void *)&acFontTrebuchetMS16_0x29 } /* code 0029 */
  ,{   4,   4,  1, (void *)&acFontTrebuchetMS16_0x2A } /* code 002A */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x2B } /* code 002B */
  ,{   4,   4,  1, (void *)&acFontTrebuchetMS16_0x2C } /* code 002C */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x2D } /* code 002D */
  ,{   4,   4,  1, (void *)&acFontTrebuchetMS16_0x2E } /* code 002E */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x2F } /* code 002F */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x30 } /* code 0030 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x31 } /* code 0031 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x32 } /* code 0032 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x33 } /* code 0033 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x34 } /* code 0034 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x35 } /* code 0035 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x36 } /* code 0036 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x37 } /* code 0037 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x38 } /* code 0038 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x39 } /* code 0039 */
  ,{   4,   4,  1, (void *)&acFontTrebuchetMS16_0x3A } /* code 003A */
  ,{   4,   4,  1, (void *)&acFontTrebuchetMS16_0x3B } /* code 003B */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x3C } /* code 003C */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x3D } /* code 003D */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x3E } /* code 003E */
  ,{   4,   4,  1, (void *)&acFontTrebuchetMS16_0x3F } /* code 003F */
  ,{   9,   9,  2, (void *)&acFontTrebuchetMS16_0x40 } /* code 0040 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x41 } /* code 0041 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x42 } /* code 0042 */
  ,{   7,   7,  1, (void *)&acFontTrebuchetMS16_0x43 } /* code 0043 */
  ,{   7,   7,  1, (void *)&acFontTrebuchetMS16_0x44 } /* code 0044 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x45 } /* code 0045 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x46 } /* code 0046 */
  ,{   7,   7,  1, (void *)&acFontTrebuchetMS16_0x47 } /* code 0047 */
  ,{   7,   7,  1, (void *)&acFontTrebuchetMS16_0x48 } /* code 0048 */
  ,{   4,   4,  1, (void *)&acFontTrebuchetMS16_0x49 } /* code 0049 */
  ,{   5,   5,  1, (void *)&acFontTrebuchetMS16_0x4A } /* code 004A */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x4B } /* code 004B */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x4C } /* code 004C */
  ,{   8,   8,  1, (void *)&acFontTrebuchetMS16_0x4D } /* code 004D */
  ,{   7,   7,  1, (void *)&acFontTrebuchetMS16_0x4E } /* code 004E */
  ,{   8,   8,  1, (void *)&acFontTrebuchetMS16_0x4F } /* code 004F */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x50 } /* code 0050 */
  ,{   8,   8,  1, (void *)&acFontTrebuchetMS16_0x51 } /* code 0051 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x52 } /* code 0052 */
  ,{   5,   5,  1, (void *)&acFontTrebuchetMS16_0x53 } /* code 0053 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x54 } /* code 0054 */
  ,{   7,   7,  1, (void *)&acFontTrebuchetMS16_0x55 } /* code 0055 */
  ,{   7,   7,  1, (void *)&acFontTrebuchetMS16_0x56 } /* code 0056 */
  ,{  10,  10,  2, (void *)&acFontTrebuchetMS16_0x57 } /* code 0057 */
  ,{   7,   7,  1, (void *)&acFontTrebuchetMS16_0x58 } /* code 0058 */
  ,{   7,   7,  1, (void *)&acFontTrebuchetMS16_0x59 } /* code 0059 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x5A } /* code 005A */
  ,{   4,   4,  1, (void *)&acFontTrebuchetMS16_0x5B } /* code 005B */
  ,{   4,   4,  1, (void *)&acFontTrebuchetMS16_0x5C } /* code 005C */
  ,{   4,   4,  1, (void *)&acFontTrebuchetMS16_0x5D } /* code 005D */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x5E } /* code 005E */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x5F } /* code 005F */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x60 } /* code 0060 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x61 } /* code 0061 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x62 } /* code 0062 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x63 } /* code 0063 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x64 } /* code 0064 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x65 } /* code 0065 */
  ,{   4,   4,  1, (void *)&acFontTrebuchetMS16_0x66 } /* code 0066 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x67 } /* code 0067 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x68 } /* code 0068 */
  ,{   3,   3,  1, (void *)&acFontTrebuchetMS16_0x69 } /* code 0069 */
  ,{   4,   4,  1, (void *)&acFontTrebuchetMS16_0x6A } /* code 006A */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x6B } /* code 006B */
  ,{   2,   2,  1, (void *)&acFontTrebuchetMS16_0x6C } /* code 006C */
  ,{   8,   8,  1, (void *)&acFontTrebuchetMS16_0x6D } /* code 006D */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x6E } /* code 006E */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x6F } /* code 006F */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x70 } /* code 0070 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x71 } /* code 0071 */
  ,{   5,   5,  1, (void *)&acFontTrebuchetMS16_0x72 } /* code 0072 */
  ,{   5,   5,  1, (void *)&acFontTrebuchetMS16_0x73 } /* code 0073 */
  ,{   5,   5,  1, (void *)&acFontTrebuchetMS16_0x74 } /* code 0074 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x75 } /* code 0075 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x76 } /* code 0076 */
  ,{   8,   8,  1, (void *)&acFontTrebuchetMS16_0x77 } /* code 0077 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x78 } /* code 0078 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x79 } /* code 0079 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x7A } /* code 007A */
  ,{   4,   4,  1, (void *)&acFontTrebuchetMS16_0x7B } /* code 007B */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x7C } /* code 007C */
  ,{   4,   4,  1, (void *)&acFontTrebuchetMS16_0x7D } /* code 007D */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x7E } /* code 007E */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0x7F } /* code 007F */
  ,{   3,   3,  1, (void *)&acFontTrebuchetMS16_0xA0 } /* code 00A0 */
  ,{   4,   4,  1, (void *)&acFontTrebuchetMS16_0xA1 } /* code 00A1 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xA2 } /* code 00A2 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xA3 } /* code 00A3 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xA4 } /* code 00A4 */
  ,{   7,   7,  1, (void *)&acFontTrebuchetMS16_0xA5 } /* code 00A5 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xA6 } /* code 00A6 */
  ,{   5,   5,  1, (void *)&acFontTrebuchetMS16_0xA7 } /* code 00A7 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xA8 } /* code 00A8 */
  ,{   8,   8,  1, (void *)&acFontTrebuchetMS16_0xA9 } /* code 00A9 */
  ,{   4,   4,  1, (void *)&acFontTrebuchetMS16_0xAA } /* code 00AA */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xAB } /* code 00AB */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xAC } /* code 00AC */
  ,{   4,   4,  1, (void *)&acFontTrebuchetMS16_0xAD } /* code 00AD */
  ,{   8,   8,  1, (void *)&acFontTrebuchetMS16_0xAE } /* code 00AE */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xAF } /* code 00AF */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xB0 } /* code 00B0 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xB1 } /* code 00B1 */
  ,{   5,   5,  1, (void *)&acFontTrebuchetMS16_0xB2 } /* code 00B2 */
  ,{   5,   5,  1, (void *)&acFontTrebuchetMS16_0xB3 } /* code 00B3 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xB4 } /* code 00B4 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xB5 } /* code 00B5 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xB6 } /* code 00B6 */
  ,{   4,   4,  1, (void *)&acFontTrebuchetMS16_0xB7 } /* code 00B7 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xB8 } /* code 00B8 */
  ,{   5,   5,  1, (void *)&acFontTrebuchetMS16_0xB9 } /* code 00B9 */
  ,{   4,   4,  1, (void *)&acFontTrebuchetMS16_0xBA } /* code 00BA */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xBB } /* code 00BB */
  ,{   9,   9,  2, (void *)&acFontTrebuchetMS16_0xBC } /* code 00BC */
  ,{   9,   9,  2, (void *)&acFontTrebuchetMS16_0xBD } /* code 00BD */
  ,{   9,   9,  2, (void *)&acFontTrebuchetMS16_0xBE } /* code 00BE */
  ,{   4,   4,  1, (void *)&acFontTrebuchetMS16_0xBF } /* code 00BF */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xC0 } /* code 00C0 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xC1 } /* code 00C1 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xC2 } /* code 00C2 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xC3 } /* code 00C3 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xC4 } /* code 00C4 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xC5 } /* code 00C5 */
  ,{  10,  10,  2, (void *)&acFontTrebuchetMS16_0xC6 } /* code 00C6 */
  ,{   7,   7,  1, (void *)&acFontTrebuchetMS16_0xC7 } /* code 00C7 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xC8 } /* code 00C8 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xC9 } /* code 00C9 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xCA } /* code 00CA */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xCB } /* code 00CB */
  ,{   4,   4,  1, (void *)&acFontTrebuchetMS16_0xCC } /* code 00CC */
  ,{   4,   4,  1, (void *)&acFontTrebuchetMS16_0xCD } /* code 00CD */
  ,{   4,   4,  1, (void *)&acFontTrebuchetMS16_0xCE } /* code 00CE */
  ,{   4,   4,  1, (void *)&acFontTrebuchetMS16_0xCF } /* code 00CF */
  ,{   7,   7,  1, (void *)&acFontTrebuchetMS16_0xD0 } /* code 00D0 */
  ,{   7,   7,  1, (void *)&acFontTrebuchetMS16_0xD1 } /* code 00D1 */
  ,{   8,   8,  1, (void *)&acFontTrebuchetMS16_0xD2 } /* code 00D2 */
  ,{   8,   8,  1, (void *)&acFontTrebuchetMS16_0xD3 } /* code 00D3 */
  ,{   8,   8,  1, (void *)&acFontTrebuchetMS16_0xD4 } /* code 00D4 */
  ,{   8,   8,  1, (void *)&acFontTrebuchetMS16_0xD5 } /* code 00D5 */
  ,{   8,   8,  1, (void *)&acFontTrebuchetMS16_0xD6 } /* code 00D6 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xD7 } /* code 00D7 */
  ,{   7,   7,  1, (void *)&acFontTrebuchetMS16_0xD8 } /* code 00D8 */
  ,{   7,   7,  1, (void *)&acFontTrebuchetMS16_0xD9 } /* code 00D9 */
  ,{   7,   7,  1, (void *)&acFontTrebuchetMS16_0xDA } /* code 00DA */
  ,{   7,   7,  1, (void *)&acFontTrebuchetMS16_0xDB } /* code 00DB */
  ,{   7,   7,  1, (void *)&acFontTrebuchetMS16_0xDC } /* code 00DC */
  ,{   7,   7,  1, (void *)&acFontTrebuchetMS16_0xDD } /* code 00DD */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xDE } /* code 00DE */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xDF } /* code 00DF */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xE0 } /* code 00E0 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xE1 } /* code 00E1 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xE2 } /* code 00E2 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xE3 } /* code 00E3 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xE4 } /* code 00E4 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xE5 } /* code 00E5 */
  ,{  10,  10,  2, (void *)&acFontTrebuchetMS16_0xE6 } /* code 00E6 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xE7 } /* code 00E7 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xE8 } /* code 00E8 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xE9 } /* code 00E9 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xEA } /* code 00EA */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xEB } /* code 00EB */
  ,{   3,   3,  1, (void *)&acFontTrebuchetMS16_0xEC } /* code 00EC */
  ,{   3,   3,  1, (void *)&acFontTrebuchetMS16_0xED } /* code 00ED */
  ,{   3,   3,  1, (void *)&acFontTrebuchetMS16_0xEE } /* code 00EE */
  ,{   3,   3,  1, (void *)&acFontTrebuchetMS16_0xEF } /* code 00EF */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xF0 } /* code 00F0 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xF1 } /* code 00F1 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xF2 } /* code 00F2 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xF3 } /* code 00F3 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xF4 } /* code 00F4 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xF5 } /* code 00F5 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xF6 } /* code 00F6 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xF7 } /* code 00F7 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xF8 } /* code 00F8 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xF9 } /* code 00F9 */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xFA } /* code 00FA */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xFB } /* code 00FB */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xFC } /* code 00FC */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xFD } /* code 00FD */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xFE } /* code 00FE */
  ,{   6,   6,  1, (void *)&acFontTrebuchetMS16_0xFF } /* code 00FF */
};

const FONT_PROP FontTrebuchetMS16_Prop2 = {
   0xA0 /* first character */
  ,0xFF /* last character  */
  ,&FontTrebuchetMS16_CharInfo[ 96] /* address of first character */
  ,(void*)0 /* pointer to next FONT_PROP */
};

const FONT_PROP FontTrebuchetMS16_Prop1 = {
   0x20 /* first character */
  ,0x7F /* last character  */
  ,&FontTrebuchetMS16_CharInfo[  0] /* address of first character */
  ,(void *)&FontTrebuchetMS16_Prop2 /* pointer to next FONT_PROP */
};

const FONT FontTrebuchetMS16 = {
  16 /* height of font  */
  ,16 /* space of font y */
  ,(void *)&FontTrebuchetMS16_Prop1
};

void * FontGetCharInfo(const FONT *pFont, unsigned char asciip)
{
    const FONT_PROP *pProperties = pFont->pProperties;
    const FONT_CHARINFO *pCharInfo = 0;

    while (pProperties)
    {
        if ((asciip >= pProperties->First) && (asciip <= pProperties->Last))
        {
            pCharInfo = pProperties->pCharInfo;
            pCharInfo += (asciip - pProperties->First);
            break;
        }
        else
        {
            pProperties = pProperties->pNext;
        }
    }

    if (!pCharInfo)
    {
        pCharInfo = &FontTrebuchetMS16_CharInfo[0x1F]; //use '?' as default if character not found
    }

    return (void *)pCharInfo;
}

unsigned char FontGetCharWidth(const FONT *pFont, unsigned char asciip)
{
    FONT_CHARINFO *pCharInfo;

    pCharInfo = (FONT_CHARINFO *)FontGetCharInfo(pFont, asciip);

    return pCharInfo->Width;
}

unsigned char FontGetNextWordWidth(const FONT *pFont, const unsigned char *pWord)
{
    FONT_CHARINFO *pCharInfo;
    unsigned char width = 0;

    if (*pWord == 0)
    {
        return 0;
    }
    else if (*pWord == ' ')
    {
        pCharInfo = FontGetCharInfo(pFont, ' ');
        return (pCharInfo->Distance);
    }
    else
    {
        while ((*pWord) && (*pWord != ' '))
        {
            pCharInfo = FontGetCharInfo(pFont, *pWord);
            width += pCharInfo->Distance;
            pWord++;
        }
        return width;
    }
}
