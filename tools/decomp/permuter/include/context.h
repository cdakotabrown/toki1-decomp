/**
 * context.h - M2C Context File for Tokimeki Memorial Decompilation
 *
 * This file provides type definitions and placeholders for m2c to generate
 * valid, compilable C code. It should be used with the --context flag.
 *
 * Key features:
 * - M2C placeholder types (UNK_TYPE1, UNK_TYPE2, UNK_TYPE4, etc.)
 * - Generic placeholder structs for unknown memory accesses
 * - M2C_ERROR macro that compiles (but marks issues)
 * - Function pointer types for callbacks
 * - Memory access macros (MEM8, MEM16, MEM32)
 *
 * Based on SOTN-decomp patterns and Decompedia recommendations.
 */

#ifndef CONTEXT_H
#define CONTEXT_H

/*===========================================================================
 * Include base types
 *===========================================================================*/
#include "types.h"

/*===========================================================================
 * M2C Placeholder Types
 *
 * These are recognized by m2c and used for unknown struct fields.
 * Format: UNK_TYPE<size> where size is in bytes
 *===========================================================================*/

/* Standard m2c unknown types */
typedef u8  UNK_TYPE;       /* Default unknown type */
typedef u8  UNK_TYPE1;      /* 1-byte unknown */
typedef u16 UNK_TYPE2;      /* 2-byte unknown */
typedef u32 UNK_TYPE4;      /* 4-byte unknown */
typedef u64 UNK_TYPE8;      /* 8-byte unknown */

/* Alternative m2c naming convention (for struct inference, not --valid-syntax) */
typedef s8  M2C_UNK1;
typedef s16 M2C_UNK2;
typedef s32 M2C_UNK4;
/* Note: M2C_UNK8 conflicts with --valid-syntax output, so we use the s64 version only once */

/* Pointer types for unknowns */
typedef void* UNK_PTR;
typedef void* M2C_UNK_PTR;

/*===========================================================================
 * M2C Valid Syntax Macros (from m2c_macros.h)
 *
 * These macros are emitted by m2c in "valid syntax" mode (--valid-syntax).
 * They make the output compilable without human intervention.
 *===========================================================================*/

/* M2C unknown type - used for parameters with unknown types */
typedef s32 M2C_UNK;
typedef s8  M2C_UNK8;
typedef s16 M2C_UNK16;
typedef s32 M2C_UNK32;
typedef s64 M2C_UNK64;

/* Unknown field access, like `*(type_ptr) &expr->unk_offset` */
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

/* Bitwise (reinterpret) cast */
#define M2C_BITWISE(type, expr) ((type)(expr))

/* Unaligned reads - inline assembly for lwl/lwr/swl/swr instructions */
#define M2C_LWL(expr) (expr)
#define M2C_FIRST3BYTES(expr) (expr)
#define M2C_UNALIGNED32(expr) (expr)

/*===========================================================================
 * Unaligned Memory Access Macros (for Level 3 matching)
 *
 * These generate the actual lwl/lwr/swl/swr MIPS instructions that the
 * original PSY-Q compiler used for unaligned 32-bit memory access.
 * Standard C lw/sw cannot be used on unaligned addresses on MIPS.
 *===========================================================================*/

/* Load unaligned 32-bit value from address */
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

/* Store unaligned 32-bit value to address */
#define UNALIGNED_STORE32(ptr, val) do { \
    __asm__ volatile ( \
        "swl %0, 3(%1)\n\t" \
        "swr %0, 0(%1)" \
        : \
        : "r"(val), "r"(ptr) \
        : "memory" \
    ); \
} while(0)

/* Load two consecutive unaligned 32-bit values (8 bytes total) */
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

/* Store two consecutive unaligned 32-bit values (8 bytes total) */
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

/* Copy 8 bytes using unaligned access (matches original pattern) */
#define UNALIGNED_COPY8(dst, src) do { \
    u32 __tmp1, __tmp2; \
    UNALIGNED_LOAD64(src, __tmp1, __tmp2); \
    UNALIGNED_STORE64(dst, __tmp1, __tmp2); \
} while(0)

/* Unhandled instructions - returns 0 but compiles */
#define M2C_TRAP_IF(cond) (0)
#define M2C_BREAK() (0)
#define M2C_SYNC() (0)

