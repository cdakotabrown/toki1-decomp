/* @SymbolStart: 0x80136C9C */
/* @SymbolSize: 992 */
/* @CompileEnv: IDO52_AS1_TIE_TAKE=256 */
/* func_80136CA0 -- GYOZI overlay, Address: 0x80136CA0, Size: 988 bytes */
/* @PostProc: swap_instructions --offset1 0x224 --offset2 0x228 */
/* @PostProc: swap_instructions --offset1 0x228 --offset2 0x22c */
/* @PostProc: swap_instructions --offset1 0x22c --offset2 0x230 */
/* @PostProc: swap_instructions --offset1 0x230 --offset2 0x234 */
/* @PostProc: swap_instructions --offset1 0x234 --offset2 0x238 */
/* @PostProc: swap_instructions --offset1 0x238 --offset2 0x23c */
/* @PostProc: swap_instructions --offset1 0x23c --offset2 0x240 */
/* @PostProc: swap_instructions --offset1 0x240 --offset2 0x244 */
/* @PostProc: swap_instructions --offset1 0x244 --offset2 0x248 */
/* @PostProc: swap_instructions --offset1 0x250 --offset2 0x254 */
/* @PostProc: patch_insn_reg_field 0x3c:rd:t7:t8 0x5c:rs:t7:t8 0x64:rt:t8:t9 0x68:rs:t8:t9 0x68:rt:t8:t9 0x6c:rt:t9:t0 0x70:rs:t8:t9 0x7c:rt:t9:t0 0x90:rd:t0:t1 0x94:rd:t0:t1 0x94:rs:t0:t1 0x98:rt:t1:t2 0x9c:rs:t1:t2 0x9c:rt:t1:t2 0xa0:rd:t0:t1 0xa0:rt:t0:t1 0xa4:rs:t0:t1 0xa4:rt:t1:t2 0xa8:rt:t2:t3 0xb0:rd:t3:t5 0xb0:rt:t2:t3 0xb4:rs:t3:t5 0xc0:rt:t4:t6 0xc4:rs:t4:t6 0xc4:rt:t4:t6 0xcc:rs:s4:t6 0xcc:rt:t4:s4 0xe4:rt:t5:t7 0xe8:rs:t5:t7 0xe8:rt:t5:t7 0xf0:rs:s4:t7 0xf0:rt:t5:s4 0xfc:rt:t6:t8 0x100:rs:t6:t8 0x100:rt:t6:t8 0x108:rs:t6:t8 0x10c:rd:t7:t9 0x114:rd:t7:t9 0x128:rd:t7:t9 0x12c:rd:t8:t0 0x12c:rt:t7:t9 0x130:rs:t8:t0 0x134:rt:t9:t3 0x138:rs:t9:t3 0x138:rt:t9:t3 0x140:rs:t9:t3 0x140:rt:t0:t4 0x144:rt:t0:t4 0x148:rt:t3:t6 0x154:rs:t3:t6 0x154:rt:t4:t7 0x15c:rt:t4:t7 0x164:rd:t5:t8 0x168:rt:t5:t8 0x174:rd:t0:t1 0x178:rt:t7:t0 0x17c:rs:t7:t0 0x17c:rt:t7:t0 0x184:rs:t7:t0 0x194:rd:t8:t1 0x198:rd:t9:t2 0x198:rt:t8:t1 0x19c:rs:t9:t2 0x1ac:rt:t2:t5 0x1b0:rs:t2:t5 0x1b0:rt:t2:t5 0x1b4:rd:t0:t3 0x1b8:rt:t0:t3 0x1bc:rt:t2:t5 0x1c4:rd:t8:t1 0x1d8:rt:t3:t6 0x1dc:rt:t4:t7 0x1e0:rs:t4:t7 0x1e0:rt:t4:t7 0x1e4:rs:t3:t6 0x1e4:rt:t3:t6 0x1ec:rs:t3:t6 0x1ec:rt:t4:t7 0x1f4:rs:t4:t7 0x200:rd:v0:v1 0x204:rd:t5:t8 0x204:rt:v0:v1 0x208:rd:t6:t9 0x208:rt:t5:t8 0x20c:rs:t4:t7 0x210:rd:v0:v1 0x210:rs:t6:t9 0x214:rt:t8:t1 0x218:rs:t8:t1 0x218:rt:t8:t1 0x21c:rd:t7:t0 0x21c:rt:v0:v1 0x220:rs:t7:t0 0x220:rt:t8:t1 0x224:rs:t8:t1 0x234:rs:t8:t1 0x23c:rs:t7:t0 0x248:rd:t3:t6 0x24c:rd:t5:t8 0x24c:rt:t3:t6 0x254:rt:t7:t0 0x258:rd:t6:t9 0x25c:rd:t4:t7 0x25c:rt:t6:t9 0x260:rs:t4:t7 0x264:rd:t9:t2 0x268:rd:t0:t3 0x268:rt:t9:t2 0x26c:rd:t1:t4 0x26c:rt:t0:t3 0x270:rs:t1:t4 0x270:rt:t2:t5 0x278:rs:t5:t8 0x278:rt:t2:t5 0x27c:rs:t7:t0 0x27c:rt:t7:t0 0x284:rs:t4:t7 0x284:rt:t7:t0 0x294:rt:t8:t1 0x298:rs:t8:t1 0x298:rt:t8:t1 0x2a0:rs:t8:t1 0x2a8:rd:t9:t2 0x2ac:rd:t0:t3 0x2ac:rt:t9:t2 0x2b0:rs:t0:t3 0x2c4:rt:t2:t5 0x2c8:rs:t2:t5 0x2c8:rt:t2:t5 0x2cc:rd:t1:t4 0x2d0:rt:t1:t4 0x2d4:rt:t2:t5 0x2dc:rd:t9:t2 0x2f0:rt:t5:t8 0x2f4:rs:t5:t8 0x2f4:rt:t5:t8 0x2fc:rs:t5:t8 0x300:rd:t6:t9 0x304:rt:t6:t9 0x308:rt:t4:t7 0x310:rs:t4:t7 0x32c:rt:t7:t0 0x330:rs:t7:t0 0x330:rt:t7:t0 0x338:rd:t8:t1 0x338:rt:t7:t0 0x33c:rt:t8:t1 0x340:rt:t9:t2 0x348:rt:t9:t2 0x360:rt:t0:t3 0x364:rs:t0:t3 0x364:rt:t0:t3 0x36c:rt:t0:t3 0x37c:rt:t1:t4 0x380:rs:t1:t4 0x380:rt:t1:t4 0x384:rt:t3:t6 0x388:rs:t1:t4 0x38c:rt:t2:t5 0x390:rs:t3:t6 0x390:rt:t3:t6 0x398:rt:t3:t6 0x3a0:rt:t2:t5 0x3a8:rt:t4:t7 0x3ac:rs:t4:t7 0x3ac:rt:t4:t7 0x3b0:rd:t5:t8 0x3b4:rt:t5:t8 0x3b8:rt:t4:t7 0x3c0:rd:t6:t9 */
/* @PostProc: remap_stack_slots --from 72 --to 68 */
#include "types.h"

