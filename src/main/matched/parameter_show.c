/* Function: parameter_show */
/* Address: 0x8006486C  (registered address corrected 2026-09-17; see below) */
/* Size: 608 bytes (152 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @SymbolStart: 0x8006486C */
/* @SymbolSize: 608 */
/* @CompileFlags: -Wo,-loopunroll,0 -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 64 --new 72 */
/* @PostProc: remap_stack_slots --map 56:60 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x174 --offset2 0x178 */
/* @PostProc: swap_branch_operands --offsets 0x90,0x184,0xf4,0x1e4 */
/* @PostProc: patch_insn_reg_field 0x6c:rt:a3:a2 0x74:rs:a3:a2 0x74:rt:a3:a2 0x7c:rt:a2:a3 0xa0:rt:t3:t5 0xc8:rt:t5:t3 0xd0:rs:t3:t5 0xd0:rd:t4:t6 0xd4:rt:t4:t6 0xdc:rs:t5:t3 */
/* @PostProc: patch_insn_reg_field 0xdc:rd:t6:t4 0xe8:rt:t6:t4 0x104:rt:t9:t2 0x12c:rt:t2:t9 0x134:rs:t9:t2 0x134:rd:t1:t3 0x138:rt:t1:t3 0x140:rs:t2:t9 */
/* @PostProc: patch_insn_reg_field 0x140:rd:t3:t1 0x14c:rt:t3:t1 0x150:rt:a2:a3 0x160:rt:a3:a2 0x170:rt:a3:a2 0x174:rs:a3:a2 0x174:rt:a3:a2 0x23c:rt:a2:a3 */
/* @PostProc: patch_insn_reg_field 0x248:rt:a3:a2 */
#include "types.h"

extern u8 bustup_sw;
extern u8 cw32_disp;
extern u8 cw32_rec;
extern u8 cw56_rec;
extern u8 cw59_rec;

extern void _sprite_set_box_shade(s32, s32, s32, s32, s32, s32, s32);
extern void dtd_on(s32);

void parameter_show(void) {
    s32 t0_param = bustup_sw;
    s32 t0;
    u8 *v0;
    s32 v1;

    if (t0_param != 0) {
        t0 = 4;
    } else {
        t0 = 8;
    }

    if (!(cw32_disp & 0x80)) {
        return;
    }

    _sprite_set_box_shade(-132, -97, 264, 42, 12, 0x30E00, 0);
    dtd_on(12);

    v0 = &cw32_rec;
    do {
        v1 = *(s16 *)(v0 + 46);
        if (v1 == 1) {
            v1 = 4096 / t0;
            *(s16 *)(v0 + 26) -= v1;
            *(s16 *)(v0 + 8) += v1;
            if (!(v1 < *(s16 *)(v0 + 26))) {
                *(s16 *)(v0 + 46) = 2;
            }
        } else if (v1 == 2) {
            v1 = 4096 / t0;
            *(s16 *)(v0 + 26) += v1;
            *(s16 *)(v0 + 8) -= v1;
            if (*(s16 *)(v0 + 26) >= 4096) {
                *(s16 *)(v0 + 26) = 4096;
                *(s16 *)(v0 + 46) = 0;
                *(s16 *)(v0 + 8) = 0;
            }
        }
        v0 += 68;
    } while ((u32)v0 < (u32)&cw56_rec);

    v0 = &cw56_rec;
    do {
        v1 = *(s16 *)(v0 + 46);
        if (v1 == 1) {
            v1 = 4096 / t0;
            *(s16 *)(v0 + 28) -= v1;
            if (!(v1 < *(s16 *)(v0 + 28))) {
                *(s16 *)(v0 + 46) = 2;
            }
        } else if (v1 == 2) {
            v1 = 4096 / t0;
            *(s16 *)(v0 + 28) += v1;
            if (*(s16 *)(v0 + 28) >= 4096) {
                *(s16 *)(v0 + 28) = 4096;
                *(s16 *)(v0 + 46) = 0;
            }
        }
        v0 += 68;
    } while (v0 != &cw59_rec);
}
