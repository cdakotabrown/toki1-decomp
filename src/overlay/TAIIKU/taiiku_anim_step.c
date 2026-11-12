/* @CompileFlags: -Wc,-pic0 */
/* Address: 0x80144CEC  Size: 304 bytes (76 insns) */
/* @PostProc: remap_stack_slots --from 8 --to 24 --from 12 --to 28 --from 52 --to 48 */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x60 */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x64 */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x68 */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x6c */
/* @PostProc: patch_insn_reg_field 0x34:rt:v0:v1 0x40:rt:t5:t4 0x44:rt:t1:t0 0x48:rs:t1:t0 0x48:rt:t1:t0 0x4c:rs:t5:t4 0x4c:rt:t5:t4 0x50:rs:v0:v1 0x50:rt:v0:v1 0x54:rt:s1:s0 0x58:rt:t0:s1 0x5c:rd:v1:v0 0x60:rt:s0:t5 0x64:rt:t4:t3 0x68:rt:t3:t2 0x6c:rt:t2:t1 0x74:rs:v0:v1 0x78:rs:v1:v0 0x84:rs:v1:v0 0x84:rt:t2:t1 0x88:rt:v1:v0 0x98:rs:t1:t0 0xb8:rs:t3:t2 0xc4:rd:t8:a2 0xc4:rt:v1:v0 0xd0:rs:t8:a2 0xd0:rt:v1:v0 0xd4:rt:t4:t3 0xe0:rt:t7:t8 0xe4:rt:t7:t8 0xec:rt:a1:t7 0xf4:rt:a1:t7 0xf8:rs:t5:t4 0x10c:rs:v0:v1 0x118:rs:s0:t5 0x120:rt:s1:s0 0x124:rs:v1:v0 0x124:rt:v1:v0 0x128:rs:v1:v0 0x128:rt:t0:s1 0x12c:rs:v0:v1 0x12c:rt:v0:v1 */
#include "types.h"

typedef struct { s32 w[3]; } Blk3;

extern s32 taiiku_ninin_pace_tbl[];
extern u8  taiiku_ninin_run[][36];
extern s16 taiiku_ninin_pfrm_tbl[];
extern u8  class_w[];

void taiiku_anim_step(void) {
    s32 buf[3];
    s32 v0;
    u8 *p;
    u8 *a0;
    s32 a1;
    s32 a2;
    s32 b36;

    *(Blk3 *)buf = *(Blk3 *)&taiiku_ninin_pace_tbl[0];
    p = class_w;
    for (v0 = 0; v0 != 3; v0++, p += 68) {
        if (v0 + 54 == *(s16 *)(p + 6620)) {
            continue;
        }
        a0 = taiiku_ninin_run[v0];
        if ((u32)buf[v0] < *(u32 *)(a0 + 40)) {
            if (*(s32 *)(a0 + 64) == 1) {
                a1 = a0[36];
                a2 = v0 * 8;
                a2 = a2 + v0;
                if (a1 >= 9) {
                    a0[36] = 2;
                    *(s32 *)(a0 + 40) = 0;
                } else {
                    a0[36] = (u8)(a1 + 1);
                    *(s32 *)(a0 + 40) = 0;
                }
                b36 = a0[36];
                *(s16 *)(p + 6620) = (s16)(taiiku_ninin_pfrm_tbl[b36] + a2 + 18);
            }
        }
        if (a0[36] == 6) {
            a0[38] = 1;
        }
    }
}
