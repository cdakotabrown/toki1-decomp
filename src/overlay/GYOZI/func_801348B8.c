/* @SymbolSize: 2256 */
/* @RodataBase: 0x80145980 */
/* @CompileEnv: IDO52_COPYPROP_SITE=35,40,42,169,174,176,200,208,417,211,224,335 IDO52_AS1_XBB_VOLATILE_OK=all */
/* @PostProc: patch_stack_complete --old 120 --new 128 */
/* @PostProc: remap_stack_slots --from 96 --to 100 */
/* @PostProc: remap_stack_slots --from 72 --to 76 */
/* @PostProc: remap_stack_slots --from 56 --to 60 */
/* @PostProc: swap_instructions --offset1 0x3D0 --offset2 0x3D4 */
/* @PostProc: swap_instructions --offset1 0x448 --offset2 0x450 */
/* @PostProc: swap_instructions --offset1 0x44C --offset2 0x454 */
/* @PostProc: swap_instructions --offset1 0x450 --offset2 0x458 */
/* @PostProc: swap_instructions --offset1 0x454 --offset2 0x458 */
/* @PostProc: swap_instructions --offset1 0x470 --offset2 0x474 */
/* @PostProc: swap_instructions --offset1 0x474 --offset2 0x478 */
/* @PostProc: swap_instructions --offset1 0x47C --offset2 0x484 */
/* @PostProc: swap_instructions --offset1 0x480 --offset2 0x484 */
/* @PostProc: swap_instructions --offset1 0x488 --offset2 0x494 */
/* @PostProc: swap_instructions --offset1 0x48C --offset2 0x494 */
/* @PostProc: swap_instructions --offset1 0x490 --offset2 0x494 */
/* @PostProc: swap_instructions --offset1 0x4EC --offset2 0x4F4 */
/* @PostProc: swap_instructions --offset1 0x4F0 --offset2 0x4F8 */
/* @PostProc: swap_instructions --offset1 0x4F4 --offset2 0x4FC */
/* @PostProc: swap_instructions --offset1 0x4F8 --offset2 0x4FC */
/* @PostProc: swap_instructions --offset1 0x598 --offset2 0x5B8 */
/* @PostProc: swap_instructions --offset1 0x5AC --offset2 0x5B8 */
/* @PostProc: swap_instructions --offset1 0x5B0 --offset2 0x5B8 */
/* @PostProc: swap_instructions --offset1 0x5B4 --offset2 0x5B8 */
/* @PostProc: patch_insn_reg_field 0x1c8:rt:v1:v0 0x1cc:rs:v1:v0 0x1cc:rt:v1:v0 0x1d8:rs:v1:v0 0x1e8:rs:v1:v0 0x1e8:rt:v0:v1 0x1f0:rs:v0:v1 0x200:rs:v1:v0 0x20c:rs:v1:v0 0x210:rs:v1:v0 0x21c:rs:v1:v0 0x220:rs:v1:v0 0x238:rs:v1:v0 0x23c:rs:v0:v1 0x250:rs:v1:v0 0x25c:rs:v1:v0 0x260:rs:v1:v0 0x26c:rs:v1:v0 0x270:rs:v1:v0 0x284:rs:v1:v0 0x288:rs:v1:v0 0x288:rt:v0:v1 0x290:rs:v0:v1 0x29c:rs:v0:v1 0x2a4:rs:v0:v1 0x2c8:rt:v0:v1 0x2cc:rs:v0:v1 0x2cc:rt:v0:v1 0x2d4:rs:v0:v1 0x2e0:rs:v0:v1 0x2e8:rs:v0:v1 0x310:rt:v0:v1 0x314:rs:v0:v1 0x314:rt:v0:v1 0x31c:rs:v0:v1 0x328:rs:v0:v1 0x35c:rt:v1:v0 0x360:rs:v1:v0 0x360:rt:v1:v0 0x364:rs:v1:v0 0x364:rt:v0:v1 0x36c:rs:v0:v1 0x374:rs:v1:v0 0x380:rt:v0:v1 0x38c:rs:v0:v1 0x394:rs:v1:v0 0x3a0:rs:v0:v1 0x3ac:rs:v1:v0 0x3e4:rs:v1:v0 0x3e8:rs:v1:v0 0x3f4:rs:v1:v0 0x3f8:rs:v1:v0 0x3f8:rt:v0:v1 0x400:rs:v0:v1 0x408:rs:v0:v1 0x40c:rs:v1:v0 0x40c:rt:v0:v1 0x410:rs:v1:v0 0x414:rs:v0:v1 0x420:rs:v1:v0 0x430:rt:v0:v1 0x434:rt:v1:v0 0x438:rs:v0:v1 0x440:rs:v1:v0 0x440:rt:v1:v0 0x444:rs:v1:v0 0x47c:rs:v0:v1 0x47c:rt:v1:v0 0x488:rs:v0:v1 0x488:rt:v0:v1 0x498:rs:v0:v1 0x49c:rs:v1:v0 0x4a0:rd:v0:v1 0x4d0:rt:v0:v1 0x4d8:rs:v0:v1 0x5d8:rt:a0:v1 0x5e0:rs:a0:v1 0x5e0:rt:v0:v1 0x5f4:rs:v0:v1 0x5f8:rt:v1:v0 0x600:rs:v1:v0 0x600:rt:v1:v0 0x608:rs:v1:v0 0x624:rs:v0:v1 0x630:rs:v0:v1 0x634:rt:v0:v1 0x640:rt:v0:v1 0x684:rs:v0:v1 0x68c:rt:v0:v1 0x698:rt:v0:v1 0x6e0:rt:v0:v1 0x6ec:rt:v0:v1 */
/* func_801348B8 -- GYOZI overlay, Address: 0x801348B8, Size: 2256 bytes */
/* byte-perfect sibling func_8013708C / @RodataBase: 0x80145A48).  The */
#include "types.h"

