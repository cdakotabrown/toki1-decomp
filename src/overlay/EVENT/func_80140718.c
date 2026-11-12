/* func_80140718 -- EVENT overlay, Address: 0x80140718 */
/* @SymbolStart: 0x801406F4 */
/* @SymbolSize: 344 */
/* @PostProc: patch_insn_reg_field 0x50:rt:t9:t0 0x54:rs:t9:t0 0x5c:rt:t8:t9 0x60:rt:t0:t1 0x64:rs:t0:t1 0x64:rt:t0:t1 0x6c:rs:t0:t1 0x6c:rt:t1:t2 0x70:rt:t1:t2 0x74:rt:t2:t3 0x78:rs:t2:t3 0x78:rt:t2:t3 0x80:rs:t2:t3 0x80:rt:t3:t4 0x88:rt:t3:t4 0x8c:rt:t4:t5 0x90:rs:t4:t5 0x90:rt:t4:t5 0x98:rs:t4:t5 0x98:rt:t5:t6 0x9c:rt:t5:t6 0xa0:rt:t6:t7 0xa4:rs:t6:t7 0xa4:rt:t6:t7 0xac:rs:t6:t7 0xac:rt:t7:t8 0xb0:rt:t7:t8 0xb4:rs:t8:t9 0xb4:rt:t8:t9 0xbc:rs:t8:t9 0xbc:rt:t9:t0 0xc4:rt:t9:t0 0xec:rt:t0:t1 0xf0:rt:t1:t2 0xf4:rs:t1:t2 0xf4:rt:t1:t2 0xf8:rs:t0:t1 0xf8:rt:t0:t1 0x11c:rt:t1:t2 0x124:rt:t0:t1 */
#include "types.h"

extern s32 D_801204A8;
extern s32 D_801204BC;
extern s32 D_801204D0;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_80122BE0;
extern s32 D_80122BE4;
extern s32 D_80122BE8;
extern s16 D_80122BEC;
extern s32 D_80122BF0;
extern s32 D_80122BF4;
extern s32 D_80122BF8;
extern s32 D_80122BFC;
extern s32 D_80122C00;
extern s32 D_80122C04;
extern s32 D_80122C08;
extern volatile s16 D_800B0B3E;
extern volatile s16 D_800B0B42;
extern s16 D_800B0B46;

extern s32 func_8002328C(s32);
extern void func_8004C250(void);
extern void func_80103F40(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_80011DFC(void);

void func_80140718(void) {
    s32 sel;

    D_80120678 = D_801204A8;
    D_8012067C = D_801204BC;
    D_80120680 = D_801204D0;
    sel = func_8002328C(6);
    if ((u32)(sel & 0x7F) < 2) {
        D_800B0B3E = (s16)(D_800B0B3E + 3);
        D_800B0B42 = (s16)(D_800B0B42 + 2);
    } else {
        D_800B0B3E = (s16)(D_800B0B3E + 1);
        D_800B0B42 = (s16)(D_800B0B42 + 3);
    }
    D_800B0B46 = (s16)(D_800B0B46 - 20);
    func_8004C250();
    func_80103F40();
    func_80012D64(D_80122BF0, 17, 1, 2, 0);
    func_8004C46C(D_80122BF4, D_80122BF8, D_80122BFC, D_80122C00,
                  D_80122C04, D_80122C08);
    func_8004C6B0(D_80122BE4, D_80122BE8, D_80122BE0, D_80122BEC);
    func_80011DFC();
}
