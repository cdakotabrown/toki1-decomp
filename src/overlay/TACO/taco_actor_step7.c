/* @PostProc: patch_insn_reg_field 0x8c:rs:t3:a1 0x8c:rt:a1:t3 0xe0:rs:t9:a2 0xe0:rt:a2:t9 */
/* @SymbolStart: 0x8015690C */
/* @SymbolSize: 336 */
/* Address: 0x8015691C (real entry 0x8015690C) */
#include "types.h"

extern void taco_actor_exit(void);

extern u8 *taco_actor_rec;

#define AC(n)   (*(s16 *)(taco_actor_rec + a0 * 136 + (n)))
#define BASE(n) (*(s16 *)(taco_actor_rec + (n)))

void taco_actor_step7(s32 a0)
{
    AC(120) = AC(120) + 250;
    AC(116) = AC(116) + (BASE(116) - AC(116)) / AC(124);
    AC(118) = AC(118) + (BASE(118) - AC(118)) / AC(124);

    if (AC(126) == 0) {
        AC(104) = AC(104) + 200;
    } else {
        AC(104) = AC(104) - 200;
    }

    if (AC(120) >= 2001) {
        taco_actor_exit();
    }
}
