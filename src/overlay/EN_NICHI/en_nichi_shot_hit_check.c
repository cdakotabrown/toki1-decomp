/* @CompileFlags: -Wc,-pic0 */
/* en_nichi_shot_hit_check - EN_NICHI overlay, Address: 0x8013835C, Size: 476 bytes */
/* @PostProc: patch_insn_reg_field 0x14c:rt:v0:t8 0x154:rd:t8:t7 0x154:rt:v0:t8 0x158:rd:t7:t6 0x158:rt:t8:t7 0x15c:rs:t7:t6 0x17c:rd:t6:t3 0x180:rs:t6:t3 0x188:rt:t2:t4 0x18c:rd:t3:t2 0x190:rd:t4:t1 0x198:rt:t3:t2 0x19c:rt:t4:t1 0x1a8:rt:t2:t4 0x1b4:rd:t5:t8 0x1bc:rt:t5:t8 0x1c0:rt:t1:t5 0x1c4:rt:t1:t5 */
#include "types.h"

typedef struct { s16 h[12]; } Tbl24;
typedef struct { s32 (*f[33])(s32, s32); } FnTbl132;

extern Tbl24 en_nichi_shot_hit_check_tbl0;
extern FnTbl132 en_nichi_shot_hit_check_tbl1;
extern s16 en_nichi_shot_hit_no[];
extern u8 class_w[][68];
extern volatile s16 cw98_x;

s32 en_nichi_shot_hit_check(s32 idx)
{
    s32 pad[3];
    Tbl24 tbl;
    FnTbl132 fns;
    s32 i;
    s32 hit;
    s32 thr;
    s32 v;
    s32 k;
    s32 c;

    tbl = en_nichi_shot_hit_check_tbl0;
    fns = en_nichi_shot_hit_check_tbl1;

    hit = 0;
    for (i = 0, thr = -120; i < 12; i++, thr += 8) {
        if (*(s16 *)(class_w[idx] + 6706) < thr) {
            v = tbl.h[i];
            if (*(s16 *)(class_w[idx] + 6702) < -v || v < cw98_x) {
                hit = 1;
            }
        }
    }

    i = 11;
    if (hit != 0) {
        en_nichi_shot_hit_no[idx] = 255;
        return 0;
    }

    do {
        k = *(s16 *)(class_w[i] + 7164);
        if (fns.f[k](idx, i) != 0) {
            c = *(s32 *)(class_w[i] + 7196);
            if ((c & 0x40000000) == 0) {
                *(s32 *)(class_w[i] + 7196) = c | 0x40000000;
                en_nichi_shot_hit_no[idx] = *(s16 *)(class_w[i] + 7164);
                return 1;
            }
        }
        i--;
    } while (i >= 0);

    en_nichi_shot_hit_no[idx] = 255;
    return 0;
}
