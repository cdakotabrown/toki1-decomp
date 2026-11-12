/* @CompileEnv: IDO52_PRECOLOR_WEB=a0:9 IDO52_FORCE_WEB_COLOR=62:a0 */
/* @PostProc: patch_stack_complete --old 40 --new 64 */
/* @PostProc: remap_stack_slots --from 28 --to 44 --from 36 --to 52 --from 24 --to 56 */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x5c */
/* @PostProc: swap_instructions --offset1 0xa8 --offset2 0xb0 */
/* @PostProc: swap_instructions --offset1 0xa4 --offset2 0xa8 */
/* @PostProc: swap_instructions --offset1 0xbc --offset2 0xc0 */
/* @PostProc: swap_instructions --offset1 0xb8 --offset2 0xbc */
/* @PostProc: swap_instructions --offset1 0x114 --offset2 0x11c */
/* @PostProc: patch_insn_reg_field 0x54:rd:v1:v0 0x5c:rd:t0:v1 0x60:rs:v1:v0 0x70:rs:t0:v1 0x74:rs:t0:v1 0x74:rt:t0:v1 0x78:rs:t0:v1 0x7c:rs:v1:v0 0x7c:rt:v1:v0 0x80:rs:t0:v1 0xa0:rt:v1:v0 0xa4:rt:t0:v1 0xb4:rt:v1:v0 0xb8:rt:t0:v1 0xcc:rs:v1:v0 0xd0:rt:t1:t0 0xd4:rs:t0:v1 0xd4:rt:t2:t1 0xd8:rd:t3:t2 0xd8:rt:t2:t1 0xdc:rd:t4:t3 0xdc:rs:t1:t0 0xdc:rt:t3:t2 0xe0:rs:t4:t3 0xe0:rt:t5:t4 0xe4:rs:v1:v0 0xe4:rt:t5:t4 0xe8:rt:t6:t5 0xec:rd:t7:t6 0xf0:rd:t8:t7 0xf0:rs:t6:t5 0xf0:rt:t7:t6 0xf4:rs:t8:t7 0xf4:rt:t9:t8 0xf8:rs:v1:v0 0xf8:rt:t9:t8 0xfc:rt:t2:t9 0x104:rs:v1:v0 0x108:rs:v1:v0 0x10c:rs:v1:v0 0x110:rs:v1:v0 0x114:rs:v1:v0 0x114:rt:t2:t9 */
/* Address: 0x80141080 */
/* Size: 288 bytes (72 insns) */
#include "types.h"

extern u8 *taco_actor_rec;

extern u8 taco_effect_buf[];
extern u8 taco_effect2_buf[];

extern s32 sd_cmd_limit_send(s32, s32, s32, s32);

s32 taco_splash_spawn(s32 arg0, s32 arg1)
{
    s16 *rec;
    u8 *base;
    u8 *slot;
    s32 i;

    if (arg0 == 0) {
        rec = (s16 *)taco_actor_rec;
        base = taco_effect_buf;
    } else {
        rec = (s16 *)(taco_actor_rec + 1088);
        base = taco_effect2_buf;
    }

    if (!(arg1 & 0x20)) {
        goto ret0;
    }
    slot = base;
    i = 0;
loop:
    if ((slot[19] & 0x80) != 0) {
        i++;
        slot += 20;
        if (i != 16) {
            goto loop;
        }
    }
    if (i >= 16) {
        return -1;
    }
    if (arg0 == 0) {
        sd_cmd_limit_send(1, 1282, (s32)rec, arg0);
    }

    slot[19] = 152;
    *(s16 *)(slot + 0) = (rec[58] + ((i & 1) << 7)) - 64;
    *(s16 *)(slot + 2) = (rec[59] - (arg0 << 6)) + 64;
    *(s16 *)(slot + 4) = rec[60];
    *(s16 *)(slot + 8) = 0;
    *(s16 *)(slot + 10) = 0;
    *(s16 *)(slot + 12) = -512;
    slot[18] = 0;
    return 1;
ret0:
    return 0;
}
