/* @SymbolStart: 0x80135E9C */
/* @SymbolSize: 268 */
/* @PostProc: scheduler_normalizer --block-aware */
/* @PostProc: patch_insn_reg_field 0x3c:rd:t8:t7 0x40:rd:t9:t8 0x44:rs:t9:t8 0x4c:rt:t8:t7 0x54:rd:t0:t9 0x5c:rd:t1:t0 0x5c:rs:t0:t9 0x60:rt:t1:t0 0x64:rt:t3:t1 0x6c:rt:t4:t3 0x74:rt:t3:t1 0x78:rs:t4:t3 0xb8:rt:a0:t4 0xcc:rs:a0:t4 0xcc:rt:a1:v0 0xd0:rs:a1:v0 0xd8:rt:a0:t4 0xdc:rs:a1:v0 0xe8:rt:v0:t6 0xf0:rs:v0:t6 0xf0:rt:v0:t7 0xf4:rs:v0:t7 0xf4:rt:a0:v0 0xf8:rs:a0:v0 0x100:rt:v0:t7 0x104:rs:a0:v0 0x104:rt:t6:t8 0x108:rt:t6:t8 */
#include "types.h"

extern s32 rsin();
extern s32 rcos();
extern void tt_orbit_turn_step(s32);

extern u8 *tt_boss_wk;

void tt_orbit_step(void)
{
    u8 *p;
    s32 v0;
    s32 t4;
    s32 t7;
    u32 m;

    p = tt_boss_wk;

    v0 = rcos(*(u16 *)(p + 942));
    *(s32 *)(p + 32) = (v0 * 3) << 9;
    v0 = rsin(*(u16 *)(p + 942) * 2);
    *(s32 *)(p + 36) = (v0 << 9) + 0x500000;
    *(s16 *)(p + 22) = *(s16 *)(p + 38);
    *(s16 *)(p + 20) = *(s16 *)(p + 34);

    if (*(u16 *)(p + 6) == 2) {
        v0 = *(u16 *)(p + 942);
        if (v0 == 1024 || v0 == 5120) {
            tt_orbit_turn_step(1);
            return;
        }
        if (v0 == 3072 || v0 == 7168) {
            tt_orbit_turn_step(0);
            return;
        }
        t4 = v0 + 8;
        m = t4 & 0xFFFF;
        *(s16 *)(p + 942) = (s16)t4;
        if (m >= 8193) {
            *(s16 *)(p + 942) = (s16)(m - 8192);
        }
    } else {
        t7 = *(u16 *)(p + 942) + 8;
        m = t7 & 0xFFFF;
        *(s16 *)(p + 942) = (s16)t7;
        if (m >= 8193) {
            *(s16 *)(p + 942) = (s16)(m - 8192);
        }
    }
}
