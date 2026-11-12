#ifndef TOKIMEKI_TYPES_H
#define TOKIMEKI_TYPES_H

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;

typedef float f32;
typedef double f64;

typedef int bool;
#define TRUE 1
#define FALSE 0

#ifndef NULL
#define NULL ((void*)0)
#endif

#define CHAR_SHIORI     0
#define CHAR_AYAKO      1
#define CHAR_YUKARI     2
#define CHAR_YUINA      3
#define CHAR_MIO        4
#define CHAR_YUKO       5
#define CHAR_SAKI       6
#define CHAR_MIRA       7
#define CHAR_NOZOMI     8
#define CHAR_MEGUMI     9
#define CHAR_YUMI       10
#define CHAR_MIHARU     11
#define CHAR_REI        12
#define CHAR_YOSHIO     13
#define CHAR_BUTLER     14
#define CHAR_PLAYER     15
#define CHAR_NONE       0xFF

#define PAD_L2       0x0001
#define PAD_R2       0x0002
#define PAD_L1       0x0004
#define PAD_R1       0x0008
#define PAD_TRIANGLE 0x0010
#define PAD_CIRCLE   0x0020
#define PAD_CROSS    0x0040
#define PAD_SQUARE   0x0080
#define PAD_SELECT   0x0100
#define PAD_L3       0x0200
#define PAD_R3       0x0400
#define PAD_START    0x0800
#define PAD_UP       0x1000
#define PAD_RIGHT    0x2000
#define PAD_DOWN     0x4000
#define PAD_LEFT     0x8000

#endif
