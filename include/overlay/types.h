#ifndef OVERLAY_TYPES_H
#define OVERLAY_TYPES_H

#include "../types.h"

typedef void (*FuncPtr)(void);
typedef s32 (*FuncPtrS32)(void);
typedef void (*FuncPtrArg1)(s32);

typedef struct CharData {
    u8 data[56];
} CharData;

typedef struct CharacterStats {
    u8 data[8];
} CharacterStats;

typedef struct BatEntry {
    u8 data[16];
} BatEntry;

typedef struct Entry {
    s32 field_0;
    s32 field_4;
} Entry;

typedef struct DispatchTable8 {
    FuncPtr entries[8];
} DispatchTable8;

typedef struct DispatchTable9 {
    FuncPtr entries[9];
} DispatchTable9;

typedef struct DispatchTable13 {
    FuncPtr entries[13];
} DispatchTable13;

typedef struct DispatchTable14 {
    FuncPtr entries[14];
} DispatchTable14;

typedef struct DispatchTable16 {
    FuncPtr entries[16];
} DispatchTable16;

typedef struct DispatchTable23 {
    FuncPtr entries[23];
} DispatchTable23;

typedef struct DispatchTable25 {
    FuncPtr entries[25];
} DispatchTable25;

typedef struct DispatchTable43 {
    FuncPtr entries[43];
} DispatchTable43;

typedef struct DispatchTable44 {
    FuncPtr entries[44];
} DispatchTable44;

typedef struct DispatchTable58 {
    FuncPtr entries[58];
} DispatchTable58;

typedef struct Data8 {
    u8 data[8];
} Data8;

typedef struct Data44 {
    u8 data[44];
} Data44;

typedef struct DT31 {
    u8 data[31];
} DT31;

typedef struct FourWords {
    s32 words[4];
} FourWords;

typedef struct CopyStruct {
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_C;
} CopyStruct;

typedef struct ColorEntry {
    u8 r, g, b, a;
} ColorEntry;

#endif