extern s32  func_8005F220(s32);
extern s32  func_8005EBE0(s32);
extern void func_800BCDF0();
extern void func_8004DE1C(void);

extern s16 D_80147220;
extern s16 D_80147224[];
extern s16 D_8014723C;
extern u8  D_80147240;
extern u8  D_80147244;
extern s32 D_800F5750;
extern s16 D_800F5746;
extern u8  D_800F53A0[];
extern volatile s32 D_800F6468;
extern u8  D_801459F0[];
extern u8  D_801459F4[];
extern u8  D_801459F8[];
extern u8  D_801459FC[];

void func_80136CA0(void)
{
    s16 i;
    s16 cnt;
    s16 off;
    s16 buf[11];

    D_80147220 = 0;
    D_80147244 = 0;
    D_8014723C = 0;
    D_80147240 = 0;
    cnt = 0;
    if ((D_800F5750 << 30) >= 0) {
        if (D_800F5746 >= 20) {
            D_80147240 = 1;
        }
    }

    i = 0;
loop1:
    if ((*(s32 *)&D_800F53A0[(s32)i * 56 + 440] << 30) < 0) {
        goto arm;
    }
    if (i != 9) {
        goto next1;
    }
    if (D_80147240 != 1) {
        goto next1;
    }
arm:
    if ((u32)func_8005F220(i) < 50U) {
        if (D_80147240 != 1) {
            goto next1;
        }
        if (i != 9) {
            goto next1;
        }
    }
    if (D_80147240 == 0) {
        goto take;
    }
    if (i != 0) {
        goto take;
    }
    if ((u32)func_8005EBE0(0) < 70U) {
        goto next1;
    }
take:
    buf[cnt] = i;
    D_8014723C = (s16)(D_8014723C + 1);
    cnt = (s16)(cnt + 1);
    D_800F53A0[(s32)i * 56 + 441] |= 1;
next1:
    i = (s16)(i + 1);
    if (i < 11) {
        goto loop1;
    }

    i = 0;
    if (D_8014723C > 0) {
        do {
            func_800BCDF0(D_801459F0, buf[i]);
            i = (s16)(i + 1);
        } while (i < D_8014723C);
        i = 0;
    }
    func_800BCDF0(D_801459F4);

    off = (s16)((u32)D_800F6468 % (u32)D_8014723C);
    if (D_8014723C > 0) {
        do {
            D_80147224[i] = buf[((s32)i + off) % D_8014723C];
            i = (s16)(i + 1);
        } while (i < D_8014723C);
    }

    i = 0;
    if (D_8014723C > 0) {
        do {
            func_800BCDF0(D_801459F8, D_80147224[i]);
            i = (s16)(i + 1);
        } while (i < D_8014723C);
        i = 0;
    }
    func_800BCDF0(D_801459FC);

    if (D_8014723C > 0) {
        do {
            if (D_80147224[i] == 0) {
                if ((u32)func_8005EBE0(0) >= 70U) {
                    D_80147224[i] = D_80147224[D_8014723C - 1];
                    D_80147224[D_8014723C - 1] = 0;
                    goto done;
                }
            }
            if (D_80147224[i] == 0) {
                D_80147224[i] = D_80147224[0];
                D_80147224[0] = 0;
                goto done;
            }
            if (D_80147224[i] == 9) {
                if (D_80147240 != 0) {
                    D_80147224[i] = D_80147224[0];
                    D_80147224[0] = 9;
                    goto done;
                }
            }
            i = (s16)(i + 1);
        } while (i < D_8014723C);
    }
done:
    func_8004DE1C();
}
