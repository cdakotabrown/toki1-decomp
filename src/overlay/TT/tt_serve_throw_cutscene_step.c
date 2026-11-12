/* tt_serve_throw_cutscene_step - TT overlay, Address: 0x801392A0, Size: 1060 bytes (265 insns) */
/* - `@CompileEnv: IDO52_NO_LOOP_TEST_REPLACEMENT=bb:5,37` (Hook 20), NOT */
/* @SymbolSize: 1060 */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x5c */
/* @PostProc: swap_instructions --offset1 0x284 --offset2 0x288 */
/* @PostProc: swap_instructions --offset1 0x364 --offset2 0x368 */
/* @PostProc: swap_instructions --offset1 0x3d4 --offset2 0x3d8 */
/* @PostProc: patch_insn_reg_field 0x3c:rt:v0:a0 0x44:rs:v0:a0 0x54:rt:t0:a1 0x58:rs:t0:a1 0x58:rt:t0:a1 0x60:rt:a3:t0 0x64:rt:a1:a3 0x70:rt:v0:a0 0x74:rt:a0:v0 0x7c:rs:v0:a0 0x7c:rt:t0:a1 0x80:rs:v0:a0 0x84:rs:v0:a0 0x84:rt:a1:a3 0x88:rs:v0:a0 0x8c:rs:v0:a0 0x94:rs:a0:v0 0x9c:rs:v0:a0 0x9c:rt:v0:a0 0xa0:rs:a0:v0 0xa0:rt:a0:v0 0xa8:rt:a3:t0 0xb8:rt:v1:s0 0xc0:rt:v1:s0 0xd8:rt:v0:a0 0x12c:rt:a1:a0 0x158:rt:a1:a0 0x1cc:rt:a1:a0 0x1dc:rt:a0:s0 0x210:rt:a0:s0 0x218:rt:a0:s0 0x224:rs:t3:a2 0x224:rt:a2:t3 0x22c:rt:a0:s0 0x284:rt:t9:t1 0x288:rt:t1:t9 0x28c:rt:a0:s0 0x2a0:rs:t3:a2 0x2a0:rt:a2:t3 0x2a8:rt:a0:s0 0x2b8:rt:t6:t7 0x2bc:rt:t7:t6 0x2d4:rt:t8:t3 0x2d8:rt:t3:t8 0x2f0:rt:t7:t1 0x2f4:rt:t1:t7 0x30c:rt:t4:t3 0x310:rt:t3:t4 0x338:rs:a1:a0 0x360:rt:a1:a0 0x364:rt:t7:t6 0x368:rt:t6:t7 0x3b4:rt:v0:a0 0x3bc:rs:v0:a0 0x3c0:rd:v1:s0 0x3c4:rt:v0:a0 0x3d0:rs:v0:a0 0x3d8:rt:a0:v1 0x3dc:rs:v1:s0 0x3dc:rt:v1:s0 0x3f0:rs:v1:s0 0x3f0:rt:a0:v1 0x3fc:rt:v0:a0 0x41c:rs:v0:a0 */
/* @CompileEnv: IDO52_NO_LOOP_TEST_REPLACEMENT=bb:5,37 */
#include "types.h"

extern u8 tt_puff_seq_tbl[];
extern u8 tt_anim_tbl[];
extern s16 tt_main80_scr[];

extern u8 *tt_boss_wk;

extern void tt_cmd_once(s32 id);
extern void tt_shape_clear(void);
extern void tt_seq_step(u8 *tbl, s32 idx);

