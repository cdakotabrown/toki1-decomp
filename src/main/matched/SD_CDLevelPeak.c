/* Function: SD_CDLevelPeak */
/* Address: 0x8007C3A0 */
/* Size: 260 bytes (65 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: remap_stack_slots 12:28,8:24 */
/* @PostProc: swap_instructions 0x14 0x20 */
/* @PostProc: swap_instructions 0x18 0x1C */
/* @PostProc: swap_instructions 0x1C 0x20 */
/* @PostProc: patch_insn_reg_field --patch 0x48:rd:v1:t1 --patch 0x4c:rs:v1:t1 --patch 0x50:rd:v1:t1 --patch 0xa0:rt:v1:t1 --patch 0x5c:rt:a1:v1 --patch 0x68:rs:a1:v1 --patch 0x74:rs:a1:v1 --patch 0x74:rt:a1:v1 --patch 0x78:rs:a1:v1 --patch 0x78:rd:a1:v1 --patch 0x7c:rt:a1:v1 --patch 0x80:rd:a1:v1 --patch 0x88:rt:a1:v1 --patch 0xa4:rt:a1:v1 --patch 0x60:rt:t0:a1 --patch 0x70:rs:t0:a1 --patch 0x8c:rs:t0:a1 --patch 0x8c:rt:t0:a1 --patch 0x90:rs:t0:a1 --patch 0x90:rd:t0:a1 --patch 0x94:rt:t0:a1 --patch 0x98:rd:t0:a1 --patch 0xb0:rt:t0:a1 --patch 0xbc:rt:t0:a1 --patch 0x64:rt:t1:t0 --patch 0x88:rs:t1:t0 --patch 0xa8:rt:t4:t3 --patch 0xac:rt:t3:t4 --patch 0xb0:rs:t4:t3 --patch 0xb8:rs:t3:t4 */
#include "types.h"

extern s16 gSDCDLevelTbl[];

void SD_CDLevelPeak(u16 *arg0, u16 *arg1, u32 arg2, u32 arg3) {
    s16 v1;
    s16 a1v;
    u32 inc;
    u32 cnt;
    s16 *base;

    arg2 = (u16)arg2;
    arg3 = (u16)arg3;
    cnt = 0;
    if (arg3 != 0) {
        inc = (u16)(256U / arg3);
        do {
            base = &gSDCDLevelTbl[arg2];
            v1 = base[0];
            a1v = base[512];
            if (v1 & 0x8000) {
                v1 = ~(v1 - 1);
            }
            if (a1v & 0x8000) {
                a1v = ~(a1v - 1);
            }
            if ((u32)*arg0 < (u32)v1) {
                *arg0 = v1;
            }
            arg2 = (arg2 + inc) & 0xFFFF;
            if ((u32)*arg1 < (u32)a1v) {
                *arg1 = a1v;
            }
            cnt = (cnt + 1) & 0xFFFF;
        } while (cnt < arg3);
    }
    *arg0 *= 2;
    *arg1 *= 2;
}
