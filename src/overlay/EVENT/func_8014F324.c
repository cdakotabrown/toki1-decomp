/* Address: 0x8014F324 */
/* Size: 508 bytes (127 instructions) */
/* @SymbolStart: 0x8014F300 */
/* @SymbolSize: 544 */
/* @PostProc: patch_insn_reg_field 0xf4:rt:t7:t8 0x12c:rs:t7:t8 0x12c:rt:a2:t8 0x130:rt:a1:v1 0x134:rs:a2:t8 0x134:rt:a1:v1 0x14c:rd:t8:t7 0x150:rd:t8:t7 0x150:rs:t8:t7 0x154:rs:t8:t7 0x164:rt:t8:t7 0x168:rs:a2:t8 0x168:rt:a1:v1 0x184:rt:t8:t7 0x188:rs:a2:t8 0x188:rt:a1:v1 0x18c:rt:t8:t7 0x190:rt:t4:t5 0x194:rs:t4:t5 0x194:rt:t4:t5 0x198:rt:t8:t7 0x1ac:rs:a2:t8 0x1ac:rt:a1:v1 0x1c0:rd:t7:t4 0x1c4:rd:t5:t6 0x1c4:rs:t7:t4 0x1c4:rt:t4:t5 0x1c8:rd:t6:t7 0x1c8:rs:t5:t6 0x1cc:rs:a2:t8 0x1cc:rt:a1:v1 0x1d0:rs:t6:t7 0x1d0:rt:t8:t9 0x1d8:rt:t8:t9 0x1e0:rd:t9:t0 0x1e4:rd:t0:t1 0x1e4:rs:t9:t0 0x1e4:rt:t4:t5 0x1e8:rd:t1:t2 0x1e8:rs:t0:t1 0x1ec:rs:a2:t8 0x1ec:rt:a1:v1 0x1f0:rs:t1:t2 0x1f0:rt:t2:t3 0x1f8:rt:t2:t3 0x200:rd:t3:t4 0x204:rd:t7:t6 0x204:rs:t3:t4 0x204:rt:t4:t5 0x208:rd:t5:t7 0x208:rs:t7:t6 0x20c:rs:t5:t7 0x20c:rt:t6:t9 0x214:rt:t6:t9 */
#include "types.h"

extern s32 D_8009473C;
extern s32 D_80094740;
extern s32 D_80094744;
extern s32 D_80124214;
extern s32 D_80124280;
extern s32 D_801242EC;

extern u8 D_80094784[];
extern u8 D_800947C4[];
extern u8 D_8011FD34[];
extern u8 D_8011FD44[];
extern u8 D_8011FD5C[];

extern u8 D_800B0A27[];
extern u8 D_800B1746;
extern u8 D_800B0895;
extern u8 D_800B0896;
extern u8 D_801252E8;
extern u8 D_80125300;
extern u8 D_8012531C;
extern s32 ev_flag18;

extern u8 *D_801251F8;
extern u8 *D_801251FC;

extern s16 D_8012438C;
extern s16 D_8012438E;
extern s16 D_80124390;
extern s16 D_8012439C;
extern s16 D_8012439E;
extern s16 D_801243A0;

extern void func_80078970();
extern void func_800469F4();
extern void func_80011DFC();

void func_8014F324(void) {
    u8 *q;
    u8 *r;
    u8 gv;
    u32 j;
    u32 v;

    D_8009473C = D_80124214;
    D_80094740 = D_80124280;
    D_80094744 = D_801242EC;
    func_80078970(D_80094784, D_8011FD34);
    D_801252E8 = 0;
    if ((D_800B0A27[D_800B1746 * 52] & 1) != 0) goto lab_F3A8;
    D_8012531C = 0;
    func_80078970(D_800947C4, D_8011FD44);
    goto lab_F3C8;
lab_F3A8:
    D_8012531C = 1;
    func_80078970(D_800947C4, D_8011FD5C);
lab_F3C8:
    v = D_800B0895;
    ev_flag18 = 15;
    if (v >= 97U) goto lab_F404;
    if (v < 96U) goto lab_F414;
    if (D_800B0896 < 9U) goto lab_F414;
lab_F404:
    D_80125300 = 1;
    goto lab_F41C;
lab_F414:
    D_80125300 = 0;
lab_F41C:
    gv = D_800B1746;
    q = D_801251F8;
    j = D_8012531C * 3;
    D_8012439C = (q + gv * 12U)[j + 0];
    D_8012439E = (q + (u32)gv * 12U)[j + 1];
    r = D_801251FC;
    D_801243A0 = (q + (gv | 0U) * 12U)[j + 2];
    D_8012438C = (r + (gv ^ 0U) * 12U)[j + 0];
    D_8012438E = (r + (gv * 1U) * 12U)[j + 1];
    D_80124390 = (r + (gv >> 0) * 12U)[j + 2];
    func_800469F4(17474);
    func_80011DFC();
}