void tt_serve_throw_cutscene_step(void)
{
    u8 *s;
    u8 *p;
    u8 *b;
    u8 *h;
    s32 i;
    s32 j;
    s32 k;
    s32 id;

    s = tt_boss_wk;
    switch ((u32)*(u16 *)(s + 6)) {
    case 0:
        tt_shape_clear();
        *(s16 *)(s + 868) = 0;
        *(s16 *)(s + 980) = 0;
        p = s + 320;
        b = s + 16;
        h = s + 32;
        i = 16;
        do {
            *(s32 *)(p + 56) = (s32)tt_anim_tbl;
            p[60] = 0;
            p[61] = 4;
            p[66] = 0;
            p[72] = 0;
            i += 1;
            b[784] = 0;
            p += 20;
            b += 1;
            h += 2;
            *(s16 *)(h + 678) = 8;
            *(s16 *)(h + 582) = 0;
            *(s16 *)(h + 630) = 0;
        } while (i < 19);
        for (k = 19; k < 24; k++) {
            s[k + 784] = 0;
        }
        *(s16 *)(s + 6) = 64;
        *(s16 *)(s + 8) = 0;
        break;
    case 64:
        if (tt_main80_scr[*(u16 *)(tt_boss_wk + 980)] != -1) {
            if (tt_main80_scr[*(u16 *)(tt_boss_wk + 980)] ==
                    *(u16 *)(tt_boss_wk + 868)) {
                tt_cmd_once(1281);
                id = tt_main80_scr[*(u16 *)(s + 980) + 4];
                s[id + 784] = 1;
                s[id * 20 + 64] = 128;
                *(s16 *)(s + id * 2 + 584) =
                    tt_main80_scr[*(u16 *)(s + 980) + 1];
                *(s16 *)(s + id * 2 + 632) =
                    tt_main80_scr[*(u16 *)(s + 980) + 2];
                *(s16 *)(s + id * 2 + 680) =
                    tt_main80_scr[*(u16 *)(s + 980) + 3];
                *(s16 *)(s + 980) = (s16)(*(u16 *)(s + 980) + 5);
            }
        }
        switch ((u32)*(u16 *)(s + 8)) {
        case 0:
            if (*(u16 *)(s + 868) == 120) {
                tt_cmd_once(1285);
                for (j = 0; j < 5; j++) {
                    *(s32 *)(s + j * 4 + 996) =
                        (s32)*(s16 *)(s + j * 2 + 636) << 16;
                }
                *(s32 *)(s + 1016) = 0xFFFE0000;
                *(s32 *)(s + 1020) = 4096;
                *(s16 *)(s + 982) = 0;
                *(s16 *)(s + 8) = 64;
            }
            break;
        case 64:
            for (j = 0; j < 5; j++) {
                *(s16 *)(s + j * 2 + 636) =
                    (s16)(*(s32 *)(s + j * 4 + 996) >> 16);
                *(s32 *)(s + j * 4 + 996) =
                    *(s32 *)(s + j * 4 + 996) + *(s32 *)(s + 1016);
            }
            *(s16 *)(s + 588) = (s16)(*(s16 *)(s + 588) - 1);
            *(s16 *)(s + 594) = (s16)(*(s16 *)(s + 594) + 1);
            if (*(u16 *)(s + 982) == 1U) {
                *(u16 *)(s + 982) = 0;
                *(s16 *)(s + 590) = (s16)(*(s16 *)(s + 590) - 1);
                *(s16 *)(s + 592) = (s16)(*(s16 *)(s + 592) + 1);
            } else {
                *(u16 *)(s + 982) = 1;
            }
            *(s32 *)(s + 1016) = *(s32 *)(s + 1016) + *(s32 *)(s + 1020);
            if (*(s16 *)(s + 644) >= 177) {
                *(s16 *)(s + 8) = 80;
            }
            break;
        case 80:
            break;
        }
        for (i = 16; i < 19; i++) {
            tt_seq_step(tt_puff_seq_tbl, i);
        }
        if (*(u16 *)(s + 868) == 220) {
            s[785] = 0;
        }
        if (*(u16 *)(s + 868) == 253) {
            for (k = 0; k < 24; k++) {
                s[k + 784] = 0;
            }
            *(s16 *)(s + 10) = 0;
            *(s16 *)(s + 8) = 0;
            *(s16 *)(s + 6) = 0;
            s[15] = 0;
            s[14] = 0;
            s[13] = 0;
            *(s16 *)(s + 4) = 160;
        }
        break;
    }
    *(s16 *)(s + 868) = (s16)(*(u16 *)(s + 868) + 1);
}
