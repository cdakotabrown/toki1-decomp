/* @SymbolStart: 0x80146240 */
/* @SymbolSize: 660 */
/* Address: 0x8014626C (true entry 0x80146240, which func_801457B0 `jal`s) */
/* Size: 660 bytes (165 insns) including the hoisted head block */
#include "types.h"

typedef struct {
    u8 b0;
    u8 b1;
    s16 h2;
    s32 w4;
    s32 w8;
    s32 wC;
    s32 w10;
    s32 w14;
    s16 h18;
    s16 h1A;
} TaiikuHdr;

typedef struct {
    u8  b00;
    u8  b01;
    u8  pad02[2];
    s32 w04;
    u8  pad08[4];
    s32 w12;
    u8  pad16[4];
    s32 w20;
    s16 h24;
    s16 h26;
} TaiikuSlot;

typedef struct {
    s16 h0;
    s16 h2;
    s16 h4;
    s16 h6;
    s32 w8;
    u8 bC;
    u8 bD;
    u8 bE;
    u8 bF;
    s32 w10;
    u16 h14;
    u16 h16;
    u16 h18;
    u16 h1A;
} TaiikuEnt;

u8 taiiku_spoon_cue[4] = {0};
u8 taiiku_spoon_btn = 0;
u8 taiiku_spoon_pos[8] = {0};
s32 taiiku_spoon_gain = 0;
s32 taiiku_spoon_vmax = 0;
u8 taiiku_spoon_rank[4] = {0};
s32 taiiku_spoon_scrl = 0;
s32 taiiku_spoon_scrv = 0;
TaiikuHdr taiiku_spoon_pl = {0};
TaiikuSlot taiiku_spoon_cpu[3] = {0};
TaiikuEnt taiiku_spoon_bal[4] = {0};

extern volatile s16 para_un;
extern s32 vblnk_counter;

extern void srand(s32);

void taiiku_race_init4(void)
{
    TaiikuSlot *p;
    s32 i;
    s32 level;

    taiiku_spoon_pl.b0 = 0;
    taiiku_spoon_pl.w8 = 0;
    taiiku_spoon_pl.wC = 0;
    taiiku_spoon_pl.w4 = 0;
    taiiku_spoon_pl.h18 = 0;
    taiiku_spoon_pl.w14 = 0;
    taiiku_spoon_pl.b1 = 0;
    taiiku_spoon_pl.h2 = 0;

    p = &taiiku_spoon_cpu[0];
    i = 8;
    do {
        p->h26 = i;
        i += 8;
        p++;
        p[-1].w12 = 0;
        p[-1].w04 = 0;
        p[-1].b00 = 0;
        p[-1].w20 = 0;
        p[-1].h24 = 0;
        p[-1].b01 = 0;
    } while (i < 32);

    for (i = 0; i < 4; i++) {
        taiiku_spoon_rank[i] = 0;
        taiiku_spoon_bal[i].h18 = 0;
        taiiku_spoon_bal[i].h2 = 55;
        taiiku_spoon_bal[i].h6 = 25;
        taiiku_spoon_bal[i].w8 = 0;
        taiiku_spoon_bal[i].h14 = 0;
        taiiku_spoon_bal[i].h16 = 0;
        taiiku_spoon_bal[i].bC = 0;
        taiiku_spoon_bal[i].bD = 0;
        taiiku_spoon_bal[i].bE = 0;
        taiiku_spoon_bal[i].bF = 0;
    }

    taiiku_spoon_pl.h1A = -160;
    taiiku_spoon_scrv = 0;
    taiiku_spoon_scrl = 0;
    level = para_un;
    taiiku_spoon_btn = 0;

    if (level >= 160) {
        taiiku_spoon_gain = 640;
        taiiku_spoon_vmax = 0x20000;
    } else if (level >= 125) {
        taiiku_spoon_gain = 544;
        taiiku_spoon_vmax = 0x1E000;
    } else if (level >= 100) {
        taiiku_spoon_gain = 448;
        taiiku_spoon_vmax = 0x1C000;
    } else if (level >= 75) {
        taiiku_spoon_gain = 352;
        taiiku_spoon_vmax = 0x1A000;
    } else {
        taiiku_spoon_gain = 256;
        taiiku_spoon_vmax = 0x18000;
    }

    srand(vblnk_counter);
}
