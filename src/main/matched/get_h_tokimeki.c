/* Function: get_h_tokimeki */
/* Address: 0x80052A80 */
/* Size: 1576 bytes (394 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -Wc,-pic1 */
/* @SymbolStart: 0x80052A68 */
/* @SymbolSize: 1600 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0x1c:rt:t1:a3 0x6c:rs:t1:a3 0x6c:rd:t1:a3 0x74:rs:t1:a3 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x74 */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x70 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x68 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x64 */
/* @PostProc: swap_instructions --offset1 0x5c --offset2 0x60 */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x5c */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x50 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=2:s2,36:a3,43:t0,53:t5,58:t4,63:t3,68:t2,73:t1,22:t5,48:s0,17:t1 */
#include "types.h"

extern u8 screen_w[];
extern u8 get_h_tokimeki_tbl0[];
extern u8 get_h_tokimeki_tbl1[];
extern u8 get_h_tokimeki_tbl2[];
extern s8 h_tokimeki_mod_tbl[];
extern u8 get_h_tokimeki_tbl3;
extern u8 get_h_tokimeki_tbl4;
extern u8 get_h_tokimeki_tbl5;
extern u8 get_h_tokimeki_tbl6;
extern u8 get_h_tokimeki_tbl7;
extern u8 get_h_tokimeki_tbl8;
extern u8 get_h_tokimeki_tbl9;

#define STAT(off) (*(s16 *)(screen_w + (off) + 254))
#define MOD(n)    (h_tokimeki_mod_tbl[idx * 8 + (n)])

s32 get_h_tokimeki(s32 who)
{
    s32 idx;
    s32 v1;
    s32 flags[7];
    u8 mask, mode;
    u8 thr0, thr1, thr2, thr3, thr4, thr5, thr6;
    s32 bit;
    s32 off;

    idx = who % 13;
    v1 = (*(s16 *)(screen_w + idx * 56 + 446) * get_h_tokimeki_tbl0[idx]) / 100;
    mask = get_h_tokimeki_tbl1[idx];

    bit = 0;
    do {
        if (mask & (1 << (bit + 1))) {
            flags[bit] = 1;
        } else {
            flags[bit] = 0;
        }
        bit++;
    } while (bit + 1 < 8);

    mode = get_h_tokimeki_tbl2[idx];
    thr0 = get_h_tokimeki_tbl3;
    thr6 = get_h_tokimeki_tbl9;
    thr5 = get_h_tokimeki_tbl8;
    thr4 = get_h_tokimeki_tbl7;
    thr3 = get_h_tokimeki_tbl6;
    thr2 = get_h_tokimeki_tbl5;
    thr1 = get_h_tokimeki_tbl4;

    off = 4;
    bit = 0;
    do {
        if (flags[bit] != 0) {
            switch (mode) {
            case 1:
                if ((u32)(thr0 + 50) < (u32)STAT(off)) { v1 += MOD(0); }
                else if ((u32)(thr1 + 35) < (u32)STAT(off)) { v1 += MOD(1); }
                else if ((u32)(thr2 + 20) < (u32)STAT(off)) { v1 += MOD(2); }
                else if ((u32)(thr3 + 20) < (u32)STAT(off)) { v1 += MOD(3); }
                else if ((u32)(thr4 + 5) < (u32)STAT(off)) { v1 += MOD(4); }
                else if ((u32)(thr5 + 5) < (u32)STAT(off)) { v1 += MOD(5); }
                else if ((u32)thr6 < (u32)STAT(off)) { v1 += MOD(6); }
                else { v1 += MOD(7); }
                break;
            case 2:
                if ((u32)thr0 < (u32)STAT(off)) { v1 += MOD(0); }
                else if ((u32)thr1 < (u32)STAT(off)) { v1 += MOD(1); }
                else if ((u32)thr2 < (u32)STAT(off)) { v1 += MOD(2); }
                else if ((u32)thr3 < (u32)STAT(off)) { v1 += MOD(3); }
                else if ((u32)thr4 < (u32)STAT(off)) { v1 += MOD(4); }
                else if ((u32)thr5 < (u32)STAT(off)) { v1 += MOD(5); }
                else if ((u32)thr6 < (u32)STAT(off)) { v1 += MOD(6); }
                else { v1 += MOD(7); }
                break;
            case 3:
                if ((u32)(thr0 + 50) < (u32)STAT(off)) { v1 += MOD(0); }
                else if ((u32)(thr1 + 50) < (u32)STAT(off)) { v1 += MOD(1); }
                else if ((u32)(thr2 + 50) < (u32)STAT(off)) { v1 += MOD(2); }
                else if ((u32)(thr3 + 20) < (u32)STAT(off)) { v1 += MOD(3); }
                else if ((u32)(thr4 + 20) < (u32)STAT(off)) { v1 += MOD(4); }
                else if ((u32)(thr5 + 20) < (u32)STAT(off)) { v1 += MOD(5); }
                else if ((u32)(thr6 + 20) < (u32)STAT(off)) { v1 += MOD(6); }
                else { v1 += MOD(7); }
                break;
            case 4:
                if ((u32)(thr0 + 10) < (u32)STAT(off)) { v1 += MOD(0); }
                else if ((u32)(thr1 + 10) < (u32)STAT(off)) { v1 += MOD(1); }
                else if ((u32)(thr2 + 10) < (u32)STAT(off)) { v1 += MOD(2); }
                else if ((u32)(thr3 + 10) < (u32)STAT(off)) { v1 += MOD(3); }
                else if ((u32)thr4 < (u32)STAT(off)) { v1 += MOD(4); }
                else if ((u32)thr5 < (u32)STAT(off)) { v1 += MOD(5); }
                else if ((u32)thr6 < (u32)STAT(off)) { v1 += MOD(6); }
                else { v1 += MOD(7); }
                break;
            case 5:
                if ((u32)(thr0 + 20) < (u32)STAT(off)) { v1 += MOD(0); }
                else if ((u32)(thr1 + 20) < (u32)STAT(off)) { v1 += MOD(1); }
                else if ((u32)(thr2 + 20) < (u32)STAT(off)) { v1 += MOD(2); }
                else if ((u32)(thr3 + 20) < (u32)STAT(off)) { v1 += MOD(3); }
                else if ((u32)thr4 < (u32)STAT(off)) { v1 += MOD(4); }
                else if ((u32)(thr5 - 5) < (u32)STAT(off)) { v1 += MOD(5); }
                else if ((u32)thr6 < (u32)STAT(off)) { v1 += MOD(6); }
                else { v1 += MOD(7); }
                break;
            case 6:
                if ((u32)thr0 < (u32)STAT(off)) { v1 += MOD(0); }
                else if ((u32)thr1 < (u32)STAT(off)) { v1 += MOD(1); }
                else if ((u32)thr2 < (u32)STAT(off)) { v1 += MOD(2); }
                else if ((u32)thr3 < (u32)STAT(off)) { v1 += MOD(3); }
                else if ((u32)(thr4 - 5) < (u32)STAT(off)) { v1 += MOD(4); }
                else if ((u32)(thr5 - 5) < (u32)STAT(off)) { v1 += MOD(5); }
                else if ((u32)(thr6 - 5) < (u32)STAT(off)) { v1 += MOD(6); }
                else { v1 += MOD(7); }
                break;
            }
        }
        off += 4;
        bit++;
    } while (bit < 6);

    if (v1 >= 101) {
        return 100;
    }
    if (v1 < 0) {
        return 0;
    }
    return (u8)v1;
}
