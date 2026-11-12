/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=occ:2 */
/* @PostProc: swap_instructions --offset1 0x174 --offset2 0x180 */
/* @PostProc: swap_instructions --offset1 0x17c --offset2 0x184 */
/* @PostProc: swap_instructions --offset1 0x188 --offset2 0x194 */
/* @PostProc: swap_instructions --offset1 0x18c --offset2 0x194 */
/* @PostProc: swap_instructions --offset1 0x190 --offset2 0x194 */
/* @PostProc: patch_insn_reg_field 0x98:rt:a3:v1 0x9c:rs:v0:v1 0x9c:rt:a3:v0 0xa4:rt:a3:v1 0x16c:rt:a1:t4 0x170:rs:a1:t4 0x170:rt:a1:t4 0x174:rt:t5:t6 0x178:rs:a1:t4 0x178:rt:a3:v1 0x17c:rs:t5:t6 0x17c:rt:t5:t6 0x19c:rd:t4:t5 0x1a0:rd:t6:t7 0x1a0:rs:t4:t5 0x1a0:rt:t5:t6 0x1a4:rd:t7:t8 0x1a4:rs:t6:t7 0x1a8:rs:a1:t4 0x1a8:rt:a3:v1 0x1ac:rs:t7:t8 0x1ac:rt:t8:t9 0x1b4:rt:t8:t9 0x1bc:rd:t9:t0 0x1c0:rd:t0:t1 0x1c0:rs:t9:t0 0x1c0:rt:t5:t6 0x1c4:rd:t1:t2 0x1c4:rs:t0:t1 0x1c8:rs:a1:t4 0x1c8:rt:a3:v1 0x1cc:rs:t1:t2 0x1cc:rt:t2:t3 0x1d0:rt:t9:t1 0x1d4:rs:t9:t1 0x1d4:rt:t9:t1 0x1d8:rt:t2:t3 0x1e0:rd:t3:t5 0x1e4:rd:t4:t7 0x1e4:rs:t3:t5 0x1e4:rt:t5:t6 0x1e8:rd:t6:t8 0x1e8:rs:t4:t7 0x1ec:rs:a1:t4 0x1ec:rt:a3:v1 0x1f0:rs:t6:t8 0x1f0:rt:t7:t9 0x1f8:rt:t7:t9 0x200:rd:t8:t0 0x204:rd:t0:t2 0x204:rs:t8:t0 0x204:rt:t9:t1 0x208:rd:t1:t3 0x208:rs:t0:t2 0x20c:rs:a1:t4 0x20c:rt:a3:v1 0x210:rs:t1:t3 0x210:rt:t2:t5 0x218:rt:t2:t5 0x220:rd:t3:t6 0x224:rd:t5:t7 0x224:rs:t3:t6 0x224:rt:t9:t1 0x228:rd:t4:t8 0x228:rs:t5:t7 0x22c:rs:a1:t4 0x22c:rt:a3:v1 0x230:rs:t4:t8 0x230:rt:t6:t9 0x238:rt:t6:t9 0x240:rd:t7:t0 0x244:rd:t8:t2 0x244:rs:t7:t0 0x244:rt:t9:t1 0x248:rd:t0:t3 0x248:rs:t8:t2 0x24c:rs:t0:t3 0x24c:rt:t1:t5 0x254:rt:t1:t5 */
/* @SymbolStart: 0x8014FA40 */
/* @SymbolSize: 608 */
/* Address: 0x8014FA40 (cross-prologue head; .sym draws the start at 0x8014FA64) */
/* Size: 608 bytes (152 instructions) */
#include "types.h"

extern s32 D_80124244;
extern s32 D_801242B0;
extern s32 D_8012431C;
extern s32 D_8009473C;
extern s32 D_80094740;
extern s32 D_80094744;

extern s32 ev_flag18;

extern u8 D_800B0896;
extern volatile u8 D_800B1746;
extern u8 D_800B0A32[];
extern u8 D_800B0A33[];
extern u8 D_801252E8;
extern u8 D_8012531C;
extern u8 D_8012532C;

extern u8 D_80094784[];
extern u8 D_800947C4[];
extern u8 D_8011FDE0[];
extern u8 D_8011FDEC[];

extern u8 *D_80125248;
extern u8 *D_8012524C;

extern s16 D_8012438C;
extern s16 D_8012438E;
extern s16 D_80124390;
extern s16 D_8012439C;
extern s16 D_8012439E;
extern s16 D_801243A0;

extern void func_80078970(u8 *, u8 *);
extern void func_800469F4(s32);
extern void func_80011DFC(void);

void func_8014FA64(void) {
    u32 slot;
    u32 use33;
    u32 sub;
    u8 gv;
    u8 *pa;
    u8 *pb;

    D_8009473C = D_80124244;
    D_80094740 = D_801242B0;
    D_80094744 = D_8012431C;
    func_80078970(D_80094784, D_8011FDE0);
    func_80078970(D_800947C4, D_8011FDEC);

    slot = D_800B0896;
    D_801252E8 = 3;
    if (slot == 2 || slot == 3 || slot == 12) {
        use33 = 1;
    } else {
        use33 = 0;
    }
    D_8012532C = use33;

    if (use33 != 0) {
        if ((D_800B0A33[D_800B1746 * 52] & 1) == 0) {
            D_8012531C = 2;
        } else {
            D_8012531C = 3;
        }
        ev_flag18 = 27;
    } else {
        if ((D_800B0A32[D_800B1746 * 52] & 1) == 0) {
            D_8012531C = 0;
        } else {
            D_8012531C = 1;
        }
        ev_flag18 = 26;
    }

    gv = D_800B1746;
    pa = D_80125248;
    sub = D_8012531C * 3;
    D_8012439C = (pa + gv * 12U)[sub + 0];
    D_8012439E = (pa + (u32)gv * 12U)[sub + 1];
    pb = D_8012524C;
    D_801243A0 = (pa + (gv | 0U) * 12U)[sub + 2];
    D_8012438C = (pb + (gv ^ 0U) * 12U)[sub + 0];
    D_8012438E = (pb + (gv * 1U) * 12U)[sub + 1];
    D_80124390 = (pb + (gv >> 0) * 12U)[sub + 2];
    func_800469F4(17764);
    func_80011DFC();
}
