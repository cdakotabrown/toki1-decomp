/* @SymbolStart: 0x801558E8 */
/* @SymbolSize: 376 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v1:v0 0x14:rs:v1:v0 0x1c:rt:a1:a0 0x20:rs:a1:a0 0x28:rt:a0:v1 0x2c:rs:a0:v1 0x2c:rt:a0:v1 0x34:rs:a0:v1 0x38:rt:a2:v1 0x3c:rs:a2:v1 0x40:rt:a0:v0 0x4c:rs:a0:v0 0x4c:rt:a0:v0 0x54:rs:a0:v0 0x5c:rs:a0:v0 0x78:rs:a1:a0 0x94:rt:a0:v1 0x98:rs:a0:v1 0x98:rt:a0:v1 0xa8:rs:a0:v1 0xe0:rs:a1:v1 0x148:rs:a0:v1 */
/* Address: 0x801558F0 */
/* Size: 368 bytes (92 instructions) */
#include "types.h"

extern u8 D_800B1746;
extern u8 D_80125320;
extern u8 D_800B0E31;
extern u8 D_800B0896;
extern u8 D_800B1AF6;
extern u8 D_800B0AFF;
extern u8 D_800B0BCE;

extern u8 func_8002328C();
extern s32 func_8002336C();
extern void func_80011DFC();

void func_801558F0(void) {
    u8 sel;
    u8 st;
    u8 v1;
    u8 c;
    u8 r1;
    s32 r2;

    sel = D_800B1746;
    if (sel == 1U && D_80125320 == 3) {
        v1 = D_800B0E31;
        if (v1 == 0) {
            c = D_800B0896;
            if (c < 3U || c >= 6U) {
                D_800B1AF6 += 13;
            }
        }
        if (v1 == 1U) {
            D_800B1AF6 += 13;
        }
    } else {
        st = D_80125320;
        if (sel == 4 && st == 3 && D_800B0AFF != 0) {
            D_800B1AF6 += 6;
        } else if (sel == 7 && st == 1U) {
            r1 = func_8002328C(7);
            r2 = func_8002336C(7);
            if (r1 == 128 && r2 == 0) {
                D_800B1AF6 += 10;
            } else {
                D_800B1AF6 += 11;
            }
        } else if (sel == 8 && st == 2 && D_800B0BCE != 0) {
            D_800B1AF6 += 6;
        }
    }
    func_80011DFC();
}
