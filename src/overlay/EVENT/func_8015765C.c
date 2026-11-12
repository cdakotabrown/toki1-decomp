/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_COPYPROP_SITE=lod:1:deny,lod:2:deny,lod:3:deny,lod:4:deny,lod:5:deny,lod:6:deny,lod:7:deny,lod:8:deny,lod:9:deny,lod:10:deny,lod:11:deny,lod:12:deny,lod:19:deny,lod:20:deny,lod:21:deny,lod:22:deny,lod:23:deny,lod:24:deny IDO52_COPYPROP_VAR=-16:1 IDO52_FORCE_WEB_COLOR=6:t1,26:a3,32:t0,57:v1,106:v1,82:a3 IDO52_UGEN_DEST_TEMP=2 IDO52_AS1_NO_PASS1_PROP=10,12,14,16,19,22 IDO52_AS1_NO_COPY_PROP=1,6 */
/* @SymbolStart: 0x8015760C */
/* @SymbolSize: 572 */
/* Address: 0x8015760C (hoisted entry block; .sym draws the start at 0x8015765C) */
/* Size: 572 bytes (143 instructions) including the absorbed entry block */
#include "types.h"

typedef struct {
    s32 flags;
    s16 x;
    s16 y;
    s16 unk08;
    s16 unk0A;
    s16 unk0C;
    u8  unk0E;
    u8  unk0F;
    s16 unk10;
    s16 unk12;
    u8  r;
    u8  g;
    u8  b;
    u8  pad17;
    s16 unk18;
    s16 unk1A;
    s16 sx;
    s16 sy;
    s32 unk20;
} EventObj;

extern u8 D_80125864;
extern volatile u8 D_800E9E63;
extern u8 D_800E9E5F;
extern EventObj D_800EC190[];
extern u8 D_800E9620[];
extern s32 D_800B0860[];
extern s32 ev_flag04;
extern s32 ev_flag24;
extern s32 D_80094710;

extern void func_80012D64();
extern void func_80011DFC();

void func_8015765C()
{
    s32 x;
    s32 level;
    s32 mode;
    s32 a;
    s32 b;
    s32 c;
    short i;
    short j;
    short k;
    short m;
    s32 u;
    s32 w;

    x = D_80125864;
    level = x - 4;
    a = level;
    D_800EC190[0].g = a;
    D_800EC190[0].r = a;
    D_800EC190[1].g = a;
    D_800EC190[1].r = a;
    D_80125864 = level;
    b = a;
    D_800EC190[0].b = b;
    c = a;
    D_800EC190[1].b = c;

    D_800E9E63 = level; for (i = 0; i < 4; i++) {
        D_800E9620[i * 68 + 6535] = D_800E9E63;
    }
    level = D_800E9E63;
    a = level;
    for (j = 0; j < 6; j++) {
        D_800EC190[j + 4].b = a;
        D_800EC190[j + 4].g = a;
        D_800EC190[j + 4].r = a;
    }
    D_800E9E63 = level;
    if (level != 0) {
        return;
    }
    ev_flag24 = 0;
    mode = D_800E9E5F;
    ev_flag04 = 0;
    D_800E9E5F = mode & 0xFF7F;
    u = 128; D_800EC190[0].g = u;
    D_800EC190[0].r = u;
    w = 128; D_800EC190[1].g = w;
    D_800EC190[1].r = w;
    D_800EC190[0].flags |= 0x80000000;
    D_800EC190[1].flags |= 0x80000000;
    D_800EC190[0].b = u;
    D_800EC190[1].b = w;
    for (k = 0; k < 6; k++) {
        D_800EC190[k + 4].flags |= 0x80000000;
    }
    func_80012D64(D_80094710, 16, 1, 2, 0);
    for (m = 11; m < 16; m++) {
        D_800B0860[m + 1258] = 119;
        D_800B0860[m + 1290] = 119;
    }
    for (m = 21; m < 26; m++) {
        D_800B0860[m + 1258] = 119;
        D_800B0860[m + 1290] = 119;
    }
    func_80011DFC();
}
