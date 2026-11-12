/* en_nichi_obj_spawn - EN_NICHI overlay, Address: 0x801343D4, Size: 664 bytes */
/* @PostProc: patch_stack_complete --old 96 --new 104 */
/* @PostProc: remap_stack_slots --map 76:96,80:100 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x54 */
#include "types.h"

typedef struct {
    s32 kind;
    s32 u04;
    s32 u08;
    s32 u0C;
    s32 u10;
    s32 x;
    s32 y;
    s32 ang;
    s32 spd;
    s32 tmr;
    s32 sel;
    s32 inp;
} EnNichiObj;

extern EnNichiObj en_nichi_obj_wk[];

extern void en_nichi_obj_count(s32 *, s32 *);
extern s32 rand(void);
extern void en_nichi_obj_pos_set(s32, s32);

void en_nichi_obj_spawn(s32 arg)
{
    s32 i;
    s32 r;
    s32 m;
    s32 nMajor;
    s32 nMinor;

    en_nichi_obj_count(&nMajor, &nMinor);

    i = 0;

    do {
        if (en_nichi_obj_wk[i].kind == 0) {
            r = rand();
            if (nMajor * 10 < r % 100) {
                r = rand();
                if (nMinor == 0 && r % 100 >= 61) {
                    en_nichi_obj_wk[i].kind = 4;
                    nMinor = nMinor + 1;
                } else {
                    r = rand();
                    en_nichi_obj_wk[i].kind = r % 3 + 1;
                    nMajor = nMajor + 1;
                }

                r = rand();
                m = r % 100;
                if (m < 20) {
                    en_nichi_obj_wk[i].u04 = 0;
                } else if (m < 60) {
                    en_nichi_obj_wk[i].u04 = 1;
                } else {
                    en_nichi_obj_wk[i].u04 = 2;
                }

                r = rand();
                m = r % 100;
                if (m < 20) {
                    en_nichi_obj_wk[i].u08 = 0;
                } else if (m < 60) {
                    en_nichi_obj_wk[i].u08 = 1;
                } else {
                    en_nichi_obj_wk[i].u08 = 2;
                }

                if (en_nichi_obj_wk[i].kind == 4) {
                    en_nichi_obj_wk[i].u04 = 0;
                }
                en_nichi_obj_wk[i].u0C = 0;
                en_nichi_obj_wk[i].u10 = 0;
                en_nichi_obj_pos_set(i, arg);
            }
        }
        i = i + 1;
    } while (i != 10);
}
