#ifndef CONTEXT_H
#define CONTEXT_H

#include "types.h"

typedef u8  UNK_TYPE;
typedef u8  UNK_TYPE1;
typedef u16 UNK_TYPE2;
typedef u32 UNK_TYPE4;
typedef u64 UNK_TYPE8;

typedef s8  M2C_UNK1;
typedef s16 M2C_UNK2;
typedef s32 M2C_UNK4;

typedef void* UNK_PTR;
typedef void* M2C_UNK_PTR;

typedef s32 M2C_UNK;
typedef s8  M2C_UNK8;
typedef s16 M2C_UNK16;
typedef s32 M2C_UNK32;
typedef s64 M2C_UNK64;

#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

#define M2C_BITWISE(type, expr) ((type)(expr))

#define M2C_LWL(expr) (expr)
#define M2C_FIRST3BYTES(expr) (expr)
#define M2C_UNALIGNED32(expr) (expr)

#define UNALIGNED_LOAD32(ptr) ({ \
    u32 __val; \
    __asm__ volatile ( \
        "lwl %0, 3(%1)\n\t" \
        "lwr %0, 0(%1)" \
        : "=r"(__val) \
        : "r"(ptr) \
    ); \
    __val; \
})

#define UNALIGNED_STORE32(ptr, val) do { \
    __asm__ volatile ( \
        "swl %0, 3(%1)\n\t" \
        "swr %0, 0(%1)" \
        : \
        : "r"(val), "r"(ptr) \
        : "memory" \
    ); \
} while(0)

#define UNALIGNED_LOAD64(ptr, val1, val2) do { \
    __asm__ volatile ( \
        "lwl %0, 3(%2)\n\t" \
        "lwl %1, 7(%2)\n\t" \
        "lwr %0, 0(%2)\n\t" \
        "lwr %1, 4(%2)" \
        : "=r"(val1), "=r"(val2) \
        : "r"(ptr) \
    ); \
} while(0)

#define UNALIGNED_STORE64(ptr, val1, val2) do { \
    __asm__ volatile ( \
        "swl %0, 3(%2)\n\t" \
        "swl %1, 7(%2)\n\t" \
        "swr %0, 0(%2)\n\t" \
        "swr %1, 4(%2)" \
        : \
        : "r"(val1), "r"(val2), "r"(ptr) \
        : "memory" \
    ); \
} while(0)

#define UNALIGNED_COPY8(dst, src) do { \
    u32 __tmp1, __tmp2; \
    UNALIGNED_LOAD64(src, __tmp1, __tmp2); \
    UNALIGNED_STORE64(dst, __tmp1, __tmp2); \
} while(0)

#define M2C_TRAP_IF(cond) (0)
#define M2C_BREAK() (0)
#define M2C_SYNC() (0)

#define GLUE_F64(a, b) (0.0)
#define MULT_HI(a, b) (0)
#define MULTU_HI(a, b) (0)
#define DMULT_HI(a, b) (0)
#define DMULTU_HI(a, b) (0)
#define CLZ(x) (0)

#define M2C_CARRY 0

#define M2C_MEMCPY_ALIGNED memcpy
#define M2C_MEMCPY_UNALIGNED memcpy

#define M2C_ERROR(msg) (0)

typedef union {
    u8  u8_arr[0x100];
    s8  s8_arr[0x100];
    u16 u16_arr[0x80];
    s16 s16_arr[0x80];
    u32 u32_arr[0x40];
    s32 s32_arr[0x40];
    char unk_0[0x100];
} ET_Small;

typedef union {
    u8  u8_arr[0x400];
    s8  s8_arr[0x400];
    u16 u16_arr[0x200];
    s16 s16_arr[0x200];
    u32 u32_arr[0x100];
    s32 s32_arr[0x100];
    char unk_0[0x400];
} ET_Medium;

