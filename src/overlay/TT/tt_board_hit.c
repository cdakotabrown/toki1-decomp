/* @PostProc: patch_stack_complete --old 88 --new 152 */
/* @PostProc: remap_stack_slots --from 84 --to 104 --from 86 --to 106 --from 80 --to 84 --from 68 --to 88 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x30 */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x168 --offset2 0x170 */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x5c */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x68 */
/* @PostProc: swap_instructions --offset1 0x5c --offset2 0x60 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x64 */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x68 */
/* @PostProc: patch_insn_reg_field 0x20:rt:s4:s2 0x34:rt:s3:s4 0x38:rt:s6:s7 0x3c:rs:s4:s2 0x3c:rt:s4:s2 0x4c:rs:s6:s7 0x4c:rt:s6:s7 0x50:rs:s3:s4 0x50:rt:s3:s4 0x54:rt:s2:s3 0x58:rt:fp:s5 0x5c:rt:s5:s6 0x60:rt:s7:fp 0x8c:rs:s4:s2 0x9c:rs:s4:s2 0xa0:rs:s4:s2 0xb0:rs:s2:s3 0xb8:rs:s3:s4 0xd0:rs:s4:s2 0xd4:rs:s4:s2 0xd8:rs:s4:s2 0xdc:rs:s4:s2 0xe0:rs:s4:s2 0xe4:rs:s4:s2 0xe8:rs:s4:s2 0x110:rs:s5:s6 0x114:rs:s6:s7 0x118:rs:s2:s3 0x120:rs:s3:s4 0x12c:rt:s7:fp 0x14c:rt:fp:s5 */
/* tt_board_hit - TT overlay, Address: 0x801374C0, Size: 404 bytes (101 insns) */
#include "types.h"
#include "tokimeki/tt_box.h"

extern u8 *tt_player_wk;
extern u8 *tt_player_shot_buf;
extern u8 tt_hit_box_tbl[];
extern TT_BOX tt_target_box_tbl[];

extern s32 tt_box_overlap_check(u8 *a0, u8 *a1, s16 *a2, u8 *a3);
extern void tt_widget_pair_init(void);

void tt_board_hit(u8 *arg0)
{
    s16 buf[2];
    u8 *p;
    u8 *s0;
    u8 *s2;
    s32 i;
    s32 j;

    s2 = tt_player_wk;
    p = arg0;
    i = 0;
    do {
        buf[0] = *(s16 *)(p + 30);
        buf[1] = *(s16 *)(p + 158);
        if (s2[82] == 0) {
            if (s2[0] != 0) {
                if (tt_box_overlap_check(s2 + 20, tt_hit_box_tbl, buf,
                                  ((u8 *)&tt_target_box_tbl[2].x)) != 0) {
                    tt_widget_pair_init();
                    *(s16 *)(s2 + 4) = 96;
                    *(s16 *)(s2 + 10) = 0;
                    *(s16 *)(s2 + 8) = 0;
                    *(s16 *)(s2 + 6) = 0;
                    s2[15] = 0;
                    s2[14] = 0;
                    s2[13] = 0;
                }
            }
        }
        s0 = tt_player_shot_buf;
        for (j = 0; j != 12; j++) {
            if (s0[0] != 0) {
                if (*(u16 *)(s0 + 4) == 64) {
                    if (tt_box_overlap_check(s0 + 20, ((u8 *)&((TT_BOX *)tt_hit_box_tbl)[1].x), buf,
                                      ((u8 *)&tt_target_box_tbl[2].x)) != 0) {
                        *(s16 *)(s0 + 4) = 128;
                        *(s16 *)(s0 + 10) = 0;
                        *(s16 *)(s0 + 8) = 0;
                        *(s16 *)(s0 + 6) = 0;
                        s0[15] = 0;
                        s0[14] = 0;
                        s0[13] = 0;
                    }
                }
            }
            s0 += 84;
        }
        i += 4;
        p += 4;
    } while (i != 32);
}
