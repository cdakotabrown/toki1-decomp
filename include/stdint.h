#ifdef PC_PORT

#include_next <stdint.h>

#else

#ifndef _STDINT_H
#define _STDINT_H

#include "types.h"

typedef s8  int8_t;
typedef s16 int16_t;
typedef s32 int32_t;
typedef u8  uint8_t;
typedef u16 uint16_t;
typedef u32 uint32_t;

typedef s32 intptr_t;
typedef u32 uintptr_t;

#endif

#endif
