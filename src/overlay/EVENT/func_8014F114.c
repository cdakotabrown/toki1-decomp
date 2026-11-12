/* Address: 0x8014F114 */
/* Size: 492 bytes (123 instructions) */
/* @SymbolStart: 0x8014F0F0 */
/* @SymbolSize: 528 */
/* @PostProc: patch_insn_reg_field 0x5c:rt:v0:t0 0x60:rs:v0:t0 0x60:rt:v0:t0 0x64:rt:t1:t2 0x68:rd:t0:t1 0x68:rt:v0:t0 0x6c:rd:t0:t1 0x6c:rs:t0:t1 0x6c:rt:v0:t0 0x70:rd:t0:t1 0x70:rt:t0:t1 0x74:rd:t0:t1 0x74:rs:t0:t1 0x74:rt:v0:t0 0x78:rd:t0:t1 0x78:rt:t0:t1 0x7c:rd:t1:t2 0x7c:rs:t1:t2 0x7c:rt:t0:t1 0x80:rs:t1:t2 0x80:rt:t1:t2 0x8c:rs:t1:t2 0x8c:rt:t2:t3 0x90:rs:t2:t3 0xbc:rt:t3:t4 0xcc:rt:t3:t4 0xec:rt:t4:t5 0xf8:rt:t4:t5 0x104:rt:t5:t6 0x10c:rt:t5:t6 0x110:rt:t7:t9 0x114:rs:t7:t9 0x114:rt:a2:t9 0x118:rt:a1:v1 0x11c:rs:a2:t9 0x11c:rt:a1:v1 0x120:rt:t0:t1 0x124:rs:t0:t1 0x124:rt:t0:t1 0x130:rt:t6:t7 0x140:rt:t6:t7 0x14c:rd:t9:t0 0x150:rd:t1:t2 0x150:rs:t9:t0 0x150:rt:t0:t1 0x154:rd:t2:t3 0x154:rs:t1:t2 0x158:rs:a2:t9 0x158:rt:a1:v1 0x15c:rs:t2:t3 0x15c:rt:t3:t4 0x164:rt:t3:t4 0x16c:rd:t4:t5 0x170:rd:t5:t6 0x170:rs:t4:t5 0x170:rt:t0:t1 0x174:rd:t6:t7 0x174:rs:t5:t6 0x178:rs:a2:t9 0x178:rt:a1:v1 0x17c:rs:t6:t7 0x180:rt:t4:t6 0x184:rs:t4:t6 0x184:rt:t4:t6 0x190:rd:t9:t0 0x194:rd:t1:t2 0x194:rs:t9:t0 0x194:rt:t0:t1 0x198:rd:t2:t3 0x198:rs:t1:t2 0x19c:rs:a2:t9 0x19c:rt:a1:v1 0x1a0:rs:t2:t3 0x1a0:rt:t3:t4 0x1a8:rt:t3:t4 0x1b0:rd:t7:t5 0x1b4:rd:t5:t7 0x1b4:rs:t7:t5 0x1b4:rt:t4:t6 0x1b8:rd:t6:t8 0x1b8:rs:t5:t7 0x1bc:rs:a2:t9 0x1bc:rt:a1:v1 0x1c0:rs:t6:t8 0x1c0:rt:t8:t0 0x1c8:rt:t8:t0 0x1d0:rd:t9:t1 0x1d4:rd:t0:t2 0x1d4:rs:t9:t1 0x1d4:rt:t4:t6 0x1d8:rd:t1:t3 0x1d8:rs:t0:t2 0x1dc:rs:a2:t9 0x1dc:rt:a1:v1 0x1e0:rs:t1:t3 0x1e0:rt:t2:t4 0x1e8:rt:t2:t4 0x1f0:rd:t3:t5 0x1f4:rs:t3:t5 0x1f4:rt:t4:t6 0x1f8:rd:t5:t8 0x1fc:rs:t5:t8 0x1fc:rt:t6:t0 0x204:rt:t6:t0 */
#include "types.h"

extern s32 D_8009473C;
extern s32 D_80094740;
extern s32 D_80094744;
extern s32 D_8012424C;
extern s32 D_801242B8;
extern s32 D_80124324;

extern u8 D_80094784[];
extern u8 D_800947C4[];
extern u8 D_8011FD14[];
extern u8 D_8011FD1C[];
extern u8 D_8011FD28[];

extern u8 D_800B0A26[];
extern u8 D_800B1746;
extern u8 D_801252E8;
extern u8 D_8012531C;
extern s16 D_80094730;
extern s32 ev_flag18;

extern u8 *D_80125258;
extern u8 *D_8012525C;

extern s16 D_8012438C;
extern s16 D_8012438E;
extern s16 D_80124390;
extern s16 D_8012439C;
extern s16 D_8012439E;
extern s16 D_801243A0;

extern void func_80078970();
extern void func_800469F4();
extern void func_80011DFC();

void func_8014F114(void) {
    u8 *q;
    u8 *r;
    u8 gv;
    u32 j;
    u32 v;

    D_8009473C = D_8012424C;
    D_80094740 = D_801242B8;
    D_80094744 = D_80124324;
    func_80078970(D_80094784, D_8011FD14);
    D_801252E8 = 3;
    if ((D_800B0A26[D_800B1746 * 52] & 1) != 0) goto lab_F19C;
    D_8012531C = 0;
    func_80078970(D_800947C4, D_8011FD1C);
    goto lab_F1F0;
lab_F19C:
    D_8012531C = 1;
    func_80078970(D_800947C4, D_8011FD28);
    v = D_800B1746;
    if (v != 7) goto lab_F1DC;
    D_80094730 = 3;
    goto lab_F1F0;
lab_F1DC:
    if (v != 10) goto lab_F1F0;
    D_80094730 = 1;
lab_F1F0:
    gv = D_800B1746;
    q = D_80125258;
    j = D_8012531C * 3;
    ev_flag18 = 14;
    D_8012439C = (q + gv * 6U)[j + 0];
    D_8012439E = (q + (u32)gv * 6U)[j + 1];
    r = D_8012525C;
    D_801243A0 = (q + (gv | 0U) * 6U)[j + 2];
    D_8012438C = (r + (gv ^ 0U) * 6U)[j + 0];
    D_8012438E = (r + (gv * 1U) * 6U)[j + 1];
    D_80124390 = (r + (gv >> 0) * 6U)[j + 2];
    func_800469F4(17584);
    func_80011DFC();
}