/* Multiplication high word helpers */
#define GLUE_F64(a, b) (0.0)
#define MULT_HI(a, b) (0)
#define MULTU_HI(a, b) (0)
#define DMULT_HI(a, b) (0)
#define DMULTU_HI(a, b) (0)
#define CLZ(x) (0)

/* Carry bit from partially-implemented instructions */
#define M2C_CARRY 0

/* Memcpy patterns */
#define M2C_MEMCPY_ALIGNED memcpy
#define M2C_MEMCPY_UNALIGNED memcpy

/*===========================================================================
 * M2C_ERROR Macro
 *
 * m2c generates M2C_ERROR(...) when it can't determine a value.
 * With --valid-syntax, m2c uses M2C_FIELD instead of direct ->unk access,
 * so M2C_ERROR just returns 0.
 *===========================================================================*/

/* M2C_ERROR returns 0 - code with this macro needs manual attention */
#define M2C_ERROR(msg) (0)

/*===========================================================================
 * Generic Placeholder Struct for Unknown Memory Regions
 *
 * m2c generates code like: var->unk0, var->unk4, var->unk10, etc.
 * This union allows those patterns to compile.
 *
 * Fields named unk_XX are treated as "unknown space" by m2c.
 *===========================================================================*/

/* Small placeholder union for struct fields (0x100 bytes) */
typedef union {
    u8  u8_arr[0x100];
    s8  s8_arr[0x100];
    u16 u16_arr[0x80];
    s16 s16_arr[0x80];
    u32 u32_arr[0x40];
    s32 s32_arr[0x40];
    char unk_0[0x100];     /* m2c recognizes unk_ prefix as unknown space */
} ET_Small;

/* Medium placeholder union for struct fields (0x400 bytes / 1KB) */
typedef union {
    u8  u8_arr[0x400];
    s8  s8_arr[0x400];
    u16 u16_arr[0x200];
    s16 s16_arr[0x200];
    u32 u32_arr[0x100];
    s32 s32_arr[0x100];
    char unk_0[0x400];
} ET_Medium;

/* Large placeholder union (0x1000 bytes / 4KB) - based on SOTN pattern */
typedef union {
    u8  u8_arr[0x1000];
    s8  s8_arr[0x1000];
    u16 u16_arr[0x800];
    s16 s16_arr[0x800];
    u32 u32_arr[0x400];
    s32 s32_arr[0x400];
    char unk_0[0x1000];
} ET_Large;

/* Extra-large placeholder (0x10000 bytes / 64KB) for game state structures */
typedef union {
    u8  u8_arr[0x10000];
    s8  s8_arr[0x10000];
    u16 u16_arr[0x8000];
    s16 s16_arr[0x8000];
    u32 u32_arr[0x4000];
    s32 s32_arr[0x4000];
    char unk_0[0x10000];
} ET_Huge;

/* Generic placeholder struct template (SOTN-style) */
typedef union {
    u8 u8[0x3C];
    s8 s8[0x3C];
    u16 u16[0x1E];
    s16 s16[0x1E];
    u32 u32[0xF];
    s32 s32[0xF];
} ET_Placeholder;

/*===========================================================================
 * Template struct for unknown memory at specific addresses
 *
 * Use this pattern when m2c accesses memory at specific addresses:
 *   *(type*)0x800XXXXX
 *
 * These can be replaced with proper globals as they are identified.
 *===========================================================================*/

typedef struct UnkStruct_Generic {
    /* 0x0000 */ char unk_0[0x10000];    /* Large unknown area */
} UnkStruct_Generic;

/*===========================================================================
 * Function Pointer Types
 *
 * Common callback signatures used in PS1 games.
 *===========================================================================*/

/* Basic function pointers */
typedef void (*VoidFunc)(void);
typedef s32  (*IntFunc)(void);
typedef void (*VoidFuncArg)(void*);
typedef s32  (*IntFuncArg)(void*);
typedef void (*VoidFuncInt)(s32);
typedef s32  (*IntFuncInt)(s32);

