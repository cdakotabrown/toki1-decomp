/* @PostProc: patch_insn_reg_field 0x90:rd:s2:s1 0xf4:rt:s1:s2 0x108:rs:s2:s1 0x10c:rt:t3:t4 0x114:rd:a1:a2 0x114:rs:t3:t4 0x114:rt:s2:s1 0x124:rs:s2:s1 0x128:rd:t4:t3 0x12c:rd:a2:a1 0x12c:rs:t4:t2 0x12c:rt:t2:t3 0x130:rs:a2:a1 0x144:rs:a1:a2 0x148:rs:t0:v0 0x148:rt:v0:t0 0x164:rs:a1:a2 0x170:rs:a2:a1 0x188:rs:a1:a2 0x18c:rs:t0:v0 0x18c:rt:v0:t0 0x1a8:rs:a1:a2 0x1b8:rt:s2:s1 0x1f0:rs:s2:s1 0x1f0:rt:s2:s1 0x1f8:rt:s1:s2 0x1fc:rt:s1:s2 0x324:rs:s2:s1 0x324:rt:s2:s1 0x32c:rt:s1:s2 0x33c:rt:s1:s2 0x454:rs:s2:s1 0x454:rt:s2:s1 0x45c:rt:s1:s2 0x46c:rt:s1:s2 0x580:rs:s2:s1 0x580:rt:s2:s1 0x584:rs:s2:s1 */
/* sanshoku_bar_disp - RPG_BAT overlay, Address: 0x8015528C, Size: 1500 bytes */
/* @PostProc: swap_instructions --offset1 0x4C --offset2 0x68 */
#include "types.h"

extern s32 rpg_bat_target_no[];
extern void rpg_bat_sanshoku_disp();
extern void dtd_on_tpage();

void sanshoku_bar_disp(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4,
                   s32 arg5, s32 arg6, s32 arg7, s32 arg8)
{
    s32 i;
    s32 three;
    s32 three2;
    s32 base;
    s32 lo;
    s32 hi;
    s32 sel;
    s32 idx;
    s32 col;

    base = rpg_bat_target_no[arg6];
    three = 3;
    three2 = 3;
    if (arg3 < base / 10) {
        rpg_bat_target_no[arg6] = base - arg3 * 10;
        if (--rpg_bat_target_no[arg7] < 0) {
            rpg_bat_target_no[arg7] = 2;
        }
    }
    i = 0;
    col = arg8;
    do {
        base = rpg_bat_target_no[arg6] / 10;
        idx = rpg_bat_target_no[arg7] + i;
        if (base + i * arg3 < arg0) {
            sel = idx % three;
            lo = base + (i - 1) * arg3;
            hi = base + i * arg3;
            base = 0;
            if (lo < 0) {
                lo = 0;
            }
        } else {
            sel = idx % three2;
            lo = base + (i - 1) * arg3;
            hi = arg0;
            i = 100;
            if (lo < 0) {
                lo = 0;
            }
        }
        switch (sel) {
        case 0:
            rpg_bat_sanshoku_disp(lo, (arg1 * lo) / arg0, (arg2 * lo) / arg0, hi,
                          (arg1 * hi) / arg0, (arg2 * hi) / arg0,
                          col << 16, col << 8, 4);
            break;
        case 1:
            rpg_bat_sanshoku_disp(lo, (arg1 * lo) / arg0, (arg2 * lo) / arg0, hi,
                          (arg1 * hi) / arg0, (arg2 * hi) / arg0,
                          col << 8, col, 4);
            break;
        case 2:
            rpg_bat_sanshoku_disp(lo, (arg1 * lo) / arg0, (arg2 * lo) / arg0, hi,
                          (arg1 * hi) / arg0, (arg2 * hi) / arg0,
                          col, col << 16, 4);
            break;
        }
        i++;
    } while (i < 50);

    dtd_on_tpage(0, 0, 4, arg5 - 4, 0);
    rpg_bat_target_no[arg6] = rpg_bat_target_no[arg6] + arg4;
}
