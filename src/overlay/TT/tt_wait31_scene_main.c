/* @SymbolSize: 100 */
/* @PostProc: patch_insn_reg_field 0x14:rt:v0:v1 0x18:rt:v1:v0 0x1c:rs:v0:v1 0x20:rs:v1:v0 0x20:rt:v1:v0 0x40:rs:v0:v1 0x48:rs:v0:v1 0x4c:rs:v0:v1 0x4c:rt:a0:t8 0x50:rt:t9:t1 0x54:rs:a0:t8 0x54:rt:a0:t9 0x58:rs:a0:t9 0x58:rt:t8:t0 0x5c:rs:t8:t0 0x64:rs:v0:v1 0x64:rt:a0:t9 0x68:rs:v1:v0 0x68:rt:t9:t1 */
#include "types.h"

extern u8 *tt_step_wk;
extern u8 *tt_game_wk;

void tt_wait31_scene_main(void)
{
    u8 *a;
    u8 *b;
    u16 m;
    u32 n;

    a = tt_step_wk;
    b = tt_game_wk;
    m = *(u16 *)(a + 2);
    switch (m) {
    case 0:
        *(u16 *)(a + 38) = 0;
        *(u16 *)(a + 2) = 64;
        break;
    case 64:
        n = *(u16 *)(a + 38) + 1;
        *(u16 *)(a + 38) = n;
        if ((n & 0xFFFFU) >= 31U) {
            *(b + 85) = 1;
        }
        break;
    }
}
