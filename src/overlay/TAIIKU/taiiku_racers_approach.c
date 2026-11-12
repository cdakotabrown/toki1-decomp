/* @CompileEnv: IDO52_UGEN_TEMP_REG=17:t5 */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x30 */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x34 */
/* @PostProc: swap_instructions --offset1 0x2c --offset2 0x34 */
/* @PostProc: swap_instructions --offset1 0x100 --offset2 0x108 */
/* Address: 0x80137324, Size: 260 bytes (65 instructions) */
#include "types.h"
#include "tokimeki/class_w.h"

extern s16 taiiku_kari_scrl_hi;
extern u8  taiiku_kari_rank[];
extern u8  class_w[];
extern u8  taiiku_kari_run[];

void taiiku_racers_approach(void)
{
    u8 *ent;
    u8 *rec;
    s32 i;
    s32 k;
    s32 d;
    s32 base;
    s32 f;
    s32 acc;

    i = 0;
    ent = taiiku_kari_run;
    acc = 0;
    base = taiiku_kari_scrl_hi;
    do {
        *(s32 *)(ent + 68) += *(s32 *)(ent + 60);
        if (((CLASS_W *)class_w)[111].x.half[1] + acc + 8 < (d = *(s16 *)(ent + 70) - base) - 96) {
            f = ent[42];
            if (f == 0) {
                ((CLASS_W *)class_w)[112].frame = 141;
                if (f == 0) {
                    k = 0;
                    while (taiiku_kari_rank[k] != 0) {
                        k++;
                    }
                    taiiku_kari_rank[k] = (u8)(i + 2);
                }
                ent[42] = 1;
                d = *(s16 *)(ent + 70) - base;
            }
        }
        acc += 8;
        rec = class_w + i * 68;
        *(s16 *)(rec + 6634) = (s16)d - 160;
        if (d >= 481) goto lbl_zero;
        if (d >= -160) goto lbl_164;
    lbl_zero:
        rec[6599] = 0;
        goto lbl_done;
    lbl_164:
        rec[6599] = 164;
    lbl_done:
        i++;
        ent += 36;
    } while (i != 3);
}
