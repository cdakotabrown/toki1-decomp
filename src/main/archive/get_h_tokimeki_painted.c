/* Function: get_h_tokimeki */
/* Address: 0x80052A80 */
/* Size: 1576 bytes (394 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: strip_arg_homes --offsets 0x20 --shrink */
/* @PostProc: strip_trailing_instructions --count 1 --nop-only */
/* @PostProc: custom_script func_80052A80_postproc.py */
#include "types.h"

extern u8 D_800BA5C0[];
extern u8 D_800BA5D0[];
extern u8 D_800BA5E0[];
extern s8 get_h_tokimeki_tbl1[];
extern u8 D_800BA658;
extern u8 D_800BA659;
extern u8 D_800BA65A;
extern u8 D_800BA65B;
extern u8 D_800BA65C;
extern u8 D_800BA65D;
extern u8 D_800BA65E;

#define STAT(off) (*(s16 *)(a2 + (off) + 254))

s32 get_h_tokimeki(s32 a0_unused, s32 a1, u8 *a2) {
    s32 flags[6];
    s32 v1;
    s8 *mods;
    u8 thr0, thr1, thr2, thr3, thr4, thr5, thr6;
    u8 mask, mode;
    s32 bit;
    s32 off;

    v1 = (D_800BA5C0[a1] * *(s16 *)(a2 + a1 * 56 + 446)) / 100;
    mask = D_800BA5D0[a1];

    bit = 1;
    do {
        if (mask & (1 << bit)) {
            flags[bit - 1] = 1;
        } else {
            flags[bit - 1] = 0;
        }
        bit++;
    } while (bit < 8);

    mode = D_800BA5E0[a1];
    thr0 = D_800BA658;
    thr6 = D_800BA65E;
    thr5 = D_800BA65D;
    thr4 = D_800BA65C;
    thr3 = D_800BA65B;
    thr2 = D_800BA65A;
    thr1 = D_800BA659;

    mods = &get_h_tokimeki_tbl1[a1 * 8];

    off = 4;
    bit = 0;
    do {
        if (flags[bit] != 0) {
            switch (mode) {
            case 1:
                if ((u32)(thr0 + 50) < (u32)STAT(off)) { v1 += mods[0]; }
                else if ((u32)(thr1 + 35) < (u32)STAT(off)) { v1 += mods[1]; }
                else if ((u32)(thr2 + 20) < (u32)STAT(off)) { v1 += mods[2]; }
                else if ((u32)(thr3 + 20) < (u32)STAT(off)) { v1 += mods[3]; }
                else if ((u32)(thr4 + 5) < (u32)STAT(off)) { v1 += mods[4]; }
                else if ((u32)(thr5 + 5) < (u32)STAT(off)) { v1 += mods[5]; }
                else if ((u32)thr6 < (u32)STAT(off)) { v1 += mods[6]; }
                else { v1 += mods[7]; }
                break;
            case 2:
                if ((u32)thr0 < (u32)STAT(off)) { v1 += mods[0]; }
                else if ((u32)thr1 < (u32)STAT(off)) { v1 += mods[1]; }
                else if ((u32)thr2 < (u32)STAT(off)) { v1 += mods[2]; }
                else if ((u32)thr3 < (u32)STAT(off)) { v1 += mods[3]; }
                else if ((u32)thr4 < (u32)STAT(off)) { v1 += mods[4]; }
                else if ((u32)thr5 < (u32)STAT(off)) { v1 += mods[5]; }
                else if ((u32)thr6 < (u32)STAT(off)) { v1 += mods[6]; }
                else { v1 += mods[7]; }
                break;
            case 3:
                if ((u32)(thr0 + 50) < (u32)STAT(off)) { v1 += mods[0]; }
                else if ((u32)(thr1 + 50) < (u32)STAT(off)) { v1 += mods[1]; }
                else if ((u32)(thr2 + 50) < (u32)STAT(off)) { v1 += mods[2]; }
                else if ((u32)(thr3 + 20) < (u32)STAT(off)) { v1 += mods[3]; }
                else if ((u32)(thr4 + 20) < (u32)STAT(off)) { v1 += mods[4]; }
                else if ((u32)(thr5 + 20) < (u32)STAT(off)) { v1 += mods[5]; }
                else if ((u32)(thr6 + 20) < (u32)STAT(off)) { v1 += mods[6]; }
                else { v1 += mods[7]; }
                break;
            case 4:
                if ((u32)(thr0 + 10) < (u32)STAT(off)) { v1 += mods[0]; }
                else if ((u32)(thr1 + 10) < (u32)STAT(off)) { v1 += mods[1]; }
                else if ((u32)(thr2 + 10) < (u32)STAT(off)) { v1 += mods[2]; }
                else if ((u32)(thr3 + 10) < (u32)STAT(off)) { v1 += mods[3]; }
                else if ((u32)thr4 < (u32)STAT(off)) { v1 += mods[4]; }
                else if ((u32)thr5 < (u32)STAT(off)) { v1 += mods[5]; }
                else if ((u32)thr6 < (u32)STAT(off)) { v1 += mods[6]; }
                else { v1 += mods[7]; }
                break;
            case 5:
                if ((u32)(thr0 + 20) < (u32)STAT(off)) { v1 += mods[0]; }
                else if ((u32)(thr1 + 20) < (u32)STAT(off)) { v1 += mods[1]; }
                else if ((u32)(thr2 + 20) < (u32)STAT(off)) { v1 += mods[2]; }
                else if ((u32)(thr3 + 20) < (u32)STAT(off)) { v1 += mods[3]; }
                else if ((u32)thr4 < (u32)STAT(off)) { v1 += mods[4]; }
                else if ((u32)(thr5 - 5) < (u32)STAT(off)) { v1 += mods[5]; }
                else if ((u32)thr6 < (u32)STAT(off)) { v1 += mods[6]; }
                else { v1 += mods[7]; }
                break;
            case 6:
                if ((u32)thr0 < (u32)STAT(off)) { v1 += mods[0]; }
                else if ((u32)thr1 < (u32)STAT(off)) { v1 += mods[1]; }
                else if ((u32)thr2 < (u32)STAT(off)) { v1 += mods[2]; }
                else if ((u32)thr3 < (u32)STAT(off)) { v1 += mods[3]; }
                else if ((u32)(thr4 - 5) < (u32)STAT(off)) { v1 += mods[4]; }
                else if ((u32)(thr5 - 5) < (u32)STAT(off)) { v1 += mods[5]; }
                else if ((u32)(thr6 - 5) < (u32)STAT(off)) { v1 += mods[6]; }
                else { v1 += mods[7]; }
                break;
            }
        }
        off += 4;
        bit++;
    } while (off < 28);

    if (v1 >= 101) {
        return 100;
    }
    if (v1 < 0) {
        return 0;
    }
    return (u8)v1;
}
