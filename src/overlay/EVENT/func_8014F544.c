/* Address: 0x8014F544 */
/* Size: 440 bytes (110 instructions) */
/* @SymbolStart: 0x8014F520 */
/* @SymbolSize: 476 */
/* @PostProc: patch_insn_reg_field 0xdc:rt:t6:t7 0xe0:rs:t6:t7 0xe0:rt:a2:t7 0xe4:rt:a1:v1 0xe8:rs:a2:t7 0xe8:rt:a1:v1 0x100:rd:t7:t6 0x104:rd:t7:t6 0x104:rs:t7:t6 0x114:rs:t7:t6 0x120:rt:t7:t6 0x124:rs:a2:t7 0x124:rt:a1:v1 0x140:rt:t7:t6 0x144:rs:a2:t7 0x144:rt:a1:v1 0x148:rt:t7:t6 0x14c:rt:t3:t4 0x150:rs:t3:t4 0x150:rt:t3:t4 0x154:rt:t7:t6 0x168:rs:a2:t7 0x168:rt:a1:v1 0x17c:rd:t6:t3 0x180:rd:t4:t5 0x180:rs:t6:t3 0x180:rt:t3:t4 0x184:rd:t5:t6 0x184:rs:t4:t5 0x188:rs:a2:t7 0x188:rt:a1:v1 0x18c:rs:t5:t6 0x18c:rt:t7:t8 0x194:rt:t7:t8 0x19c:rd:t8:t9 0x1a0:rd:t9:t0 0x1a0:rs:t8:t9 0x1a0:rt:t3:t4 0x1a4:rd:t0:t1 0x1a4:rs:t9:t0 0x1a8:rs:a2:t7 0x1a8:rt:a1:v1 0x1ac:rs:t0:t1 0x1ac:rt:t1:t2 0x1b4:rt:t1:t2 0x1bc:rd:t2:t3 0x1c0:rd:t6:t5 0x1c0:rs:t2:t3 0x1c0:rt:t3:t4 0x1c4:rd:t4:t6 0x1c4:rs:t6:t5 0x1c8:rs:t4:t6 0x1c8:rt:t5:t8 0x1d0:rt:t5:t8 */
#include "types.h"

extern s32 D_8009473C;
extern s32 D_80094740;
extern s32 D_80094744;
extern s32 D_80124204;
extern s32 D_80124270;
extern s32 D_801242DC;

extern u8 D_80094784[];
extern u8 D_800947C4[];
extern u8 D_8011FD74[];
extern u8 D_8011FD84[];
extern u8 D_8011FD94[];

extern u8 D_800B0A29[];
extern u8 D_800B1746;
extern u8 D_801252E8;
extern u8 D_8012531C;
extern s32 ev_flag18;

extern u8 *D_801251D8;
extern u8 *D_801251DC;

extern s16 D_8012438C;
extern s16 D_8012438E;
extern s16 D_80124390;
extern s16 D_8012439C;
extern s16 D_8012439E;
extern s16 D_801243A0;

extern void func_80078970();
extern void func_800469F4();
extern void func_80011DFC();

void func_8014F544(void) {
    u8 *q;
    u8 *r;
    u8 gv;
    u32 j;

    D_8009473C = D_80124204;
    D_80094740 = D_80124270;
    D_80094744 = D_801242DC;
    func_80078970(D_80094784, D_8011FD74);
    D_801252E8 = 3;
    if ((D_800B0A29[D_800B1746 * 52] & 1) != 0) goto lab_F5CC;
    D_8012531C = 0;
    func_80078970(D_800947C4, D_8011FD84);
    goto lab_F5EC;
lab_F5CC:
    D_8012531C = 1;
    func_80078970(D_800947C4, D_8011FD94);
lab_F5EC:
    gv = D_800B1746;
    q = D_801251D8;
    j = D_8012531C * 3;
    ev_flag18 = 17;
    D_8012439C = (q + gv * 6U)[j + 0];
    D_8012439E = (q + (u32)gv * 6U)[j + 1];
    D_801243A0 = (q + (gv | 0U) * 6U)[j + 2];
    r = D_801251DC;
    D_8012438C = (r + (gv ^ 0U) * 6U)[j + 0];
    D_8012438E = (r + (gv * 1U) * 6U)[j + 1];
    D_80124390 = (r + (gv >> 0) * 6U)[j + 2];
    func_800469F4(17821);
    func_80011DFC();
}
