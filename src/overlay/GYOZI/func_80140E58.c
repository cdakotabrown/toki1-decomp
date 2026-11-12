/* @SymbolStart: 0x80140E50 */
/* @SymbolSize: 284 */
/* @PostProc: patch_insn_reg_field 0x1c:rd:t7:t8 0x20:rs:t7:t8 0x28:rt:t8:t9 0x2c:rs:t8:t9 0x2c:rt:t8:t9 0x30:rt:t9:t0 0x34:rs:t8:t9 0x40:rs:t9:t0 0x40:rt:t9:t0 0x48:rs:t9:t0 0x48:rt:t0:t1 0x4c:rs:t0:t1 0x54:rt:t1:t3 0x58:rt:t3:t5 0x5c:rs:t1:t3 0x5c:rt:t2:t4 0x60:rt:t2:t4 0x68:rt:t4:t6 0x6c:rs:t4:t6 0x6c:rt:t4:t6 0x70:rt:t3:t5 0x78:rt:t5:t7 0x7c:rs:t5:t7 0x7c:rt:t5:t7 0x80:rt:t4:t6 0x88:rt:t6:t8 0x8c:rs:t6:t8 0x8c:rt:t6:t8 0x90:rt:t5:t7 0x98:rt:t6:t8 0xa0:rt:t7:t9 0xa4:rt:t7:t9 */
/* Address: 0x80140E58 (window starts at the absorbed cross-prologue head */
/* Size: 284 bytes (71 insns) including the head */
#include "types.h"

extern void func_8004DE1C(void);
extern void func_8008F618(s32);
extern void func_80090960(s32, s32);

extern s32 D_800F5750;
extern s16 D_800F5746;
extern s32 D_800F6468;
extern volatile u8 D_800F53A0[];
extern u8  D_800F62CF;
extern s32 D_801481F0;
extern s32 D_80148224;
extern s32 D_80148258;
extern s32 D_80148614;
extern s32 D_80148618;
extern s32 D_8014861C;
extern s16 D_80148620;

void func_80140E58(void)
{
    if ((D_800F5750 << 30) >= 0 && D_800F5746 >= 20 &&
        (D_800F6468 & 1) != 0) {
        D_800F53A0[944] |= 2;
        D_80148620 = 5;
        D_80148614 = D_801481F0;
        D_80148618 = D_80148224;
        D_8014861C = D_80148258;
        D_800F62CF = 9;
        func_80090960(4, 1);
        func_8008F618(D_800F62CF);
        func_8004DE1C();
    } else {
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
    }
}
