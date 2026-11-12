/* @CompileFlags: -Wc,-pic0 */
/* @SymbolStart: 0x801373C8 */
/* @SymbolSize: 784 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=a1:17 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* @PostProc: swap_instructions --offset1 0x210 --offset2 0x214 */
/* @PostProc: swap_instructions --offset1 0x218 --offset2 0x21C */
/* @PostProc: patch_insn_reg_field 0xf8:rt:v0:v1 0xfc:rs:v0:v1 0xfc:rt:v0:v1 0x108:rs:v0:v1 0x108:rt:v0:v1 0x10c:rt:v0:v1 0x124:rt:v0:v1 0x128:rs:v0:v1 0x128:rt:v0:v1 0x130:rs:v0:v1 0x130:rt:v0:v1 0x140:rt:v0:v1 0x188:rt:v0:a1 0x190:rs:v0:a1 0x1a4:rt:t0:v0 0x1a8:rs:t0:v0 0x1a8:rt:t0:v0 0x1b0:rs:t0:v0 0x1cc:rt:v0:a1 0x1d4:rt:v1:v0 0x1d8:rs:v1:v0 0x1d8:rt:v1:v0 0x1e0:rs:v0:a1 0x1e4:rt:t1:t0 0x1ec:rs:t1:t0 0x1ec:rt:t1:t0 0x1f0:rt:v1:v0 0x1f8:rs:t1:t0 0x200:rt:v1:v0 0x204:rs:v1:v0 */
/* func_801373D0 -- GYOZI overlay, Address: 0x801373D0, Size: 776 bytes */
#include "types.h"

extern void func_800BCE10(void *, void *);
extern u32 func_8005EBE0(u32);
extern void func_80090960(s32, s32);
extern void func_8008A0D4(u32);
extern s32 func_8004DE1C(void);

extern u8 D_800D92E0[];
extern u8 D_800F62CF;
extern u32 D_800F6468;
extern u8 D_80145A00[];
extern u8 D_80145A08[];
extern u8 D_80145A10[];
extern u8 D_80145A18[];
extern u8 D_80145A20[];
extern u8 D_80145A28[];
extern u8 D_80145A30[];
extern u8 D_80145A38[];
extern s16 D_80147220;
extern s16 D_8014723C;
extern u8 D_80147240;
extern u8 D_80147244;
extern volatile u8 D_80147244_v;

s32 func_801373D0(void)
{
    u32 g;
    u32 t;
    u32 v;
    u32 w;
    u32 tone;

    g = D_800F6468;
    t = g & 7;
    if (t == 0) {
        v = 3;
    } else if (t < 4) {
        v = 0;
    } else {
        v = 1;
    }
    if (D_800F62CF == 0) {
        v = (u8)g;
        v = v & 1;
    }

    if (v == 0) {
        goto case0;
    }
    if (v == 1) {
        goto case1;
    }
    D_80147244 = (u8)v;
    if (v != 2) {
        goto case3;
    }
    goto case2;
case0:
    D_80147244 = (u8)v;
    func_800BCE10(D_800D92E0, D_80145A00);
    tone = 16365;
    goto endsw;
case1:
    D_80147244 = (u8)v;
    func_800BCE10(D_800D92E0, D_80145A08);
    tone = 16383;
    goto endsw;
case2:
    func_800BCE10(D_800D92E0, D_80145A10);
    tone = 16410;
    goto endsw;
case3:
    func_800BCE10(D_800D92E0, D_80145A18);
    tone = 16419;
endsw:

    if (D_80147220 == D_8014723C - 1) {
        func_800BCE10(D_800D92E0, D_80145A20);
        tone = 16346;
    }
    if (D_80147220 == D_8014723C - 1) {
        if (D_800F62CF == 0) {
            if (func_8005EBE0(D_800F62CF) >= 70) {
                func_800BCE10(D_800D92E0, D_80145A28);
                tone = 16621;
            }
        }
    }

    if (D_800F62CF == 9) {
        if (D_80147240 == 1) {
            if (D_80147244 >= 2U) {
                func_800BCE10(D_800D92E0, D_80145A30);
                tone = 16373;
            }
        }
    }

    w = D_80147244_v;
    if (D_800F62CF == 9) {
        D_80147244_v = (u8)w;
        if (D_80147240 == 1) {
            D_80147244_v = (u8)w;
            if (w < 2U) {
                tone = 16365;
                func_800BCE10(D_800D92E0, D_80145A38);
            }
        }
    }

    switch (tone) {
    case 16365:
        func_80090960(6, 0);
        break;
    case 16373:
        func_80090960(6, 1);
        break;
    case 16621:
        func_80090960(6, 5);
        break;
    case 16346:
        func_80090960(6, 4);
        break;
    case 16410:
        func_80090960(6, 3);
        break;
    default:
        func_80090960(6, 2);
        break;
    }
    func_8008A0D4(tone);
    return func_8004DE1C();
}
