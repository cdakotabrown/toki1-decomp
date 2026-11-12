/* func_8013EF90 -- GYOZI overlay, Address: 0x8013EF90, Size: 788 bytes */
/* @SymbolSize: 788 */
/* @CompileEnv: IDO52_UGEN_TREG_ORDER_WINDOW=7:10:-1 IDO52_AS1_XBB_VOLATILE_OK=0,12 IDO52_AS1_NO_LA_PULL=13 */
/* @PostProc: swap_instructions --offset1 0x22c --offset2 0x248 */
/* @PostProc: swap_instructions --offset1 0x234 --offset2 0x248 */
/* @PostProc: swap_instructions --offset1 0x238 --offset2 0x248 */
/* @PostProc: swap_instructions --offset1 0x23c --offset2 0x248 */
/* @PostProc: swap_instructions --offset1 0x240 --offset2 0x248 */
/* @PostProc: patch_insn_reg_field 0x3c:rt:v1:v0 0x40:rt:v1:v0 0x5c:rs:t6:s3 0x5c:rt:s3:t6 0x60:rt:v0:t8 0x68:rd:t8:t9 0x68:rt:v0:t8 0x6c:rd:t9:t0 0x6c:rt:t8:t9 0x70:rs:t9:t0 0x70:rt:t0:t1 0x74:rs:t0:t1 0xa4:rs:s0:t6 0xa4:rt:t6:s0 0xc4:rd:v0:t8 0xc8:rs:v0:t8 0xc8:rt:v0:t9 0xcc:rt:v0:t9 0xd4:rs:v0:t9 0xd8:rt:v1:t2 0xdc:rs:v1:t2 0xdc:rt:v1:t2 0xe4:rs:v1:t2 0xe8:rd:t8:t0 0xec:rd:t9:t1 0xec:rt:t8:t0 0xf0:rs:t9:t1 0xf4:rs:v1:t2 0xf8:rd:t0:t3 0xfc:rd:t1:t4 0xfc:rs:t0:s3 0xfc:rt:s3:t3 0x100:rs:t1:t4 0x100:rt:v0:v1 0x108:rd:t2:t5 0x108:rt:v0:v1 0x10c:rd:v0:v1 0x10c:rt:t2:t5 0x110:rs:v0:v1 0x120:rs:v0:v1 0x14c:rt:t9:a0 0x154:rt:t9:a0 0x174:rt:t7:t2 0x178:rt:t1:t3 0x17c:rs:t1:t3 0x184:rt:t2:t4 0x188:rs:t2:t4 0x188:rt:t2:t4 0x190:rs:t2:t4 0x190:rt:t6:t5 0x194:rt:t6:t5 0x19c:rs:t7:t2 0x1a0:rt:t8:t7 0x1a4:rs:t8:t7 0x1a4:rt:t8:t7 0x1ac:rs:t8:t7 0x1ac:rt:t9:t8 0x1b0:rt:t9:t8 0x1cc:rt:v1:t9 0x1d0:rs:v1:t9 0x1d0:rt:v0:t9 0x1d4:rt:t6:t5 0x1d8:rs:v0:t9 0x1dc:rt:t9:t8 0x1e8:rd:a2:v1 0x1e8:rs:t0:s3 0x1e8:rt:s3:t0 0x1ec:rs:a2:v1 0x1f4:rd:t2:t4 0x1f8:rs:t2:t4 0x200:rs:t6:t5 0x200:rt:t6:t5 0x208:rs:t6:t5 0x210:rs:v0:t9 0x218:rs:a2:v1 0x218:rt:t7:t2 0x220:rs:t7:t2 0x220:rt:t8:t7 0x224:rs:a2:v1 0x224:rt:t8:t7 0x228:rs:t9:t8 0x228:rt:t9:t8 0x230:rt:t9:t8 0x24c:rt:t6:t4 0x250:rs:t6:t4 0x250:rt:t6:t4 0x254:rt:t2:t3 0x258:rd:t7:t5 0x258:rt:t6:t4 0x25c:rd:t8:t9 0x25c:rt:t7:t5 0x260:rs:t8:t9 0x260:rt:t2:t3 0x274:rs:a2:v1 0x274:rt:v0:t6 0x278:rt:v1:a0 0x27c:rs:v0:t6 0x27c:rt:t9:t2 0x280:rs:a2:v1 0x280:rt:t9:t2 0x284:rt:a0:v1 0x294:rd:t0:t7 0x298:rs:t0:t7 0x298:rt:t1:t8 0x2a0:rt:t1:t8 0x2a8:rd:t6:t0 0x2ac:rd:t7:t1 0x2ac:rt:t6:t0 0x2b0:rs:t7:t1 0x2b8:rs:t7:t1 0x2bc:rt:t7:t1 0x2c0:rt:t2:t4 0x2c4:rs:t2:t4 0x2c4:rt:t2:t4 0x2cc:rs:t2:t4 0x2e0:rd:t8:t5 0x2e4:rs:t8:s3 0x2e4:rt:s3:t5 0x2e8:rt:t0:t3 0x2f0:rd:t1:t6 0x2f0:rs:t9:t3 0x2f0:rt:t0:t9 0x2f8:rt:t1:t6 */
/* @PostProc: swap_instructions --offset1 0x278 --offset2 0x284 */
/* @PostProc: patch_insn_reg_field 0x278:rt:v1:a0 0x284:rt:a0:v1 */
#include "types.h"

