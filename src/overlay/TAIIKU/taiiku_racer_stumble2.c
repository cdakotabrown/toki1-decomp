/* @SymbolStart: 0x801483C4 */
/* @SymbolSize: 148 */
/* @PostProc: scheduler_normalizer --block-aware */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x68 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x64 */
/* @PostProc: swap_instructions --offset1 0x5c --offset2 0x60 */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x7c */
/* @PostProc: patch_insn_reg_field 0x40:rt:a1:t3 0x44:rt:a2:t5 0x5c:rt:t5:t7 0x60:rt:t6:t8 0x64:rs:a1:t3 0x64:rt:t3:t4 0x68:rs:a2:t5 0x68:rt:t4:t6 0x78:rt:t3:t4 0x7c:rt:t4:t6 0x80:rt:t5:t7 0x88:rt:t6:t8 */
#include "types.h"

extern u8 class_w[];
extern u8 taiiku_spoon_bal[];
extern void device_cmd_send(s32 id);

void taiiku_racer_stumble2(s32 idx)
{
    u8 *rec;
    u8 *spr;
    s16 bx;
    s16 by;

    rec = taiiku_spoon_bal + idx * 28;
    spr = class_w + idx * 68;
    rec[12] = 2;
    bx = *(s16 *)(spr + 6566);
    by = *(s16 *)(spr + 6570);
    rec[13] = 0;
    *(s16 *)(spr + 6552) = (s16)(idx * 2 + 82);
    *(s16 *)(spr + 6838) = (s16)(bx - 23);
    *(s16 *)(spr + 6842) = (s16)(by + 50);
    *(s16 *)(rec + 2) = -23;
    *(s16 *)(rec + 6) = 50;
    if (idx == 0) {
        device_cmd_send(1283);
    }
}
