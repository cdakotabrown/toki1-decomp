/* @CompileEnv: IDO52_FORCE_WEB_COLOR=16:v1,183:v0 */
/* Size: 480 bytes */
/* @SymbolSize: 480 */
#include "types.h"

typedef struct {
    u8 pad0[6];
    u16 h6;
    s32 w8;
    s32 wC;
    s32 w10;
    u8 pad14[12];
} TaiikuHdr;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
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

TaiikuHdr taiiku_spoon_scrv = {0};
TaiikuSlot taiiku_spoon_cpu[3] = {0};
TaiikuEnt taiiku_spoon_bal[4] = {0};

void taiiku_count_inc(void)
{
    TaiikuSlot *p;
    s32 i;

    taiiku_spoon_scrv.w10++;
    taiiku_spoon_scrv.wC++;
    taiiku_spoon_scrv.w8++;
    taiiku_spoon_scrv.h6++;

    for (p = taiiku_spoon_cpu; p < &taiiku_spoon_cpu[3]; p++) {
        p->unkC++;
        p->unk4++;
    }

    for (i = 0; i < 4; i++) {
        taiiku_spoon_bal[i].h18++;
        taiiku_spoon_bal[i].h14++;
        taiiku_spoon_bal[i].h16++;
        taiiku_spoon_bal[i].bD++;
        taiiku_spoon_bal[i].bF++;
    }
}
