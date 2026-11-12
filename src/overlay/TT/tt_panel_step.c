/* tt_panel_step - TT overlay, Address: 0x8013F9D0, Size: 496 bytes (124 insns) */
/* @SymbolSize: 496 */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0xa0 */
/* @PostProc: swap_instructions --offset1 0x98 --offset2 0xa0 */
#include "types.h"

extern u8 *tt_boss_wk;
extern u8 tt_anim_tbl[];
extern u8 tt_panel_seq_tbl[];

extern void tt_cmd_once(s32 id);
extern void tt_seq_step(u8 *table, s32 count);

void tt_panel_step(void)
{
    u8 *p;
    s32 i;
    s32 v;
    s32 k;

    p = tt_boss_wk;
    switch ((u32)*(u16 *)(p + 6)) {
    case 0:
        if (*(s32 *)(tt_boss_wk + 36) >= 0x00600000) {
            *(s32 *)(p + 36) = 0x00600000;
            *(s16 *)(p + 6) = 32;
        } else {
            *(s32 *)(p + 36) = *(s32 *)(p + 36) + 0x8000;
        }
        break;
    case 32:
        for (i = 11; i < 13; i++) {
            *(s32 *)(p + 56 + 20 * i) = (s32)tt_anim_tbl;
            p[60 + 20 * i] = 0;
            p[61 + 20 * i] = 10;
            p[64 + 20 * i] = 96;
            p[67 + 20 * i] = 15;
            p[72 + 20 * i] = 0;
            p[784 + i] = 1;
            *(s16 *)(p + 680 + 2 * i) = 8;
            *(s16 *)(p + 584 + 2 * i) = 0;
            *(s16 *)(p + 632 + 2 * i) = -120;
        }
        p[282] = 24;
        p[302] = 12;
        *(s16 *)(p + 6) = 64;
        break;
    case 64:
        v = *(s16 *)(tt_boss_wk + 654);
        if (v >= -48) {
            tt_cmd_once(1282);
            *(s16 *)(p + 654) = -48;
            *(s16 *)(p + 656) = -48;
            *(s16 *)(p + 6) = 80;
        } else {
            if (v >= -64) {
                k = tt_boss_wk[242];
                if (k == 22) {
                    p[242] = 60;
                } else if (k == 23) {
                    p[242] = 61;
                }
            }
            *(s16 *)(p + 654) = *(s16 *)(p + 654) + 1;
            *(s16 *)(p + 656) = *(s16 *)(p + 654);
        }
        break;
    case 80:
        tt_seq_step(tt_panel_seq_tbl, 11);
        if ((p[284] & 1) != 0) {
            *(s16 *)(p + 4) = 128;
            *(s16 *)(p + 6) = 0;
        }
        break;
    }
}
