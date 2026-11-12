/* @PostProc: patch_insn_reg_field 0x18:rt:v0:v1 0x20:rs:v0:v1 0x20:rt:v0:v1 0x24:rt:v1:v0 0x2c:rs:v1:v0 0x2c:rt:v1:v0 0x3c:rs:v0:v1 0x3c:rt:v0:v1 0x40:rt:a2:a1 0x48:rs:v0:v1 0x48:rt:a2:a1 0x4c:rs:v0:v1 0x50:rt:a2:a1 0x54:rs:v1:v0 0x64:rt:v0:v1 0x6c:rs:v0:v1 0x74:rs:v0:v1 0x80:rs:v1:v0 0x80:rt:a2:a1 0x84:rs:v1:v0 0x84:rt:v0:v1 0x8c:rs:v0:v1 0x94:rs:v0:v1 0x9c:rs:v1:v0 0x9c:rt:a2:a1 0xa0:rs:v1:v0 */
/* tt_pad_timeout_step - TT overlay, Address: 0x8013A040, Size: 156 bytes (39 insns) */
#include "types.h"

extern u8 *tt_game_wk;
extern u8 *tt_text_buf;
extern u8 *tt_step_wk;

void tt_pad_timeout_step(void)
{
    u8 *a;
    u8 *q;
    u8 *c;
    u8 *p;
    s32 f;
    u16 h;
    u16 n;

    a = tt_game_wk;
    q = tt_text_buf + 2520;
    c = tt_step_wk;
    p = a + 16;

    if ((((u32)*(u16 *)(a + 8) >> 4) & 1) != 0) {
        f = 1;
        *(s16 *)(q + 6) = f;
    } else {
        *(s16 *)(q + 6) = 0;
        f = 1;
    }

    if (c[34] == 0) {
        h = *(u16 *)(p + 4);
        if ((h & 0x800) != 0 || (h & 0x20) != 0) {
            c[9] = f;
        }
    }

    n = *(u16 *)(c + 38);
    if ((u32)n >= 420U) {
        c[9] = f;
    } else {
        *(s16 *)(c + 38) = n + 1;
    }
}
