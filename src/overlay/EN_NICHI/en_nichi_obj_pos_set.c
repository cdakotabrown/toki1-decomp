/* en_nichi_obj_pos_set - EN_NICHI overlay, Address: 0x8013466C, Size: 660 bytes */
/* @PostProc: patch_insn_reg_field 0x27c:rt:v1:t4 0x280:rt:v1:t4 0x284:rs:v1:t4 0x28c:rt:t4:t5 0x290:rt:t4:t5 */
#include "types.h"

typedef struct {
    s32 kind;
    s32 u04;
    s32 u08;
    s32 u0C;
    s32 u10;
    s16 xl;
    s16 xh;
    s16 yl;
    s16 yh;
    s32 ang;
    s32 spd;
    s32 tmr;
    s32 sel;
    s32 inp;
} EnNichiObj;

extern EnNichiObj en_nichi_obj_wk[];

extern s32 rand(void);

void en_nichi_obj_pos_set(s32 idx, s32 mode)
{
    s32 v;
    s32 t;

    if (mode == 1) {
        switch (rand() % 4) {
        case 0:
            en_nichi_obj_wk[idx].xh = (s16)(rand() % 320 - 160);
            en_nichi_obj_wk[idx].yh = -150;
            en_nichi_obj_wk[idx].ang = rand() % 1024 + 512;
            break;
        case 1:
            en_nichi_obj_wk[idx].xh = 190;
            en_nichi_obj_wk[idx].yh = (s16)(rand() % 240 - 120);
            en_nichi_obj_wk[idx].ang = rand() % 1024 + 1536;
            break;
        case 2:
            en_nichi_obj_wk[idx].xh = (s16)(rand() % 320 - 160);
            en_nichi_obj_wk[idx].yh = 150;
            en_nichi_obj_wk[idx].ang = rand() % 1024 + 2560;
            break;
        case 3:
            en_nichi_obj_wk[idx].xh = -190;
            en_nichi_obj_wk[idx].yh = (s16)(rand() % 240 - 120);
            en_nichi_obj_wk[idx].ang = rand() % 1024 + 3584;
            break;
        }
    } else {
        en_nichi_obj_wk[idx].xh = (s16)(rand() % 320 - 160);
        en_nichi_obj_wk[idx].yh = (s16)(rand() % 240 - 120);
        en_nichi_obj_wk[idx].ang = rand() % 4096;
    }

    v = en_nichi_obj_wk[idx].ang;
    en_nichi_obj_wk[idx].tmr = 1;
    if (v >= 4096) {
        t = v - 4096;
        en_nichi_obj_wk[idx].ang = t;
        v = t;
    }
    if (v < 0) {
        en_nichi_obj_wk[idx].ang = v + 4096;
    }
}
