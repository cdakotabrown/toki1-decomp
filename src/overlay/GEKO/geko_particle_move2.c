/* @SymbolStart: 0x8013FFA8 */
/* @SymbolSize: 300 */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x28 */
/* @PostProc: patch_insn_reg_field 0x10:rt:t5:t2 0x18:rt:t0:a0 0x1c:rs:t5:t2 0x1c:rt:t5:t2 0x24:rs:t0:a0 0x24:rt:t0:a0 0x2c:rt:t4:t1 0x30:rt:t3:t0 0x34:rt:t2:a3 0x38:rt:t1:a2 0x3c:rt:a3:a1 0x40:rt:t1:a2 0x48:rs:t5:t2 0x54:rt:a3:a1 0x5c:rs:t7:a0 0x5c:rt:t0:t7 0x60:rt:t2:a3 0x64:rt:t3:t0 0x68:rt:a3:a1 0x74:rs:t8:a0 0x74:rt:t0:t8 0x78:rt:a0:t5 0x7c:rt:a1:t6 0x80:rt:a2:t9 0x84:rd:t9:t7 0x84:rs:a0:t5 0x84:rt:a1:t6 0x88:rt:t9:t7 0x8c:rt:t6:t3 0x94:rd:t7:t4 0x94:rs:t6:t9 0x94:rt:a2:t3 0xa0:rt:t7:t4 0xac:rd:t6:t3 0xac:rt:t4:t1 0xb0:rt:t6:t3 0xb4:rd:t7:t4 0xb8:rt:t7:t4 0xc8:rd:a0:v0 0xd0:rt:a3:a1 0xd8:rd:t6:t3 0xdc:rt:t6:t3 0xe4:rd:t9:t6 0xe8:rd:v0:t7 0xe8:rs:t9:a0 0xe8:rt:t0:t6 0xec:rs:v0:t7 0x100:rt:a0:v0 0x10c:rs:a0:v0 0x11c:rt:t6:t2 0x120:rs:t6:t2 0x120:rt:t6:t2 0x12c:rs:t6:t2 0x12c:rt:t7:t2 0x130:rt:t7:t2 */
/* geko_particle_move2 -- GEKO overlay, Address: 0x8013FFAC (true start 0x8013FFA8: */
/* Size: 296 bytes (74 insns) + 4 hoisted = 300. */
#include "types.h"

extern s32 rand(void);

extern s32 geko_ptc_cnt;
extern u8 class_w[];

void geko_particle_move2(void)
{
    s32 lim;
    s32 v0;
    s32 v1;
    s32 m1;
    s32 vel;
    s32 acc;
    s32 pos;
    u8 *base;
    u8 *e;

    lim = geko_ptc_cnt;
    base = class_w;
    v1 = 0;
    m1 = -1;
    do {
        if ((u32)lim >= (u32)(v1 * 30)) {
            e = base + v1 * 68;
            *(u16 *)(e + 6778) = m1;
            *(u16 *)(e + 6782) = 1;
        }
        e = base + v1 * 68;
        v1 = v1 + 1;
        vel = *(s32 *)(e + 6772);
        acc = *(s32 *)(e + 6780);
        pos = *(s32 *)(e + 6768);
        *(s32 *)(e + 6772) = vel + acc;
        *(s32 *)(e + 6768) = pos + *(s32 *)(e + 6776);
        if (*(s16 *)(e + 6774) >= 161) {
            *(u8 *)(e + 6735) = (u8)(*(u8 *)(e + 6735) & -129);
        }
        v1 = (s16)v1;
    } while (v1 < 10);

    v0 = 0;
    v1 = 0;
    do {
        e = base + v1 * 68;
        v1 = v1 + 1;
        v1 = (s16)v1;
        if (*(u8 *)(e + 6735) & 0x80) {
            v0 = 1;
        }
    } while (v1 < 10);

    if (v0 == 0) {
        rand();
    }
    geko_ptc_cnt = geko_ptc_cnt + 1;
}
