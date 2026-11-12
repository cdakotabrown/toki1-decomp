#ifndef TYPES_H
#define TYPES_H

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef signed long long s64;

typedef int bool;
#define true 1
#define false 0

#ifndef RECT_DEFINED
typedef struct {
    s16 x, y, w, h;
} RECT;
#define RECT_DEFINED
#endif

#ifdef __GNUC__
static __inline__ u32 read_u32_unaligned(const void *ptr) {
    u32 val;
    __builtin_memcpy(&val, ptr, sizeof(val));
    return val;
}
static __inline__ s32 read_s32_unaligned(const void *ptr) {
    s32 val;
    __builtin_memcpy(&val, ptr, sizeof(val));
    return val;
}
#else

static u32 read_u32_unaligned(const void *ptr) {
    const u8 *p = (const u8 *)ptr;
    return (u32)p[0] | ((u32)p[1] << 8) | ((u32)p[2] << 16) | ((u32)p[3] << 24);
}
static s32 read_s32_unaligned(const void *ptr) {
    return (s32)read_u32_unaligned(ptr);
}
#endif

#endif
