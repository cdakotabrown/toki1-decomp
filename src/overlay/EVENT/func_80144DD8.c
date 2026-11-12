/* @PostProc: patch_insn_reg_field 0x78:rt:t9:t5 0x7c:rt:t3:t6 0x80:rt:t5:t9 0x84:rd:t4:t7 0x84:rs:t9:t5 0x84:rt:t3:t6 0x88:rt:t4:t7 0x8c:rt:t6:t3 0x94:rd:t7:t4 0x94:rs:t5:t9 0x94:rt:t6:t3 0xa0:rt:t7:t4 */
/* @SymbolStart: 0x80144DD4 */
/* @SymbolSize: 380 */
/* Address: 0x80144DD4 (hoisted lui; .sym draws the start at 0x80144DD8) */
/* Size: 380 bytes (95 instructions) including the absorbed head */
#include "types.h"

extern u32 D_8012398C;
extern u32 D_800B1AE4;
extern u8 D_800E9620[];

extern s32 func_800789A0(void);
extern void func_800433D0(s32);

void func_80144DD8(void) {
    u32 counter;
    s32 alive;
    s16 i;

    counter = D_8012398C;

    for (i = 0; i < 10; i++) {
        if (counter >= i * 30) {
            *(s16 *)&D_800E9620[i * 68 + 6778] = -1;
            *(s16 *)&D_800E9620[i * 68 + 6782] = 1;
        }
        *(s32 *)&D_800E9620[i * 68 + 6772] +=
            *(s32 *)&D_800E9620[i * 68 + 6780];
        *(s32 *)&D_800E9620[i * 68 + 6768] +=
            *(s32 *)&D_800E9620[i * 68 + 6776];
        if (*(s16 *)&D_800E9620[i * 68 + 6774] >= 161) {
            D_800E9620[i * 68 + 6735] &= ~128;
        }
    }

    alive = 0;
    for (i = 0; i < 10; i++) {
        if (D_800E9620[i * 68 + 6735] & 0x80) {
            alive = 1;
        }
    }

    if (alive == 0) {
        if (D_800B1AE4 % 60 == func_800789A0() % 60) {
            func_800433D0((u16)(func_800789A0() % 7 + 1281));
        }
    }

    D_8012398C = D_8012398C + 1;
}
