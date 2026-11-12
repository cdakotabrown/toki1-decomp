/* @CompileEnv: IDO52_SKIP_PASSES=eliminduct IDO52_PRECOLOR_WEB=a3:1 */
/* @SymbolSize: 416 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: patch_stack_complete --old 104 --new 96 */
/* @PostProc: remap_stack_slots --map '92:72' --keep-frame */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x70 */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x74 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x7c */
/* @PostProc: swap_instructions --offset1 0x7c --offset2 0x80 */
/* @PostProc: swap_instructions --offset1 0x170 --offset2 0x18c */
/* @PostProc: patch_insn_reg_field 0x8c:rd:t2:t1 0xb4:rd:t4:t2 0xb4:rt:t2:t1 0xb8:rs:t4:t2 0xb8:rt:t1:t3 0xbc:rt:t2:t1 0xc0:rs:t1:t3 0xc8:rt:t3:t4 0xd0:rd:t9:t6 0xd0:rs:t1:t3 0xd0:rt:t3:t4 0xd4:rt:t3:t4 0xdc:rt:t6:t9 0xe0:rs:t4:t2 0xe0:rt:t9:t6 0xe4:rs:t6:t9 */
/* Address: 0x80144B4C, Size: 416 bytes (104 instructions) */
/* $sp+72 (the target keeps another 12-byte local above it).  `@CompileFlags: */
#include "types.h"
#include "tokimeki/class_w.h"

typedef struct { s32 w[3]; } Blk12;

extern Blk12 taiiku_race_approach_tbl;
extern s16 taiiku_ninin_scrl_hi;
extern s32 taiiku_ninin_bonus[];
extern u8  class_w[];
extern u8  taiiku_ninin_run[];
extern u8  taiiku_ninin_rank[];

void taiiku_race_approach(void)
{
    Blk12 blk;
    s32 *tab;
    u8 *rec;
    u8 *ent;
    u8 *pp;
    s32 i;
    s32 k;
    s32 acc;
    s32 d;
    s32 base;
    s32 tv;
    s32 bv;

    blk = taiiku_race_approach_tbl;
    base = taiiku_ninin_scrl_hi;
    tab = taiiku_ninin_bonus;
    rec = class_w;
    ent = taiiku_ninin_run;
    acc = 0;
    for (i = 0; i != 3; i++) {
        *(s32 *)(ent + 56) = *(s32 *)(ent + 48) + *(s32 *)(ent + 56);
        d = *(s16 *)(ent + 58) - base;
        *(s16 *)(rec + 6634) = (s16)(d - 160);
        if (i + 54 != *(s16 *)(rec + 6620)) {
            tv = tab[i];
            if (tv > 0) {
                bv = blk.w[i];
                *(s32 *)(ent + 56) = *(s32 *)(ent + 56) + bv;
                d = *(s16 *)(ent + 58) - base;
                tab[i] = tv - bv;
            }
        }
        if (((CLASS_W *)class_w)[101].x.half[1] + acc + 8 < d - 96) {
            if (ent[37] == 0) {
                ((CLASS_W *)class_w)[102].frame = 50;
                ((CLASS_W *)class_w)[102].x.half[1] = (s16)(((CLASS_W *)class_w)[101].x.half[1] + 32);
                k = 0;
                while (taiiku_ninin_rank[k] != 0) {
                    k++;
                }
                taiiku_ninin_rank[k] = (u8)(i + 2);
                ent[37] = 1;
                d = *(s16 *)(ent + 58) - base;
            }
        }
        if (d >= 481) goto lbl_zero;
        if (d >= -160) goto lbl_164;
    lbl_zero:
        rec[6599] = 0;
        goto lbl_done;
    lbl_164:
        rec[6599] = 164;
    lbl_done:
        acc += 8;
        ent += 36;
        rec += 68;
    }
}