typedef struct { u8 b[21]; } Desc1;
typedef struct { u8 b[23]; } Desc2;
typedef struct { u8 b[15]; } Desc3;

typedef union {
    s32 f;
    struct {
        s16 frac;
        s16 i;
    } p;
} Fx;

typedef struct {
    u8 pad00[0x24];
    Fx pos;
    s32 unk28;
    Fx vel;
    u8 pad30[0x14];
} EntG;

extern void func_800BCDF0(u8 *, s32);
extern void func_8005AE00(s32, s32, s32, void *, s32);
extern void func_800BCEA0(void *, void *, void *);
extern void func_80086AB0(s32);
extern void func_80137188(void);
extern void func_8004DE1C(void);
extern s32 func_8005B32C(void);
extern void func_8005B06C(s32);

extern Desc1 D_80145900;
extern Desc2 D_80145918;
extern Desc3 D_80145930;
extern u8 D_80145940[];
extern u8 D_8014594C[];
extern u8 D_80145958[];
extern u8 D_80145968[];

extern EntG D_80129F40[];

extern s32 D_8012E674;
extern s32 D_8012E680;

extern s32 D_800F6474;
extern u8 D_800F647D;
extern s32 D_800F62F8;
extern u8 D_800F53DC;
extern u8 D_800F53DE;
extern u8 D_80129FCB;
extern s32 D_800F6468;

extern s32 D_8012B8E4;
extern volatile s16 D_8012B8EE;
extern s16 D_8012B8D6;
extern s16 D_8012B8D8;
extern s8 D_8012B906;
extern volatile s16 D_8012B90C;
extern s8 D_8012B94A;
extern volatile s16 D_8012B950;
extern s8 D_8012B98E;
extern u8 D_8012B98F;
extern s16 D_8012B9A2;
extern s16 D_8012B9A4;
extern s32 D_8012B8F0;
extern volatile s16 D_8012B8EA;
extern u8 D_8012B8C2;
extern u8 D_8012B8C3;

extern u8 D_801461A0;
extern u8 D_80146178;

extern u8 D_8014617C[];

