/* @SymbolStart: 0x801526A8 */
/* @SymbolSize: 480 */
/* @PostProc: patch_insn_reg_field 0x9c:rt:t4:t5 0xa0:rs:t4:t5 0xa0:rt:a1:t5 0xa4:rt:a0:v1 0xa8:rs:a1:t5 0xa8:rt:a0:v1 0xc0:rd:t5:t4 0xc4:rd:t5:t4 0xc4:rs:t5:t4 0xd4:rs:t5:t4 0xe0:rt:t5:t4 0xe4:rs:a1:t5 0xe4:rt:a0:v1 0x100:rt:t5:t4 0x104:rs:a1:t5 0x104:rt:a0:v1 0x108:rt:t5:t4 0x10c:rt:t1:t2 0x110:rs:t1:t2 0x110:rt:t1:t2 0x114:rt:t5:t4 0x128:rs:a1:t5 0x128:rt:a0:v1 0x13c:rd:t4:t1 0x140:rd:t2:t3 0x140:rs:t4:t1 0x140:rt:t1:t2 0x144:rd:t3:t4 0x144:rs:t2:t3 0x148:rs:a1:t5 0x148:rt:a0:v1 0x14c:rs:t3:t4 0x14c:rt:t5:t6 0x154:rt:t5:t6 0x15c:rd:t6:t7 0x160:rd:t7:t8 0x160:rs:t6:t7 0x160:rt:t1:t2 0x164:rd:t8:t9 0x164:rs:t7:t8 0x168:rs:a1:t5 0x168:rt:a0:v1 0x16c:rs:t8:t9 0x16c:rt:t9:t0 0x174:rt:t9:t0 0x17c:rd:t0:t1 0x180:rd:t4:t3 0x180:rs:t0:t1 0x180:rt:t1:t2 0x184:rd:t2:t4 0x184:rs:t4:t3 0x188:rs:t2:t4 0x188:rt:t3:t6 0x190:rt:t3:t6 0x194:rt:t6:t8 0x198:rs:t6:t8 0x198:rt:t6:t8 0x19c:rt:t5:t7 0x1a0:rs:t5:t7 0x1a0:rt:t5:t7 0x1a4:rd:t7:t9 0x1a4:rt:t6:t8 0x1ac:rt:t8:t5 0x1b0:rd:v0:t0 0x1b0:rs:t5:t7 0x1b0:rt:t7:t9 0x1b4:rs:t8:t5 0x1b4:rt:t8:t5 0x1b8:rt:v0:t0 0x1c0:rs:t8:t5 0x1c4:rs:t6:t8 0x1cc:rs:v0:t0 0x1cc:rt:t9:t3 0x1d4:rt:t9:t3 */
/* Address: 0x801526A8 (cross-prologue head; .sym draws the start at 0x801526B0) */
/* Size: 480 bytes (120 instructions) */
#include "types.h"

extern s32 D_800EECBC;
extern s32 ev_flag18;

extern u8 D_800B1746;
extern u8 D_800B0A28[];
extern u8 D_8012531C;

extern u8 D_800947C4[];
extern u8 D_8011FF4C[];
extern u8 D_8011FF64[];

extern u8 *D_801251F8;
extern u8 *D_801251FC;

extern s16 D_8012438C;
extern s16 D_8012438E;
extern s16 D_80124390;
extern s16 D_8012439C;
extern s16 D_8012439E;
extern s16 D_801243A0;
extern s16 D_80094730;

extern void func_80078970(u8 *, u8 *);
extern void func_80011DFC(void);

void func_801526B0(void) {
    u32 sub;
    u32 pose;
    s32 cursor;
    u8 gv;
    u8 *pa;
    u8 *pb;

    if (D_800EECBC != 0) {
        if ((D_800B0A28[D_800B1746 * 52] & 1) == 0) {
            D_8012531C = 2;
            func_80078970(D_800947C4, D_8011FF4C);
        } else {
            D_8012531C = 3;
            func_80078970(D_800947C4, D_8011FF64);
        }

        gv = D_800B1746;
        pa = D_801251F8;
        sub = D_8012531C * 3;
        ev_flag18 = 16;
        D_8012439C = (pa + gv * 12U)[sub + 0];
        D_8012439E = (pa + (u32)gv * 12U)[sub + 1];
        pb = D_801251FC;
        D_801243A0 = (pa + (gv | 0U) * 12U)[sub + 2];
        D_8012438C = (pb + (gv ^ 0U) * 12U)[sub + 0];
        D_8012438E = (pb + (gv * 1U) * 12U)[sub + 1];
        D_80124390 = (pb + (gv >> 0) * 12U)[sub + 2];
    }

    pose = D_8012531C;
    cursor = D_80094730 + pose * 2;
    D_80094730 = cursor;
    if (D_800B1746 == 7 && pose >= 2) {
        D_80094730 = cursor + 3;
    }
    func_80011DFC();
}
