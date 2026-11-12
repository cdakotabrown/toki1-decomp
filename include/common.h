#ifndef COMMON_H
#define COMMON_H

#include "types.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

#define LEN(x) ((s32)(sizeof(x) / sizeof(*(x))))
#define OFF(type, field) ((size_t)(&((type*)0)->field))
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define LOBU(x) (*(u8*)&(x))
#define LOH(x) (*(s16*)&(x))
#define HIH(x) (((s16*)&(x))[1])
#define LOHU(x) (*(u16*)&(x))
#define HIHU(x) (((u16*)&(x))[1])
#define LOW(x) (*(s32*)&(x))
#define LOWU(x) (*(u32*)&(x))

#endif
