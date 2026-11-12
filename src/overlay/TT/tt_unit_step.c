/* @PostProc: swap_instructions --offset1 0x7c --offset2 0x80 */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x7c */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0xd0 --offset2 0xd8 */
/* @PostProc: patch_insn_reg_field 0x18:rt:v1:t6 0x44:rs:v1:t6 0x44:rt:v0:t9 0x48:rs:v1:t6 0x4c:rs:v0:t9 0x4c:rt:v0:t0 0x50:rs:v0:t0 0x50:rt:t9:t1 0x54:rs:t9:t1 0x5c:rs:v1:t6 0x60:rs:v1:t6 0x68:rs:v1:t6 0x68:rt:v0:t0 0x6c:rs:v1:t6 0x94:rt:t0:t2 0x98:rd:v0:v1 0x9c:rs:t0:t2 0xa0:rt:v0:v1 0xc4:rt:t1:t3 0xc8:rd:t2:t4 0xc8:rt:v0:v1 0xd0:rt:t2:t4 0xd8:rt:t1:t3 0xe0:rs:v0:v1 0xe0:rt:v1:v0 0xe4:rd:v1:v0 0xe4:rs:v0:v1 0xe8:rd:t3:t5 0xe8:rt:v1:v0 0xec:rd:t4:t7 0xec:rs:t3:s3 0xec:rt:s3:t5 0xf0:rs:t4:t7 0xf0:rt:t5:t8 0xfc:rt:t5:t8 0x104:rs:v0:v1 0x104:rt:v1:v0 0x108:rd:v1:v0 0x108:rs:v0:v1 0x10c:rd:t7:t6 0x10c:rt:v1:v0 0x110:rd:t8:t9 0x110:rs:t7:s3 0x110:rt:s3:t6 0x114:rs:t8:t9 0x114:rt:t6:t0 0x120:rt:t6:t0 0x124:rt:t9:t1 0x12c:rs:t9:t1 0x12c:rt:t0:t2 0x130:rt:t0:t2 0x154:rt:t1:t3 0x158:rt:t1:t3 0x15c:rt:t2:t4 0x164:rt:t2:t4 0x168:rt:t3:t5 0x170:rt:t3:t5 0x198:rt:t4:t7 0x1a0:rs:t4:t7 0x1a8:rd:v0:v1 0x1b4:rt:v0:v1 0x1bc:rs:v0:v1 0x1bc:rt:v1:v0 0x1c0:rd:v1:v0 0x1c0:rs:v0:v1 0x1c4:rd:t5:t8 0x1c4:rt:v1:v0 0x1c8:rs:t5:t8 0x1e0:rt:t7:t6 0x1e8:rs:t7:t6 0x1f0:rt:t8:t9 0x1f8:rs:t8:t9 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* Address: 0x8013B4A0, Size: 552 bytes (138 insns) */
#include "types.h"

extern u8 tt_anim_tbl[];
extern s16 tt_unit_seq_tbl[];

extern u8 *tt_boss_wk;

extern void tt_fall_pose_set(void);
extern void tt_seq_step(s16 *p, s32 n);

void tt_unit_step(void)
{
    u8 *s;
    u8 *q;
    u8 *p;
    u8 *b;
    u8 *tbl;
    s32 n;
    s32 i;

    s = tt_boss_wk;
    q = s;
    n = *(volatile u16 *)(q + 940) + 1;
    q[142] = (u8)((u32)*(volatile u16 *)(q + 940) >> 1);
    *(volatile u16 *)(q + 940) = n;
    if ((u16)n >= 6U) {
        *(volatile u16 *)(q + 940) = 0;
    }

    tbl = tt_anim_tbl;
    i = 0;
    b = s;
    p = s;
    do {
        if (b[835] != 0) {
            *(s32 *)(p + 376) = (s32)tbl;
            p[380] = 0;
            p[381] = 4;
            p[384] = 128;
            p[382] = 52;
            p[386] = 0;
            p[392] = 0;
            b[800] = 1;
            *(s16 *)(s + 712 + 2 * i) = 8;
            *(s16 *)(s + 616 + 2 * i) =
                *(s16 *)(s + 584 + 2 * (i >= 2 ? i * 2 + 2 : i * 2));
            *(s16 *)(s + 664 + 2 * i) =
                *(s16 *)(s + 632 + 2 * (i >= 2 ? i * 2 + 2 : i * 2));
            if (i >= 2) {
                *(s16 *)(s + 664 + 2 * i) =
                    *(s16 *)(s + 664 + 2 * i) + 12;
            }

            *(s32 *)(p + 456) = (s32)tbl;
            p[460] = 0;
            p[461] = 4;
            p[464] = 128;
            p[462] = 52;
            p[466] = 0;
            p[472] = 0;
            b[804] = 1;
            *(s16 *)(s + 720 + 2 * i) = 8;
            *(s16 *)(s + 624 + 2 * i) = *(s16 *)(s + 584 + 2 * (i * 2 + 1));
            *(s16 *)(s + 672 + 2 * i) = *(s16 *)(s + 632 + 2 * (i * 2 + 1));
            b[835] = 0;
        }
        tt_seq_step(tt_unit_seq_tbl, i + 16);
        tt_seq_step(tt_unit_seq_tbl, i + 20);
        b += 1;
        if (p[382] == 56) {
            s[784 + (i >= 2 ? i * 2 + 2 : i * 2)] = 0;
            s[785 + 2 * i] = 0;
        }
        i += 1;
        p += 20;
    } while (i != 4);

    if (*(u16 *)(s + 4) != 32) {
        if (*(u16 *)(s + 782) == 0) {
            tt_fall_pose_set();
        }
    }
}