s32 func_801348B8(void)
{
    Desc1 local1;
    Desc2 local2;
    Desc3 local3;
    s32 v1;
    u32 sub;
    u32 chk;
    u32 idx;
    volatile EntG *w;

    w = D_80129F40;

    local1 = D_80145900;
    local2 = D_80145918;
    local3 = D_80145930;

    func_800BCDF0(D_80145940, D_800F6474);

    switch ((u32)D_800F6474) {
    case 0:
        func_8005AE00(-104, 48, 0, &local1, 0);
        D_8012E674 = 1;
        D_8012B8EE = -4;
        D_800F6474 += 1;
        return 0;

    case 1:
        func_8005AE00(-104, 48, 0, &local2, 0);
        D_8012E674 = 1;
        D_801461A0 = 0;
        D_8012B98F |= 0x80;
        D_8012B98E = 1;
        D_8012B9A2 = 1;
        D_8012B9A4 = 0;
        D_800F6474 += 1;
        return 0;

    case 2:
        D_8012B8E4 += w[96].vel.f;
        v1 = w[96].pos.p.i;
        if (v1 < -111) {
            D_8012B9A2 = 0;
            w[96].vel.p.i = -w[96].vel.p.i;
            w[96].vel.p.i += 2;
            if (w[96].vel.p.i >= 21) {
                w[96].vel.p.i = 20;
            }
        } else if (v1 >= 112) {
            D_8012B9A2 = 1;
            w[96].vel.p.i = -w[96].vel.p.i;
            w[96].vel.p.i -= 2;
            if (w[96].vel.p.i < -20) {
                w[96].vel.p.i = -20;
            }
        }

        v1 = w[96].vel.p.i;
        if (v1 < -8 || v1 >= 9) {
            D_8012B90C += 1;
            D_8012B950 += 1;
            v1 = D_8012B8EE;
        }
        if (v1 < -12 || v1 >= 13) {
            D_8012B90C += 2;
            D_8012B950 += 2;
            v1 = D_8012B8EE;
        }
        if (v1 < -16 || v1 >= 17) {
            D_8012B90C += 4;
            D_8012B950 += 4;
        }
        break;

    case 3:
        v1 = w[96].pos.p.i;
        if (v1 >= 0) {
            if (v1 <= -w[96].vel.p.i) {
                goto settle;
            }
        }
        if (v1 > 0) {
            goto drift;
        }
        if (v1 < -w[96].vel.p.i) {
            goto drift;
        }
settle:
        w[96].pos.p.i = 0;
        D_801461A0 = 1;
        D_8012B98F &= 0x7F;
        D_800F6474 += 1;
        return 0;
drift:
        w[96].pos.f += w[96].vel.f;
        v1 = w[96].pos.p.i;
        if (v1 < -111) {
            v1 = w[96].pos.p.i;
            w[96].vel.p.i = 10;
        }
        if (v1 >= 112) {
            w[96].vel.p.i = -10;
        }
        return 0;

    case 4:
    case 5:
    case 8:
    case 9:
        sub = D_800F647D;
        if (sub == 0) {
            w[96].vel.f = 0;
            D_8012B8F0 = 0xFFFC0000;
            D_8012B8C3 |= 0x10;
        }
        D_8012B906 = 0;
        D_8012B94A = 0;
        chk = sub ^ 30;
        if (D_8012B8EA < -180) {
            D_8012B8EA = -180;
        }
        sub = (sub + 1) & 0xFF;
        if (chk == 0) {
            D_8012B8F0 = 0;
            D_8012B8D6 = D_800F53DE - 95;
            D_8012B8C2 = 5;
        }
        D_800F647D = sub;
        if (sub == 75) {
            D_801461A0 = 0;
            D_8012B8C3 &= 0x7F;
            D_8012B98F |= 0x80;
            D_8012B98E = 9;
            D_8012B9A2 = 2;
            D_8012B9A4 = 0;
            func_80086AB0(1280);
        }
        if ((u32)D_800F647D < 135) {
            return 0;
        }
        func_80137188();
        func_8005AE00(-104, 48, 0, &local3, 0);
        D_800F6474 += 1;
        return 0;

    case 6:
        if ((D_8012B8C2 != 1) | 8) {
            D_8012E674 = 1;
            D_800F6474 += 1;
        }
        return 0;

    case 7:
        D_8012B8C3 |= 0x80;
        idx = D_80146178;
        D_8012B8C2 = 64;
        D_8012B8D8 = idx + 13;
        w[96].pos.p.i = -160;
        D_8012B8EA = -120;
        D_8012B8F0 = 0;
        if (idx == 3 || idx == 4) {
            func_800BCEA0(&local2, D_8014594C, &D_8014617C[idx * 5]);
            D_8012B98F |= 0x80;
            D_8012B98E = 64;
            D_8012B9A4 = 7;
        } else if (idx != 0) {
            func_800BCEA0(&local2, D_80145958, &D_8014617C[idx * 5]);
            D_8012B9A4 = 6;
            D_801461A0 = 1;
            D_8012B98F &= 0x7F;
        } else {
            func_800BCEA0(&local2, D_80145968, &D_8014617C[idx * 5]);
            D_8012B9A4 = 6;
            D_801461A0 = 1;
            D_8012B98F &= 0x7F;
        }
        func_8005AE00(-104, 48, 0, &local2, 0);
        D_8012E674 = 1;
        D_800F6474 += 1;
        return 0;

    default:
        func_8004DE1C();
        break;
    }

    if (D_8012E674 != 0) {
        if (D_800F62F8 & 0x00400040) {
            if (func_8005B32C() != 0) {
                if (D_800F53DC == 2) {
                    func_8005B06C(1);
                    D_8012E674 = 0;
                    D_8012E680 = 0;
                    D_80129FCB &= 0xFF7F;
                    D_800F6474 += 1;
                    return 1;
                }
            }
        }
        if (D_800F62F8 & 0x00200020) {
            if (func_8005B32C() != 0) {
                func_8005B06C(1);
                D_8012E674 = 0;
                D_8012E680 = 0;
                D_80129FCB &= 0xFF7F;
                D_800F6474 += 1;
                return 1;
            }
        }
        if (func_8005B32C() != 0) {
            if ((u32)D_800F6468 % 30 < 15) {
                D_80129FCB &= 0x7F;
            } else {
                D_80129FCB |= 0x80;
            }
        }
    }

}