extern void func_800BCDF0(u8 *, s32, u8);
extern s32 system_config_disp(void);
extern void func_80141550(void);
extern void func_8008F618(s32);
extern void func_80090960(s32, s32);
extern void func_8004DEAC(s32);
extern void func_8004DE1C(void);

extern u8 D_800F53A0[];
extern s32 D_800F6468;
extern u8 D_800F62CE;
extern volatile u8 D_800F62CF;
extern u8 D_800F6478;
extern u8 D_800F6479;
extern s16 D_800F54AE;
extern u8 D_80145D40[];
extern volatile u8 D_80148750;
extern u8 D_80148754;
extern s16 D_80148620;
extern s16 D_8014875C[];
extern s16 D_8014876C[];

void func_8013EF90(void)
{
    u8 *base;
    u8 *p;
    u8 *q;
    s16 count;
    s16 i;
    s16 lim;
    s16 add;
    s32 c56;
    u32 c56b;
    u32 c10;
    u8 sel;
    u8 cur;
    s32 flags;
    s32 r;

    base = D_800F53A0;
    count = 0;
    c56 = 56;
    for (i = 0; i < 11; i++) {
        flags = *(s32 *)(base + i * c56 + 440);
        count += ((((u32)flags << 30) >> 31) == 0) ^ 1;
    }

    i = 0;
    c56b = 56;
    c10 = 10;
loop:
    sel = (u8)((u32)(D_800F6468 + i) % c10 + 1);
    D_800F62CF = sel;
    func_800BCDF0(D_80145D40, count, sel);
    cur = D_800F62CF;
    i++;
    flags = *(s32 *)(base + cur * c56b + 440);
    flags = (s32)(((u32)flags << 30) >> 31);
    if (flags == 0 && (u32)cur >= 9) {
        goto loop;
    }
    if (flags == 0 && count >= 2) {
        goto loop;
    }

    if (D_80148750 != 0) {
        D_800F62CF = 9;
    }
    D_80148754 = D_800F62CF;
    D_80148620 = 0;
    r = system_config_disp();

    if (D_800F62CF == 6) {
        if ((u32)(r & 0x7F) >= 2U) {
            D_80148620 = D_80148620 + 1;
        }
    }
    if ((r & 0x7F) == 4) {
        D_80148620 = D_80148620 + 1;
    }
    func_80141550();
    func_8008F618(D_800F62CF);

    cur = D_800F62CF;
    p = base + cur * c56b;
    if ((*(s32 *)(p + 440) << 30) >= 0 || (D_80148750 != 0 && cur == 9)) {
        p[440] = p[440] | 2;
        D_800F53A0[1808] = D_800F6478;
        D_800F53A0[1809] = D_800F6479;
        D_800F53A0[1810] = 17;
        func_80090960(4, 0);
        D_800F53A0[1841 + D_800F62CE * 8] = 2;
        func_8004DEAC(5);
    } else {
        add = *(s16 *)(p + 434);
        *(s16 *)(p + 434) = add + 4;
        lim = D_800F54AE;
        for (i = 0; i < 7; i++) {
            if (lim < D_8014875C[i]) {
                break;
            }
        }
        q = base + D_800F62CF * c56b;
        *(s16 *)(q + 430) = *(s16 *)(q + 430) + D_8014876C[i];
        func_80090960(4, 0);
        func_8004DE1C();
    }
}
