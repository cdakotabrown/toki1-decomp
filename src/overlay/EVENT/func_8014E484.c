/* Address: 0x8014E484 */
/* Size: 716 bytes (179 instructions) */
/* @SymbolStart: 0x8014E460 */
/* @SymbolSize: 752 */
/* @PostProc: swap_instructions --offset1 0x208 --offset2 0x210 */
/* @PostProc: swap_instructions --offset1 0x20c --offset2 0x214 */
/* @PostProc: swap_instructions --offset1 0x224 --offset2 0x220 */
/* @PostProc: swap_instructions --offset1 0x220 --offset2 0x21c */
/* @PostProc: swap_instructions --offset1 0x21c --offset2 0x218 */
/* @PostProc: patch_insn_reg_field 0x8c:rd:v1:v0 0x90:rs:v1:v0 0x94:rt:v0:v1 0xa0:rs:v0:v1 0xa0:rt:v0:v1 0xa8:rs:v0:v1 0xb8:rs:v0:v1 0xf8:rs:v1:v0 0x1f8:rt:a2:t0 0x1fc:rs:a2:t0 0x1fc:rt:a2:t0 0x200:rt:a1:v1 0x204:rs:a2:t0 0x204:rt:a1:v1 0x208:rt:t1:t2 0x20c:rs:t1:t2 0x20c:rt:t1:t2 0x22c:rd:t0:t1 0x230:rd:t2:t3 0x230:rs:t0:t1 0x230:rt:t1:t2 0x234:rd:t3:t4 0x234:rs:t2:t3 0x238:rs:a2:t0 0x238:rt:a1:v1 0x23c:rs:t3:t4 0x23c:rt:t4:t5 0x244:rt:t4:t5 0x24c:rd:t5:t6 0x250:rd:t6:t7 0x250:rs:t5:t6 0x250:rt:t1:t2 0x254:rd:t7:t8 0x254:rs:t6:t7 0x258:rs:a2:t0 0x258:rt:a1:v1 0x25c:rs:t7:t8 0x25c:rt:t8:t9 0x260:rt:t5:t7 0x264:rs:t5:t7 0x264:rt:t5:t7 0x268:rt:t8:t9 0x270:rd:t9:t1 0x274:rd:t0:t3 0x274:rs:t9:t1 0x274:rt:t1:t2 0x278:rd:t2:t4 0x278:rs:t0:t3 0x27c:rs:a2:t0 0x27c:rt:a1:v1 0x280:rs:t2:t4 0x280:rt:t3:t5 0x288:rt:t3:t5 0x290:rd:t4:t6 0x294:rd:t6:t8 0x294:rs:t4:t6 0x294:rt:t5:t7 0x298:rd:t7:t9 0x298:rs:t6:t8 0x29c:rs:a2:t0 0x29c:rt:a1:v1 0x2a0:rs:t7:t9 0x2a0:rt:t8:t1 0x2a8:rt:t8:t1 0x2b0:rd:t9:t2 0x2b4:rd:t1:t3 0x2b4:rs:t9:t2 0x2b4:rt:t5:t7 0x2b8:rd:t0:t4 0x2b8:rs:t1:t3 0x2bc:rs:a2:t0 0x2bc:rt:a1:v1 0x2c0:rs:t0:t4 0x2c0:rt:t2:t5 0x2c8:rt:t2:t5 0x2d0:rd:t3:t6 0x2d4:rd:t4:t8 0x2d4:rs:t3:t6 0x2d4:rt:t5:t7 0x2d8:rd:t6:t9 0x2d8:rs:t4:t8 0x2dc:rs:t6:t9 0x2dc:rt:t7:t1 0x2e4:rt:t7:t1 */
#include "types.h"

extern s32 D_8009473C;
extern s32 D_80094740;
extern s32 D_80094744;
extern s32 D_8012420C;
extern s32 D_80124278;
extern s32 D_801242E4;

extern u8 D_80094784[];
extern u8 D_800947C4[];
extern u8 D_8011FC84[];
extern u8 D_8011FC8C[];
extern u8 D_8011FC94[];
extern u8 D_8011FC9C[];
extern u8 D_8011FCA4[];

extern u8 D_800B0860[];
extern u8 D_800B0A1E[];
extern volatile u8 D_800B1746;
extern u8 D_800B0895;
extern u8 D_800B0896;
extern u8 D_801252E8;
extern u8 D_8012531C;
extern s32 ev_flag18;

extern u8 *D_801251E8;
extern u8 *D_801251EC;

extern s16 D_8012438C;
extern s16 D_8012438E;
extern s16 D_80124390;
extern s16 D_8012439C;
extern s16 D_8012439E;
extern s16 D_801243A0;

extern void func_80078970();
extern void func_800469F4();
extern void func_80011DFC();

void func_8014E484(void) {
    u8 *p;
    u8 *q;
    u8 *r;
    u8 gv;
    u32 j;
    u32 m;

    D_8009473C = D_8012420C;
    D_80094740 = D_80124278;
    D_80094744 = D_801242E4;
    func_80078970(D_80094784, D_8011FC84);
    D_801252E8 = 3;
    p = &D_800B0860[D_800B1746 * 52];
    if (p[447] != 0) goto lab_E548;
    if (D_800B0895 >= 96U) goto lab_E520;
    if (D_800B0895 != 95) goto lab_E548;
    if (D_800B0896 < 12U) goto lab_E548;
lab_E520:
    D_8012531C = 2;
    func_80078970(D_800947C4, D_8011FC8C);
    ev_flag18 = 7;
    goto lab_E5AC;
lab_E548:
    if ((p[446] & 1) != 0) goto lab_E588;
    D_8012531C = 0;
    func_80078970(D_800947C4, D_8011FC94);
    ev_flag18 = 6;
    goto lab_E5AC;
lab_E588:
    D_8012531C = 1;
    func_80078970(D_800947C4, D_8011FC9C);
    ev_flag18 = 6;
lab_E5AC:
    if (D_800B1746 != 7) goto lab_E648;
    ev_flag18 = 6;
    func_80078970(D_800947C4, D_8011FCA4);
    m = D_800B0A1E[D_800B1746 * 52] % 3U;
    if (m != 0) goto lab_E628;
    D_8012531C = 0;
    goto lab_E648;
lab_E628:
    if (m != 1) goto lab_E640;
    D_8012531C = 1;
    goto lab_E648;
lab_E640:
    D_8012531C = 2;
lab_E648:
    gv = D_800B1746;
    q = D_801251E8;
    j = D_8012531C * 3;
    D_8012439C = (q + gv * 9U)[j + 0];
    D_8012439E = (q + (u32)gv * 9U)[j + 1];
    r = D_801251EC;
    D_801243A0 = (q + (gv | 0U) * 9U)[j + 2];
    D_8012438C = (r + (gv ^ 0U) * 9U)[j + 0];
    D_8012438E = (r + (gv * 1U) * 9U)[j + 1];
    D_80124390 = (r + (gv >> 0) * 9U)[j + 2];
    func_800469F4(17641);
    func_80011DFC();
}
