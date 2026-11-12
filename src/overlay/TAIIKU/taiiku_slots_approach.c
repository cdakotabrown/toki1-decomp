/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x30 */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x2c */
/* Address: 0x801473AC, Size: 264 bytes (66 instructions) */
#include "types.h"
#include "tokimeki/class_w.h"

extern s16 taiiku_spoon_scrl_hi;
extern u8  taiiku_spoon_rank[];
extern u8  class_w[];
extern u8  taiiku_spoon_cpu[];

void taiiku_slots_approach(void)
{
    u8 *rec;
    u8 *ent;
    s32 i;
    s32 k;
    s32 d;
    s32 base;

    i = 0;
    ent = taiiku_spoon_cpu;
    rec = class_w;
    base = taiiku_spoon_scrl_hi;
    do {
        *(s32 *)(ent + 24) += *(s32 *)(ent + 20);
        d = *(s16 *)(ent + 26) - base;
        *(s16 *)(rec + 6634) = (s16)(d - 160);
        if (((CLASS_W *)class_w)[104].x.half[1] + i * 8 + 8 < *(s16 *)(rec + 6634) + 64) {
            if (ent[1] == 0) {
                ((CLASS_W *)class_w)[105].frame = 79;
                k = 0;
                while (taiiku_spoon_rank[k] != 0) {
                    k++;
                }
                taiiku_spoon_rank[k] = (u8)(i + 2);
                ent[1] = 1;
                d = *(s16 *)(ent + 26) - base;
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
        i++;
        ent += 28;
        rec += 68;
    } while (i != 3);
}
