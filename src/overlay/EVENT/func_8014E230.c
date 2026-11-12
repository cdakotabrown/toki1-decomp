/* Address: 0x8014E230 */
/* Size: 560 bytes (140 instructions) */
/* @SymbolStart: 0x8014E20C */
/* @SymbolSize: 596 */
/* @PostProc: patch_insn_reg_field 0x8c:rd:v1:v0 0x90:rs:v1:v0 0x94:rt:v0:v1 0xa0:rs:v0:v1 0xa0:rt:v0:v1 0xa8:rs:v0:v1 0xb8:rs:v0:v1 0xf8:rs:v1:v0 0x15c:rt:t2:t3 0x160:rs:t2:t3 0x160:rt:a2:t3 0x164:rt:a1:v1 0x168:rs:a2:t3 0x168:rt:a1:v1 0x180:rd:t3:t2 0x184:rd:t3:t2 0x184:rs:t3:t2 0x188:rs:t3:t2 0x198:rt:t3:t2 0x19c:rs:a2:t3 0x19c:rt:a1:v1 0x1b8:rt:t3:t2 0x1bc:rs:a2:t3 0x1bc:rt:a1:v1 0x1c0:rt:t3:t2 0x1c4:rt:t9:t0 0x1c8:rs:t9:t0 0x1c8:rt:t9:t0 0x1cc:rt:t3:t2 0x1e0:rs:a2:t3 0x1e0:rt:a1:v1 0x1f4:rd:t2:t9 0x1f8:rd:t0:t1 0x1f8:rs:t2:t9 0x1f8:rt:t9:t0 0x1fc:rd:t1:t2 0x1fc:rs:t0:t1 0x200:rs:a2:t3 0x200:rt:a1:v1 0x204:rs:t1:t2 0x204:rt:t3:t4 0x20c:rt:t3:t4 0x214:rd:t4:t5 0x218:rd:t5:t6 0x218:rs:t4:t5 0x218:rt:t9:t0 0x21c:rd:t6:t7 0x21c:rs:t5:t6 0x220:rs:a2:t3 0x220:rt:a1:v1 0x224:rs:t6:t7 0x224:rt:t7:t8 0x22c:rt:t7:t8 0x234:rd:t8:t9 0x238:rd:t2:t1 0x238:rs:t8:t9 0x238:rt:t9:t0 0x23c:rd:t0:t2 0x23c:rs:t2:t1 0x240:rs:t0:t2 0x240:rt:t1:t4 0x248:rt:t1:t4 */
#include "types.h"

extern s32 D_8009473C;
extern s32 D_80094740;
extern s32 D_80094744;
extern s32 D_80124248;
extern s32 D_801242B4;
extern s32 D_80124320;

extern u8 D_80094784[];
extern u8 D_800947C4[];
extern u8 D_8011FC5C[];
extern u8 D_8011FC64[];
extern u8 D_8011FC74[];
extern u8 D_8011FC7C[];

extern u8 D_800B0860[];
extern u8 D_800B1746;
extern u8 D_800B0895;
extern u8 D_800B0896;
extern u8 D_801252E8;
extern u8 D_8012531C;
extern s32 ev_flag18;

extern u8 *D_80125250;
extern u8 *D_80125254;

extern s16 D_8012438C;
extern s16 D_8012438E;
extern s16 D_80124390;
extern s16 D_8012439C;
extern s16 D_8012439E;
extern s16 D_801243A0;

extern void func_80078970();
extern void func_800469F4();
extern void func_80011DFC();

void func_8014E230(void) {
    u8 *p;
    u8 *q;
    u8 *r;
    u8 gv;
    u32 j;

    D_8009473C = D_80124248;
    D_80094740 = D_801242B4;
    D_80094744 = D_80124320;
    func_80078970(D_80094784, D_8011FC5C);
    p = &D_800B0860[D_800B1746 * 52];
    D_801252E8 = 3;
    if (p[445] != 0) goto lab_E2F4;
    if (D_800B0895 >= 97U) goto lab_E2CC;
    if (D_800B0895 != 96) goto lab_E2F4;
    if (D_800B0896 < 3U) goto lab_E2F4;
lab_E2CC:
    D_8012531C = 2;
    func_80078970(D_800947C4, D_8011FC64);
    ev_flag18 = 5;
    goto lab_E358;
lab_E2F4:
    if ((p[444] & 1) != 0) goto lab_E334;
    D_8012531C = 0;
    func_80078970(D_800947C4, D_8011FC74);
    ev_flag18 = 4;
    goto lab_E358;
lab_E334:
    D_8012531C = 1;
    func_80078970(D_800947C4, D_8011FC7C);
    ev_flag18 = 4;
lab_E358:
    gv = D_800B1746;
    q = D_80125250;
    j = D_8012531C * 3;
    D_8012439C = (q + gv * 9U)[j + 0];
    D_8012439E = (q + (u32)gv * 9U)[j + 1];
    r = D_80125254;
    D_801243A0 = (q + (gv | 0U) * 9U)[j + 2];
    D_8012438C = (r + (gv ^ 0U) * 9U)[j + 0];
    D_8012438E = (r + (gv * 1U) * 9U)[j + 1];
    D_80124390 = (r + (gv >> 0) * 9U)[j + 2];
    func_800469F4(17661);
    func_80011DFC();
}
