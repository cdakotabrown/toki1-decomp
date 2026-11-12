/**
 * m2c_context.h - Preprocessed M2C Context File for Tokimeki Memorial
 *
 * This file is preprocessed and ready for m2c's --context flag.
 * It contains NO preprocessor directives (no #ifndef, #define, #include).
 *
 * Use: python3 m2c.py --context include/m2c_context.h ...
 */

/* Basic PSX types */
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

/* M2C Placeholder Types - recognized by m2c for unknown struct fields */
typedef u8  UNK_TYPE;
typedef u8  UNK_TYPE1;
typedef u16 UNK_TYPE2;
typedef u32 UNK_TYPE4;
typedef u64 UNK_TYPE8;

typedef s8  M2C_UNK1;
typedef s16 M2C_UNK2;
typedef s32 M2C_UNK4;
typedef s64 M2C_UNK8;

typedef void* UNK_PTR;
typedef void* M2C_UNK_PTR;

/* Generic placeholder struct for unknown memory (SOTN-style) */
typedef union {
    u8 u8_arr[0x3C];
    s8 s8_arr[0x3C];
    u16 u16_arr[0x1E];
    s16 s16_arr[0x1E];
    u32 u32_arr[0xF];
    s32 s32_arr[0xF];
} ET_Placeholder;

/* Large placeholder for big structs */
typedef union {
    u8 u8_arr[0x10000];
    s8 s8_arr[0x10000];
    u16 u16_arr[0x8000];
    s16 s16_arr[0x8000];
    u32 u32_arr[0x4000];
    s32 s32_arr[0x4000];
    char unk_0[0x10000];
} ET_Large;

/* M2C unknown type - used for parameters with unknown types */
typedef s32 M2C_UNK;
typedef s8  M2C_UNK8;
typedef s16 M2C_UNK16;
typedef s32 M2C_UNK32;
typedef s64 M2C_UNK64;

/* Function pointer types */
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
typedef s32 (*IntFunc2)(s32, s32);
typedef s32 (*IntFunc3)(s32, s32, s32);
