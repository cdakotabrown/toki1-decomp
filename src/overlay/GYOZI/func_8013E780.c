/* func_8013E780 -- GYOZI overlay, Address: 0x8013E780, Size: 236 bytes */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:21 */
/* @PostProc: patch_stack_complete --old 128 --new 136 */
/* @PostProc: remap_stack_slots --from 36 --to 40 */
/* @PostProc: patch_insn_reg_field 0xac:rt:t7:t8 0xb4:rs:t7:t8 0xcc:rd:t6:t7 0xd0:rt:t6:t7 */
#include "types.h"

typedef s32 (*FuncPtr)(s32);

typedef struct {
    FuncPtr v[23];
} Table23;

extern u8 D_800F647A;
extern Table23 D_801486E8;

extern u8 D_801317EB;
extern volatile u8 D_8012B8C7;
extern volatile s16 D_8012B8C8;
extern volatile s16 D_8012B8D6;
extern volatile s16 D_8012B8D8;

s32 func_8013E780(void)
{
    Table23 table;
    s16 counter;
    u8 *pg;

    table = D_801486E8;

    table.v[D_800F647A](128);

    D_8012B8C7 = D_801317EB;

    pg = (u8 *)0x80130000;
    if (D_8012B8D8 != 0) {
        return;
    }
    if (*(volatile s16 *)(pg - 18232) != 0) {
        return;
    }

    counter = (s16)(D_8012B8D6 + 1);
    D_8012B8D6 = counter;
    if (counter >= 3) {
        D_8012B8D6 = 0;
        counter = 0;
    }
    return counter;
}
