/* Address: 0x801438D0, Size: 288 bytes (72 instructions) */
/* @CompileEnv: IDO52_LDA_NOINREG_CLASSES=4:1:1 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @StaticAddr: etc_roll_idx 0x80150164 */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x7c */
/* @PostProc: swap_instructions --offset1 0x7c --offset2 0x80 */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0x84 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x7c */
/* @PostProc: patch_insn_reg_field 0x64:rt:s0:s1 0x68:rt:s6:s5 0x6c:rt:s5:s4 0x74:rs:s5:s4 0x74:rt:s5:s4 0x78:rs:s6:s5 0x78:rt:s6:s5 0x7c:rs:s0:s1 0x7c:rt:s0:s1 0x80:rd:s1:s0 0x84:rt:s4:s7 0x88:rt:s7:s6 0x90:rs:s0:s1 0x94:rs:s1:s0 0x9c:rs:s0:s1 0xa0:rs:s0:s1 0xa4:rs:s1:s0 0xb4:rs:s5:s4 0xdc:rt:s7:s6 0xe0:rs:s1:s0 0xe0:rt:s1:s0 0xe4:rs:s0:s1 0xe4:rt:s0:s1 0xec:rs:s6:s5 0xf4:rs:s1:s0 0xf4:rt:s4:s7 */
#include "types.h"

extern void debug_hex_show();
extern void etc_roll_set();
static s32 etc_roll_idx;
extern u8 etc_roll_tbl[];
extern u8 class_w[];

void etc_roll_move(void)
{
    s32 s0;
    s32 s2;
    u8 *s1;
    s32 s7v;
    s32 s6v;
    s32 s3v;

    if (etc_roll_idx >= 285) {
        return;
    }
    debug_hex_show(etc_roll_idx, 40, 40);
    s1 = class_w;
    s7v = 7;
    s6v = 68;
    s3v = 4096;
    for (s0 = 0; s0 != s7v; s0++) {
        s2 = s0 + 96;
        *(s16 *)(s1 + 6570) = *(s16 *)(s1 + 6570) - 1;
        if (*(s16 *)(s1 + 6570) < -140) {
            etc_roll_set(s0, etc_roll_tbl[etc_roll_idx * 2], etc_roll_tbl[etc_roll_idx * 2 + 1]);
            etc_roll_idx++;
        }
        *(s16 *)&class_w[s2 * s6v + 26] = s3v;
        *(s16 *)&class_w[s2 * s6v + 28] = s3v;
        s1 += 68;
    }
}
