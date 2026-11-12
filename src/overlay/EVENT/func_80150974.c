/* @SymbolStart: 0x80150948 */
/* @SymbolSize: 452 */
/* @PostProc: patch_insn_reg_field 0xb0:rt:t6:t7 0xc8:rs:t6:t7 0xc8:rt:a2:t7 0xcc:rt:a1:v1 0xd0:rs:a2:t7 0xd0:rt:a1:v1 0xe8:rd:t7:t6 0xec:rd:t7:t6 0xec:rs:t7:t6 0xfc:rs:t7:t6 0x108:rt:t7:t6 0x10c:rs:a2:t7 0x10c:rt:a1:v1 0x128:rt:t7:t6 0x12c:rs:a2:t7 0x12c:rt:a1:v1 0x130:rt:t7:t6 0x134:rt:t3:t4 0x138:rs:t3:t4 0x138:rt:t3:t4 0x13c:rt:t7:t6 0x150:rs:a2:t7 0x150:rt:a1:v1 0x164:rd:t6:t3 0x168:rd:t4:t5 0x168:rs:t6:t3 0x168:rt:t3:t4 0x16c:rd:t5:t6 0x16c:rs:t4:t5 0x170:rs:a2:t7 0x170:rt:a1:v1 0x174:rs:t5:t6 0x174:rt:t7:t8 0x17c:rt:t7:t8 0x184:rd:t8:t9 0x188:rd:t9:t0 0x188:rs:t8:t9 0x188:rt:t3:t4 0x18c:rd:t0:t1 0x18c:rs:t9:t0 0x190:rs:a2:t7 0x190:rt:a1:v1 0x194:rs:t0:t1 0x194:rt:t1:t2 0x19c:rt:t1:t2 0x1a4:rd:t2:t3 0x1a8:rd:t6:t5 0x1a8:rs:t2:t3 0x1a8:rt:t3:t4 0x1ac:rd:t4:t6 0x1ac:rs:t6:t5 0x1b0:rs:t4:t6 0x1b0:rt:t5:t8 0x1b8:rt:t5:t8 */
/* Address: 0x80150948 (cross-prologue head; .sym draws the start at 0x80150974) */
/* Size: 452 bytes (113 instructions) including the absorbed head */
#include "types.h"

extern u8 D_800B1746;

extern s32 D_80124240;
extern s32 D_801242AC;
extern s32 D_80124318;
extern s32 D_8009473C;
extern s32 D_80094740;
extern s32 D_80094744;

extern u8 D_801252E8;
extern u8 D_80094784[];
extern u8 D_8011FE80[];
extern u8 D_800947C4[];
extern u8 D_8011FE8C[];

extern u8 D_800B0A37[];
extern u8 D_8012531C;
extern s32 ev_flag18;
extern u8 *D_80125240;
extern u8 *D_80125244;

extern s16 D_8012438C;
extern s16 D_8012438E;
extern s16 D_80124390;
extern s16 D_8012439C;
extern s16 D_8012439E;
extern s16 D_801243A0;

extern void func_80078970();
extern void func_800469F4();
extern void func_80011DFC();

void func_80150974(void) {
    u32 ch;
    u32 off;
    u8 gv;
    u8 *pa;
    u8 *pb;

    D_8009473C = D_80124240;
    D_80094740 = D_801242AC;
    D_80094744 = D_80124318;

    D_801252E8 = 3;
    func_80078970(D_80094784, D_8011FE80);
    func_80078970(D_800947C4, D_8011FE8C);

    ch = D_800B1746;
    if (!(D_800B0A37[ch * 52] & 1)) {
        D_8012531C = 0;
    } else {
        D_8012531C = 1;
    }

    gv = D_800B1746;
    pa = D_80125240;
    off = D_8012531C * 3U;
    ev_flag18 = 31;

    D_8012439C = (pa + gv * 6U)[off + 0];
    D_8012439E = (pa + (u32)gv * 6U)[off + 1];
    D_801243A0 = (pa + (gv | 0U) * 6U)[off + 2];
    pb = D_80125244;
    D_8012438C = (pb + (gv ^ 0U) * 6U)[off + 0];
    D_8012438E = (pb + (gv * 1U) * 6U)[off + 1];
    D_80124390 = (pb + (gv >> 0) * 6U)[off + 2];

    func_800469F4(17604);
    func_80011DFC();
}
