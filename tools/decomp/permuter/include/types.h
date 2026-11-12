#ifndef TYPES_H
#define TYPES_H

/* Standard sized types for PS1 decompilation */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef signed long long s64;

/* Boolean type */
typedef int bool;
#define true 1
#define false 0

/* PSY-Q standard structures */
typedef struct {
    s16 x, y, w, h;
} RECT;

#endif /* TYPES_H */