/* Function pointers with multiple args */
typedef void (*VoidFunc2)(s32, s32);
typedef void (*VoidFunc3)(s32, s32, s32);
typedef void (*VoidFunc4)(s32, s32, s32, s32);
typedef void (*VoidFunc5)(s32, s32, s32, s32, s32);
typedef void (*VoidFunc6)(s32, s32, s32, s32, s32, s32);
typedef void (*VoidFunc7)(s32, s32, s32, s32, s32, s32, s32);

/* Return int with multiple args */
typedef s32 (*IntFunc2)(s32, s32);
typedef s32 (*IntFunc3)(s32, s32, s32);
typedef s32 (*IntFunc4)(s32, s32, s32, s32);

/*===========================================================================
 * Memory Access Macros
 *
 * For reading/writing hardware registers and memory-mapped I/O.
 * Using volatile to prevent compiler optimization of hardware accesses.
 *===========================================================================*/

/* Direct memory access macros */
#define MEM8(addr)  (*(volatile u8*)(addr))
#define MEM16(addr) (*(volatile u16*)(addr))
#define MEM32(addr) (*(volatile u32*)(addr))

/* Signed versions */
#define MEMS8(addr)  (*(volatile s8*)(addr))
#define MEMS16(addr) (*(volatile s16*)(addr))
#define MEMS32(addr) (*(volatile s32*)(addr))

/* Pointer dereference macros (for casting) */
#define PTR8(addr)  ((volatile u8*)(addr))
#define PTR16(addr) ((volatile u16*)(addr))
#define PTR32(addr) ((volatile u32*)(addr))

/*===========================================================================
 * PS1 Hardware Addresses
 *
 * Common PlayStation 1 hardware register addresses.
 * These are unlikely to need decompilation but helpful for context.
 *===========================================================================*/

/* GPU registers */
#define GPU_DATA    0x1F801810
#define GPU_STATUS  0x1F801814
#define GPU_CONTROL 0x1F801814

/* SPU registers */
#define SPU_BASE    0x1F801C00

/* CD-ROM registers */
#define CDROM_BASE  0x1F801800

/* DMA registers */
#define DMA_BASE    0x1F801080

/* Timer registers */
#define TIMER_BASE  0x1F801100

/* Interrupt control */
#define I_STAT      0x1F801070
#define I_MASK      0x1F801074

/*===========================================================================
 * PS1 RAM Layout
 *
 * The PS1 has 2MB of main RAM at 0x80000000-0x801FFFFF.
 * This is also mirrored at 0x00000000 and 0xA0000000.
 *===========================================================================*/

/* RAM regions (for documentation) */
#define RAM_BASE        0x80000000
#define RAM_SIZE        0x00200000  /* 2MB */
#define RAM_END         0x801FFFFF

/* Scratchpad (1KB fast RAM) */
#define SCRATCHPAD_BASE 0x1F800000
#define SCRATCHPAD_SIZE 0x00000400  /* 1KB */

/*===========================================================================
 * NULL and common constants
 *===========================================================================*/

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/*===========================================================================
 * Common macros from common.h (duplicated for standalone use)
 *===========================================================================*/

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

/*===========================================================================
 * Utility macros for type punning (same as common.h)
 *===========================================================================*/

#ifndef LOBU
#define LOBU(x) (*(u8*)&(x))
#define LOH(x) (*(s16*)&(x))
#define HIH(x) (((s16*)&(x))[1])
#define LOHU(x) (*(u16*)&(x))
#define HIHU(x) (((u16*)&(x))[1])
#define LOW(x) (*(s32*)&(x))
#define LOWU(x) (*(u32*)&(x))
#endif

/*===========================================================================
 * Struct field access pattern helper
 *
 * When m2c generates code like ptr->unk123, this macro can help:
 *   STRUCT_FIELD(ptr, offset, type)
 *   Example: STRUCT_FIELD(ptr, 0x123, s32) instead of ptr->unk123
 *===========================================================================*/

#define STRUCT_FIELD(ptr, offset, type) (*(type*)((u8*)(ptr) + (offset)))

/*===========================================================================
 * Global declarations
 *===========================================================================*/

/* Define the M2C_ERROR placeholder instance */
#ifndef M2C_ERROR_IMPL
/* Declaration only - will be defined in a .c file */
#endif

#endif /* CONTEXT_H */
