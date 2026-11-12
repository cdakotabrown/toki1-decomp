/**
 * Tokimeki Memorial - Common Types
 */

#ifndef TOKIMEKI_TYPES_H
#define TOKIMEKI_TYPES_H

/* Fixed-width integer types */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;

/* Float types */
typedef float f32;
typedef double f64;

/* Boolean */
typedef int bool;
#define TRUE 1
#define FALSE 0

/* NULL */
#ifndef NULL
#define NULL ((void*)0)
#endif

/* Character IDs (from tokisite documentation) */
#define CHAR_SHIORI     0   /* Fujisaki Shiori */
#define CHAR_AYAKO      1   /* Katagiri Ayako */
#define CHAR_YUKARI     2   /* Koshiki Yukari */
#define CHAR_YUINA      3   /* Himoo Yuina */
#define CHAR_MIO        4   /* Kisaragi Mio */
#define CHAR_YUKO       5   /* Asahina Yuko */
#define CHAR_SAKI       6   /* Nijino Saki */
#define CHAR_MIRA       7   /* Kagami Mira */
#define CHAR_NOZOMI     8   /* Kiyokawa Nozomi */
#define CHAR_MEGUMI     9   /* Mikihara Megumi */
#define CHAR_YUMI       10  /* Saotome Yumi */
#define CHAR_MIHARU     11  /* Tatebayashi Miharu */
#define CHAR_REI        12  /* Ijuuin Rei */
#define CHAR_YOSHIO     13  /* Saotome Yoshio */
#define CHAR_BUTLER     14  /* Ijuuin butler */
#define CHAR_PLAYER     15  /* Player character */
#define CHAR_NONE       0xFF /* No character */

/* Button masks */
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

#endif /* TOKIMEKI_TYPES_H */
