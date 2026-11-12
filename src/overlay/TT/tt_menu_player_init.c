/* @SymbolStart: 0x80149D54 */
/* @SymbolSize: 224 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x2c --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x34 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0xc4 --offset2 0xc8 */
/* @PostProc: patch_insn_reg_field 0x24:rt:t2:t3 0x2c:rt:a2:t8 0x30:rt:t8:t9 0x34:rt:t9:t0 0x38:rt:t0:t1 0x3c:rt:t1:t2 0x40:rs:t2:t3 0x40:rt:t2:t3 0x44:rt:t3:t4 0x48:rt:t4:t5 0x4c:rt:t5:t7 0x70:rt:a2:t8 0x74:rt:t8:t9 0x78:rt:t9:t0 0x7c:rt:t0:t1 0x80:rt:t1:t2 0x8c:rt:t2:t3 0x94:rt:t3:t4 0x98:rt:t4:t5 0x9c:rt:t5:t7 0xc8:rt:a2:a0 0xd8:rt:a2:a0 */
/* Address: 0x80149D54 (cross-prologue head absorbed), Size: 224 bytes (56 insns) */
#include "types.h"

extern void anim_cel_step(u8 *rec);

extern u8 *tt_player_wk;
extern u8 *tt_cel_buf;
extern u8 tt_anim_tbl[];

void tt_menu_player_init(void)
{
    u8 *p;
    u8 *q;
    s32 i;

    p = tt_player_wk;

    p[0] = 1;
    p[12] = 0;
    *(s16 *)(p + 4) = 0;
    p[13] = 0;
    p[14] = 0;
    p[15] = 0;
    *(s16 *)(p + 6) = 0;
    *(s16 *)(p + 8) = 0;
    *(s16 *)(p + 10) = 0;
    *(s16 *)(p + 16) = 2;
    *(s32 *)(p + 32) = 0x00480000;
    *(s32 *)(p + 36) = 0x00840000;
    *(s16 *)(p + 20) = 72;
    *(s16 *)(p + 22) = 156;
    *(s32 *)(p + 40) = 0;
    *(s32 *)(p + 44) = 0;
    *(s32 *)(p + 56) = (s32)tt_anim_tbl;
    p[60] = 0;
    p[61] = 11;
    p[62] = 4;
    p[64] = 96;
    p[72] = 0;

    anim_cel_step(p + 56);

    *(s16 *)(p + 80) = 0;
    *(s16 *)(p + 106) = 0;

    q = tt_cel_buf;
    i = 0;
    do {
        i += 1;
        q += 96;
        q[-96] = 0;
        *(s32 *)(q - 40) = 0;
    } while (i != 2);
}