typedef union {
    u8  u8_arr[0x1000];
    s8  s8_arr[0x1000];
    u16 u16_arr[0x800];
    s16 s16_arr[0x800];
    u32 u32_arr[0x400];
    s32 s32_arr[0x400];
    char unk_0[0x1000];
} ET_Large;

typedef union {
    u8  u8_arr[0x10000];
    s8  s8_arr[0x10000];
    u16 u16_arr[0x8000];
    s16 s16_arr[0x8000];
    u32 u32_arr[0x4000];
    s32 s32_arr[0x4000];
    char unk_0[0x10000];
} ET_Huge;

typedef union {
    u8 u8[0x3C];
    s8 s8[0x3C];
    u16 u16[0x1E];
    s16 s16[0x1E];
    u32 u32[0xF];
    s32 s32[0xF];
} ET_Placeholder;

typedef struct UnkStruct_Generic {
      char unk_0[0x10000];
} UnkStruct_Generic;

typedef void (*VoidFunc)(void);
typedef s32  (*IntFunc)(void);
typedef void (*VoidFuncArg)(void*);
typedef s32  (*IntFuncArg)(void*);
typedef void (*VoidFuncInt)(s32);
typedef s32  (*IntFuncInt)(s32);

typedef void (*VoidFunc2)(s32, s32);
typedef void (*VoidFunc3)(s32, s32, s32);
typedef void (*VoidFunc4)(s32, s32, s32, s32);
typedef void (*VoidFunc5)(s32, s32, s32, s32, s32);
typedef void (*VoidFunc6)(s32, s32, s32, s32, s32, s32);
typedef void (*VoidFunc7)(s32, s32, s32, s32, s32, s32, s32);

typedef s32 (*IntFunc2)(s32, s32);
typedef s32 (*IntFunc3)(s32, s32, s32);
typedef s32 (*IntFunc4)(s32, s32, s32, s32);

#define MEM8(addr)  (*(volatile u8*)(addr))
#define MEM16(addr) (*(volatile u16*)(addr))
#define MEM32(addr) (*(volatile u32*)(addr))

#define MEMS8(addr)  (*(volatile s8*)(addr))
#define MEMS16(addr) (*(volatile s16*)(addr))
#define MEMS32(addr) (*(volatile s32*)(addr))

#define PTR8(addr)  ((volatile u8*)(addr))
#define PTR16(addr) ((volatile u16*)(addr))
#define PTR32(addr) ((volatile u32*)(addr))

#define GPU_DATA    0x1F801810
#define GPU_STATUS  0x1F801814
#define GPU_CONTROL 0x1F801814

#define SPU_BASE    0x1F801C00

#define CDROM_BASE  0x1F801800

#define DMA_BASE    0x1F801080

#define TIMER_BASE  0x1F801100

#define I_STAT      0x1F801070
#define I_MASK      0x1F801074

#define RAM_BASE        0x80000000
#define RAM_SIZE        0x00200000
#define RAM_END         0x801FFFFF

#define SCRATCHPAD_BASE 0x1F800000
#define SCRATCHPAD_SIZE 0x00000400

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef LEN
#define LEN(x) ((s32)(sizeof(x) / sizeof(*(x))))
#endif

#ifndef MIN
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#endif

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef ABS
#define ABS(x) ((x) < 0 ? -(x) : (x))
#endif

#ifndef CLAMP
#define CLAMP(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#endif

#ifndef LOBU
#define LOBU(x) (*(u8*)&(x))
#define LOH(x) (*(s16*)&(x))
#define HIH(x) (((s16*)&(x))[1])
#define LOHU(x) (*(u16*)&(x))
#define HIHU(x) (((u16*)&(x))[1])
#define LOW(x) (*(s32*)&(x))
#define LOWU(x) (*(u32*)&(x))
#endif

#define STRUCT_FIELD(ptr, offset, type) (*(type*)((u8*)(ptr) + (offset)))

#ifndef M2C_ERROR_IMPL

#endif

#endif
