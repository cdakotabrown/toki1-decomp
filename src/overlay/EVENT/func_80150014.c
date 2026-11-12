/* @SymbolStart: 0x8014FFD4 */
/* @SymbolSize: 788 */
/* @PostProc: patch_insn_reg_field 0x21c:rt:t3:t4 0x220:rs:t3:t4 0x220:rt:a2:t4 0x224:rt:a1:v1 0x228:rs:a2:t4 0x228:rt:a1:v1 0x240:rd:t4:t3 0x244:rd:t4:t3 0x244:rs:t4:t3 0x248:rs:t4:t3 0x258:rt:t4:t3 0x25c:rs:a2:t4 0x25c:rt:a1:v1 0x278:rt:t4:t3 0x27c:rs:a2:t4 0x27c:rt:a1:v1 0x280:rt:t4:t3 0x284:rt:t0:t1 0x288:rs:t0:t1 0x288:rt:t0:t1 0x28c:rt:t4:t3 0x2a0:rs:a2:t4 0x2a0:rt:a1:v1 0x2b4:rd:t3:t0 0x2b8:rd:t1:t2 0x2b8:rs:t3:t0 0x2b8:rt:t0:t1 0x2bc:rd:t2:t3 0x2bc:rs:t1:t2 0x2c0:rs:a2:t4 0x2c0:rt:a1:v1 0x2c4:rs:t2:t3 0x2c4:rt:t4:t5 0x2cc:rt:t4:t5 0x2d4:rd:t5:t6 0x2d8:rd:t6:t7 0x2d8:rs:t5:t6 0x2d8:rt:t0:t1 0x2dc:rd:t7:t8 0x2dc:rs:t6:t7 0x2e0:rs:a2:t4 0x2e0:rt:a1:v1 0x2e4:rs:t7:t8 0x2e4:rt:t8:t9 0x2ec:rt:t8:t9 0x2f4:rd:t9:t0 0x2f8:rd:t3:t2 0x2f8:rs:t9:t0 0x2f8:rt:t0:t1 0x2fc:rd:t1:t3 0x2fc:rs:t3:t2 0x300:rs:t1:t3 0x300:rt:t2:t5 0x308:rt:t2:t5 */
/* Address: 0x8014FFD4 (cross-prologue head; .sym draws the start at 0x80150014) */
/* Size: 788 bytes (197 instructions) */
#include "types.h"

extern s32 D_80124208;
extern s32 D_80124274;
extern s32 D_801242E0;
extern s32 D_8009473C;
extern s32 D_80094740;
extern s32 D_80094744;

extern u8 D_800B0895;
extern u8 D_800B0896;
extern u8 D_800B1746;
extern u8 D_801252E8;
extern u8 D_80125318;

extern s32 ev_flag18;

extern u8 D_80094784[];
extern u8 D_800947C4[];
extern u8 D_8011FE0C[];
extern u8 D_8011FE1C[];

extern u8 *D_801251E0;
extern u8 *D_801251E4;

extern s16 D_8012438C;
extern s16 D_8012438E;
extern s16 D_80124390;
extern s16 D_8012439C;
extern s16 D_8012439E;
extern s16 D_801243A0;

extern void func_80078970(u8 *, u8 *);
extern void func_800469F4(s32);
extern void func_80011DFC(void);

void func_80150014(void) {
    u32 sel;
    u32 slot;
    u32 sub;
    u8 gv;
    u8 *pa;
    u8 *pb;

    D_8009473C = D_80124208;
    D_80094740 = D_80124274;
    D_80094744 = D_801242E0;
    sel = D_800B0895;
    D_801252E8 = 0;

    if (sel == 95) {
        slot = D_800B0896;
        if (slot >= 9 && slot < 12) {
            D_80125318 = 2;
        } else {
            D_80125318 = 3;
        }
    } else if (sel == 96) {
        slot = D_800B0896;
        if (slot != 0 && slot < 3) {
            D_80125318 = 3;
        } else if (slot >= 3 && slot < 6) {
            D_80125318 = 4;
        } else if (slot >= 6 && slot < 9) {
            D_80125318 = 5;
        } else if (slot >= 9 && slot < 12) {
            D_80125318 = 6;
        } else {
            D_80125318 = 7;
        }
    } else if (sel == 97) {
        slot = D_800B0896;
        if (slot != 0 && slot < 3) {
            D_80125318 = 7;
        } else if (slot >= 3 && slot < 6) {
            D_80125318 = 8;
        } else if (slot >= 6 && slot < 9) {
            D_80125318 = 9;
        } else if (slot >= 9 && slot < 12) {
            D_80125318 = 10;
        } else {
            D_80125318 = 11;
        }
    } else {
        D_80125318 = 11;
    }

    ev_flag18 = -1;
    func_80078970(D_80094784, D_8011FE0C);
    func_80078970(D_800947C4, D_8011FE1C);

    gv = D_800B1746;
    pa = D_801251E0;
    sub = D_80125318 * 3;
    D_8012439C = (pa + gv * 36U)[sub + 0];
    D_8012439E = (pa + (u32)gv * 36U)[sub + 1];
    pb = D_801251E4;
    D_801243A0 = (pa + (gv | 0U) * 36U)[sub + 2];
    D_8012438C = (pb + (gv ^ 0U) * 36U)[sub + 0];
    D_8012438E = (pb + (gv * 1U) * 36U)[sub + 1];
    D_80124390 = (pb + (gv >> 0) * 36U)[sub + 2];
    func_800469F4(17366);
    func_80011DFC();
}
